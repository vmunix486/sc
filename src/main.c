/*
 * sci interpreter v2
 *
 * The original sci interpreter was written for a more C-style
 * version of the language, but since then, I realized that
 * BASIC-style is a lot easier to interpret than C-style.
*/

#include <stdio.h>
#include <limits.h> /* For LINE_MAX */
#include <stdlib.h> /* For malloc() */
#include <string.h> /* For strlen() */

#define MAX_STRING_LEN 500
#define MAX_VARS       100

/* fake bools in C89 */
#define false 0
#define true  1

struct {
	int inprint, pstringchar, inescapechar, inquotes;
	char pstring[MAX_STRING_LEN];
} funcs;

int main(int argc, char *argv[]) {
	unsigned  int cnum;
	char *line;
	FILE *filecontents;

	if (argc < 2) {
		puts("Usage: sci [script.sc]");
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
		switch (line[cnum]) {
			case 'p':
				if (funcs.inquotes == true) break;

				if (funcs.inprint == false) {
					funcs.inprint = true;

					if (line[cnum+1] == ' ')
						cnum++; /* To skip over the space after the p */

				} else {
					funcs.pstring[funcs.pstringchar] = 'p';
					funcs.pstringchar++;
#ifdef _DEBUG
					puts("Added p to funcs.pstring");
#endif
				}
				break;
			case '\n':
				if (funcs.inescapechar == true)
					funcs.inescapechar = false;
				
				if (funcs.inprint == true) {
					printf("%s", funcs.pstring);
					funcs.inprint = false;
				}

				if (funcs.inquotes == true)
					funcs.inquotes = false;

				break;
			case '\\':
				if (funcs.inprint      == true &&
				    funcs.inescapechar == true) {
					funcs.pstring[funcs.pstringchar] = '\\';
					funcs.pstringchar++;
					funcs.inescapechar = false;
#ifdef _DEBUG
					puts("Added \\ to funcs.pstring");
#endif			
				} else if (funcs.inprint == false &&
					   funcs.inescapechar == false) {
					printf("%d: Random \\ hangoing out.\n", cnum);
					return 1;
				} else if (funcs.inprint == true &&
					   funcs.inescapechar == false) { 
#ifdef _DEBUG
					puts("Turning on funcs.inescapechar");
#endif
					funcs.inescapechar = true;
				} else if (funcs.inquotes == true) {
					break;
				} else {
					printf("%d: Unknown error, to do with backslashes.\n", cnum);
				}
				break;
			case 'n':
				if (funcs.inprint      == true &&
				    funcs.inescapechar == true) {
					funcs.pstring[funcs.pstringchar] = '\n';
					funcs.pstringchar++;
#ifdef _DEBUG
					puts("Added newline to print string");
#endif
				} else if (funcs.inprint == true &&
					   funcs.inescapechar == false) {
					funcs.pstring[funcs.pstringchar] = 'n';
					funcs.pstringchar++;
#ifdef _DEBUG
					puts("Added n to print string");
#endif
				} else if (funcs.inquotes == true) {
					break;
				} else {
					printf("%d: Random n hanging out.\n", cnum);
					return 1;
				}
				break;
			case '#':
				if (funcs.inescapechar == true) {
					funcs.pstring[funcs.pstringchar] = '#';
					funcs.pstringchar++;
#ifdef _DEBUG
					puts("Added # to print string!");
#endif
				} else funcs.inquotes = true;

				break;
			default:
				if (funcs.inprint == true) {
					funcs.pstring[funcs.pstringchar] = line[cnum];
					funcs.pstringchar++;
				} else if (funcs.inescapechar == true) {
					printf("%d: Unknown escape code '\\%c'\n", cnum, line[cnum]);
					return 1;
				} else if (funcs.inquotes == true) {
					break;
				} else {
					printf("%d: Unknown keyword/function %c\n", cnum, line[cnum]);
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
