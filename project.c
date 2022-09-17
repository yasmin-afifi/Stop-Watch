/******************************************************************************
 *
 * File Name: Mini_Project_2.c
 *
 * Description: Implement a Stop Watch system
 *
 * Author: Yasmin Afifi
 *
 *******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

unsigned char seg[6] = { 0, 1, 2, 3, 4, 5 };
unsigned char value[6] = { 0, 0, 0, 0, 0, 0 };
unsigned char g_tick = 0;

ISR(TIMER1_COMPA_vect) {
	g_tick = 1;           //make a tick
}

ISR(INT0_vect){     //REST

	unsigned char i;
	for (i = 0; i < 6; i++) {
		value[i] = 0;
	}

}

ISR(INT1_vect){     //PAUSE

	TCCR1B &= ~(1 << CS12);
	TCCR1B &= ~(1 << CS10);

}


ISR(INT2_vect){      //RESUME

	TCCR1B |= (1 << CS12) | (1 << CS10);

}

void INT0_Init(void){

	SREG &= ~(1 << 7);
	DDRD &= ~(1<< PD2);
	PORTD |= (1<< PD2); //internal pull-up resistor
	MCUCR |= (1<< ISC01);
	GICR |= (1<< INT0);
	SREG |= (1<< 7);

}

void INT1_Init(void){

	SREG &= ~(1 << 7);
	DDRD &= ~(1<< PD3);
	MCUCR |= (1<< ISC10) | (1<< ISC11);
	GICR |= (1<< INT1);
	SREG |= (1<< 7);
}


void INT2_Init(void){

	SREG &= ~(1 << 7);
	DDRD &= ~(1<< PB2);
	PORTD |= (1<< PB2); //internal pull-up resistor
	MCUCR |= (1<< ISC2);
	GICR |= (1<< INT2);
	SREG |= (1<< 7);

}


void Timer1_CTC_Init(void){   //TIMER CTC MODE

	TCNT1 = 0;

	OCR1A = 1000;

	TIMSK |= (1 << OCIE1A);

	TCCR1A = (1<<FOC1A);

	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //CTC Mode and prescaler Fcpu/1024

}


int main(void){

	DDRC |= 0X0F;
	PORTC &= ~0X0F;

	DDRA |= 0X3F;
	PORTA |= 0X3F;

	SREG |= (1 << 7);

	INT0_Init();
	INT1_Init();
	INT2_Init();
	Timer1_CTC_Init();

	while(1){

		if(g_tick ==1){
			g_tick = 0;
								//7-Segment Displaying
			if(value[0] == 9){
				value[0] = 0;

				if(value[1] == 5){
					value[1] = 0;

					if(value[2] == 9){
						value[2] = 0;

						if(value[3] == 5){
							value[3] = 0;

							if(value[4] == 9){
								value[4] = 0;

								if(value[5] == 9){
									value[5] = 0;
								}
								else value[5]++;
							}
							else value[4]++;
						}
						else value[3]++;
					}
					else value[2]++;
				}
				else value[1]++;
			}
			else value[0]++;



			while (1) {

				unsigned char i;
				for (i = 0; i < 6; i++) {
					PORTA &= ~0X3F;
					PORTA |= (1 << seg[i]);
					PORTC = (PORTC & 0xF0) | (value[i] & 0x0F);
					_delay_us(100);
				}



				if (g_tick == 1) //to handle a new interrupt
					break;

			}

		}
	}

}
