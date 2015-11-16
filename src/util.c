/**
 * util.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void *
xmalloc (size_t size)
{
	void *p = NULL;

	p = malloc (size);
	if (!p) {
		die ("Could not allocate %lu bytes. Exiting\n", size);
	}
	return p;
}

char *
string_new (const char *text, ...)
{
	va_list args;
	char *arg;
	size_t size;
	char *string;

	size = strlen (text) + 1;
	va_start (args, text);
	arg = va_arg (args, char *);
	while (arg) {
		size += strlen (arg);
		arg = va_arg (args, char *);
	}
	va_end (args);

	string = xmalloc (size);
	va_start (args, text);
	vsnprintf (string, size, text, args);
	va_end (args);

	return string;
}

void
warn (const char *msg, ...)
{
	va_list ap;

	va_start (ap, msg);
	vfprintf (stderr, msg, ap);
	va_end (ap);
}

void
die (const char *msg, ...)
{
	warn (msg);
	exit (EXIT_FAILURE);
}

