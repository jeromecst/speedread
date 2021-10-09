#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ncurses.h>
#include <signal.h>
#include "speedread.h"
#include "utf8.h"
#include <pthread.h>
#include <stdatomic.h>
// ʌ ← maybe use this char as well

#define LISTCOLORS " - black\n - blue\n - green\n - cyan\n - red\n - magenta\n - yellow\n - white\n"

unsigned mid[] = {0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
struct Word word_tab[3][SIZE];
extern int WPM;
extern long SPEED;
extern int position_to_go;
extern char color[];
int word_count = 0;
int offset = 3;
int pause_ = 0;
atomic_int draw_screen = 1;
float speed_ratio = 1;

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

void display_word(char *word, unsigned *midword){
	attron(A_BOLD);
	int char_size;
	int tab_size = 0;
	char unicode_char[4];
	for(int i = 0; i < len(word); ){
		char_size = u8_seqlen(&word[i]);
		// create a string containg a unicode character
		for(int k = 0; k < 4; k++){
			if(k < char_size) unicode_char[k] = word[i+k];
			else unicode_char[k] = '\0';
		}
		if(tab_size != *midword) printw("%s", unicode_char);
		else{
			attron(COLOR_PAIR(2));
			printw("%s", unicode_char); 
			attron(COLOR_PAIR(1));
		}
		i += char_size;
		tab_size++;
	}
	printw(" ");
	attroff(A_BOLD);
}

void display_cursor(int h, int w){
	move(h/2 - offset, w/2);
	addch('v' | A_BOLD | COLOR_PAIR(2) );
}

void display_word_tab(int h, int w){
	unsigned midword = word_tab[1]->size > 13 ? 4 : mid[word_tab[1]->size];
	int lim_size = w/2 - word_tab[0]->size - midword - 1;
	move(h/2 - offset + 1, lim_size);
	for(int i = 0; i < 3; i++){
		if(i == 1) display_word(word_tab[i]->w, &midword);
		else{ 
			attron(COLOR_PAIR(3));
			printw("%s ", word_tab[i]->w);
			attron(COLOR_PAIR(1));
		}
	}
}

void display_bottom_text(int h, int w){
	move(h,w);
	printw("WPM: %d | WORDS: %d", WPM, word_count);
	if(pause_) printw(" | PAUSED");
}

void draw(){
	clear();
	int h, w;
	getmaxyx(stdscr, h, w);
	display_cursor(h, w);
	display_word_tab(h, w);
	display_bottom_text(h-1, 1);
}

void read_word(char *word, double time_add){
	word_count++;
	if(position_to_go > -1 && word_count < position_to_go) return;
	unsigned strlen  = u8_strlen(word);
	add_str(word, strlen, time_add*SPEED);
	speed_ratio = ((float)strlen)/10 > 1 ? ((float)strlen)/10 : 1;
	draw_screen = 1;
	msleep((long)((60000/WPM)*speed_ratio + word_tab[1]->time));
	while(pause_){
		msleep(20);
	}
}


void *manage_input(){
	int h;
	while (1){
		msleep(10);
		// h et l pour parcourir le word_tableau (pas encore implémenté)
		// j et k pour changer le wpm
		int ch;
		ch = getch();
		switch (ch){
			case ' ' : pause_ = !pause_; draw_screen = 1; break;
			case 'j' : WPM = WPM > 10 ? WPM - 10 : WPM; draw_screen = 1; break;
			case 'k' : WPM += 10; draw(); break;
			case 'J' : h = getmaxx(stdscr); offset = h/2 - offset < h - 3 ? offset - 1 : offset; draw_screen = 1; break;
			case 'K' : h = getmaxx(stdscr); offset = h/2 - offset > 0 ? offset + 1 : offset; draw_screen = 1; break;
			case 'q' : endwin(); printf("Words read: %d\n", word_count); exit(0);
			default: break;
		}
		if (draw_screen == 1) draw();
		draw_screen = 0;
	}
}

void end_(int sig){
	endwin();
	printf("Restore with speedread -p %d\n", word_count);
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

void add_str(char *str, unsigned str_size, unsigned add){
	char stmp[SIZE];
	char stmp2[SIZE];
	strcopy(word_tab[2]->w,stmp);
	strcopy(str, stmp2);

	int tmpsize = word_tab[2]->size;
	int tmpsize2 = str_size;

	int tmptime = word_tab[2]->time;
	int tmptime2 = add;
	for(int i = 2; i >= 0; i--){
		strcopy(word_tab[i]->w, stmp);
		strcopy(stmp2, word_tab[i]->w);
		strcopy(stmp, stmp2);
		 
		tmpsize = word_tab[i]->size;
		word_tab[i]->size = tmpsize2;
		tmpsize2 = tmpsize;

		tmptime = word_tab[i]->time;
		word_tab[i]->time = tmptime2;
		tmptime2 = tmptime;
	}
}
