MPD Notify
==========

**MPD Notify** sends you a notification whenever MPD's state changes (play/pause/stop) as well as
when the currently playing song changes. For this to work, it requires a running notification
daemon: it is not a notification daemon by itself.

Usage
-----

MPD Notify supports connecting to remote instances. You can pass the `-c` flag to change the host
and the `-p` flag to change the port:

	-c host	  the server's host name, IP address or Unix socket path.
	-p port	  the TCP port to connect to, or 0 for the default port. If "host" is a Unix socket
    		  path, this parameter is ignored.

Installation
------------

To be able compile and run MPD Notify, you will need to have the following dependencies installed:
* libnotify
* libmpdclient

To build and install, just run the following commands:

	$ make
	# make clean install

Bugs
----

For any bug or request [fill an issue](https://github.com/Unia/mpd-notify/issues) on [GitHub][github].

License
-------

Please see [LICENSE](https://github.com/Unia/mpd-notify/blob/master/LICENSE) on [GitHub][github].

MPD Notify is a fork of [MPD Notification](https://github.com/eworm-de/mpd-notification) by
Christian Hesse.

  [github]: https://github.com/Unia/mpd-notify
