#include <hidef.h>      /* for EnableInterrupts macro */
#include "derivative.h" /* Include MCU specific definitions */

volatile unsigned char dutyCycle = 10; // Start at 10% duty cycle

/* Initialize PWM on Channel 0 (PP0) */
void PWM_Init(void) {
    PWMPRCLK = 0x02;   // Set PWM clock prescaler to /4 (24MHz/4 = 6MHz)
    PWMSCLA = 3;       // Further scale clock (6MHz/3 = 2MHz)
    PWMCLK = 0x01;     // Select scaled clock for PWM0
    PWMPER0 = 200;     // Set PWM period (200 ticks at 2MHz → 1ms period)
    PWMDTY0 = (dutyCycle * PWMPER0) / 100; // Set initial duty cycle (10%)
    PWME |= 0x01;      // Enable PWM0
}

/* Real-Time Interrupt (RTI) ISR for Soft Start */
interrupt 7 void RTI_ISR(void) {
    CRGFLG |= 0x80; // Clear RTI interrupt flag
    if (dutyCycle < 100) {
        dutyCycle += 10; // Increase duty cycle by 10%
        PWMDTY0 = (dutyCycle * PWMPER0) / 100; // Scale duty cycle
    }
}

/* Initialize RTI for Soft Start */
void RTI_Init(void) {
    CRGINT |= 0x80;  // Enable RTI interrupt
    RTICTL = 0x50;   // Set RTI timeout to ~16ms
    EnableInterrupts;
}

/* Gradual acceleration (Soft Start) */
void Soft_Start(void) {
    dutyCycle = 10;
    PWMDTY0 = (dutyCycle * PWMPER0) / 100; // Set initial duty cycle
}

/* Gradual deceleration (Soft Stop) */
void Soft_Stop(void) {
    while (dutyCycle > 0) {
        dutyCycle -= 10;  // Decrease duty cycle
        PWMDTY0 = (dutyCycle * PWMPER0) / 100;
        _delay_ms(100); // Wait for smooth transition
    }
}

/* Main function */
void main(void) {
    PWM_Init();  // Initialize PWM
    RTI_Init();  // Initialize RTI for smooth updates

    Soft_Start(); // Start motor gradually
    _delay_ms(5000); // Keep motor running

    Soft_Stop(); // Stop motor gradually

    for (;;) {} // Infinite loop
}
