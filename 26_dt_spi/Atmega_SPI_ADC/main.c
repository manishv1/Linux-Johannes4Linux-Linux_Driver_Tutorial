#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SPI_DDR DDRB
#define MISO_GPIO 4

#define CMD_IDLE             0x00
#define CMD_GET_STATE        0x11
#define CMD_SET_STATE        0x22
#define CMD_GET_CHANNEL      0x33
#define CMD_SET_CHANNEL      0x44
#define CMD_GET_ADC_VAL      0x55
#define CMD_ECHO             0x66

#define STATE_OFF        0x0
#define STATE_ON         0x1


volatile uint8_t adc_value = 0;				/* ADC value*/
volatile uint8_t sel_adc = 0;
volatile uint8_t state = STATE_OFF;
volatile uint8_t cmd = CMD_IDLE;

int main() {
	/* All Pins Input except MISO Pin */
	SPI_DDR |= 1 | (1<<MISO_GPIO);

	/* Set SPI IP to Slave Mode and IRQ enable */
	SPCR = (1<<SPE) | (1<<SPIE);
	
	/* Enable IRQs */
	sei();

	SPDR = 0x5a;
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
	return 0;
}

ISR(SPI_STC_vect) {
	uint8_t data;
	/* Get current received byte */
	data = SPDR;
	/* State machine */
	switch(cmd) {
		case CMD_IDLE:
			switch(data) {
				case CMD_GET_STATE:
					SPDR = state;
					cmd = data;
					break;
				case CMD_SET_STATE:
					cmd = data;
					break;
				case CMD_GET_CHANNEL:
					SPDR = sel_adc;
					cmd = data;
					break;
				case CMD_SET_CHANNEL:
					cmd = data;
					break;
				case CMD_GET_ADC_VAL:
					SPDR = adc_value;
					cmd = data;
					break;
				case CMD_ECHO:
					SPDR = data;
					cmd = data;
					break;
				default:
					break;
			}
			break;
		case CMD_GET_STATE:
			cmd = CMD_IDLE;
			break;
		case CMD_SET_STATE:
			state = data;
			cmd = CMD_IDLE;
			break;
		case CMD_GET_CHANNEL:
			cmd = CMD_IDLE;
			break;
		case CMD_SET_CHANNEL:
			sel_adc = data;
			cmd = CMD_IDLE;
			break;
		case CMD_GET_ADC_VAL:
			cmd = CMD_IDLE;
			break;
		default:
			cmd = CMD_IDLE;
			break;
	}
}

