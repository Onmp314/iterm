#  This software is subject to the terms of the Common Public License
#  You must accept the terms of this license to use this software.
#
#  Copyright (C) 2002, International Business Machines Corporation
#  and others.  All Rights Reserved.
#
#  Further information about Common Public License Version 0.5 is obtained 
#  from url http://oss.software.ibm.com/developer/opensource/license-cpl.html

bin_PROGRAMS = fbiterm

fbiterm_SOURCES = \
	fbiterm.c fb.c fb_true8.c fb_true16.c fb_true24.c fb_true32.c \
	font.c font_stubs.c input.c term.c util.c \
	vt.c VTScreenView.c fbiterm.h palette.h VTScreenView.h

fbiterm_LDADD = \
	-L$(libdir) @X_LIBS@ -lXfont -literm

CFLAGS = -I$(includedir) @X_CFLAGS@ -I$(top_srcdir)/include -I/usr/local/include $(EXTRA_CFLAGS)

install-exec-hook:
	chmod u+s $(DESTDIR)$(bindir)/fbiterm
