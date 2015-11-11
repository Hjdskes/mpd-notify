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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpd.h"
#include "notify.h"
#include "util.h"

int
main (int argc, char **argv)
{
	const char *err;
	char *host = NULL;
	unsigned int port = 0;
	int opt;

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

	if (!init_mpd (host, port, &err)) {
		die ("Could not connect to MPD: %s. Exiting\n", err);
	}

	if (!init_notify ()) {
		die ("Could not initiate libnotify. Exiting\n");
	}

	mpd_mainloop ();

	uninit_mpd ();
	uninit_notify ();
	return EXIT_SUCCESS;
}
