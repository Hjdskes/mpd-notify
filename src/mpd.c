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
#include <string.h>
#include <glib.h>
#include <mpd/client.h>

#include "mpd.h"
#include "notify.h"
#include "util.h"

#define TEXT_TITLE      "%s"
#define TEXT_ARTIST     "%s - %s"
#define TEXT_PAUSE      "Paused playback"
#define TEXT_STOP       "Stopped playback"
#define TEXT_UNKNOWN    "(unknown)"

static struct mpd_connection *conn;

static void
get_tag (struct mpd_song *song, enum mpd_tag_type tag, char **out)
{
	const char *tmp;

	tmp = mpd_song_get_tag (song, tag, 0);
	if (tmp) {
		*out = g_markup_escape_text (tmp, -1);
		return;
	}
	*out = TEXT_UNKNOWN;
}

static void
get_artist (struct mpd_song *song, char **artist)
{
	get_tag (song, MPD_TAG_ARTIST, artist);
}

static void
get_title (struct mpd_song *song, char **title)
{
	get_tag (song, MPD_TAG_TITLE, title);

	if (strncmp (*title, TEXT_UNKNOWN, strlen (TEXT_UNKNOWN)) == 0) {
		gchar *base;

		free (*title);
		base = g_path_get_basename (mpd_song_get_uri (song));
		*title = g_markup_escape_text (base, -1);
		g_free (base);
	}
}

static void
get_album (struct mpd_song *song, char **album)
{
	get_tag (song, MPD_TAG_ALBUM, album);
}

static void
real_mainloop (struct mpd_connection *conn)
{
	char *summary, *body = NULL, *title, *artist, *album;
	struct mpd_song *song = NULL;
	struct mpd_status *status = NULL;
	enum mpd_state state;

	mpd_command_list_begin (conn, true);
	mpd_send_status (conn);
	mpd_send_current_song (conn);
	mpd_command_list_end (conn);

	status = mpd_recv_status (conn);
	if (!status) {
		warn ("Could not receive status from MPD. Notification won't be sent\n");
		return;
	}

	state = mpd_status_get_state (status);
	mpd_status_free (status);
	switch (state) {
	case (MPD_STATE_PLAY):
		mpd_response_next (conn);
		song = mpd_recv_song (conn);
		get_title (song, &title);
		get_artist (song, &artist);
		get_album (song, &album);
		summary = string_new (TEXT_TITLE, title, NULL);
		body = string_new (TEXT_ARTIST, artist, album, NULL);
		g_free (title);
		g_free (artist);
		g_free (album);
		mpd_song_free (song);
		break;
	case (MPD_STATE_PAUSE):
		summary = string_new (TEXT_PAUSE, NULL);
		break;
	case (MPD_STATE_STOP):
		summary = string_new (TEXT_STOP, NULL);
		break;
	default: /* MPD_STATUS_UNKNOWN */
		summary = string_new (TEXT_ARTIST, TEXT_UNKNOWN, TEXT_UNKNOWN, NULL);
		break;
	}
	mpd_response_finish (conn);

	send_notification (summary, body);
	free (summary);
	free (body);
}

void
mpd_mainloop ()
{
	while (mpd_run_idle_mask (conn, MPD_IDLE_PLAYER)) {
		real_mainloop (conn);
	}
}

bool
init_mpd (const char *host, unsigned int port)
{
	conn = mpd_connection_new (host, port, 30000);
	if (mpd_connection_get_error (conn) != MPD_ERROR_SUCCESS) {
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

