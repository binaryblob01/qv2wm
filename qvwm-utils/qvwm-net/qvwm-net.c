//M.Ali VARDAR 30 October 2003 
//For changes please readme file
//qvwm-net 
//this application based on xmodemlights.c1.0
//10/08/2004 Ben FORD 

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <X11/Xmu/Drawing.h>
#include <X11/extensions/shape.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "xpm/disconnected.xpm"
#include "xpm/connected.xpm"
#include "xpm/litup.xpm"
#include "xpm/lightregions"

/* some handy macros */

#define XSCM(img) XShapeCombineMask(display, window, ShapeBounding, \
	0, 0, img, ShapeSet);

#define XCPFD(xpm,img,shp) XpmCreatePixmapFromData(display, window, \
	xpm, img, shp, (XpmAttributes *) NULL);

#define XCWF XClearWindow(display, window);\
	XFlush(display);
	
#define _DOUBLECLICK_	500
#define _DEFAULTDEV_	"ppp0"
#define _DEFAULTACTION_	"xmessage No Action Configured &"

/* global variables */
Display *				display;
int						screen, depth;
Window					window;
XSetWindowAttributes	winattr;
unsigned long			winmask;
XSizeHints				hints;
char					winname[] = "qvwm-net";
Pixmap					disconnectedshape, connectedshape, disconnected, connected, litup;
XGCValues				xgcvals;
GC						gc;
char *					device_name;
char *					exe;


/* functions */
int pppup();
void pppwait();
void pppmon();
void getsr(long int * r, long int * s);

/******************* main *******************
 *                                          */
int main(int argc, char ** argv)
{
	//XEvent	ev1, ev2;
	XButtonPressedEvent ev1, ev2;
	int i;
	
	display = XOpenDisplay("");
	if(display == NULL)
	{
		fprintf(stderr, "\n%s: %s\n\n", argv[0],"Unable to connect to X Server.");
		exit(1);
	}

	//argument control for devices
	if (argc > 1)
	{
		if (argc % 2 == 1)
		{
			for(i = 1; i < argc; i += 2)
			{
				if (strcmp(argv[i], "--device")==0 || strcmp(argv[i], "-d")==0)
				{
					device_name = argv[i + 1];
				}
				else if(strcmp(argv[i], "--exe")==0 || strcmp(argv[i], "-x")==0)
				{
					/* add the ampersand to make the system call background and return */
					exe = malloc(strlen(argv[i + 1]) + 2);
					sprintf(exe, "%s &", argv[i + 1]);
				}
			}
		}
		else 
		{
			printf("\nUse:\tqvwm-net [{--device|-d} devicename] [{--exe|-x} executable]\n\n");
			printf("\t-d, --device\tNetwork device to be monitored.\n");
			printf("\t-x, --exe\tProgram to be executed when icon is double clicked.\n\n");
			exit(1);
		}
	}
	
	

	if (device_name == NULL)
	{
		device_name = _DEFAULTDEV_;
	}

	if (exe == NULL)
	{
		exe = _DEFAULTACTION_;
	}

	screen = DefaultScreen(display);
	depth = DefaultDepth(display, screen);
	winmask = CWEventMask;
	
	winattr.event_mask = StructureNotifyMask | SubstructureNotifyMask | ResizeRedirectMask ;
	
	window = XCreateWindow(display, RootWindow(display, screen),
		0, 0, 16, 16, 0, depth, InputOutput, CopyFromParent, winmask, &winattr);
	
	XStoreName(display, window, winname);
	
	hints.flags = PSize;
	hints.width = 16;
	hints.height = 16;
	XSetNormalHints(display, window, &hints);
	
	/* create the pixmaps from the xpm data */
	XCPFD(disconnected_xpm, &disconnected, &disconnectedshape);
	XCPFD(connected_xpm, &connected, &connectedshape);
	XCPFD(litup_xpm, &litup, NULL);
	
	xgcvals.graphics_exposures = False;
	gc = XCreateGC(display, window, GCGraphicsExposures, &xgcvals);
	XSelectInput(display, window, ButtonPressMask);
	
	XMapWindow(display, window);
	
	while(1)
	{
		/* shift events */
		ev2 = ev1;
		if(XCheckMaskEvent(display, ButtonPressMask, &ev1))
		{
			if(ev1.time - ev2.time < _DOUBLECLICK_)
				system(exe);
		}
		
		if(pppup())
		{
			pppmon(); /* monitor link 'til it dies */
		}
		else
		{
			pppwait(); /* wait for the link to come up */
		}
	}
}
/*                                          *
 **************** end of main ***************/

int pppup() 
{
	static char str[128];
	static FILE * procnetdev;
	
	procnetdev = fopen("/proc/net/dev", "r");
	while((fgets(str, 128, procnetdev)))
	{
		if (strstr(str, device_name)!=NULL)
			{
				fclose(procnetdev);
				return 1;
			}
	}
	fclose(procnetdev);
	return 0;	
}



/*
 * refresh the display 20 times, then return
 */
void pppwait()
{
	int count;

	XSCM(disconnectedshape);
	XSetWindowBackgroundPixmap(display, window, disconnected);
	XCWF;

		/* refresh the display 20 times, then return */
		XNoOp(display);
		usleep(10000);
	
	return;
}


/*
 * retrieve the number of packets received/sent from /proc/net/dev
 */
void getsr(long int * r, long int * s) {
	static char str[128];
	static char dev[32];
	static FILE * procnetdev;
	long int j1, j2, j3, j4, j5, j6, j7, j8, j9;
		
	procnetdev = fopen("/proc/net/dev", "r");
	while((fgets(str, 128, procnetdev)))
	{
		if(strstr(str, device_name))
			break;
	}
	fclose(procnetdev);
	
	sscanf(str, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
		&dev, r, &j1, &j2, &j3, &j4, &j5, &j6, &j7, s, &j8, &j9);
}


/* 
 * monitor the ppp0 link while it's alive, loop 20 times and return
 */
void pppmon() {
	long int sent, rcvd, lsent, lrcvd;
	int count;
	XEvent event;
	Bool flush = True;

	XSCM(connectedshape);
	XSetWindowBackgroundPixmap(display, window, connected);
	XCWF;
	XClearWindow(display, window);
	
	for(count = 0; count < 20; ++count)
	{
		/* get first send/recv values */
		getsr(&lrcvd, &lsent);
		XNoOp(display);
		usleep(1000);
		/* get second pair for comparison after sleeping */
		getsr(&rcvd, &sent);	
		
		if(sent != lsent) {
			XCopyArea(display, litup, window, gc, send_x, send_y, send_w, send_h, send_x, send_y);
			flush = True;	
		}
		
		if(rcvd != lrcvd) {
			XCopyArea(display, litup, window, gc, receive_x, receive_y, receive_w, receive_h, receive_x, receive_y);
			flush = True;
		}
		
		if(flush)
			XFlush(display);
		usleep(2000);
		
		flush = False;
	}
	return;
}
