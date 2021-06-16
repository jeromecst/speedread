%option noyywrap

%{
#include "speedread.h"
#include <signal.h>
#include <ncurses.h>
#include <locale.h>
int WPM = 350;
int SPEED = 0;
int pos = -1;
%}

WORD [^ \n\t!\?,:\.0-9]+
NUMBER [^ \n\t!\?,:\.]+
POINT [!\?,:]+

%%

"..."|"[...]"				display(yytext, 1);
({WORD}|{NUMBER})" "?\.{WORD}?		display(yytext, 1); 
({WORD}|{NUMBER})" "?{POINT}{WORD}?	display(yytext, .85); 
{WORD}					display(yytext, 0);
{NUMBER}				display(yytext, .2);
[ \t\n]+
. 					display(yytext, 0);

%%

int main(int argc, char *argv[])
{
	int opt;
	char color[20];
	strcopy("green", color);
	while ((opt = getopt(argc, argv, "w:c:p:h")) != -1) {
		switch (opt) {
			case 'w':
				WPM = atoi(optarg);
				break;
			case 'p':
				pos = atoi(optarg);
				break;
			case 'c':
				strcopy(optarg, color);
				break;
			case 'h':
			default: 
				fprintf(stderr, "Usage: %s [-w wpm] [-c color] [-n num] [-p pos] [-h] file\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	SPEED = 60000/WPM;
	printf("\n");
	if (optind >= argc) 
		yyin = stdin;
	else
		yyin = fopen(argv[optind], "r");
	setlocale(LC_ALL, "");
	initscr(); // window initialization
	curs_set(0);
	if (has_colors()){
		start_color();
		use_default_colors();
		switch_colors(color);
		init_pair(3, COLOR_BLACK, -1);
		init_pair(1, -1, -1);
	}
	cbreak(); // one char at a time input
	noecho(); // do not write char to terminal
	nodelay(stdscr, TRUE); // do not wait for user input
	keypad(stdscr, TRUE); // allow arrow keys
	yylex(); // launch file analyze
	display("", 0);
	extern int count;
	printf("\nWords read: %d\n", count);
	endwin();
}
