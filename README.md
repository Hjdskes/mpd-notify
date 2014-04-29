MPD Notify
==========

**MPD Notify** notifies you on MPD's state. It will send a notification on song change and on play/pause/stop; it is not a notification daemon by itself.

Usage
-----

MPD Notify supports connecting to remote instances. You can pass the `-c` option to change host and `-p` to change ports:

	-c host	  the server's host name, IP address or Unix socket path.
	-p port	  the TCP port to connect to, 0 for default port. If "host" is a Unix socket path, this parameter is ignored.

Installation
------------

To be able compile and run MPD Notify, you will need to have the following dependencies installed:
* libnotify
* libmpdclient

To build and install, just run the following commands:

	$ make
	# make clean install

License
-------

I have forked the original project by Christian Hesse (mail@eworm.de) and as such, all credit should still be given to him. 
I asked for his permission to upload this and it has been granted.

This project is licensed under the GNU General Public License and should be treated as such. See `COPYING` for more information.
