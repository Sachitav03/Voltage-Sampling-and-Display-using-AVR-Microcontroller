/*
Computer Science 145: Embedded Systems
Project 2:
*/

#include <stdio.h>
#include "avr.h"
#include "lcd.h"

#define MAX_MONTH 12
#define MIN_MONTH 1
#define MAX_DAY   31
#define MIN_DAY   1
#define MAX_YEAR  2030
#define MIN_YEAR  2016
#define MAX_HOUR  23
#define MIN_HOUR  0
#define MAX_MIN   59
#define MIN_MIN   0
#define MAX_SECOND   59
#define MIN_SECOND   0
#define true 1
#define false 0

typedef struct {
	int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
} DateTime;

int monthsw30days[] = {4, 6, 9, 11}; // Array storing months with 30 days
int monthsw31days[] = {1, 3, 5, 7, 8, 10}; // Array storing months with 31 days
// Other would be directly accessed as {2}

/*
Default values on the display screen.
Values chosen test military time hour change.
*/
void init_dt(DateTime *dt) {
	dt->year = 2008;
	dt->month = 2;
	dt->day = 28;
	dt->hour = 23;
	dt->minute = 59;
	dt->second = 57;
}

const char keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
};

/*
The function checks if the month is in the
30 or 30 days array (monthsw30days[] or monthsw31days[])
*/
int contains_month(int month, int *months, int len_months) {
	for (int i = 0; i < len_months; i++) {
		if (month == months[i]) {
			return 1;
		}
	}
	return 0;
}

/*
@brief: Advances the date and time in
nested logic, ensuring that when the second
is updated, it runs a series of checks that ensure
that the new seconds value is valid, and subsequent
minute, hour, day, month, and year values correspond
to the user's settings.
*/
void advance_dt(DateTime *dt, int isMilitary)
{
	++dt->second;
	if (dt->second == 60)
	{
		dt->minute++;
		dt->second = 0;
		
		if ((dt->minute == 60))
		{
			dt->hour++;
			dt->minute = 0;
			if(dt->hour == 24){
				dt->hour = 0;
				dt->day++;
				if (dt->year % 4 == 0 && dt->day > 29)
				{
					if (dt->month == 2){
						dt->day = 1;
						dt->month++;
					}
				}
				else if (dt->year % 4 != 0 && dt->day > 28){
					if(dt->month == 2){
						dt->day = 1;
						dt->month++;
					}
				}
				else if((dt->day == 30) && (contains_month(dt->month, monthsw30days, 4))){
					dt->month++;
					dt->day = 1;
				}
				else if((dt->day == 31) && (contains_month(dt->month, monthsw31days, 4))){
					dt->month++;
					dt->day = 1;
				}
				if(dt->month > 12){
					dt->year++;
					dt->month = 1;
				}
				
			}
		}
		
	}
	
	if ((isMilitary) && (dt->hour >= 24)) {
		dt->day++;
		dt->hour = 0;
	}
	if((isMilitary) && (dt->hour == 12)){
		dt->hour = 0;
	}
	else if ((!isMilitary) && (dt->hour > 12)) {
		if(dt->minute == 0 && dt->hour == 0){
			dt->day++;
		}
		dt->hour = 1;
	}
	else if ((!isMilitary) && dt->hour == 0){
		dt->hour = 12;
	}
}

// Incremental updates of value on the editing mode
int increment_value(int value, int max_limit, int min_limit) {
	return (value == max_limit) ? min_limit : value + 1;
}

// Decremental updates of value on the editing mode
int decrement_value(int value, int min_limit, int max_limit) {
	return (value == min_limit) ? max_limit : value - 1;
}

