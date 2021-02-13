#include <gtk/gtk.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>
#include <locale.h>

#include <iterm/core.h>
#include <iterm/unix/ttyio.h>

#include "gtkiterm.h"
#include "VTScreenView.h"

gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  return FALSE;
}

void destroy(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

void connection_closed(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *iterm;

  setlocale(LC_ALL,"");
  gtk_init(&argc,&argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  iterm = gtk_iterm_new();

  g_signal_connect(G_OBJECT(window),"delete_event",
                   G_CALLBACK(gtk_widget_destroy), G_OBJECT(window));
  g_signal_connect(G_OBJECT(window),"destroy",
                   G_CALLBACK(destroy), NULL);

  g_signal_connect(G_OBJECT(iterm),"connection_closed",
                   G_CALLBACK(connection_closed), NULL);
  gtk_widget_show(iterm);
  gtk_widget_show(window);
  gtk_container_add(GTK_CONTAINER (window), iterm);

  gtk_main();
  return 0;
}
