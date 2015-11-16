/*
 * main.c
 * Copyright (C) 2011-2012 Christian Hesse <mail@eworm.de>
 * Copyright (C) 2013-2015 Jente Hidskes <hjdskes@gmail.com>
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

#define _POSIX_C_SOURCE 200809L /* strndup */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpd.h"
#include "notify.h"
#include "util.h"

static void
version (void)
{
	fprintf (stdout, "mpd-notify version "VERSION"\n");
	exit (EXIT_SUCCESS);
}

static void
usage (void)
{
	fprintf (stdout, "Usage: mpd-notify [-c host] [-p port] [-h] [-v]\n"
			"\t-c host   the server's host name, IP address or Unix socket path\n"
			"\t-p port   the TCP port to connect to, or 0 for the default port. If\n"
			"\t          \"host\" is a Unix socket path, this parameter is ignored\n"
			"\t-h        display this help message and exit\n"
			"\t-v        display version number and exit\n");
	exit (EXIT_SUCCESS);
}

static void
parse_options (int argc, char **argv, char **host, unsigned int *port)
{
	int opt;

	while ((opt = getopt (argc, argv, "c:p:hv")) != -1) {
		switch (opt) {
		case 'c':
			*host = strndup (optarg, strlen (optarg));
			break;
		case 'p':
			{
				char *end;

				*port = strtol (optarg, &end, 10);
				if (*end) {
					free (*host);
					die ("\"port\" requires a valid integer argument, "
							"but got \"%s\" instead\n", optarg);
				}
			}
			break;
		case '?':
			/* Error message is automatically printed. */
			exit (EXIT_FAILURE);
		case 'v':
			version ();
		case 'h': /* Fallthrough. */
		default:
			usage ();
		}
	}
}

int
main (int argc, char **argv)
{
	bool success;
	char *host = NULL;
	unsigned int port = 0;

	parse_options (argc, argv, &host, &port);

	success = init_mpd (host, port);
	free (host);
	if (!success) {
		die ("Could not connect to MPD. Exiting\n");
	}

	success = init_notify ();
	if (!success) {
		die ("Could not initiate libnotify. Exiting\n");
	}

	mpd_mainloop ();

	uninit_mpd ();
	uninit_notify ();
	return EXIT_SUCCESS;
}