/*
Function updates changes in editing mode.
If function is changed in the editing mode,
depending on the case, the switch case changed
the value on the display accordingly
*/
void update_dt(DateTime *dt, char isNoted, int isMilitary) {
	switch(isNoted){
		// Date: Row 1
		case '1':
		dt->month = increment_value(dt->month, MAX_MONTH-1, MIN_MONTH); 
		break;
		
		case '2':
		dt->month = decrement_value(dt->month, MIN_MONTH, MAX_MONTH);
		break;
		
		case '3':
		if(contains_month(dt->month, monthsw30days, 4)){
			dt->day = increment_value(dt->day, MAX_DAY-1, MIN_DAY);
		}
		else if(contains_month(dt->month, monthsw31days, 6)){
			dt->day = increment_value(dt->day, MAX_DAY, MIN_DAY);
		}
		else if(dt->month == 2){
			if(dt->year % 400 == 0){
				dt->day = increment_value(dt->day, MAX_DAY-2, MIN_DAY);
			}
			else {
				dt->day = increment_value(dt->day, MAX_DAY-3, MIN_DAY);
			}
		}
		break;
		
		case '4':
		dt->day = decrement_value(dt->day, MIN_DAY, MAX_DAY);
		break;
		
		case '5':
		dt->year = increment_value(dt->year, MAX_YEAR, MIN_YEAR);
		break;
		
		case '6':
		dt->year = decrement_value(dt->year, MIN_YEAR, MAX_YEAR);
		break;
		
		// Time Row: 2
		case '7':
		if(isMilitary){
			dt->hour = increment_value(dt->hour, MAX_HOUR, MIN_HOUR);
		}
		else{
			dt->hour = increment_value(dt->hour, MAX_HOUR-11, MIN_HOUR);
		}
		break;
		
		case '8':
		if(isMilitary){
			dt->hour = decrement_value(dt->hour, MIN_HOUR, MAX_HOUR);
		}
		else{
			dt->hour = decrement_value(dt->hour, MIN_HOUR, MAX_HOUR-11);
		}
		break;

		case '9':
		dt->minute = increment_value(dt->minute, MAX_MIN, MIN_MIN);
		break;
		
		case 'A':
		dt->minute = decrement_value(dt->minute, MIN_MIN, MAX_MIN);
		break;
		
		case '0':
		dt->second = increment_value(dt->second, MAX_SECOND, MIN_SECOND);
		break;
		
		case 'C':
		dt->second = decrement_value(dt->second, MIN_SECOND, MAX_SECOND);
		break;
	}
}

/*
Prints date and time on the LCD.
The format is based on the military_time setting
(24 hour clock or A.M./P.M.).
*/
void print_dt(const DateTime *dt, int isMilitary)
{
	char buf[17];
	lcd_pos(0,0);
	sprintf(buf, "%04d-%02d-%02d", dt->year, dt->month, dt->day);
	lcd_puts2(buf);

	char buf2[17];
	lcd_pos(1,0);
	if (isMilitary) {
		sprintf(buf2, "%02d:%02d:%02d", dt->hour, dt->minute, dt->second);
		lcd_puts2(buf2);
		} else {
		if (dt->hour < 13) {
			sprintf(buf2, "%02d:%02d:%02d AM", dt->hour, dt->minute, dt->second);
			lcd_puts2(buf2);
			} else {
			sprintf(buf2, "%02d:%02d:%02d PM", dt->hour, dt->minute, dt->second);
			lcd_puts2(buf2);
		}
	}
}

// Checks if key is pressed on the Keypad
//----ALL 8 GPIOs TO N/C---
int is_pressed(int r, int c){

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
char get_key(){
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

int main(){
	avr_init();
	DateTime dt;
	lcd_init();
	init_dt(&dt);
	int military_time = true;
	print_dt(&dt, military_time);
	while(1){
		avr_wait(1000);
		char key = get_key();
		avr_wait(100);
		if(key == '*'){
			while(key != '#') {
				key = get_key();
				update_dt(&dt, key, military_time);
				print_dt(&dt, military_time);
			}
			} else if (key == 'D') {
			military_time = !military_time;
			print_dt(&dt, military_time);
		}
		lcd_clr();
		advance_dt(&dt, military_time);
		print_dt(&dt, military_time);
		avr_wait(1000);
	}
	return 0;
}