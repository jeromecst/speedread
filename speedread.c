#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include "speedread.h"

#define LISTCOLORS " - black\n - blue\n - green\n - cyan\n - red\n - magenta\n - yellow\n - white\n"

unsigned mid[] = {0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
struct Word tab[3][SIZE];
extern int WPM;
extern long SPEED;
extern int pos;
extern char color[];
int count = 0;
int offset = 3;
int pause_ = 0;
int skip = 0;

int msleep(unsigned long msec) {
	struct timespec ts;
	int res;

	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;

	do {
		res = nanosleep(&ts, &ts);
	} while (res && errno == EINTR);

	return res;
}

void add_str(char *s, unsigned n, unsigned add){
	char stmp[SIZE];
	char stmp2[SIZE];
	strcopy(tab[2]->w,stmp);
	strcopy(s, stmp2);

	int tmpsize = tab[2]->size;
	int tmpsize2 = n;

	int tmptime = tab[2]->time;
	int tmptime2 = add;
	for(int i = 2; i >= 0; i--){
		strcopy(tab[i]->w, stmp);
		strcopy(stmp2, tab[i]->w);
		strcopy(stmp, stmp2);
		 
		tmpsize = tab[i]->size;
		tab[i]->size = tmpsize2;
		tmpsize2 = tmpsize;

		tmptime = tab[i]->time;
		tab[i]->time = tmptime2;
		tmptime2 = tmptime;
	}
}

void display_word(char *s, unsigned *midword){
	attron(A_BOLD);
	int a;
	int size = 0;
	char str[4];
	for(int i = 0; i < len(s); ){
		a = u8_seqlen(&s[i]);
		for(int k = 0; k < 4; k++){
			if(k < a) str[k] = s[i+k];
			else str[k] = '\0';
		}
		if(size != *midword) printw("%s", str);
		else{
			attron(COLOR_PAIR(2));
			printw("%s", str); 
			attron(COLOR_PAIR(1));
		}
		i += a;
		size++;
	}
	printw(" ");
	attroff(A_BOLD);
}

void display_cursor(){
	int h, w;
	getmaxyx(stdscr, h, w);
	move(h/2 - offset, w/2);
	addch('v' | A_BOLD | COLOR_PAIR(2) );
}

void display_tab(){
	int h, w;
	getmaxyx(stdscr, h, w);
	unsigned midword = tab[1]->size > 13 ? 4 : mid[tab[1]->size];
	int lim = w/2 - tab[0]->size - midword - 1;
	if(lim < 0){
		while(lim < 0){
			msleep(20);
			clear();
			move(h/2 - offset + 1, 0);
			printw("Terminal is too narrow");
			refresh();
			getmaxyx(stdscr, h, w);
			lim = w/2 - tab[0]->size - midword - 1;
		}
	}
	move(h/2 - offset + 1, lim);
	for(int i = 0; i < 3; i++){
		if(i == 1) display_word(tab[i]->w, &midword);
		else{ 
			attron(COLOR_PAIR(3));
			printw("%s ", tab[i]->w);
			attron(COLOR_PAIR(1));
		}
	}
}

void redraw(){
	int h, w;
	clear();
	display_cursor();
	display_tab();
	getmaxyx(stdscr, h, w);
	move(h-1,1);
	attron(A_NORMAL);
	if(pause_)
		printw("WPM: %d | WORDS: %d | PAUSED", WPM, count);
	else
		printw("WPM: %d | WORDS: %d", WPM, count);
}

void manage_input(){
	int h, w;
	skip = 0;
	do{
		getmaxyx(stdscr, h, w);
		// h et l pour parcourir le tableau
		// j et k pour changer le wpm
		int ch;
		ch = getch();
		switch (ch){
			case ' ' : pause_ = !pause_; break;
			case 'j' : WPM = WPM > 10 ? WPM - 10 : WPM ; break;
			case 'k' : WPM += 10; break;
			case 'J' : offset = h/2 - offset < h - 3 ? offset - 1 : offset; break;
			case 'K' : offset = h/2 - offset > 0 ? offset + 1 : offset ; break;
			case 'q' : endwin(); exit(0);
			case 'l' : skip = 1; return;
				   // le skip ne fonctionnera pas hors pause
				   // tant que manage input ne sera pas sur un
				   // autre thread qui pourra tuer le sleep dès
				   // qu'il y a un input pressé
			default: break;
		}
		if(pause_){ 
			redraw();
			refresh();
			msleep(20);
		}
	} while(pause_);
	redraw();
	refresh();
}

void display(char *s, double add){
	count ++;
	if(pos > -1 && count < pos) return;
	unsigned n = u8_strlen(s);
	add_str(s, n, add*SPEED);
	float n2 = ((float)n)/10 > 1 ? ((float)n)/10 : 1;
	redraw();
	SPEED = 60000/WPM;
	refresh();
	manage_input();	
	msleep((1-skip)*(long)(SPEED*n2 + tab[1]->time));
	manage_input();	
}

void sig_handler(int signum){
	endwin();
	printf("\nRestore with speedread -p %d\n", count);
	exit(0);
}

void switch_colors(char *c){
	switch (hash(c)){
		case(210707760194): init_pair(2, COLOR_BLACK, -1); break; // black
		case(6385084301): init_pair(2, COLOR_BLUE, -1); break; // blue
		case(210713909846): init_pair(2, COLOR_GREEN, -1); break; // green
		case(6385133744): init_pair(2, COLOR_CYAN, -1); break; // cyan
		case(193504576): init_pair(2, COLOR_RED, -1); break; // red
		case(6953915280413): init_pair(2, COLOR_MAGENTA, -1); break; // magenta
		case(6954248304353): init_pair(2, COLOR_YELLOW, -1); break; // yellow
		case(210732530054): init_pair(2, COLOR_WHITE, -1); break; // white
		default: 
				    endwin();
				    printf("unrecognized color\n");
				    printf("availaibled are\n");
				    printf(LISTCOLORS);
				    exit(1);
	}
}
