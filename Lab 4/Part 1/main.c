#include <hcs12.h>
#include <dbug12.h>
#include <vectors12.h>  // Your vectors12.h
#include <lcd.h>

// Global variables
volatile unsigned int duty_cycle = 50; // Start at 0° (1ms pulse width)
volatile unsigned char state = 1;  // 1 = soft start, 2 = soft stop
volatile unsigned int counter_for_real_time_interrupt = 0;
volatile unsigned int counter_for_real_time_interrupt_limit = 2;  // RTI limit (~87 ms per update)

// Function to execute PWM jobs (move servo)
void execute_pwm_jobs(void) {
    if (state == 1) {  // Soft start (move servo towards 90°)
        if (duty_cycle < 100) {  // 100 corresponds to ~1.5ms pulse (90°)
            duty_cycle += 5;  // Increase pulse width
            PWMDTY1 = duty_cycle;  // Update servo position
        } else {
            state = 2;  // Switch to soft stop
        }
    }
    else if (state == 2) {  // Soft stop (move servo back to 0°)
        if (duty_cycle > 50) {  // 50 corresponds to ~1ms pulse (0°)
            duty_cycle -= 5;  // Decrease pulse width
            PWMDTY1 = duty_cycle;  // Update servo position
        } else {
            state = 1;  // Reset to soft start after stopping
            duty_cycle = 50;  // Ensure it stops at 0°
        }
    }
}

// RTI Interrupt Service Routine
void INTERRUPT rti_isr(void) {
    CRGFLG = 0x80;  // Clear RTI flag (RTIF = 0x80)
    if (counter_for_real_time_interrupt == counter_for_real_time_interrupt_limit) {
        counter_for_real_time_interrupt = 0;  // Reset counter
        execute_pwm_jobs();  // Update servo position
        printf("Servo duty cycle: %d\n", duty_cycle);
    } else {
        counter_for_real_time_interrupt++;  // Increment counter
    }
}

// Initialize PWM Channel 1 for Servo Motor
void pwm_init(void) {
    PWME |= 0x10;      // Enable PWM Channel 1 (PWME1 = 1)
    PWMPOL |= 0x10;    // PWM signal starts high (PPOL1 = 1)
    PWMCLK = 0x10;   // Use Clock A for Channel 1 (PCLK1 = 0)
    PWMPRCLK = 0x05;   // Clock A prescaler = 32 (12 MHz / 32 = 375 kHz)
    PWMPER1 = 200;     // Period = 200 counts (~20ms period)
    PWMDTY1 = duty_cycle;  // Start at 0° (1ms pulse width)
    DDRP = 0xff;       // Set Port P as output (PP1 for PWM1)
}

// Initialize RTI (Real-Time Interrupt)
void rti_init(unsigned char rti_ctl_value, unsigned int counter_limit) {
    UserRTI = (unsigned int)&rti_isr;  // Set RTI vector to rti_isr
    counter_for_real_time_interrupt_limit = counter_limit;  // Set RTI counter limit
    RTICTL = rti_ctl_value;            // Set RTI control register
    counter_for_real_time_interrupt = 0;  // Reset counter
    CRGINT |= 0x80;                    // Enable RTI interrupts (RTIE = 0x80)
    CRGFLG = 0x80;                     // Clear RTI flag
}

// Main function
int main(void) {
    pwm_init();         // Initialize PWM for servo
    rti_init(0x7F, 1);  // Initialize RTI (~43.69 ms * 2 = ~87 ms per update)
    __asm("cli");       // Enable interrupts

    state = 1;          // Start with soft start

    while (1) {
        // Main loop can handle other tasks (e.g., keypad or LCD)
    }
}
