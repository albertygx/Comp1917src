//Guanxiong Ye z5095134
//tue09-sitar Jade Giacoppo
//12-Mar-2016
//Tiny program that find the longest piece of pi that can be printed by the pi emulator
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "strlib.h"
#include "bigPi.h"

#define READ_SIZE 1500000
#define MAX_VALUE 15
#define MAX_OPERATION 15

int readNextNumber(char* str, int place);

int main(int argc, char *argv[]) {
	FILE *piFile;
	char pi[READ_SIZE] = { };
	char temp[2];
	char finalString[20];
	char currentString[20];
	int currentNumber = 0;
	int nextNumber = 0;
	int difference = 0;
	int operation = 0;
	int place = 0;
	int stringLen = 0;
	piFile = fopen("..\\bigPi\\1Mpi.txt", "r");
	if (piFile == NULL) {
		perror("Error in opening file");
		return 1;
	}
	fread(pi, READ_SIZE, 1, piFile);
	fclose(piFile);
	strcpy(pi, removeChar(pi));
	while (pi[place] != 0) {
		stringLen = place;
		operation = 0;
		currentNumber = 0;
		currentString[0] = 0;
		while (operation < MAX_OPERATION) {
			nextNumber = readNextNumber(pi, stringLen);

			if (nextNumber < currentNumber) {
				difference = nextNumber - currentNumber + MAX_VALUE + 1;
			} else {
				difference = nextNumber - currentNumber;
			}
			switch (difference) {
			case 0:
				break;
			case 1:
			case 2:
			case 4:
			case 8:
				operation++;
				break;
			case 3:
			case 5:
			case 6:
			case 9:
			case 10:
			case 12:
				operation += 2;
				break;
			case 7:
			case 11:
			case 13:
			case 14:
				operation += 3;
				break;
			case 15:
				operation += 4;
				break;
			default:
				printf("That is all!");
				return 0;
			}
			if (operation >= MAX_OPERATION) {
				break;
			} else {
				//empty the temp array
				temp[0] = 0;
				sprintf(temp, "%d", nextNumber);
				strcat(currentString, temp);
				currentNumber = nextNumber;
				operation++;
				if (currentNumber > 9) {
					stringLen += 2;
				} else {
					stringLen++;
				}
			}
		}

		if (strlen(currentString) >= strlen(finalString)) {
			strcpy(finalString, currentString);
			printf("%s\n", finalString);
			printf("From digit%d\n", place);
		}
		place++;
	}
	return 0;
}

int readNextNumber(char* str, int place) {
	int nextNum = str[place] - '0';
	int secDigi = str[place + 1] - '0';
	if (str[place] == '1' && secDigi + 10 <= MAX_VALUE) {
		nextNum = nextNum * 10 + secDigi;
	}
	return nextNum;

}

