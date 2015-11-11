/**
 * mpd.c
 * Copyright (C) 2015 Jente Hidskes <hjdskes@gmail.com>
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

#include <libgen.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glib.h>
#include <mpd/client.h>

#include "mpd.h"
#include "notify.h"
#include "util.h"

#define TEXT_PLAY       "<b>%s</b>\n - <i>%s</i>"
#define TEXT_PAUSE      "Paused playback"
#define TEXT_STOP       "Stopped playback"
#define TEXT_UNKNOWN    "(unknown)"

static struct mpd_connection *conn;

static void
get_artist (struct mpd_song *song, char **artist)
{
	const char *tmp;

	tmp = mpd_song_get_tag (song, MPD_TAG_ARTIST, 0);
	if (tmp) {
		*artist = g_markup_escape_text (tmp, -1);
		return;
	}
	*artist = TEXT_UNKNOWN;
}

static void
get_title (struct mpd_song *song, char **title)
{
	const char *tmp;

	tmp = mpd_song_get_tag (song, MPD_TAG_TITLE, 0);
	if (tmp) {
		*title = g_markup_escape_text (tmp, -1);
		return;
	}

	gchar *base = g_path_get_basename (mpd_song_get_uri (song));
	*title = g_markup_escape_text (base, -1);
	g_free (base);
}

static void
real_mainloop (struct mpd_connection *conn)
{
	char *notification, *title, *artist;
	struct mpd_song *song = NULL;
	struct mpd_status *status = NULL;
	enum mpd_state state;

	mpd_command_list_begin (conn, true);
	mpd_send_status (conn);
	mpd_send_current_song (conn);
	mpd_command_list_end (conn);

	status = mpd_recv_status (conn);
	if (!status) {
		/* FIXME: read into error code and try to fix it? */
		mpd_connection_free (conn);
		die ("Could not connect to MPD. Exiting\n");
	}

	state = mpd_status_get_state (status);
	mpd_status_free (status);
	switch (state) {
	case (MPD_STATE_PLAY):
		mpd_response_next (conn);
		song = mpd_recv_song (conn);
		get_artist (song, &artist);
		get_title (song, &title);
		notification = notification_new (TEXT_PLAY, artist, title, NULL);
		g_free (artist);
		g_free (title);
		mpd_song_free (song);
		break;
	case (MPD_STATE_PAUSE):
		notification = notification_new (TEXT_PAUSE, NULL);
		break;
	case (MPD_STATE_STOP):
		notification = notification_new (TEXT_STOP, NULL);
		break;
	default: /* MPD_STATUS_UNKNOWN */
		notification = notification_new (TEXT_PLAY, TEXT_UNKNOWN, TEXT_UNKNOWN, NULL);
		break;
	}
	mpd_response_finish (conn);

	send_notification (notification);
	free (notification);
}

void
mpd_mainloop ()
{
	while (mpd_run_idle_mask (conn, MPD_IDLE_PLAYER)) {
		real_mainloop (conn);
	}
}

bool
init_mpd (const char *host, unsigned int port, const char **err)
{
	conn = mpd_connection_new (host, port, 30000);
	if (mpd_connection_get_error (conn) != MPD_ERROR_SUCCESS) {
		*err = mpd_connection_get_error_message (conn);
		mpd_connection_free (conn);
		return false;
	}
	return true;
}

void
uninit_mpd ()
{
	mpd_connection_free (conn);
}

