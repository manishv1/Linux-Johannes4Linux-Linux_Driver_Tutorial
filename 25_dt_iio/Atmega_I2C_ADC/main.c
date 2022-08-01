/* I2C Echo Example */
#include "I2CSlave.h"
#include <util/delay.h>
#include <avr/io.h>

#define I2C_ADDR 0x10

#define CMD_IDLE             0x00
#define CMD_GET_STATE        0x11
#define CMD_SET_STATE        0x22
#define CMD_GET_CHANNEL      0x33
#define CMD_SET_CHANNEL      0x44
#define CMD_GET_ADC_VAL      0x55

#define STATE_OFF        0x0
#define STATE_ON         0x1



volatile uint8_t adc_value = 0;				/* ADC value*/
volatile uint8_t sel_adc = 0;
volatile uint8_t state = STATE_OFF;
volatile uint8_t cmd = CMD_IDLE;

void I2C_received(uint8_t received_data)
{
	switch(cmd) {
		case CMD_IDLE:
			switch(received_data) {
				case CMD_GET_STATE:
				case CMD_SET_STATE:
				case CMD_GET_CHANNEL:
				case CMD_SET_CHANNEL:
				case CMD_GET_ADC_VAL:
					cmd = received_data;
					break;
				default:
					break;
			}
			break;
		case CMD_SET_STATE:
			state = received_data;
			cmd = CMD_IDLE;
			break;
		case CMD_SET_CHANNEL:
			sel_adc = received_data;
			cmd = CMD_IDLE;
			break;
		default:
			break;
	}
}

void I2C_requested()
{
	switch(cmd) {
		case CMD_GET_STATE:
			I2C_transmitByte(state);
			cmd = CMD_IDLE;
			break;
		case CMD_GET_CHANNEL:
			I2C_transmitByte(sel_adc);
			cmd = CMD_IDLE;
			break;
		case CMD_GET_ADC_VAL:
			I2C_transmitByte(adc_value);
			cmd = CMD_IDLE;
			break;
		default:
			I2C_transmitByte(0x51);
			break;
	}
}

void setup()
{
  // set received/requested callbacks
  I2C_setCallbacks(I2C_received, I2C_requested);

  // init I2C
  I2C_init(I2C_ADDR);
}

int main()
{
	setup();
	DDRB = 0x1;

	// Main program loop
	while(1) {
		if(state == STATE_ON) {
			/* Select ADC */
			ADMUX = ((1<<REFS0) | (sel_adc & 0xf));

			/* Start conversion */
			ADCSRA = ((1<<ADEN) | (1<<ADSC));

			/* Wait for conversion */
			while(ADCSRA & (1<<ADSC));

			/* Update ADC value */
			adc_value = ((uint8_t) (ADC>>2));
			PORTB |= 0x1;
		}
		else
			PORTB &= 0xfe;

		_delay_ms(50);
	}
}
