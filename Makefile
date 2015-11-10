PROG       = mpd-notify
CC        ?= cc
PREFIX    ?= /usr/local
BINPREFIX  = ${PREFIX}/bin

SOURCES=$(PROG).c
OBJECTS=$(SOURCES:%.c=%.o)

CPPFLAGS   = -DVERSION="\"1.0\"" -DDATE="\"`date -u`\""
LDFLAGS    = `pkg-config --libs libmpdclient libnotify`
CFLAGS    += -std=c11 -g -Wall -Wextra -Werror -Werror-implicit-function-declaration \
	     -Werror=format-security -Wformat -Wformat-security -Wundef -Wstrict-prototypes \
	     -Wno-conversion -Wunreachable-code -fno-common -fstack-protector \
	     `pkg-config --cflags libmpdclient libnotify`

all: options $(PROG)

options:
	@echo $(PROG) options:
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "CPPFLAGS = $(CPPFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "CC       = $(CC)"

%.o: %.c
	@echo CC $<
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(PROG): $(OBJECTS)
	@echo CC -o $@
	@$(CC) -o $@ $^ $(LDFLAGS)

install: $(PROG)
	@echo installing
	@install -Dm755 $(PROG) $(DESTDIR)$(BINPREFIX)/

uninstall:
	@echo uninstalling
	@rm -f $(BINPREFIX)/$(PROG)

clean:
	@echo cleaning
	@rm -f $(PROG) $(OBJECTS)

.PHONY: all options clean install uninstall
