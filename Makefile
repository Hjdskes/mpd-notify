PROG     = mpd-notify
PREFIX  ?= /usr/local

mpd-notify: ${PROG}.c
	gcc -o ${PROG} ${PROG}.c `pkg-config --cflags --libs libmpdclient libnotify x11` -DPROGNAME="\"mpd-notify\"" -DVERSION="\"1.0\"" -DDATE="\"`date -u`\""

install:
	install -Dm755 ${PROG} ${DESTDIR}${PREFIX}/bin/${PROG}

uninstall:
	rm ${DESTDIR}${PREFIX}/bin/${PROG}

clean:
	rm mpd-notify
