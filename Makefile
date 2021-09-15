# speedread - cli speedreader
# See LICENSE file for copyright and license details.

include config.mk

SRC = speedread.c utf8.c lex.yy.c
OBJ = $(SRC:.c=.o)

all: options speedread

options:
	@echo speedread build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

lex.yy.c: speedread.l
	${FLEX} speedread.l lex.yy.c

.c.o:
	${CC} -Wall -c ${CFLAGS} $<

speedread:	lex.yy.c ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: speedread
	install -m 755 speedread ${PREFIX}/bin/speedread

clean:
	rm -f lex.yy.c ${OBJ} speedread

.PHONY: all options speedread install clean
