/*
 * main.h
 *
 * Copyright (C) 1995-2000 Kenichi Kourai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with qvwm; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _MAIN_H_
#define _MAIN_H_

class Qvwm;
class Rect;
class Event;
class FocusMgr;
class Desktop;
class QvImage;
#ifdef USE_XSMP
class Session;
#endif
#ifdef USE_SS
class ScreenSaver;
#endif

extern char*		displayName;
extern Display*		display;
extern int		screen;
extern unsigned int	depth;
extern Colormap		colormap;
extern Window		root;
extern Qvwm*		rootQvwm;
extern Rect		rcScreen;
extern GC		gc, gcXor, gcTile, gcDash;
extern QvImage		*imgLogo, *imgLargeLogo;
extern Cursor		cursor[9];
extern XFontSet		fsDefault;
extern XFontSet		fsTitle, fsTaskbar, fsBoldTaskbar, fsIcon;
extern XFontSet		fsCtrlMenu, fsCascadeMenu, fsStartMenu, fsDialog;
extern XColor		gray, darkGray, lightGray, grey, blue, lightBlue;
extern XColor		black, white, green, yellow;
extern Bool		shapeSupport;
extern char**           qvArgv;
extern Bool		start;
extern Bool restart;
extern Bool noParse;
extern Event event;
extern FocusMgr focusMgr;
extern Desktop desktop;
#ifdef USE_XSMP
extern Session* session;
#endif
#ifdef USE_SS
extern ScreenSaver* scrSaver;
#endif

extern unsigned long* gradPattern;
extern unsigned long* gradActivePattern;

extern Atom _XA_WM_CHANGE_STATE;
extern Atom _XA_WM_STATE;
extern Atom _XA_WM_COLORMAP_WINDOWS;
extern Atom _XA_WM_PROTOCOLS;
extern Atom _XA_WM_TAKE_FOCUS;
extern Atom _XA_WM_DELETE_WINDOW;
extern Atom _XA_WM_DESKTOP;
extern Atom _XA_WM_ICON_SIZE;
#ifdef USE_XSMP
extern Atom _XA_WM_CLIENT_LEADER;
extern Atom _XA_SM_CLIENT_ID;
extern Atom _XA_WM_WINDOW_ROLE;
#endif

class Taskbar;
class StartMenu;
class TaskSwitcher;
class Paging;
class Pager;
class Menu;
class ExitDialog;
class ConfirmDialog;
class Timer;
class InfoDisplay;

extern Taskbar*		taskBar;
extern StartMenu*	startMenu;
extern TaskSwitcher*	taskSwitcher;
extern Paging*		paging;
extern Pager*		pager;
extern Menu*		ctrlMenu;
extern ExitDialog*	exitDlg;
extern ConfirmDialog*	confirmDlg;
extern Timer*           timer;
extern InfoDisplay*     infoDisp;

#ifdef HAVE_VPRINTF
extern void QvwmError(const char* fmt, ...);
#else
#define QvwmError(fmt, args...) \
	printf("qvwm: "), printf(fmt, ## args), printf("\n")
#endif

extern void FinishQvwm();
extern void RestartQvwm(Bool minimumRestart = False, int count = 0,
			Bool cleanup = True);

extern Pixmap CreateGradPixmap(unsigned long* pat, int width, Window win);
extern void CreateGradPattern();

#endif // _MAIN_H_
