//M.Ali VARDAR 

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <X11/Xmu/Drawing.h>
#include <X11/extensions/shape.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "xpm/sound.xpm"

#define XSCM(img) XShapeCombineMask(display, window, ShapeBounding, \
	0, 0, img, ShapeSet);

#define XCPFD(xpm,img,shp) XpmCreatePixmapFromData(display, window, \
	xpm, img, shp, (XpmAttributes *) NULL);

#define XCWF XClearWindow(display, window);\
	XFlush(display);
	
#define _DOUBLECLICK_	500


/* global variables */
Display *				display;
int						screen, depth;
Window					window;
XSetWindowAttributes	winattr;
unsigned long			winmask;
XSizeHints				hints;
char					winname[] = "qvwm-sound";
Pixmap					sound, soundshape;
XGCValues				xgcvals;
GC					gc;

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
				if(strcmp(argv[i], "--exe")==0 || strcmp(argv[i], "-x")==0)
				{
					/* add the ampersand to make the system call background and return */
					exe = malloc(strlen(argv[i + 1]) + 2);
					sprintf(exe, "%s &", argv[i + 1]);
				}
			}
		}
		else 
		{
			printf("\nUse:\tqvwm-sound [{--exe|-x} executable]\n\n");
			printf("\t-x, --exe\tProgram to be executed when icon is double clicked.\n\n");
			printf("\tsample :\tqvwm-sound --exe aumix\n\n");			exit(1);
		}
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
	XCPFD(sound_xpm, &sound, &soundshape);
	
	xgcvals.graphics_exposures = False;
	gc = XCreateGC(display, window, GCGraphicsExposures, &xgcvals);
	XSelectInput(display, window, ButtonPressMask);
	
	XMapWindow(display, window);
	
	XSCM(soundshape);
	XSetWindowBackgroundPixmap(display, window, sound);
	XCWF;
	
	while(1)
	{
		/* shift events */
		ev2 = ev1;
		if(XCheckMaskEvent(display, ButtonPressMask, &ev1))
		{
			if(ev1.time - ev2.time < _DOUBLECLICK_)
				system(exe);
		}
		
	XNoOp(display);
	usleep(10000);
		
	}
}
