#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "utf8.h"

#define SIZE 100

struct Word {
	char w[SIZE];
	unsigned time;
	unsigned size;
};

int msleep(unsigned long msec);
unsigned len(char *c);
void copy(char *a, char *b);
void add_str(char *s, unsigned n, unsigned add);
unsigned size_bytes(char *s);
unsigned display_word(char *s, unsigned *midword);
void clear_lines(unsigned a);
void display_cursor();
void display_tab();
void get_mid_screen();
void display(char *s, double add);
void sig_handler(int signum);
