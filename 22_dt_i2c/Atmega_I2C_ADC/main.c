/* I2C Echo Example */
#include "I2CSlave.h"
#include <util/delay.h>
#include <avr/io.h>

#define I2C_ADDR 0x10

volatile uint8_t update_interval = 100;		/* Update Interval in 10ms */
volatile uint8_t adc_value = 0;				/* ADC value*/

void I2C_received(uint8_t received_data)
{
  update_interval = received_data;
}

void I2C_requested()
{
  I2C_transmitByte(adc_value);
}

void setup()
{
  // set received/requested callbacks
  I2C_setCallbacks(I2C_received, I2C_requested);

  // init I2C
  I2C_init(I2C_ADDR);

  // Init ADC
  ADMUX = (1<<REFS0);
}

/* From: https://stackoverflow.com/questions/30422367/how-to-fix-error-message-builtin-avr-delay-cycles-expects-a-compile-time-inte */
void my_delay_ms_10ms_steps(int ms)
{
	while (0 < ms)
	{  
		_delay_ms(10);
		ms -= 10;
	}
}

int main()
{
	setup();
	DDRB = 0x1;

	// Main program loop
	while(1) {
		/* Start conversion */
		ADCSRA = ((1<<ADEN) | (1<<ADSC));

		/* Wait for conversion */
		while(ADCSRA & (1<<ADSC));

		/* Update ADC value */
		adc_value = ((uint8_t) (ADC>>2));
		PORTB ^= 0x1;



		my_delay_ms_10ms_steps(update_interval);
	}
}
