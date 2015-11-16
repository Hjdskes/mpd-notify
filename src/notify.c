/**
 * notify.c
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

#include <stdbool.h>
#include <libnotify/notify.h>

#include "notify.h"
#include "util.h"

void
send_notification (const char *summary, const char *body)
{
	NotifyNotification *notification;
	GError *error = NULL;

	notification = notify_notification_new (summary, body, "sound");
	notify_notification_set_urgency (notification, NOTIFY_URGENCY_NORMAL);

	if (!notify_notification_show (notification, &error)) {
		warn ("Could not show notification: \"%s\"\n", error->message);
		g_clear_error (&error);
	}
}

bool
init_notify ()
{
	return notify_init ("MPD Notify");
}

void
uninit_notify ()
{
	notify_uninit ();
}

