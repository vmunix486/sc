#include <stdio.h>
#include <limits.h> /* For LINE_MAX */
#include <stdlib.h> /* For malloc() */
#include <string.h> /* For strlen() */

/* The longest a printable string should be is 500 characters.
 * You can change this here, but it should be enough. The only
 * case you'd need to increase this is if you're printing entire
 * bible. 							*/
#define MAX_STRING_LEN 500

/* This struct defines all functions that sc uses. This will grow
 * along with features. */
struct {
	int print;         /* Whether or not it's in a print statement. */
	int inparentheses; /* Whether it's in parenthesis () or not     */
	int inquotes;      /* Whether it's in quotes "" or not          */
	int escapechar;    /* Whether or not there is an initializing backslash for an escape character */
	int quotedchar; 
	char quotedstring[MAX_STRING_LEN];
	/* quotedchar and quotedstring[] both handle how strings work in sc.
	 * 
	 * Whenever it comes along a statement that needs a string, it checks
	 * for quotes "", and that is stored inside of inquotes. If there are
	 * quotes, then it is a string. So every character that is found while
	 * inquotes = 1, gets added to the quotedstring array, which has a
	 * default length of 500 characters. Every time it adds a character to
	 * quotedstring[], it increments quotedchar by 1, so the string
	 *
	 * "Hello, World!"
	 *
	 * gets initialized as a string due to the quotes, then it adds the H to 
	 * quotedstring[0], then quotedchar gets incremented to 1, so the next
	 * character to get added to quotedstring would be e, which gets added to
	 * quotedstring[1], of course.
	 *
	 * And that's how strings work so far in sc. 
	 *
	 * -vmunix 10/3/26
	 */
} funcs;

int main(int argc, char *argv[]) {
	FILE  *filecontents;
	char *line;  char c;
	unsigned int   cnum;
#ifdef _NO_MEMSET
	size_t i;
#endif

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

	/* Opens the file */
	if (fopen(argv[1], "r")) {
		filecontents = fopen(argv[1], "r");
	} else {
		puts("File does not exist.");
		return 1;
	}

	/* Goes through the file one line at a time */
	while (fgets(line, LINE_MAX + 1, filecontents) != NULL) {
	/* Goes through the line one character at a time */
	for (cnum = 0; cnum < strlen(line); cnum++) {
	c = line[cnum];
	switch (c) {
		case 'p': /* print statement */
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
				printf("Length of funcs.quotedstring: %d\n", strlen(funcs.quotedstring));
#endif
				printf("%s", funcs.quotedstring);
#ifdef _NO_MEMSET
				for (i = 0; i < strlen(funcs.quotedstring); i++)
					funcs.quotedstring[i] = 0;
#else
				memset(funcs.quotedstring, 0, strlen(funcs.quotedstring));
#endif
				funcs.quotedchar = 0;
			}
					
			break;
		case ')':
			if (funcs.inquotes == 1) {
				funcs.quotedstring[funcs.quotedchar] = ')';
				funcs.quotedchar++;
				break;
			}

			if (funcs.inparentheses == 1) {
#ifdef _DEBUG
				puts("Not in parenthesis anymore");
#endif
				funcs.inparentheses = 0;
			} else {
				puts("Closing parentheses closing nothing.");
				printf("Line: %s\n", line);
				return 1;
			}

			break;
		case ';':
			if (funcs.inquotes == 1) {
#ifdef _DEBUG
				puts("Added ; to string");
#endif
				funcs.quotedstring[funcs.quotedchar] = ';';
				funcs.quotedchar++;
			} else if (funcs.inquotes == 0 && funcs.inparentheses == 1) {
				puts("Error: Random semicolon in parenthesis.");
				printf("Line: %s\n", line);
			} 

			break;
		case '\\':
			if (funcs.inquotes == 1) {
#ifdef _DEBUG
				puts("Backslash found, enabling escapechar variable");
#endif
				funcs.escapechar = 1;
			} else if (funcs.escapechar == 1) {
#ifdef _DEBUG
				puts("Adding a backslash to the string (\\\\)");
#endif
				funcs.quotedstring[funcs.quotedchar] = '\\';
				funcs.quotedchar++;
			} else {
				puts("Error: There's just a random backslash here:");
				printf("Line: %s\n", line);
				return 1;
			}
			break;
		case 'n':
			if (funcs.escapechar == 1) {
#ifdef _DEBUG
				puts("Newline (\\n) found. Putting in string");
#endif
				funcs.quotedstring[funcs.quotedchar] = '\n';
				funcs.quotedchar++;
				funcs.escapechar = 0;
			} else if (funcs.inquotes == 1) {
#ifdef _DEBUG
				puts("Found an n not for a newline. Adding to string");
#endif
				funcs.quotedstring[funcs.quotedchar] = 'n';
				funcs.quotedchar++;
			} else {
				puts("Error: There's just a random n here:");
				printf("Line: %s\n", line);
				return 1;
			}
			break;
		case ' ':
			if (funcs.inquotes == 1) {
				funcs.quotedstring[funcs.quotedchar] = ' ';
				funcs.quotedchar++;
#ifdef _DEBUG
				puts("Added space to string");
#endif
			}

			break;
		case '\n':
			break;
		default:
			if (funcs.inquotes == 1) {
				if (funcs.escapechar == 1) {
					puts("Error: Unknown escape sequence");
					printf("Line: %s\n", line);
					return 1;
				}
#ifdef _DEBUG
				printf("Adding character to string: %c\n", c);
#endif
				funcs.quotedstring[funcs.quotedchar] = c;
				funcs.quotedchar++;
#ifdef _DEBUG
				printf("String afterwards: %s\n", funcs.quotedstring);
#endif
			} else {
				puts("Error: Unrecognized character.");
				printf("Line: %s\nCharacter: %c\n", line, c);
				return 1;
			}

			break;
		}
	}
	}

	free(line);


	fclose(filecontents);	

	return 0;
}
