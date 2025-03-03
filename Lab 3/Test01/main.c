#include <hcs12.h>
#include <dbug12.h>

#include "lcd.h"
#include "keypad.h"
#include "util.h"

void mSDelay(unsigned int);

unsigned int segment_decoder[]={
    0x3f,0x06,0x5b,0x4f,0x66,
    0x6d,0x7d,0x07,0x7f,0x6f,
    0x77,0x7c,0x39,0x5e,0x79,
    0x71,0x3d,0x76
};

const unsigned char keypad[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

unsigned int digit_decoder[] = {0x07, 0x0B, 0x0D, 0x0E};
unsigned char column, row;
int num1 = 0, num2 = 0, opCode = 0, res = 0;

void resetCalculator() {
    num1 = 0;
    num2 = 0;
    opCode = 0;
    res = 0;
    PORTB = 0x00;  // Clear display
}

unsigned char getKeyPress() {
    while (1) {
        PORTA = 0x0F;  // Set columns high
        row = PORTA & 0xF0;
        if (row) break;
    }
    
    mSDelay(15);  // Debounce delay
    while (PORTA & 0xF0);  // Wait for key release
    
    for (column = 0; column < 4; column++) {
        PORTA = ~(1 << column);
        row = PORTA & 0xF0;
        if (row) break;
    }
    
    return keypad[row >> 4][column];
}

int main(void) {
    DDRB = 0xFF;
    DDRJ |= 0x02;
    PTJ &= ~0x02;
    DDRP |= 0x0F;
    DDRA = 0x0F;

    while (1) {
        unsigned char ipt = getKeyPress();

        if (num1 != -1 && num2 != -1) {
            if (ipt >= '0' && ipt <= '9') {
                num1 = ipt - '0';
                PORTB = segment_decoder[num1];
            } else {
                switch (ipt) {
                    case 'A':
                        num2 = num1;
                        num1 = 0;
                        PORTB = segment_decoder[10];
                        opCode = 1;
                        break;
                    case 'B':
                        num2 = num1;
                        num1 = 0;
                        PORTB = segment_decoder[11];
                        opCode = 2;
                        break;
                    case 'C':
                        resetCalculator();
                        break;
                    case 'D':
                        PORTB = segment_decoder[13];
                        res = (opCode == 1) ? (num1 + num2) : (opCode == 2) ? (num1 * num2) : res;
                        num1 = -1;
                        break;
                }
            }
            PTP = digit_decoder[0];
        } else {
            int temp = res, i = 0;
            do {
                PORTB = segment_decoder[temp % 10];
                PTP = digit_decoder[i++];
                mSDelay(175);
                temp /= 10;
            } while (temp && i < 4);
        }
    }
}

void mSDelay(unsigned int itime) {
    for (unsigned int i = 0; i < itime; i++)
        for (unsigned int j = 0; j < 4000; j++);
}
