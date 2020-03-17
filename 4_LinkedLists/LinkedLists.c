/*
without using this directive, I recieve the error code:
 error C4996:  'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
https://stackoverflow.com/questions/14386/fopen-deprecated-warning
*/
#pragma warning (disable : 4996)
#include <stdio.h>
#include <intrin.h>

// node is a structure dataype which stores its fields in contiguous memory addresses
typedef struct {
	char data[255];
	struct node* next;
} node;

node* head = NULL;

//ASCII DICTIONARY
node* insert_dictionary_order(char strIn[]) {

	// created a pointer to the memory allocated for a new node datatype
	node* newNode = malloc(sizeof(node));

	// using the pointer operator, I use the newNode pointer (declared and instantiated above) to access the data field of the node structure
	// then I use strcopy to copy the string  from the input stream into my data field
	strcpy(newNode->data, strIn);
	newNode->next = NULL;

	// if statement checks to see if the linked list is empty (when head points to NULL then linked list is empty).
	// if it is empty then it points the head and tail pointers to the newNode being inserted
	if (head == NULL) {
		head = newNode;
	}
	// this program only inserts newNodes at the end of the linked list
	// if the linkedlist is not empty than the program has the tail pointer access the next pointer of the node structure and assign it the newNode pointer
	// with the previous last Node now pointing to the newly inserted node, the program reassigns the tail pointer to point to the new node.
	else {

		node* iterator = head;

		// use previous pointer to keep track of the node previous to the iterator to allow program to insert nodes before the node iterator currently points to
		node* previous = NULL;

		while (iterator != NULL) {
			// strcmp = string compare function returns an int.
			int compareResult = strcmp(strIn, iterator->data);

			// dictionary is in ascending ordering (a->z) therefore IF the newNode string < string from the node pointed to by the iterator THEN place newNode AFTER the current node in linked list
			if (compareResult < 0) {
				
				// if the newNode string is less than the string of the first node than insert newNode at the beginning of the list
				if (previous == NULL) {
					head = newNode;
				}
				// otherwise insert newNode after the node previous is currently point to.
				else {
					previous->next = newNode;
				}
				// regardless of how if statement executes, always point the next pointer of the newNode to the memory address pointed to by the iterator
				newNode->next = iterator;
				break;
			}
			// else-if statement prevents duplicate words from being added to the list and frees the nodes memory
			else if (compareResult == 0) {
				free(newNode);
				break;
			}

			// to iterate through list, the iterator pointer is assigned to the memory address pointed to by the next pointer (next is a field in my node structure)
			// previous pointer is assigned the memory address currently pointed to by the iterator 
			previous = iterator;
 			iterator = iterator->next;
		}
		// if statement handles case when the newNode string > than everything in the list and must be placed at the end of the list.
		if (iterator == NULL) {
			previous->next = newNode;
		}
		
	}

	return newNode;

}

void print_list() {
	
	for (node* iterator = head; iterator != NULL; iterator = iterator->next) {
		printf("%s\n", iterator->data);
	}
}

int main()
{
	char str[255];

	// prompt for user input
	printf("Please enter a string.  Strings are seperated by white-spacs, tab, or newline. \n");
	
	int flag = 1;

	while (flag > 0) {
		// scanf returns the number of items of the argument list successfully read.
		flag = scanf("%s", str);

		int length = strlen(str);

		// terminating dot should be a string array with a single index filled with a '.' therefore statement checks for terminating dot.  When found, statement sets flag to zero to break the loop.
		if (str[0] == '.' && str[1] == NULL) {
			flag = -1;
		}

		// outer flag catches end conditions that are not terminating period
		if (flag > 0) {
			
			// if statement strips any special character from the last entry
			// strings are character arrays so generally any special character should be the last element in the string.  Therefore the the special character is stripped by setting the last element of the array to null.
			// HOWEVER any special character strings will have the last special character stripped
			// therefore if special characters strings are accepted get rid of if statement and leave entries like "down." or "hi!" included (also includes (damian.bowness@gmail.com)
			if ((str[length - 1] >= 33 && str[length - 1] <= 47) || (str[length - 1] >= 58 && str[length - 1] <= 64) || (str[length - 1] >= 91 && str[length - 1] <= 96) || (str[length - 1] >= 123 && str[length - 1] <= 126)) {

				// second nested if statement checks to see if string is a string of special characters by checking if there is a special character in the second to last index
				// if statement left empty because you want to leave the string as is otherwise else statement strips the special character that appears in the last indexo
				//if ((str[length - 2] >= 33 && str[length - 2] <= 47) || (str[length - 2] >= 58 && str[length - 2] <= 64) || (str[length - 2] >= 91 && str[length - 2] <= 96) || (str[length - 2] >= 123 && str[length - 2] <= 126)) {
					 
				//}else{

					str[length - 1] = NULL;
					// if length of the character array is less than 0, then array contained a null string.
					length = length - 1;
					//flag = 0;
				//}
			}

			// prevents null strings from being added
			if (length > 0) {
				insert_dictionary_order(str);
			}
			
		}
		
	}

	print_list();
	
}


