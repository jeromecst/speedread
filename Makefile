# speedread - cli speedreader
# See LICENSE file for copyright and license details.

include config.mk

SRC = speedread.c utils.c speedread.l.c
OBJ = $(SRC:.c=.o)

all: options speedread

options:
	@echo speedread build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

%.l.c: %.l
	${FLEX} -o $@ $< $@

.c.o:
	${CC} -Wall -c ${CFLAGS} $<

speedread:	${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: speedread
	install -m 755 speedread ${PREFIX}/bin/speedread

clean:
	rm -f ${OBJ} speedread
