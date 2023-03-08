/********************************************
 *
 *  Name: Amman C
 *  Email: chuhan@usc.edu
 *  Section: 12:30 F
 *  Assignment: Lab 5 - Analog-to-digital conversion
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
#include "adc.h"
#include "stdio.h"

void rand_init(void);

int main(void)
{

    lcd_init(); // Initialize the LCD

    adc_init();  // Initialize the ADC

    rand_init(); // Initialize the random number function

    // Write splash screen and delay for 1 second

    lcd_moveto(0,0);

    lcd_stringout("Amman C.");

    lcd_moveto(1,0);

    char birthday[15];

    char day = 6;
    char month = 1;
    char year = 3;

    snprintf(birthday, 15, "%02d/%02d/%02d", month, day, year);

    lcd_stringout(birthday);

    _delay_ms(1000);

    lcd_clear(); // helper function to clear screen

    // Find three random numbers and display on top line

    unsigned char randNum[4];
    randNum[0]= rand() % 32;
    randNum[1]= rand() % 32;
    randNum[2]= rand() % 32;

    lcd_moveto(0,0);

    char randNums[11];

    snprintf(randNums, 11, "%3d%3d%3d", randNum[0], randNum[1], randNum[2]);

    lcd_stringout(randNums);

    // Show the number selector ('>') on the bottom line

    lcd_moveto(1,0);

    lcd_stringout(">");

    unsigned char lcd_currentx = 1;
    unsigned char lcd_currenty = 0;
    unsigned char count = 0;

    unsigned char unlock[3];
    unlock[0] = 0;
    unlock[1] = 0;
    unlock[2] = 0;

    _delay_ms(1000);

    while (1) {                 // Loop forever


	// Convert ADC channel for buttons to 0-255

    uint8_t button_adc = adc_sample(0);

        // If right or left button pressed, move '>' to new position

        if(button_adc < 10) // right gave signal 0
        {
            lcd_moveto(lcd_currentx, lcd_currenty);
            lcd_stringout(" ");
            lcd_currenty += 3;
            _delay_ms(100);
            lcd_moveto(1, lcd_currenty);
            lcd_stringout(">");
        }
        else if (145 < button_adc && button_adc < 165) // left gave signal 155
        {
            lcd_moveto(lcd_currentx, lcd_currenty);
            lcd_stringout(" ");
            lcd_currenty -= 3;
            _delay_ms(100);
            lcd_moveto(1, lcd_currenty);
            lcd_stringout(">");
        }

        // Read potentiometer ADC channel

        uint8_t pot_adc = adc_sample(3);


	// Convert ADC channel for potentiometer to 0-255, change to 0-31 and display

        pot_adc = pot_adc / (255/31);

        char print[7];

        snprintf(print, 7, "%2d", pot_adc);
        
        lcd_moveto(1, 13);

        lcd_stringout(print);

	// If select button pressed copy number to one of the lock inputs positions

    if(195 < button_adc && button_adc < 215)
    {

        lcd_moveto(lcd_currentx, lcd_currenty);

        char temp = pot_adc;

        if (pot_adc == randNum[count])
        {
            unlock[count] = 1;
        }

        char tempString[4];

        snprintf(tempString, 4, "%3d", temp);
        
        lcd_stringout(tempString);

        if(count == 2)
        {
            count = 0;
        }
        else
        {
            count++;
        }

    }

	// Check to see if all three lock inputs match the combination

        if(unlock[0] == 1 && unlock[1] == 1 && unlock[2] == 1)
        {
            lcd_moveto(0, 10);
            lcd_stringout("Unlock");
        }

    }

    return 0;   /* never reached */
}

void rand_init()
{
    int16_t seed = 0;
    uint8_t i, j, x;

    // Build a 15-bit number from the LSBs of an ADC
    // conversion of the channels 1-5, 3 times each
    for (i = 0; i < 3; i++) {
	for (j = 1; j < 6; j++) {
	    x = adc_sample(j);
	    x &= 1;	// Get the LSB of the result
	    seed = (seed << 1) + x; // Build up the 15-bit result
	}
    }
    srand(seed);	// Seed the rand function
}
