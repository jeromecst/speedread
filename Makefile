# speedread - cli speedreader
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

SRC = speedread.c utf8.c lex.yy.c
OBJ = $(SRC:.c=.o)

all: options speedread

options:
	@echo speedread build options:
	@echo "CFLAGS  = $(STCFLAGS)"
	@echo "LDFLAGS = $(STLDFLAGS)"
	@echo "CC      = $(CC)"

lex.yy.c:
	${FLEX} speedread.l lex.yy.c

.c.o:
	${CC} -c ${CFLAGS} $<

speedread:	lex.yy.c ${OBJ}
	${CC} -o $@ ${OBJ} ${STLDFLAGS}

install: speedread
	install -m 755 speedread ${PREFIX}/bin/speedread

clean:
	rm -f lex.yy.c ${OBJ} speedread
