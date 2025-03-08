#include <hcs12.h>
#include <dbug12.h>

#include "lcd.h"
#include "keypad.h"
#include "util.h"

void mSDelay(unsigned int);

unsigned int segment_decoder[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F,
    0x77, 0x7C, 0x39, 0x5E, 0x79,
    0x71, 0x3D, 0x76
};

unsigned int digit_decoder[] = {0x07, 0x0B}; // Select left and right digit

unsigned char column, row;
int num1 = 0, num2 = 0, opCode = 0, res = 0;
int digits[2] = {0, 0}; // Stores ones and tens place
int current_digit = 0; // Tracks which digit is active

int main(void) {
    DDRB = 0xFF; // Make PORTB output (7-segment data)
    DDRJ |= 0x02;
    PTJ &= ~0x02; // Activate LED array on PORTB
    DDRP |= 0x0F; // Make PORTP output (Digit select)
    DDRA = 0x0F; // Rows input, Columns output

    while (1) {
        do {
            PORTA |= 0x0F; // Set columns high
            row = PORTA & 0xF0; // Read rows
        } while (row == 0x00); // Wait for keypress

        do {
            do {
                mSDelay(1);
                row = PORTA & 0xF0; // Read rows
            } while (row == 0x00);
            mSDelay(15); // Debounce delay
            row = PORTA & 0xF0;
        } while (row == 0x00); // Confirm keypress

        while (1) {
            PORTA &= 0xF0; // Clear column
            PORTA |= 0x01; // Column 0 set high
            row = PORTA & 0xF0;
            if (row != 0x00) { column = 0; break; }

            PORTA &= 0xF0; PORTA |= 0x02; // Column 1 set high
            row = PORTA & 0xF0;
            if (row != 0x00) { column = 1; break; }

            PORTA &= 0xF0; PORTA |= 0x04; // Column 2 set high
            row = PORTA & 0xF0;
            if (row != 0x00) { column = 2; break; }

            PORTA &= 0xF0; PORTA |= 0x08; // Column 3 set high
            row = PORTA & 0xF0;
            if (row != 0x00) { column = 3; break; }

            row = 0; // No key found
            break;
        }

        unsigned char ipt = 0;
        if (row == 0x10) ipt = "123A"[column];
        else if (row == 0x20) ipt = "456B"[column];
        else if (row == 0x40) ipt = "789C"[column];
        else if (row == 0x80) ipt = "*0#D"[column];

        if (ipt >= '0' && ipt <= '9') {
            num1 = num1 * 10 + (ipt - '0'); // Handle multi-digit input
        } else {
            if (ipt == 'C') { // Reset
                num1 = 0; num2 = 0; opCode = 0; res = 0;
            }
            if (ipt == 'D') { // Execute operation
                if (opCode == 1) res = num1 + num2;
                else if (opCode == 2) res = num1 * num2;
                else res = num1;

                digits[0] = res % 10;       // Ones place
                digits[1] = (res / 10) % 10; // Tens place
            }
            if (ipt == 'A') { num2 = num1; num1 = 0; opCode = 1; } // Addition
            if (ipt == 'B') { num2 = num1; num1 = 0; opCode = 2; } // Multiplication
        }

        // Digit Multiplexing - Alternates between displaying the two digits
        if (current_digit == 0) {
            PTP = digit_decoder[0]; // Select left digit
            PORTB = segment_decoder[digits[1]]; // Display tens place
        } else {
            PTP = digit_decoder[1]; // Select right digit
            PORTB = segment_decoder[digits[0]]; // Display ones place
        }
        current_digit = 1 - current_digit; // Toggle active digit

        do {
            mSDelay(15);
            PORTA |= 0x0F;
            row = PORTA & 0xF0;
        } while (row != 0x00); // Wait until key is released
    }
}

// Delay function
void mSDelay(unsigned int itime) {
    unsigned int i, j;
    for (i = 0; i < itime; i++)
        for (j = 0; j < 4000; j++);
}
