%option noyywrap

%{
#include "speedread.h"
#include <signal.h>
#include <ncurses.h>
#include <locale.h>
int WPM = 350;
int SPEED = 0;
int position_to_go = -1;
%}

WORD [^ \n\t\r!\?,:\.0-9]+
NUMBER [^ \n\t\r!\?:]+
NOSPACE [^\t\r\n]+
ADRESS (https?:\/\/)+{NOSPACE}\.{NOSPACE}
POINT ("... "|[!\?,:])
URL {ADRESS}(({NOSPACE}\/)+({NOSPACE})?)?({POINT})?

%%

"..."\.+				read_word(".....", 1);
{URL}					read_word("<matched_url>", 1);
"..."|"[...]"				read_word(yytext, 1);
({WORD}|{NUMBER})" "?\.{WORD}?		read_word(yytext, 1); 
({WORD}|{NUMBER})" "?{POINT}{WORD}?	read_word(yytext, .85); 
{WORD}					read_word(yytext, 0);
{NUMBER}				read_word(yytext, .2);
[ \t\n]+
. 					read_word(yytext, 0);

%%

int main(int argc, char *argv[])
{
	signal(SIGINT,sig_handler);
	pthread_t thread_input;
	int opt;
	char color[20];
	strcopy("green", color); //default color is green
	while ((opt = getopt(argc, argv, "w:c:p:h")) != -1) {
		switch (opt) {
			case 'w':
				WPM = atoi(optarg);
				break;
			case 'p':
				position_to_go = atoi(optarg);
				break;
			case 'c':
				strcopy(optarg, color);
				break;
			case 'h':
			default: 
				fprintf(stderr, "Usage: %s [-w wpm] [-c color] [-n num] [-p position_to_go] [-h] file\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	SPEED = 60000/WPM;
	if (optind >= argc) {
		// transform sdtin to a file
		FILE *sdtinFile = fdopen(dup(fileno(stdin)), "r");
		yyin = sdtinFile;
		// reopen stdin on the terminal (assuming stdout is still connected to the terminal)
		freopen(ttyname(fileno(stdout)), "r", stdin);
	}
	else yyin = fopen(argv[optind], "r");
	setlocale(LC_ALL, "");
	initscr(); // window initialization 
	curs_set(0); // hide cursor
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
	pthread_create(&thread_input, NULL, manage_input, NULL);
	yylex(); // launch file analyze
	read_word("", 0);
	extern int word_count;
	nodelay(stdscr, FALSE); // do not wait for user input
	getch();
	endwin();
	printf("Words read: %d\n", word_count);
	exit(0);
}
