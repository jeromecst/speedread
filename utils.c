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
	if (c <= 0xBF) {
		return 1;
	} else if (c <= 0xCF) {
		return 2;
	} else if (c <= 0xDF) {
		return 3;
	} else if (c <= 0xFC) {
		return 4;
	}
	return -1;
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
