#include "utf8.h"

#define SIZE 100

struct Word {
	char w[SIZE];
	unsigned time;
	unsigned size;
};

int msleep(unsigned long msec);
unsigned size_bytes(char *s);
void display_word(char *s, unsigned *midword);
void clear_lines(unsigned a);
void display_cursor();
void display_tab();
void get_mid_screen();
void display(char *s, double add);
void sig_handler(int signum);
const unsigned long hash(const char *str);
void switch_colors(char *c);
void manage_input();
