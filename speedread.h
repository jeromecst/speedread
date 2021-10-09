#ifndef SPEEDREAD
#define SPEEDREAD
#define SIZE 100

struct Word {
	char w[SIZE];
	unsigned time;
	unsigned size;
};

int msleep(unsigned long msec);
unsigned size_bytes(char *s);
void display_word(char *word, unsigned *midword);
void clear_lines(unsigned a);
void display_cursor();
void display_tab();
void get_mid_screen();
void read_word(char *word, double time_add);
void end_(int sig);
const unsigned long hash(const char *str);
void switch_colors(char *c);
void *manage_input();
void add_str(char *str, unsigned str_size, unsigned add);

#endif
