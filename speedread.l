%option noyywrap

%{
#include "speedread.h"
#include <signal.h>
int WPM = 350;
int SPEED = 0;
int pos = -1;
char color[] = "\033[1;32m";
%}

WORD [^ \n\t!\?,:\.0-9]+
NUMBER [^ \n\t!\?,:\.]+
POINT [!\?,:]+

%%

({WORD}|{NUMBER})" "?\.{WORD}?		display(yytext, 1); 
({WORD}|{NUMBER})" "?{POINT}{WORD}?	display(yytext, .85); 
{WORD}					display(yytext, 0);
{NUMBER}				display(yytext, .2);
[ \t\n]+
. 					display(yytext, 0);

%%

int main(int argc, char *argv[])
{
	//printf("%lu\n\n", hash("white"));
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
			case 'c':
				switch_colors(optarg, color);
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
