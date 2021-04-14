%option noyywrap

%{
#include "speedread.h"
int WPM = 350;
int SPEED = 0;
int pos = -1;
%}

WORD [^" ""?"\.,:\n\t0-9]+
NUMBER [^" ""?"\.«»",":\n\t]+
LINK "http"[^ ]+
POINT [!\?",":]

%%

{WORD}				display(yytext, 0);
{NUMBER}			display(yytext, .1);
({WORD}|{NUMBER})" "?\."”"?	display(yytext, 1);
({WORD}|{NUMBER})" "?{POINT} 	display(yytext, .7);
[ \t\n]+
{LINK}
. 				display(yytext, 0);

%%

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_handler);
	printf("\n");
	int opt;
	while ((opt = getopt(argc, argv, "w:c:n:p:")) != -1) {
		switch (opt) {
			case 'w':
				WPM = atoi(optarg);
				break;
			case 'p':
				pos = atoi(optarg);
				break;
			default: /* '?' */
				fprintf(stderr, "Usage: %s [-w wpm] [-c color] [-n num] [-p pos] name\n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	SPEED = 60000/WPM;
	printf("\n");
	if (optind >= argc) 
		yyin = stdin;
	else
		yyin = fopen(argv[optind], "r");
	yylex();
	display("", 0);
	extern int count;
	printf("\nWords read: %d\n", count);
}
