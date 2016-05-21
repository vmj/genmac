genmac -- Generate a random MAC address
***************************************

genmac provides a program for producing a random MAC address.  You can
specify, via command line options, what kind of address to generate.

| Homepage: http://linuxbox.fi/~vmj/genmac/
| Download: http://linuxbox.fi/~vmj/genmac/archive/
| Source code: http://github.com/vmj/genmac

.. contents::


Basic usage
===========

The program `genmac(1)`_ produces a single MAC address.  Following
shows a typical invocation::

    $ genmac --local --unicast --eui48

.. _genmac(1): http://www.linuxbox.fi/~vmj/genmac/genmac.1.html


Requirements
============

At runtime, GNU C libraries on Linux.

To compile, GNU Make, AsciiDoc, xmlto, and Docutils is required.  On Debian
based systems, you can install asciidoc, xmlto, and python-docutils packages.
On Slackware, linuxdoc-tools contains the asciidoc and xmlto tools, and
docutils is available from SlackBuilds.org.


Installation
============

Type 'make install' to install the binary into '/usr/local/bin' and
manual page to '/usr/local/man/man1'.

You can also define PREFIX to install genmac in a different prefix:
'make install PREFIX=/usr' for example.  Similarly, there's DESTDIR,
BINDIR, MANDIR, and MAN1DIR for those who need them.


Authors
=======

Original author and current maintainer is Mikko Värri
(mikko@varri.fi).


License
=======

genmac is Free Software, licensed under GNU General Public License
(GPL), version 3 or later.  See LICENSE.txt file for details.
