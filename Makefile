PROG       = mpd-notify
CC         = gcc
PREFIX    ?= /usr/local
BINPREFIX  = ${PREFIX}/bin

LIBS       = -lX11 `pkg-config --cflags --libs libmpdclient libnotify` -DVERSION="\"1.0\"" -DDATE="\"`date -u`\""
CFLAGS    += -Os -Wall

debug: CFLAGS += -O0 -g -pedantic -Wextra
debug: ${PROG}

${PROG}: ${PROG}.c
	@${CC} ${CFLAGS} ${LIBS} ${PROG}.c -o ${PROG}

install:
	install -Dm755 ${PROG} ${DESTDIR}${BINPREFIX}/

uninstall:
	rm -f ${BINPREFIX}/${PROG}

clean:
	rm -f ${PROG}

.PHONY: debug clean install uninstall
