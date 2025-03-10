#include <hidef.h>      /* For EnableInterrupts macro */
#include "mc9s12dg256.h" /* MCU-specific definitions */

volatile unsigned char dutyCycle = 10; // Start at 10% brightness

/* Initialize PWM on Channel 0 (PP0) for LED brightness */
void PWM_Init(void) {
    PWMPRCLK = 0x02;   // Set PWM clock prescaler to /4 (24MHz/4 = 6MHz)
    PWMSCLA = 3;       // Further scale clock (6MHz/3 = 2MHz)
    PWMCLK = 0x01;     // Select scaled clock for PWM0
    PWMPER0 = 200;     // Set PWM period (200 ticks at 2MHz → 1ms period)
    PWMDTY0 = (dutyCycle * PWMPER0) / 100; // Set initial brightness
    PWME |= 0x01;      // Enable PWM0 on PP0 (LED control)
}

/* RTI Interrupt Service Routine */
#pragma CODE_SEG __NEAR_SEG NON_BANKED  // Ensure ISR is in correct memory space
interrupt VectorNumber_Vrti void RTI_ISR(void) {
    CRGFLG |= 0x80; // Clear RTI interrupt flag
    if (dutyCycle < 100) {
        dutyCycle += 10; // Increase brightness
        PWMDTY0 = (dutyCycle * PWMPER0) / 100;
    }
}

/* Initialize RTI (Real-Time Interrupt) */
void RTI_Init(void) {
    CRGINT |= 0x80;  // Enable RTI interrupt
    RTICTL = 0x50;   // Set RTI timeout (~16ms)
    EnableInterrupts; // Enable global interrupts
}

/* Gradual Brightening (Soft Start) */
void Soft_Start(void) {
    dutyCycle = 10;
    PWMDTY0 = (dutyCycle * PWMPER0) / 100; // Set initial brightness
}

/* Gradual Dimming (Soft Stop) */
void Soft_Stop(void) {
    while (dutyCycle > 0) {
        dutyCycle -= 10;  // Decrease brightness by 10%
        PWMDTY0 = (dutyCycle * PWMPER0) / 100;
        _delay_ms(100); // Smooth transition
    }
}

/* Main Function */
void main(void) {
    PWM_Init();  // Initialize PWM
    RTI_Init();  // Initialize RTI for smooth updates

    Soft_Start(); // Brighten LED gradually
    _delay_ms(5000); // LED stays fully lit

    Soft_Stop(); // Dim LED gradually

    for (;;) {} // Infinite loop
}
