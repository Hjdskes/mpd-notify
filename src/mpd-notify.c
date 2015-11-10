/*
 * mpd-notify.c
 * Copyright (C) 2011-2012 Christian Hesse <mail@eworm.de>
 * Copyright (C) 2013-2014 Jente Hidskes <hjdskes@gmail.com>
 *
 * MPD Notify is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPD Notify is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _POSIX_C_SOURCE 200809L /* strdup */

#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpd/client.h>
#include <libnotify/notify.h>

#include "util.h"

#define TEXT_PLAY       "<b>%s</b>\n - <i>%s</i>"
#define TEXT_PAUSE      "Paused playback"
#define TEXT_STOP       "Stopped playback"
#define TEXT_UNKNOWN    "(unknown)"

int
main(int argc, char **argv) {
	NotifyNotification *netlink = NULL;
	struct mpd_connection *conn = NULL;
	struct mpd_status *status = NULL;
	struct mpd_song *song = NULL;
	const char *temp;
	char *host = NULL, *notification, *title, *artist, *copy;
	int opt, errcount, port = 0, status_type;
	size_t size;
	GError *error = NULL;

	while((opt = getopt(argc, argv, "c:p:h")) != -1) {
		switch(opt) {
			case 'c':
				host = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case '?': /* error message is automatically printed */
				exit(EXIT_FAILURE);
			case 'h': /* fallthrough */
			default:
				fprintf (stdout, "Usage: %s [-c host] [-p port] [-h]\n", argv[0]);
				exit (EXIT_SUCCESS);
		}
	}

	conn = mpd_connection_new(host, port, 30000);
	if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		const char *err = mpd_connection_get_error_message(conn);
		mpd_connection_free(conn);
		die ("Could not connect to MPD: %s. Exiting\n", err);
	}

	if(notify_init("MPD Notify") == FALSE) {
		die ("Could not create notification. Exiting\n");
	}

	netlink = notify_notification_new("MPD Notify", NULL, "sound");

	while(mpd_run_idle_mask(conn, MPD_IDLE_PLAYER) != 0) {
		mpd_command_list_begin(conn, true);
		mpd_send_status(conn);
		mpd_send_current_song(conn);
		mpd_command_list_end(conn);

		status = mpd_recv_status(conn);
		if(status == FALSE) {
			mpd_connection_free(conn);
			die ("Could not connect to MPD. Exiting\n");
		} else {
			status_type = mpd_status_get_state(status);
			switch(status_type) {
				case(MPD_STATE_PLAY):
					mpd_response_next(conn);
					song = mpd_recv_song(conn);
					temp = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
					if(!temp) {
						temp = mpd_song_get_uri(song);
						copy = strdup(temp);
						temp = basename(copy);
						free(copy);
					}
					title = g_markup_escape_text(temp, -1);
					temp = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
					if(!temp)
						temp = TEXT_UNKNOWN;
					artist = g_markup_escape_text(temp, -1);
					size = strlen(TEXT_PLAY) + strlen(title) + strlen(artist);
					notification = xmalloc (size);
					snprintf(notification, size, TEXT_PLAY, title, artist);
					g_free(title);
					g_free(artist);
					mpd_song_free(song);
					break;
				case(MPD_STATE_PAUSE):
					size = strlen(TEXT_PAUSE) + 1; /* null-byte */
					notification = xmalloc (size);
					snprintf(notification, size, TEXT_PAUSE);
					break;
				case(MPD_STATE_STOP):
					size = strlen(TEXT_STOP) + 1; /* null-byte */
					notification = xmalloc (size);
					snprintf(notification, size, TEXT_STOP);
					break;
				default: /* MPD_STATUS_UNKNOWN */
					size = strlen(TEXT_UNKNOWN) * 2 + 4;
					notification = xmalloc (size);
					snprintf(notification, size + 1, "%s - %s", TEXT_UNKNOWN, TEXT_UNKNOWN);
					break;
			}
		}

		mpd_status_free(status);

		notify_notification_update(netlink, "MPD:", notification, "sound");
		notify_notification_set_urgency(netlink, NOTIFY_URGENCY_NORMAL);

		while(notify_notification_show(netlink, &error) == FALSE) {
			if(errcount > 1) {
				die ("Could not reconnect to the notification daemon. Exiting\n");
			} else {
				g_printerr("%s: error \"%s\" while trying to show notification. Trying to reconnect.\n", argv[0], error->message);
				errcount++;

				g_error_free(error);
				error = NULL;
				notify_uninit();
				sleep(1);

				if(notify_init("MPD Notify") == FALSE) {
					die ("Could not create notification. Exiting\n");
				}
			}
		}
		errcount = 0;
		free(notification);
		mpd_response_finish(conn);
	}

	mpd_connection_free(conn);
	notify_uninit();
	return EXIT_SUCCESS;
}
