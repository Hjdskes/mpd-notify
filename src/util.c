/**
 * utils.c
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

void
die (const char *msg, ...)
{
	va_list ap;

	va_start (ap, msg);
	vfprintf (stderr, msg, ap);
	va_end (ap);

	exit (EXIT_FAILURE);
}

