#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdio.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"



gchar *file_name="";
char location[255]="Center";
GtkEntry *entry1;
GtkWidget *image1;		
GtkWidget *optionmenu1;		


void on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
gtk_main_quit();
}

void on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *window2;
  window2= create_window2 ();
  gtk_widget_show (window2);
}

void on_top2_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "Top");
}

void on_bottom2_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "Bottom");
}


void on_center1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "Center");
}



void on_left1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "Left");
}


void on_right1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "Right");
}

void on_topleft1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "TopLeft");
}

void on_topright1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "TopRight");
}


void on_bottomleft1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "BottomLeft");
}


void on_bottomright1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
strcpy(location, "BottomRight");
}

//Main windows Close button
void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
gtk_main_quit();
}

//Save button
void on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	FILE *dosya;
	char satir[255];
	char home[255];
	GtkWidget *dialog1;
	
	strcpy( home, getenv("HOME") );	
	strcat(home,"/.qvwm/background");	
	
	dosya=fopen(home,"w");
	
	strcpy(satir, "[Accessories]\n" );		
	fputs(satir, dosya);
	
	sprintf(satir, "\"%s\"         %s        %s ;", gtk_entry_get_text(entry1),  location, "Background" );
	fputs(satir, dosya);

	fclose(dosya);	
	
	//now qvwm restart thanks Jim
	dosya=fopen("/tmp/qvwm-restart","w");
	fclose(dosya);	
	
  dialog1= create_dialog1 ();
  gtk_widget_show (dialog1);

}

//about window close button window2
void on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window2;  
  window2 = lookup_widget (GTK_WIDGET (button), "window2");
  gtk_widget_destroy(window2);  	
	
  return FALSE;
}

gboolean on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
gtk_main_quit();
	
  return FALSE;
}


gboolean on_window1_destroy_event               (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
gtk_main_quit();	
return FALSE;
}


gboolean on_window2_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  GtkWidget *window2;  
  window2 = lookup_widget (GTK_WIDGET (widget), "window2");
  gtk_widget_destroy(window2);  	
	
  return FALSE;
}


gboolean on_window2_destroy_event               (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  GtkWidget *window2;  
  window2 = lookup_widget (GTK_WIDGET (widget), "window2");
  gtk_widget_destroy(window2);  	
	  
  return FALSE;
}

//file selection dialog 
void on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *fileselection1;    		
  fileselection1 = create_fileselection1 ();
  gtk_widget_show (fileselection1);				
}

void on_fileselection1_close                (GtkDialog       *dialog,
                                        gpointer         user_data)
{	

}


void on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkFileSelection *fileselection1;		
	fileselection1 = lookup_widget (GTK_WIDGET (button), "fileselection1");		
	gtk_widget_destroy(fileselection1);
}

void on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{	
	GtkFileSelection *fileselection1;		
		
	fileselection1 = lookup_widget (GTK_WIDGET (button), "fileselection1");		
	file_name=gtk_file_selection_get_filename(fileselection1);
	
	gtk_widget_destroy(fileselection1); 
	
	gtk_entry_set_text(entry1, file_name);	
}

//window show time
void on_window1_show                        (GtkWidget       *widget,
                                        gpointer         user_data)
{
	FILE *dosya;
	char home[255];
	char satir[255];
	
	char *picture;
	char *location;
	char *backgr;
	
	char *okunan;
		
	strcpy( home, getenv("HOME") );	
	strcat(home,"/.qvwm/background");	
	
	dosya=fopen(home,"r");
	fgets(satir, 255, dosya);			
	fgets(satir, 255, dosya);
	picture=strtok(satir, " ");
	location=strtok(NULL, " ");
	backgr=strtok(NULL, ";");
	fclose(dosya);
	
	entry1 = lookup_widget (GTK_WIDGET (widget), "entry1");		
	
	strcpy(satir, picture);
	picture=strtok(satir, "\"");

	//resim
	image1 = lookup_widget (GTK_WIDGET (widget), "image1");		
	gtk_image_set_from_file(image1, picture);

	image1 = lookup_widget (GTK_WIDGET (widget), "image1");		
	gtk_entry_set_text(entry1, picture);
	
	optionmenu1 = lookup_widget (GTK_WIDGET (widget), "optionmenu1");		

	if (strlen(location)<1)
	{	
	if (strstr(location, "Center"))	gtk_option_menu_set_history(optionmenu1, 0);
	if (strstr(location, "Top"))	gtk_option_menu_set_history(optionmenu1, 1);
	if (strstr(location, "Bottom"))	gtk_option_menu_set_history(optionmenu1, 2);
	if (strstr(location, "Left"))	gtk_option_menu_set_history(optionmenu1, 3);
	if (strstr(location, "Right"))	gtk_option_menu_set_history(optionmenu1, 4);
	if (strstr(location, "TopLeft"))	gtk_option_menu_set_history(optionmenu1, 5);
	if (strstr(location, "TopRight"))	gtk_option_menu_set_history(optionmenu1, 6);
	if (strstr(location, "BottomLeft"))	gtk_option_menu_set_history(optionmenu1, 7);
	if (strstr(location, "BottomRight"))	gtk_option_menu_set_history(optionmenu1, 8);
	}
	
}


void on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *dialog1;		
	dialog1 = lookup_widget (GTK_WIDGET (button), "dialog1");		
	gtk_widget_destroy(dialog1);
}


static void resim_goster(gchar *dosya)
{
	GdkPixbuf *pixbuf, *scale;
	int height, width;
	pixbuf = gdk_pixbuf_new_from_file(gtk_entry_get_text(entry1), NULL);
	if (!pixbuf) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(image1), NULL);
		return;
	}

	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	scale = gdk_pixbuf_scale_simple(pixbuf, width * 200 / height, 200, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image1), scale);
	g_object_unref(G_OBJECT(pixbuf));
	g_object_unref(G_OBJECT(scale));
}

void on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
resim_goster( gtk_entry_get_text(entry1) );
}

