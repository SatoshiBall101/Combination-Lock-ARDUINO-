#include <avr/io.h>

#include "adc.h"


void adc_init(void)
{
    // Initialize the ADC

    // Configuring Thresholds (Vlow fixed to ground, Vhigh set to AVCC)

    ADMUX &= ~(1 << REFS1);
    ADMUX |= (1 << REFS0);

    // Configure for 8 bit conversion

    ADMUX |= (1 << ADLAR);

    // Use prescalar value 128 to fall within ADC KHz range

    ADCSRA |= (1 << ADPS0);
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS2);

    // enable ADC module

    ADCSRA |= (1 << ADEN);
}

uint8_t adc_sample(uint8_t channel)
{
    // Set ADC input mux bits to 'channel' value

    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // Set ADSC bit to 1

    ADCSRA |= (1 << ADSC);

    // Convert an analog input and return the 8-bit result

    while((ADCSRA & (1 << ADSC)) != 0)
    {
        // do nothin but keep checking
    }

    return ADCH;

}
