#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdio.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"



gchar *file_name="";
GtkEntry *entry1;
GtkEntry *entry2;
GtkEntry *entry3;


//create button
void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	FILE *dosya;
	char satir[255];
	char home[255];
	
	strcpy( home, getenv("HOME") );	
	strcat(home,"/.qvwm/desktop");	
	
	dosya=fopen(home,"a+");
	
	sprintf(satir, "\n\"%s\"     \"%s\"    \"%s\" 600,600 ", gtk_entry_get_text(entry1),gtk_entry_get_text(entry2), gtk_entry_get_text(entry3)  );
	fputs(satir, dosya);

	fclose(dosya);
	
	//now qvwm restart thanks Jim
	dosya=fopen("/tmp/qvwm-restart","w");
	fclose(dosya);		

	gtk_main_quit();	  
}




//cikis butonu
void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
gtk_main_quit();
}



void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	
	GtkWidget *fileselection1;    		
  	fileselection1 = create_fileselection1 ();
  	gtk_widget_show (fileselection1);			
	
}


void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *fileselection2;    		
  	fileselection2 = create_fileselection2 ();
  	gtk_widget_show (fileselection2);			
}



void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkFileSelection *fileselection1;		
		
	fileselection1 = lookup_widget (GTK_WIDGET (button), "fileselection1");		
	file_name=gtk_file_selection_get_filename(fileselection1);
	gtk_widget_destroy(fileselection1); 
	
	gtk_entry_set_text(entry2, file_name);				
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkFileSelection *fileselection1;		
	fileselection1 = lookup_widget (GTK_WIDGET (button), "fileselection1");		
	gtk_widget_destroy(fileselection1);
}


gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
gtk_main_quit();
  return FALSE;
}


gboolean
on_fileselection1_delete_event         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


gboolean
on_fileselection1_destroy_event        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

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
on_window1_show                        (GtkWidget       *widget,
                                        gpointer         user_data)
{
	entry1 = lookup_widget (GTK_WIDGET (widget), "entry1");		
	entry2 = lookup_widget (GTK_WIDGET (widget), "entry2");		
	entry3 = lookup_widget (GTK_WIDGET (widget), "entry3");		
	
}

gboolean
on_fileselection2_delete_event         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


gboolean
on_fileselection2_destroy_event        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}

void
on_ok_button2_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkFileSelection *fileselection2;		
		
	fileselection2 = lookup_widget (GTK_WIDGET (button), "fileselection2");		
	file_name=gtk_file_selection_get_filename(fileselection2);
	gtk_widget_destroy(fileselection2); 
	gtk_entry_set_text(entry3, file_name);				
}


void
on_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkFileSelection *fileselection2;		
	fileselection2 = lookup_widget (GTK_WIDGET (button), "fileselection2");		
	gtk_widget_destroy(fileselection2);

}
