/*
Embedded Systems
-   Sachita Rayapati
*/

#include <stdio.h>
#include "avr.h"
#include "lcd.h"

// Frequencies for notes on Octave 3

const char keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
};

// Checks if key is pressed on the Keypad
// ----ALL 8 GPIOs TO N/C---
int is_pressed(int r, int c) {
	DDRC = 0x00;
	PORTC = 0x00;

	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	CLR_BIT(DDRC, c+4);
	SET_BIT(PORTC, c+4);
	avr_wait(10);
	if (GET_BIT(PINC, c+4)==0){
		return 1;
	}
	return 0;
}

// If key was pressed, return the key value from the keypad.
char get_key() {
	int i, j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(is_pressed(i,j)){
				avr_wait(50);
				return keypad[i][j];
			}
		}
		avr_wait(10);
	}
	return '$';
}

// Initial output on display screen
void print_first() {
	char buf[17];
	char buff[17];
	lcd_pos(0, 0);
	sprintf(buf, "Project 4");
	
	lcd_puts2(buf);
	lcd_pos(1, 0);
	sprintf(buff, "AD Volt");
	lcd_puts2(buff);
}

int get_sample() {
	// configure the ADC
	ADMUX = 0b01000000;
	// start 
	ADCSRA = 0b11000000;
	// wait for conversion result
	while (GET_BIT(ADCSRA,6)==1);
	int input = ADCL;
	return (ADCH << 0b1000) + input;
	//return ADC;
}

double sum = 0.0;
double count = 0.0;

typedef struct {
	short inst_volt;
	short max_volt;
	short min_volt;
	double avg_volt;
} voltReads;

void print_display(short maxVolt, short minVolt, short instVolt, double avgVolt) {
	char buf[17];
	char buff[17];
	lcd_pos(0, 0);
	sprintf(buf, "%.2f V %.2f V", (maxVolt/1023.0)*5, (minVolt/1023.0)*5);
	lcd_puts2(buf);

	lcd_pos(1, 0);
	sprintf(buff, "%.2f V %.2f V", (instVolt/1023.0)*5, avgVolt);
	lcd_puts2(buff);
}

void print_sample(short instV){
	char buf[17];
	lcd_clr();
	lcd_pos(0,0);
	sprintf(buf, "%.2f V", (instV/1023.0)*5);
	lcd_puts2(buf);
}

int main() {
	SET_BIT(DDRB, 4);
	SET_BIT(PORTB, 4);
	
	lcd_init();
	print_first();
	avr_wait(1000);
	lcd_clr();

	voltReads display;
	display.inst_volt = 0;
	display.max_volt = 0;
	display.min_volt = 1023;
	display.avg_volt = 0.0;
	print_display(display.max_volt, display.min_volt, display.inst_volt, display.avg_volt);
	short instvolt = display.inst_volt;
	avr_wait(1000);
	lcd_clr();
	
	while(1) {
		char note_key = get_key();
		switch(note_key){
			case '1': // 1 represents start
				while(note_key != '2'){
					display.inst_volt = get_sample(); // Instantaneous voltage
					// Maximum voltage
					if(display.inst_volt > display.max_volt) {
						display.max_volt = display.inst_volt;
					}

					// Minimum voltage
					if(display.inst_volt < display.min_volt) {
						display.min_volt = display.inst_volt;
					}
			
					// Total sum
					sum += (display.inst_volt / 1023.0) * 5.0;
					//sum += display.inst_volt;
					++count;
					display.avg_volt = sum/count;
					instvolt = display.inst_volt;
					print_display(display.max_volt, display.min_volt, display.inst_volt, display.avg_volt);
					note_key = get_key();
					avr_wait(500);
				}
				break;
			case '2':
				break;
			case '3':
				sum = 0.0;
				count = 0.0;
				display.inst_volt = 0;
				display.avg_volt = 0.0;
				display.max_volt = 0;
				display.min_volt = 1023;
				print_display(display.max_volt, display.min_volt, display.inst_volt, display.avg_volt);
				//lcd_clr();
				break;
			case '4':
				display.inst_volt = get_sample();
				print_display(display.max_volt, display.min_volt, display.inst_volt, display.avg_volt);
				avr_wait(1000);
				lcd_clr();
		}
	}
	return 0;
}
