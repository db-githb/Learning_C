/*
without using this directive, I recieve the error code:
 error C4996:  'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
https://stackoverflow.com/questions/14386/fopen-deprecated-warning
*/
#pragma warning (disable : 4996)
#include <stdio.h>

/*
Program works like a state machine with different states that perform different tasks depending on the character from the input stream.

copy state - main state that transcribes code to output file, the rest are the exception states
slash state - whenever a slash character comes through the input stream this could mean one of two states, an end of line comment (eol) or a multi-line comment (ml)
eolComment state - if a second slash character immediately follows the first slash character this is an end of line (eol) comment and should not be transcribed
mlComment state - if an asterisk follows the slash character than this is a multiline comment and should not be transcribed until a the end of the multiline appears (second asterisk followed by a slash)
mlEnd state - while in the ml state if an asterisk is found in the multiline comment then it goes to the ml end state where it checks for a slash.  If no slash is found than it returns to the mlComment state.  Otherwise, the multi line comments ends and the program returns to the copy state
quoteCopy state - if quotation marks appear than any slashes or asterisks should be ignored and everything should be transcribed to the output file until a second set of quotation marks appear.
escapeChar state - this is to ensure that any embedded quotes do not end the quote state.  Embedded quotes would be signalled by the escape char \.  Thus if a backslash appoears during the quote state then the program moves to the escapeChar state in which it transcribes the character, immediately following the backslash, to the output file before returning to the quote state.
*/
//State variable used both in main and in functions so definted outside of main as a global variable
enum State {copy, slash, eolComment, mlComment, mlEnd, quoteCopy, escapeChar};

// c compiler requires functions definitions go BEFORE main

enum State copyFunct(char c, FILE* fpWriteIn) {
	// 47 is ASCII for /
	if (c == 47) {
		return slash;
	}

	/*
	Not sure where I am supposed to print my edited source code so I do both:
	fputc - prints to output file
	putc - prints to console
	*/
	// keep the put character function before the quoteCopy if statement so that the quote character is still transcribed
	fputc(c, fpWriteIn);
	putc(c, stdout);

	// 34 is ASCII for "
	if (c == 34) {
		return quoteCopy;
	}
	return copy;
}

enum State slashFunct(char c, FILE* fpWriteIn) {
	
	if (c == 47) {
		return eolComment;
	}

	// 42 is ASCII for *
	if (c == 42) {
		return mlComment;
	}

	fputc('/', fpWriteIn);
	putc('/', stdout);
	fputc(c, fpWriteIn);
	putc(c, stdout);
	return copy;
}

enum State eolFunct(char c, FILE* fpWriteIn) {
	// if statement maintains the line fee or carriage return of the comment (i.e. maintains the intital line the comment/s occupied)
	// ASCII - 10 = Line Feed, 13 = Carriage Return
	if (c == 10 || c == 13) {
		fputc(c, fpWriteIn);
		putc(c, stdout);
		return copy;
	}
	return eolComment;
}

enum State mlFunct(char c, FILE* fpWriteIn) {
	if (c == 42) {
		return mlEnd;
	}
	return mlComment;
}

enum State mlEndFunct(char c, FILE* fpWriteIn) {
	if (c == 47) {
		return copy;
	}
	fputc(c, fpWriteIn);
	putc(c, stdout);
	return mlComment;
}

enum State quoteFunct(char c, FILE* fpWriteIn) {
	
	fputc(c, fpWriteIn);
	putc(c, stdout);

	// 92 is \ (escape character) in Ascii.  This catches the embedded quotes that should be transcribed.
	if (c == 92) {
		return escapeChar;
	}
	
	if (c == 34) {
		return copy;
	}

	return quoteCopy;
}

enum State escapeCharFunct(char c, FILE* fpWriteIn) {
	fputc(c, fpWriteIn);
	putc(c, stdout);
	return quoteCopy;
}

/*
argc is an int representing the number of arguments passed from the command line (space seperated)
argv is a pointer to an array of strings. In C, strings are pointers. Therefore, these strings are pointers to the arguments.
*/
int main(int argc, char* argv[])
{
	char* fileName;

	if (argc == 2) {
		fileName = argv[1];
	}
	else {
		printf("Usage: A5_Q1 [source-file-name]\n");
		return -1;
	}

	// fp = file pointer
	FILE* fpRead;
	FILE* fpWrite;
	
	// fopen() function returns a pointer.  Therefore returns NULL if the system cannot find the file.
	// if statement handles exception errors
	fpRead = fopen(fileName, "r");
	if (fpRead == NULL) {
		printf("Could not open file %s", fileName);

		// uncomment line below to test quote state
		// printf("Could not open file\n // \" aaaa \" */");
		
		return 1;
	}

	fpWrite = fopen("no_comments.txt", "w");
	if (fpWrite == NULL) {
		printf("Could not create file no_comments.txt\n");
		return 1;
	}

	int charC;
	int nextC = 0;

	enum State state = copy;


	while (1) {

		charC = fgetc(fpRead);
		if (feof(fpRead)) {
			break;
		}
		switch (state)
		{
			case copy:
				state = copyFunct(charC, fpWrite);
				break;
			case slash:
				state = slashFunct(charC, fpWrite);
				break;
			case eolComment:
				state = eolFunct(charC, fpWrite);
				break;
			case mlComment:
				state = mlFunct(charC, fpWrite);
				break;
			case mlEnd:
				state = mlEndFunct(charC, fpWrite);
				break;
			case quoteCopy:
				state = quoteFunct(charC, fpWrite);
				break;
			case escapeChar:
				state = escapeCharFunct(charC, fpWrite);
				break;
		}
	}

	fclose(fpWrite);
	return 0;
}