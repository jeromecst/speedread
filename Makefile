all: 
	lex speedread.l 
	gcc lex.yy.c utf8.c speedread.c -lncurses -ltinfo -lpthread -o speedread

install: all
	install -m 755 speedread /usr/local/bin/speedread

uninstall:
	rm /usr/local/bin/speedread

clean:
	rm speedread lex.yy.c
