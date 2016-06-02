/*
 *  doomsday.c
 *  A program to test a function dayOfWeek which determines which
 *  day of the week a particular date falls on.
 *  (only for dates after the start of the Gregorian calendar).
 *
 *  Program stub created by Richard Buckland on 17/03/14
 *  This program by Guanxiong Ye
 *  Freely licensed under Creative Commons CC-BY-3.0
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define THURSDAY 0
#define FRIDAY   1
#define SATURDAY 2
#define SUNDAY   3
#define MONDAY   4
#define TUESDAY  5
#define WEDNESDAY 6

#define TRUE 1
#define FALSE 0
#define DAYS_PER_WEEK 7

int dayOfWeek(int doomsday, int leapYear, int month, int day);

int main(int argc, char *argv[]) {
	assert(dayOfWeek (THURSDAY, FALSE, 4, 4) == THURSDAY);
	assert(dayOfWeek (FRIDAY, FALSE, 6, 6) == FRIDAY);
	assert(dayOfWeek (MONDAY, FALSE, 8, 8) == MONDAY);
	assert(dayOfWeek (WEDNESDAY, FALSE, 10, 10) == WEDNESDAY);
	assert(dayOfWeek (FRIDAY, FALSE, 12, 12) == FRIDAY);
	assert(dayOfWeek (THURSDAY, FALSE, 9, 5) == THURSDAY);
	assert(dayOfWeek (FRIDAY, FALSE, 5, 9) == FRIDAY);
	assert(dayOfWeek (SUNDAY, FALSE, 7, 11) == SUNDAY);
	assert(dayOfWeek (TUESDAY, FALSE, 11, 7) == TUESDAY);
	assert(dayOfWeek (WEDNESDAY, FALSE, 3, 7) == WEDNESDAY);

	assert(dayOfWeek (THURSDAY, FALSE, 4, 5) == FRIDAY);
	assert(dayOfWeek (SATURDAY, FALSE, 6, 5) == FRIDAY);
	assert(dayOfWeek (MONDAY, FALSE, 8, 9) == TUESDAY);
	assert(dayOfWeek (WEDNESDAY, FALSE, 10, 9) == TUESDAY);
	assert(dayOfWeek (FRIDAY, FALSE, 12, 20) == SATURDAY);
	assert(dayOfWeek (THURSDAY, FALSE, 9, 9) == MONDAY);
	assert(dayOfWeek (FRIDAY, FALSE, 5, 5) == MONDAY);
	assert(dayOfWeek (SUNDAY, FALSE, 7, 7) == WEDNESDAY);
	assert(dayOfWeek (TUESDAY, FALSE, 11, 11) == SATURDAY);
	assert(dayOfWeek (THURSDAY, FALSE, 3, 30) == SATURDAY);

	assert(dayOfWeek (TUESDAY, FALSE, 2, 28) == TUESDAY);
	assert(dayOfWeek (TUESDAY, FALSE, 2, 27) == MONDAY);
	assert(dayOfWeek (THURSDAY, FALSE, 1, 3) == THURSDAY);

	printf("all tests passed - You are awesome!\n");
	return EXIT_SUCCESS;
}

// given the doomsday for a year, and whether or not it is a
// leap year, this function return the day of the week for any
// given month and day in the year.

int dayOfWeek(int doomsday, int leapYear, int month, int day) {
	int dayOfWeek = 0;
	// one of the doomsday in the month.
	int doomsDayDate = 0;
	if (month == 4 || month == 6 || month == 8 || month == 10 || month == 12) {
		//these months have a rather special doomsday that
		//the number representing dates and the months are the same.
		//for example 4/4, 6/6.
		doomsDayDate = month;
	} else if (month == 3 || month == 11) {
		//7/3 and 7/11 are doomsday.
		doomsDayDate = 7;
	} else if (month == 5) {
		//9/5 is doomsday.
		doomsDayDate = 9;
	} else if (month == 7) {
		//11/7 is doomsday.
		doomsDayDate = 11;
	} else if (month == 9) {
		//5/9 is doomsday.
		doomsDayDate = 5;
	} else if (month == 1 || month == 2) {
		if (leapYear == TRUE) {
			if (month == 1) {
				//4/1 is doomsday in leap year.
				doomsDayDate = 4;
			} else if (month == 2) {
				//29/2 is doomsday.
				doomsDayDate = 29;
			}
		} else if (leapYear == FALSE) {
			if (month == 1) {
				//3/1 is doomsday in non-leap year.
				doomsDayDate = 3;
			} else if (month == 2) {
				//28/2 is doomsday in non-leap year.
				doomsDayDate = 28;
			}
		}
	}
	//The location of given date in Days period.
	//If the given is in next period, minus DAYS_PER_WEEK to get dayOfWeek.
	//If the result is in the period before current one, plus to get dayOfWeek.
	dayOfWeek = doomsday + ( day - doomsDayDate ) % DAYS_PER_WEEK;
	if(dayOfWeek>6){
		dayOfWeek -= DAYS_PER_WEEK;
	}else if(dayOfWeek<0){
		dayOfWeek += DAYS_PER_WEEK;
	}
	return (dayOfWeek);
}
