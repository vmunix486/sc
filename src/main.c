#include <stdio.h>
#include <limits.h> /* For LINE_MAX */
#include <stdlib.h> /* For malloc() */
#include <string.h> /* For strlen() */

struct {
	int print;
	int inparentheses;
	int inquotes;
	int quotedchar;
/*	char quotedstring[];*/
	char quotedstring[16384];
} funcs;

int main(int argc, char *argv[]) {
	FILE  *filecontents;
	char *line;  char c;
	unsigned int   cnum;

	if (argc < 2) {
		puts("Usage:");
		puts("\tsci file.sc");
		return 1;
	}

	line = malloc(LINE_MAX + 1);
	if (line == NULL) {
		puts("Could not malloc memory!!!");
		return 1;
	}

	if (fopen(argv[1], "r")) {
		filecontents = fopen(argv[1], "r");
	} else {
		puts("File does not exist.");
		return 1;
	}

	while (fgets(line, LINE_MAX + 1, filecontents) != NULL) {
		for (cnum = 0; cnum < strlen(line); cnum++) {
			c = line[cnum];
			switch (c) {
				case 'p':
					if (funcs.inquotes == 1) {
						funcs.quotedstring[funcs.quotedchar] = 'p';
						funcs.quotedchar++;
					} else {
						funcs.print = 1;
#ifdef _DEBUG
						puts("In print statement");
#endif
						break;
					}
				case '(':
					if (funcs.print != 1) {
						puts("Error:");
						puts("There is no statement that these parentheses are tied to.");
						printf("Line: %s\n", line);
						return 1;
					} else if (funcs.inquotes == 1) {
#ifdef _DEBUG
						puts("Added '(' to string");
#endif
						funcs.quotedstring[funcs.quotedchar] = '(';
						funcs.quotedchar++;
					}

					funcs.inparentheses = 1;
					
#ifdef _DEBUG
					puts("In parentheses");
#endif
					break;
				case '"':
					if (funcs.inparentheses != 1) {
						puts("Error:");
						puts("There are random parenthesis jusing hanging out");
						printf("Line: %s\n", line);
						return 1;
					}
					
					if (funcs.inquotes == 0) {
						funcs.inquotes = 1;
#ifdef _DEBUG
						puts("In quotes");
#endif
					} else if (funcs.inquotes == 1) {
						funcs.inquotes = 0;
#ifdef _DEBUG
						puts("Out of quotes");

						printf("Length of funcs.quotedstring: %d", strlen(funcs.quotedstring));
#endif
						printf("%s", funcs.quotedstring);
					}
					
					break;
				default:
					if (funcs.inquotes == 1) {
#ifdef _DEBUG
						printf("Adding character to string: %c\n", c);
#endif
						funcs.quotedstring[funcs.quotedchar] = c;
						funcs.quotedchar++;
#ifdef _DEBUG
						printf("String afterwards: %s\n", funcs.quotedstring);
#endif
					}

					break;
			}
		}
	}

	free(line);


	fclose(filecontents);	

	return 0;
}
