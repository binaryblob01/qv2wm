#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"



gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_main_quit();
  return FALSE;
}


gboolean
on_window1_destroy_event               (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
gtk_main_quit();
  return FALSE;
}



void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

char uygulama[255];
GtkWidget *edit1;

edit1=lookup_widget(GTK_WIDGET(button), "entry1");
sprintf(uygulama, "%s &", gtk_entry_get_text(GTK_ENTRY(edit1) ));

system(uygulama);

gtk_main_quit();
}


void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
gtk_main_quit();
}

