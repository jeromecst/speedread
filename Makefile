# speedread - cli speedreader
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

SRC = speedread.c utf8.c lex.yy.c
OBJ = $(SRC:.c=.o)

all: options lex.yy.c speedread

options:
	@echo speedread build options:
	@echo "CFLAGS  = $(STCFLAGS)"
	@echo "LDFLAGS = $(STLDFLAGS)"
	@echo "CC      = $(CC)"

lex.yy.c:
	${FLEX} speedread.l

.c.o:
	${CC} -c ${CFLAGS} $<

speedread:	${OBJ}
	${CC} -o $@ ${OBJ} ${STLDFLAGS}

clean:
	rm -f lex.yy.c ${OBJ} speedread
