lib_LTLIBRARIES	= libXiterm.la

libXiterm_la_SOURCES =  Iterm.c Iterm.h ItermP.h VT.c VT.h VTP.h VTScreenView.c VTScreenView.h actions.c actions.h config.h scrollbar.c scrollbar.h xim.c xim.h

CPPFLAGS        = -I$(includedir) -I$(top_srcdir)/include -DBIDI

libXiterm_la_LDFLAGS     = -version-info 1:0:0

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(includedir) ;
	$(INSTALL_DATA) Iterm.h $(DESTDIR)$(includedir) ;
	$(INSTALL_DATA) ItermP.h $(DESTDIR)$(includedir) ;

