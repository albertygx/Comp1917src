//Guanxiong Ye z5095134
//tue09-sitar Jade Giacoppo
//Modified on 17-Mar-2016
//This tiny program take 3 number input and sort them ascendingly.
#include <stdio.h>

void swap(int* a, int* b);

int main(int argc, char *argv[]) {
	//comment
	int first = 0;
	int second = 0;
	int third = 0;
	scanf("%d", &first);
	scanf("%d", &second);
	scanf("%d", &third);
	//comment
	if(first>second){
		swap(&first, &second);
	}
	if(first>third){
		swap(&first, &third);
	}
	if(second>third){
		swap(&second, &third);
	}
	printf("%d\n%d\n%d\n",first,second,third);
	return 0;
}
//SWAP!
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
