mpd-notify: mpd-notify.c
	gcc -o mpd-notify mpd-notify.c `pkg-config --cflags --libs libmpdclient libnotify x11` -DPROGNAME="\"mpd-notify\"" -DVERSION="\"1.0\"" -DDATE="\"`date -u`\""

clean:
	rm mpd-notify
