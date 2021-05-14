#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "speedread.h"

#define LISTCOLORS " - black\n - blue\n - green\n - cyan \n - red\n - purple\n - brown \n - gray\n - yellow\n - white\n"

unsigned mid[] = {0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
unsigned mid_screen;
struct Word tab[3][SIZE];
struct winsize w;
extern int WPM;
extern long SPEED;
extern int pos;
extern char color[];
int count = 0;

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
	int a;
	int size = 0;
	char str[4];
	for(int i = 0; i < len(s); ){
		a = u8_seqlen(&s[i]);
		for(int k = 0; k < 4; k++){
			if(k < a) str[k] = s[i+k];
			else str[k] = '\0';
		}
		if(size != *midword) printf("\033[1m%s\033[0m", str);
		else printf("%s%s\033[0m", color, str);
		i += a;
		size++;
	}
	printf(" ");
}

void clear_lines(unsigned a){
	for(int i = 0; i < a; i++)
		printf("\033[1A\033[1000D\033[K");
}

void display_cursor(){
	get_mid_screen();
	for(int i = 0; i < mid_screen; i++) printf(" ");
	printf("%sv\033[0m\n", color);
}

void display_tab(){
	get_mid_screen();
	unsigned midword = tab[1]->size > 13 ? 4 : mid[tab[1]->size];
	int lim = mid_screen - tab[0]->size - midword - 1;
	printf("\033[s");
	if(lim < 0){
		while(lim < 0){
			get_mid_screen();
			lim = mid_screen - tab[0]->size - midword - 1;
		}
		printf("\033[u");
	}
	for(int k = 0; k < lim; k++) printf(" ");
	for(int i = 0; i < 3; i++){
		if(i == 1) display_word(tab[i]->w, &midword);
		else printf("\033[1;30m%s\033[0m ", tab[i]->w);
	}
	printf("\n");
}

void get_mid_screen(){
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	mid_screen = w.ws_col/2;
}

void display(char *s, double add){
	count ++;
	unsigned n = u8_strlen(s);
	add_str(s, n, add*SPEED);
	if(pos > -1 && count < pos) return;
	//printf(", len(%s) = %u, num(%d)",s, n, s[1]); 
	clear_lines(2);
	display_cursor();
	display_tab();
	float n2 = ((float)n)/10 > 1 ? ((float)n)/10 : 1;
	msleep((long)(SPEED*n2 + tab[1]->time));
	printf("\033[KWPM = %d, WORDS: %d", WPM, count);
}

void sig_handler(int signum){
	printf("\nRestore with speedread -p %d\n", count);
	exit(0);
}

void switch_colors(char *c, char *col){
	switch (hash(c)){
		case(210707760194): strcopy("\033[1;30m", col); break; // black
		case(6385084301): strcopy("\033[1;34m", col); break; // blue
		case(210713909846): strcopy("\033[1;32m", col); break; // green
		case(6385133744): strcopy("\033[1;36m", col); break; // cyan
		case(193504576): strcopy("\033[1;31m", col); break; // red
		case(6953915280413): strcopy("\033[1;35m", col); break; // purple
		case(210707991725): strcopy("\033[1;33m", col); break; // brown
		case(6385269880): strcopy("\033[1;37m", col); break; // gray
		case(6954248304353): strcopy("\033[1;33m", col); break; // yellow
		case(210732530054): strcopy("\033[1;37m", col); break; // white
		default: 
				    printf("Unrecognized color\n");
				    printf("Availaibled are\n");
				    printf(LISTCOLORS);
				    exit(1);
	}
}

