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

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <libnotify/notify.h>

#include "notify.h"
#include "util.h"

void
send_notification (const char *body)
{
	NotifyNotification *notification;
	GError *error = NULL;

	notification = notify_notification_new ("MPD:", body, "sound");
	notify_notification_set_urgency (notification, NOTIFY_URGENCY_NORMAL);

	if (!notify_notification_show (notification, &error)) {
		g_printerr ("Could not show notification: \"%s\"\n", error->message);
		g_clear_error (&error);
	}
}

char *
notification_new (const char *text, ...)
{
	va_list args;
	char *arg;
	size_t size;
	char *notification;

	size = strlen (text) + 1;
	va_start (args, text);
	arg = va_arg (args, char *);
	while (arg) {
		size += strlen (arg);
		arg = va_arg (args, char *);
	}
	va_end (args);

	notification = xmalloc (size);
	va_start (args, text);
	vsnprintf (notification, size, text, args);
	va_end (args);

	return notification;
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

