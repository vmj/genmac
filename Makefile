DESTDIR ?=
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/man
MAN1DIR ?= $(MANDIR)/man1

CC ?= gcc
CFLAGS ?= -std=gnu99
LDFLAGS ?=

NAME=genmac

ALL=$(NAME) $(NAME).1 $(NAME).1.html index.html

.PHONY: all clean distclean dist install dist-internal

all: $(ALL)

clean:
	-@rm *~ $(NAME).o $(NAME).1.xml 2>/dev/null || true

distclean: clean
	-@rm $(ALL) $(NAME)-*.tar.gz 2>/dev/null || true

dist:
	@$(MAKE) -s dist-internal VERSION=`grep _version $(NAME).c |sed -e 's/.* "//' -e 's/";//'`

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp $(NAME) $(DESTDIR)$(BINDIR)/
	mkdir -p $(DESTDIR)$(MAN1DIR)
	cp $(NAME).1 $(DESTDIR)$(MAN1DIR)/

dist-internal:
	-@rm -rf $(NAME)-$(VERSION) $(NAME)-$(VERSION).tar.gz 2>/dev/null || true
	@mkdir $(NAME)-$(VERSION)
	@cp -r LICENSE.txt Makefile $(NAME).c $(NAME).1.txt README.rst $(NAME)-$(VERSION)/
	@tar czf $(NAME)-$(VERSION).tar.gz $(NAME)-$(VERSION)
	@rm -rf $(NAME)-$(VERSION)


# Binary

$(NAME).o: $(NAME).c
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(NAME).o
	$(CC) $(LDFLAGS) -o $@ $<

# Manual page

$(NAME).1.xml: $(NAME).1.txt
	asciidoc -b docbook -d manpage -o $@ $<

$(NAME).1: $(NAME).1.xml
	xmlto man $<

# HTML Documentation

index.html: README.rst
	rst2html.py $< $@

$(NAME).1.html: $(NAME).1.txt
	asciidoc -b html -d manpage -o $@ $<
