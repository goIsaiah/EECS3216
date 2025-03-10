#include <hidef.h>      /* for EnableInterrupts macro */
#include "derivative.h" /* Include MCU specific definitions */

volatile unsigned int distance = 0; // Stores ADC-based distance measurement

/* Initialize PWM for Servo Motor (PP1 - PWM Channel 1) */
void PWM_Init(void) {
    PWMPRCLK = 0x02;   // Set PWM clock prescaler to /4 (24MHz/4 = 6MHz)
    PWMSCLA = 3;       // Further scale clock (6MHz/3 = 2MHz)
    PWMCLK = 0x02;     // Select scaled clock for PWM1
    PWMPER1 = 200;     // Set PWM period (1ms = 200 ticks at 2MHz)
    PWMDTY1 = 15;      // Default to 0° (1ms pulse)
    PWME |= 0x02;      // Enable PWM1 (PP1)
}

/* Configure ADC for Proximity Sensor on PAD0 */
void ADC_Init(void) {
    ATDCTL2 = 0x80;  // Enable ADC module
    ATDCTL3 = 0x08;  // Single result per sequence
    ATDCTL4 = 0x85;  // 10-bit resolution, 8MHz ADC clock
    ATDCTL5 = 0x80;  // Start conversion on Channel 0
}

/* Interrupt Service Routine for ADC Conversion */
interrupt 22 void ADC_ISR(void) {
    distance = ATDDR0; // Read ADC result (10-bit value)
    distance = (distance * 100) / 1023; // Scale 0-1023 to 0-100%
}

/* Start ADC Conversion (Trigger Reading) */
void ADC_Start_Conversion(void) {
    ATDCTL5 = 0x80; // Start ADC conversion on Channel 0
}

/* Move Servo to Specified Angle (0° to 180°) */
void Servo_Move(unsigned int angle) {
    unsigned int duty = (angle * 100) / 180 + 50; // Map 0-180° to 1ms-2ms pulse width
    PWMDTY1 = duty;
}

/* Main Function - Monitors Distance & Controls Servo */
void main(void) {
    PWM_Init();  // Initialize PWM for servo control
    ADC_Init();  // Initialize ADC module
    EnableInterrupts; // Enable global interrupts

    while (1) {
        ADC_Start_Conversion(); // Trigger ADC conversion

        if (distance < 50) { // If object is close (Threshold: 50%)
            Servo_Move(90);  // Move Servo to 90°
        } else {
            Servo_Move(0);   // Move Servo to 0°
        }

        _delay_ms(500); // Delay to stabilize readings
    }
}