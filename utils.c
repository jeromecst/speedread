#include "utils.h"
#include <errno.h>
#include <pthread.h>

unsigned long hash(const char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash;
}

int u8_seqlen(char *s) {
	unsigned char c = s[0];
	if ((c >> 7) == 0x0) {
		return 1;
	} else if ((c >> 5) == 0x6) {
		return 2;
	} else if ((c >> 4) == 0xE) {
		return 3;
	} else if ((c >> 3) == 0x1E) {
		return 4;
	}
	return -1;
}

unsigned int u8_charlen(char *s) {
	unsigned int len = 0;
	int i = 0;
	while (s[i] != '\0') {
		i += u8_seqlen(&s[i]);
		len++;
	}
	return len;
}

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
