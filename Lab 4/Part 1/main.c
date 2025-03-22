#include <hcs12.h>
#include <dbug12.h>

#include "lcd.h"
#include "keypad.h"
#include "util.h"

// PWM state definitions
#define PWM_STATE_IDLE       0
#define PWM_STATE_SOFT_START 1
#define PWM_STATE_RUNNING    2
#define PWM_STATE_SOFT_STOP  3

// PWM configuration parameters
#define PWM_MIN_DUTY      0    // 0% duty cycle
#define PWM_MAX_DUTY      100  // 100% duty cycle
#define PWM_STEP_SIZE     1    // Change duty cycle by 1% each update
#define PWM_UPDATE_FREQ   5    // Update duty cycle every 5 RTI ticks
#define PWM_PERIOD_VALUE  100000 // PWM period value (fixed constant)

// PWM control variables
volatile unsigned char pwm_state = PWM_STATE_IDLE;
volatile unsigned char pwm_current_duty = 0;
volatile unsigned int pwm_update_counter = 0;

// Flag that indicates the state transition is complete
volatile unsigned char state_complete = 0;

volatile unsigned int counter_for_real_time_interrupt;
volatile unsigned int counter_for_real_time_interrupt_limit;

// Initialize PWM for motor control on channel 5 (pin J7 PP5)
void pwm_init(void) {
    // Set PWM port to output mode (PP5)
    DDRP |= 0x20;  // Bit 5 = 1

    // Configure PWM module:
    PWMCLK = 0x20; // Use clock source A for channel 5
    PWMPOL = 0x20; // PWM output high polarity on channel 5
    PWMCTL = 0x00; // Standard PWM operation

    // Set PWM period for approximately 50Hz (typical for servos)
    PWMPRCLK = 0x33;           // Prescaler of 8 for clock A & B
    PWMPER5 = PWM_PERIOD_VALUE; // Use defined constant for period

    // Start with 0% duty cycle
    PWMDTY5 = 0;

    // Enable PWM channel 5
    PWME |= 0x20;
}

// Set the PWM duty cycle (percent, from 0 to 100)
void pwm_set_duty(unsigned char duty) {
    // Convert percentage to PWM counts
    unsigned long counts = ((unsigned long)duty * PWM_PERIOD_VALUE) / 100;
    PWMDTY5 = (unsigned int)counts;
}

// Begin the soft-start process (increasing duty from 0 to max)
void pwm_soft_start(void) {
    pwm_state = PWM_STATE_SOFT_START;
    pwm_current_duty = PWM_MIN_DUTY;
    pwm_set_duty(pwm_current_duty);
    state_complete = 0;
    DispStr(2, 1, "Soft Start     ");
}

// Begin the soft-stop process (decreasing duty from max to 0)
void pwm_soft_stop(void) {
    pwm_state = PWM_STATE_SOFT_STOP;
    state_complete = 0;
    DispStr(2, 1, "Soft Stop      ");
}

// Helper function to display a three-digit duty cycle percentage on the LCD
void display_duty_cycle(int row, int col, int value) {
    char duty_str[4];

    // Build a string with three digits (with leading zeros if necessary)
    duty_str[0] = '0' + (value / 100) % 10;
    duty_str[1] = '0' + (value / 10) % 10;
    duty_str[2] = '0' + value % 10;
    duty_str[3] = '\0';

    // Display the string at the specified position
    DispStr(row, col, duty_str);
}

// This function is called periodically via the RTI interrupt.
void update_pwm_state_machine(void) {
    switch (pwm_state) {
        case PWM_STATE_SOFT_START:
            if (pwm_current_duty < PWM_MAX_DUTY) {
                pwm_current_duty += PWM_STEP_SIZE;
                pwm_set_duty(pwm_current_duty);
            } else {
                pwm_current_duty = PWM_MAX_DUTY;
                pwm_set_duty(pwm_current_duty);
                state_complete = 1;
            }
            if (state_complete) {
                pwm_soft_stop();
            }
            break;

        case PWM_STATE_SOFT_STOP:
            if (pwm_current_duty > PWM_MIN_DUTY) {
                pwm_current_duty -= PWM_STEP_SIZE;
                pwm_set_duty(pwm_current_duty);
            } else {
                pwm_current_duty = PWM_MIN_DUTY;
                pwm_set_duty(pwm_current_duty);
                state_complete = 1;
            }
            if (state_complete) {
                pwm_soft_start();
            }
            break;

        default:
            break;
    }
    display_duty_cycle(1, 12, pwm_current_duty);
}

// Function that is periodically called using the RTI interrupt.
void execute_the_jobs(void) {
    pwm_update_counter++;
    if (pwm_update_counter >= PWM_UPDATE_FREQ) {
        pwm_update_counter = 0;
        update_pwm_state_machine();
    }
}

// RTI interrupt service routine. It calls execute_the_jobs at a fixed rate.
void INTERRUPT rti_isr(void) {
    CRGFLG = 0x80;
    if (counter_for_real_time_interrupt == counter_for_real_time_interrupt_limit) {
        counter_for_real_time_interrupt = 0;
        execute_the_jobs();
    } else {
        counter_for_real_time_interrupt++;
    }
}

// Initialize the RTI (Real-Time Interrupt) with a given control value and counter limit.
void rti_init(unsigned char rti_ctl_value, unsigned int counter_limit) {
    UserRTI = (unsigned int)&rti_isr;
    counter_for_real_time_interrupt_limit = counter_limit;
    RTICTL = rti_ctl_value;
    counter_for_real_time_interrupt = 0;
    CRGINT |= 0x80;
    CRGFLG = 0x80;
}

int main(void) {
    COPCTL = 0x00;
    set_clock_24mhz();
    pwm_init();
    rti_init(0x11, 10);
    __asm("cli");
    DispInit(2, 16);
    DispClrScr();
    DispStr(1, 1, "Duty: ");
    pwm_soft_start();
    while (1) {}
}
