bin_PROGRAMS	= xiterm

xiterm_LDADD	= -L$(libdir) @X_LIBS@ -lX11 -lXmu -lXt -lXaw -literm -lXiterm
xiterm_SOURCES	= popup_menu.c popup_menu.h  xiterm.c

CPPFLAGS        = -I$(includedir)
