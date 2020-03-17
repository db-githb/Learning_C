/*
without using this directive, I recieve the error code:
 error C4996:  'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
https://stackoverflow.com/questions/14386/fopen-deprecated-warning
*/
#pragma warning (disable : 4996)
#include <stdio.h>
#include <intrin.h>

/* 
function recieves a pointer to an array of bytes
using pointer arithmetic the outer loop iterates through the address of each byte (unsigned char = byte)
the inner loop uses the bitwise operator to determine the value of each bit in the bitstring of each byte
*/
void printBitString(unsigned char* ptrIn, int length) {

	//due to Windows little endian reverse order to show in IEEE-754 standard  
	for (int i = length - 1; i >= 0; i--) {

		// start at 128 to print bits left to right (8 bits to a byte, 10000000)
		unsigned char bitSelector = 128;

		for (int j = 0; j < 8; j++) {

			// use pointer arithmetic to iterate through the memory location. Byte is the smallest addressable memory. Therefore, I am iterating through the "address" of each byte.
			if ((*(ptrIn + i)) & bitSelector) {
				printf("X");
			}
			else {
				printf("0");
			}

			if (j == 3) {
				printf(" ");
			}
			// bitshift right to move down the byte
			bitSelector = bitSelector >> 1;
		}
		printf(" ");

	}
}

int main()
{
	// prompt for user input
	printf("[F]loat or [D]ouble? ");
	
	// get user input
	char fdInput = getchar();

	// prompt for number from user
	printf("Enter the number: ");
	
	float inputF;
	double inputD;
	
	// if statements store the user submitted number as the data type selected in the first prompt
	if (fdInput == 'F' || fdInput == 'f') {
		scanf("%f", &inputF);
		printf("Number %f in floating point representation is: ", inputF);

		// unsigned char = byte data type
		// pointer gets address of inputF and converts to a pointer for a "byte array"
		unsigned char* bytePtr = (unsigned char*) &inputF;

		printBitString(bytePtr, sizeof(inputF));
	}

	else if (fdInput == 'D' || fdInput == 'd') {
		// unsigned char = byte data type
		// pointer gets address of inputD and converts to a pointer for a "byte array"
		scanf("%lf", &inputD);
		printf("Number %lf in floating point representation is: ", inputD);

		unsigned char* bytePtr = (unsigned char*)& inputD;

		printBitString(bytePtr, sizeof(inputD));
	}
}


