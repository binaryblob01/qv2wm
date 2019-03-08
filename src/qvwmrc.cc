/*
 * qvwmrc.cc
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "main.h"
#include "taskbar.h"
#include "util.h"
#include "parse.h"
#include "qvwmrc.h"
#include "qvwm.h"
#include "key.h"
#include "desktop.h"

/*
 * Default values
 */
int DoubleClickTime		    = 400;
int DoubleClickRange		    = 5;
Bool TitlebarMotion		    = True;
int TitlebarMotionSpeed		    = 10;
int MenuDelayTime		    = 300;
int MenuDelayTime2		    = 300;
int PagingResistance		    = 250;
int PagingMovement		    = 100;
int PagingBeltSize		    = 2;
Point TopLeftPage		    = Point(0, 0);
Dim PagingSize			    = Dim(1, 1);
int PagingSpeed                     = 1;
InternGeom PagerGeometry	    = InternGeom(0, 0, 48, 48, Point(0, 0));
int EdgeResistance                  = 0;
int SnappingMove                    = 0;
int SnappingEdges		    = 0;
Bool NoResizeOverTaskbar            = False;
Bool SmartPlacement		    = False;
Bool UseTaskbar                     = True;
Taskbar::TaskbarPos TaskbarPosition = Taskbar::BOTTOM;
unsigned int TaskbarRows	    = 1;
Bool OnTopTaskbar                   = True;
char* WallPaper			    = "Windows95";
char* LocaleName		    = "";
Bool UseBoldFont		    = False;
Bool UseExitDialog		    = False;
Bool UseConfirmDialog               = False;
Bool UsePager			    = False;
Bool OpaqueMove			    = True;
Bool OpaqueResize                   = True;
Bool FullOpaque                     = True;
Bool ClickToFocus		    = True;
Bool FocusOnMap                     = False;
Bool ClickingRaises                 = True;
Bool NoDesktopFocus		    = False;
Bool AutoRaise			    = False;
Bool UseClock			    = True;
char* ClockLocaleName		    = NULL;
int AutoRaiseDelay		    = 300;
char* ImagePath		            = IMGDIR;
char* SoundPath		            = SNDDIR;
Bool TaskbarAutoHide                = False;
Bool RestoreMinimize                = False;
Bool RestartOnFailure               = True;
Bool GradMenuMap                    = False;
int GradMenuMapSpeed                = 5;
Bool GradTaskbarMotion              = True;
int GradTaskbarMotionSpeed          = 10;
Bool GradTitlebar                   = False;
int GradTitlebarColors              = 40;
Bool OnTopPager                     = False;
double ShiftMoveRatio               = 1.0;
double CtrlMoveRatio                = 1.0;
int HourGlassTime                   = 1000;
int UseInfoDisplay                  = False;
Qvwm::GradStyle GradWindowMapStyle  = Qvwm::Normal; 
int GradWindowMapSpeed              = 10;
Bool UseDebugger                    = True;
Bool ImageAnimation                 = True;
int TooltipDelayTime                = 500;
int TooltipMotionSpeed              = 5;
int TooltipDisplayTime              = 6000;
unsigned int NoFocusChangeMask      = ShiftMask;
unsigned int NoSnappingMask         = Mod1Mask;
int TaskbarShowDelay                = 0;
int TaskbarHideDelay                = 500;
Bool LockDragState                  = False;
char* ScreenSaverProg               = "xlock";
int ScreenSaverDelay                = 600;

/*
 * Default message catalogs
 */
char* StartButtonTitle		    = "Start";
char* StartButtonMessage            = "Start with this button.";
char* MinimizeButtonMessage         = "Minimize";
char* MaximizeButtonMessage         = "Maximize";
char* CloseButtonMessage            = "Close";
char* RestoreButtonMessage          = "Restore";
char* ClockFormat		    = "%R";
char* ClockMessageFormat            = "%A, %B %e, %Y";

/*
 * Default sizes
 */
int IconSize			    = 32;
int FrameTitleHeight                = Qvwm::BUTTON_HEIGHT + 4;
int FrameBorderWidth                = 2;
int TaskbarButtonHeight             = 22;
int IndicatorSize                   = 16;

/*
 * Images
 */
char* TitlebarImage                 = NULL;
char* TitlebarActiveImage           = NULL;
char* FrameImage                    = NULL;
char* FrameActiveImage              = NULL;
char* TaskbarImage                  = NULL;
char* MenuImage                     = NULL;
char* MenuActiveImage               = NULL;
char* PagerImage                    = NULL;
char* DialogImage                   = NULL;
char* SwitcherImage                 = NULL;
char* StartMenuLogoImage            = NULL;

/*
 * Sounds
 */
char* OpeningSound                  = NULL;
char* EndingSound                   = NULL;
char* RestartSound                  = NULL;
char* MaximizeSound                 = NULL;
char* MinimizeSound                 = NULL;
char* RestoreSound                  = NULL;
char* ExpandSound                   = NULL;

/*
 * Default icons
 */
char* DefaultIcon                   = NULL;
char* DefaultLargeIcon              = NULL;
char* DefaultShortcutIcon           = NULL;

/*
 * Default colors
 */
XColor IconForeColor;		// black
XColor IconBackColor;		// white
XColor IconStringColor;		// white
XColor IconStringActiveColor;	// white
XColor MiniatureColor;		// black
XColor MiniatureActiveColor;	// white
XColor TitlebarColor;		// darkGray
XColor TitlebarColor2;		// grey (not gray!)
XColor TitlebarActiveColor;	// blue
XColor TitlebarActiveColor2;	// lightBlue
XColor TitleStringColor;	// gray
XColor TitleStringActiveColor;	// white
XColor MenuColor;		// gray
XColor MenuActiveColor;		// blue
XColor MenuStringColor;		// black
XColor MenuStringActiveColor;	// white
XColor DialogColor;		// gray
XColor DialogStringColor;	// black
XColor SwitcherColor;		// gray
XColor SwitcherActiveColor;	// blue
XColor SwitcherStringColor;	// black
XColor FrameColor;		// gray
XColor FrameActiveColor;	// gray
XColor PagerColor;		// gray
XColor PagerActiveColor;	// darkGray
XColor ButtonColor;		// gray
XColor ButtonActiveColor;	// gray
XColor ButtonStringColor;	// black
XColor ButtonStringActiveColor;	// black
XColor TaskbarColor;		// gray
XColor ClockStringColor;	// black
XColor DesktopColor;		// green
XColor DesktopActiveColor;	// blue
XColor StartMenuLogoColor;	// blue
XColor CursorColor;		// white
XColor TooltipColor;		// yellow
XColor TooltipStringColor;	// black

/*
 * Default fonts
 */
char* DefaultFont                   = "-*-*-medium-r-normal-*-14-*-*-*-*-*-*-*";
char* TitleFont			    = NULL;
char* TaskbarFont		    = NULL;
char* TaskbarBoldFont		    = NULL;
char* CascadeMenuFont		    = NULL;
char* CtrlMenuFont		    = NULL;
char* StartMenuFont		    = NULL;
char* IconFont			    = NULL;
char* DialogFont		    = NULL;

/*
 * Default start menu
 */
MenuElem StartMenuTemplate[] =
  {{"Exit qvwm            ", "exit.xpm", Q_EXIT, "", -1, -1, NULL, NULL}};

/*
 * Default control menu
 */
MenuElem CtrlMenuTemplate[] =
  {{"Restore      ", "", Q_RESTORE,   "", -1, -1, &CtrlMenuTemplate[1], NULL},
   {"Move",          "", Q_MOVE,      "", -1, -1, &CtrlMenuTemplate[2], NULL},
   {"Resize",        "", Q_RESIZE,    "", -1, -1, &CtrlMenuTemplate[3], NULL},
   {"Minimize",      "", Q_MINIMIZE,  "", -1, -1, &CtrlMenuTemplate[4], NULL},
   {"Maximize",      "", Q_MAXIMIZE,  "", -1, -1, &CtrlMenuTemplate[5], NULL},
   {"",              "", Q_SEPARATOR, "", -1, -1, &CtrlMenuTemplate[6], NULL},
   {"Close",         "", Q_CLOSE,     "", -1, -1, &CtrlMenuTemplate[7], NULL},
   {"Kill",          "", Q_KILL,      "", -1, -1, NULL,                 NULL}};

MenuElem DesktopMenuTemplate[] =
  {{"(None) ", "", Q_NONE, "", -1, -1, NULL, NULL}};

MenuElem IconMenuTemplate[] =
  {{"Execute", "", Q_EXEC_ICON, "", -1, -1, &IconMenuTemplate[1], NULL},
   {"Delete",  "", Q_DELETE_ICON,  "", -1, -1, NULL, NULL}};

MenuElem TaskbarMenuTemplate[] =
  {{"(None) ", "", Q_NONE, "", -1, -1, NULL, NULL}};

MenuElem ExitDlgTemplate[] =
  {{"StaticText", "Can I exit with next way?", Q_NONE, "", -1, -1,
    &ExitDlgTemplate[1], NULL},
   {"RadioButton", "Exit qvwm.", Q_EXIT, "", -1, -1,
    &ExitDlgTemplate[2], NULL},
   {"RadioButton", "Restart qvwm.", Q_RESTART, "", -1, -1,
    &ExitDlgTemplate[3], NULL},
   {"IconPixmap", "exit2.xpm", Q_NONE, "", -1, -1,
    &ExitDlgTemplate[4], NULL},
   {"OKButton",	"Yes", Q_NONE, "", -1, -1,
    &ExitDlgTemplate[5], NULL},
   {"CancelButton", "No", Q_NONE, "", -1, -1,
    &ExitDlgTemplate[6], NULL},
   {"HelpButton", "Help", Q_NONE, "", -1, -1,
    NULL, NULL}};

SCKeyEntry scMenuKey[] =
  {{XK_Escape,   0,           Q_POPDOWN_MENU},
   {XK_Alt_L,    0,           Q_POPDOWN_ALL_MENU},
   {XK_Alt_R,    0,           Q_POPDOWN_ALL_MENU},
   {XK_Meta_L,   0,           Q_POPDOWN_ALL_MENU},
   {XK_Meta_R,   0,           Q_POPDOWN_ALL_MENU},
   {XK_Up,       0,           Q_UP_FOCUS},
   {XK_Down,     0,           Q_DOWN_FOCUS},
   {XK_Right,    0,           Q_RIGHT_FOCUS},
   {XK_Left,     0,           Q_LEFT_FOCUS},
   {XK_Return,   0,           Q_ACTION}};

MenuElem* StartMenuItem = StartMenuTemplate;
MenuElem* CtrlMenuItem = CtrlMenuTemplate;
MenuElem* DesktopMenuItem = DesktopMenuTemplate;
MenuElem* IconMenuItem = IconMenuTemplate;
MenuElem* TaskbarMenuItem = TaskbarMenuTemplate;
MenuElem* ExitDlgItem = ExitDlgTemplate;
MenuElem* ShortCutItem = NULL;
SCKeyTable* scKey = NULL;
SCKeyTable* menuKey = NULL;

/*
 * Table for used variables
 */
FuncTable funcTable[] =
  {{"QVWM_NONE",			Q_NONE},
   {"QVWM_SEPARATOR",			Q_SEPARATOR},
   {"QVWM_RESTORE",			Q_RESTORE},
   {"QVWM_MOVE",			Q_MOVE},
   {"QVWM_RESIZE",			Q_RESIZE},
   {"QVWM_MINIMIZE",			Q_MINIMIZE},
   {"QVWM_MAXIMIZE",			Q_MAXIMIZE},
   {"QVWM_EXPAND",			Q_EXPAND},
   {"QVWM_EXPAND_LEFT",			Q_EXPAND_LEFT},
   {"QVWM_EXPAND_RIGHT",		Q_EXPAND_RIGHT},
   {"QVWM_EXPAND_UP",			Q_EXPAND_UP},
   {"QVWM_EXPAND_DOWN",			Q_EXPAND_DOWN},
   {"QVWM_CLOSE",			Q_CLOSE},
   {"QVWM_KILL",			Q_KILL},
   {"QVWM_EXIT",			Q_EXIT},
   {"QVWM_BOTTOM",			Q_BOTTOM},
   {"QVWM_TOP",				Q_TOP},
   {"QVWM_LEFT",			Q_LEFT},
   {"QVWM_RIGHT",			Q_RIGHT},
   {"QVWM_RESTART",			Q_RESTART},
   {"QVWM_CHANGE_WIN",			Q_CHANGE_WINDOW},
   {"QVWM_CHANGE_WIN_BACK",		Q_CHANGE_WINDOW_BACK},
   {"QVWM_CHANGE_WIN_INSCR",		Q_CHANGE_WINDOW_INSCR},
   {"QVWM_CHANGE_WIN_BACK_INSCR",	Q_CHANGE_WINDOW_BACK_INSCR},
   {"QVWM_SWITCH_TASK",			Q_SWITCH_TASK},
   {"QVWM_SWITCH_TASK_BACK",		Q_SWITCH_TASK_BACK},
   {"QVWM_POPUP_START_MENU",		Q_POPUP_START_MENU},
   {"QVWM_POPUP_DESKTOP_MENU",		Q_POPUP_DESKTOP_MENU},
   {"QVWM_POPUP_MENU",			Q_POPUP_MENU},
   {"QVWM_LEFT_PAGING",			Q_LEFT_PAGING},
   {"QVWM_RIGHT_PAGING",		Q_RIGHT_PAGING},
   {"QVWM_UP_PAGING",			Q_UP_PAGING},
   {"QVWM_DOWN_PAGING",			Q_DOWN_PAGING},
   {"QVWM_LINEUP_ICON",			Q_LINEUP_ICON},
   {"QVWM_OVERLAP",			Q_OVERLAP},
   {"QVWM_TILE_HORZ",			Q_TILE_HORZ},
   {"QVWM_TILE_VERT",			Q_TILE_VERT},
   {"QVWM_MINIMIZE_ALL",		Q_MINIMIZE_ALL},
   {"QVWM_EXEC_ICON",			Q_EXEC_ICON},
   {"QVWM_DELETE_ICON",			Q_DELETE_ICON},
   {"QVWM_RAISE",			Q_RAISE},
   {"QVWM_LOWER",			Q_LOWER},
   {"QVWM_TOGGLE_ONTOP",		Q_TOGGLE_ONTOP},
   {"QVWM_TOGGLE_STICKY",		Q_TOGGLE_STICKY},
   {"QVWM_TOGGLE_AUTOHIDE",		Q_TOGGLE_AUTOHIDE},
   {"QVWM_DESKTOP_FOCUS",		Q_DESKTOP_FOCUS},
   {"QVWM_SHOW_TASKBAR",		Q_SHOW_TASKBAR},
   {"QVWM_HIDE_TASKBAR",		Q_HIDE_TASKBAR},
   /* for backward compatibility */
   {"QVWM_CHANGEWINDOW",		Q_CHANGE_WINDOW},
   {"QVWM_SWITCHTASK",			Q_SWITCH_TASK},
   {"QVWM_POPUPSTARTMENU",		Q_POPUP_START_MENU},
   {"QVWM_POPUPMENU",			Q_POPUP_MENU},
   {"QVWM_LEFTPAGING",			Q_LEFT_PAGING},
   {"QVWM_RIGHTPAGING",			Q_RIGHT_PAGING},
   {"QVWM_UPPAGING",			Q_UP_PAGING},
   {"QVWM_DOWNPAGING",			Q_DOWN_PAGING},
   {"QVWM_LINEUP",			Q_LINEUP_ICON},
   {"QVWM_TILEHORZ",			Q_TILE_HORZ},
   {"QVWM_TILEVERT",			Q_TILE_VERT},
   {"QVWM_MINALL",			Q_MINIMIZE_ALL},
   {"QVWM_EXECICON",			Q_EXEC_ICON},
   {"QVWM_DELICON",			Q_DELETE_ICON},
   {"QVWM_TOGGLEONTOP",			Q_TOGGLE_ONTOP},
   {"QVWM_TOGGLESTICKY",		Q_TOGGLE_STICKY},
   {"QVWM_TOGGLEAUTOHIDE",		Q_TOGGLE_AUTOHIDE}};

VariableTable varTable[] =
  {{"DoubleClickTime",		F_POSITIVE,	&DoubleClickTime},
   {"DoubleClickRange",		F_POSITIVE,	&DoubleClickRange},
   {"TitlebarMotion",		F_BOOL,		&TitlebarMotion},
   {"TitlebarMotionSpeed",	F_POSITIVE,	&TitlebarMotionSpeed},
   {"MenuDelayTime",		F_NATURAL,	&MenuDelayTime},
   {"MenuDelayTime2",		F_NATURAL,	&MenuDelayTime2},
   {"PagingResistance",		F_NATURAL, 	&PagingResistance},
   {"PagingMovement",		F_NATURAL, 	&PagingMovement},
   {"PagingBeltSize",		F_NATURAL,	&PagingBeltSize},
   {"TopLeftPage",		F_OFFSET,	&TopLeftPage},
   {"PagingSize",		F_SIZE,		&PagingSize},
   {"PagingSpeed",		F_POSITIVE,	&PagingSpeed},
   {"PagerGeometry",		F_GEOMETRY,	&PagerGeometry},
   {"EdgeResistance",		F_NATURAL,	&EdgeResistance},
   {"SnappingMove",		F_NATURAL,	&SnappingMove},
   {"SnappingEdges",		F_NATURAL,	&SnappingEdges},
   {"NoResizeOverTaskbar",	F_BOOL,		&NoResizeOverTaskbar},
   {"SmartPlacement",		F_BOOL,		&SmartPlacement},
   {"UseTaskbar",		F_BOOL,		&UseTaskbar},
   {"TaskbarPosition",		F_TASKBAR,	&TaskbarPosition},
   {"TaskbarRows",		F_POSITIVE,	&TaskbarRows},
   {"OnTopTaskbar",		F_BOOL,		&OnTopTaskbar},
   {"AutoRaiseDelay",		F_NATURAL,	&AutoRaiseDelay},
   {"WallPaper",		F_STRING,	&WallPaper},
   {"LocaleName",		F_STRING,	&LocaleName},
   {"UseBoldFont",		F_BOOL,		&UseBoldFont},
   {"UseExitDialog",		F_BOOL,		&UseExitDialog},
   {"UseConfirmDialog",		F_BOOL,		&UseConfirmDialog},
   {"UsePager",			F_BOOL,		&UsePager},
   {"OpaqueMove",		F_BOOL,		&OpaqueMove},
   {"OpaqueResize",		F_BOOL,		&OpaqueResize},
   {"FullOpaque",		F_BOOL,		&FullOpaque},
   {"ClickToFocus",		F_BOOL,		&ClickToFocus},
   {"FocusOnMap",		F_BOOL,		&FocusOnMap},
   {"ClickingRaises",		F_BOOL,		&ClickingRaises},
   {"NoDesktopFocus",		F_BOOL,		&NoDesktopFocus},
   {"AutoRaise",		F_BOOL,		&AutoRaise},
   {"UseClock",			F_BOOL,		&UseClock},
   {"ClockLocaleName",		F_STRING,	&ClockLocaleName},
   {"ImagePath",		F_STRING,	&ImagePath},
   {"SoundPath",		F_STRING,	&SoundPath},
   {"TaskbarAutoHide",		F_BOOL,		&TaskbarAutoHide},
   {"RestoreMinimize",		F_BOOL,		&RestoreMinimize},
   {"RestartOnFailure",		F_BOOL,		&RestartOnFailure},
   {"GradMenuMap",		F_BOOL,		&GradMenuMap},
   {"GradMenuMapSpeed",		F_POSITIVE,	&GradMenuMapSpeed},
   {"GradTaskbarMotion",	F_BOOL,		&GradTaskbarMotion},
   {"GradTaskbarMotionSpeed",	F_POSITIVE,	&GradTaskbarMotionSpeed},
   {"GradTitlebar",		F_BOOL,		&GradTitlebar},
   {"GradTitlebarColors",	F_POSITIVE,	&GradTitlebarColors},
   {"OnTopPager",               F_BOOL,         &OnTopPager},
   {"IconSize",			F_POSITIVE,	&IconSize},
   {"FrameTitleHeight",		F_POSITIVE,	&FrameTitleHeight},
   {"FrameBorderWidth",		F_POSITIVE,	&FrameBorderWidth},
   {"TaskbarButtonHeight",	F_POSITIVE,	&TaskbarButtonHeight},
   {"IndicatorSize",		F_POSITIVE,	&IndicatorSize},
   {"ShiftMoveRatio",		F_FLOATING,	&ShiftMoveRatio},
   {"CtrlMoveRatio",		F_FLOATING,	&CtrlMoveRatio},
   {"HourGlassTime",		F_NATURAL,	&HourGlassTime},
   {"UseInfoDisplay",		F_BOOL,		&UseInfoDisplay},
   {"GradWindowMapStyle",	F_GRADSTYLE,	&GradWindowMapStyle},
   {"GradWindowMapSpeed",	F_POSITIVE,	&GradWindowMapSpeed},
   {"UseDebugger",		F_BOOL,		&UseDebugger},
   {"ImageAnimation",		F_BOOL,		&ImageAnimation},
   {"TooltipDelayTime",		F_NATURAL,	&TooltipDelayTime},
   {"TooltipMotionSpeed",	F_POSITIVE,	&TooltipMotionSpeed},
   {"TooltipDisplayTime",	F_NATURAL,	&TooltipDisplayTime},
   {"NoFocusChangeMask",	F_MODMASK,	&NoFocusChangeMask},
   {"NoSnappingMask",		F_MODMASK,	&NoSnappingMask},
   {"TaskbarShowDelay",		F_NATURAL,	&TaskbarShowDelay},
   {"TaskbarHideDelay",		F_NATURAL,	&TaskbarHideDelay},
   {"LockDragState",		F_BOOL,		&LockDragState},
   {"ScreenSaver",		F_STRING,	&ScreenSaverProg},
   {"ScreenSaverDelay",		F_NATURAL,	&ScreenSaverDelay},
   {"StartButtonTitle",		F_STRING,	&StartButtonTitle},
   {"StartButtonMessage",	F_STRING,	&StartButtonMessage},
   {"MinimizeButtonMessage",	F_STRING,	&MinimizeButtonMessage},
   {"MaximizeButtonMessage",	F_STRING,	&MaximizeButtonMessage},
   {"CloseButtonMessage",	F_STRING,	&CloseButtonMessage},
   {"RestoreButtonMessage",	F_STRING,	&RestoreButtonMessage},
   {"ClockFormat",		F_STRING,	&ClockFormat},
   {"ClockMessageFormat",	F_STRING,	&ClockMessageFormat},
   {"TitlebarImage",		F_STRING,	&TitlebarImage},
   {"TitlebarActiveImage",	F_STRING,	&TitlebarActiveImage},
   {"FrameImage",		F_STRING,	&FrameImage},
   {"FrameActiveImage",		F_STRING,	&FrameActiveImage},
   {"TaskbarImage",		F_STRING,	&TaskbarImage},
   {"MenuImage",		F_STRING,	&MenuImage},
   {"MenuActiveImage",		F_STRING,	&MenuActiveImage},
   {"PagerImage",		F_STRING,	&PagerImage},
   {"DialogImage",		F_STRING,	&DialogImage},
   {"SwitcherImage",		F_STRING,	&SwitcherImage},
   {"StartMenuLogoImage",	F_STRING,	&StartMenuLogoImage},
   {"OpeningSound",		F_STRING,	&OpeningSound},
   {"EndingSound",		F_STRING,	&EndingSound},
   {"RestartSound",		F_STRING,	&RestartSound},
   {"MaximizeSound",		F_STRING,	&MaximizeSound},
   {"MinimizeSound",		F_STRING,	&MinimizeSound},
   {"RestoreSound",		F_STRING,	&RestoreSound},
   {"ExpandSound",		F_STRING,	&ExpandSound},
   {"DefaultIcon",		F_STRING,	&DefaultIcon},
   {"DefaultLargeIcon",		F_STRING,	&DefaultLargeIcon},
   {"DefaultShortcutIcon",	F_STRING,	&DefaultShortcutIcon},
   {"TitlebarColor",		F_COLOR,	&TitlebarColor},
   {"TitlebarColor2",		F_COLOR,	&TitlebarColor2},
   {"TitlebarActiveColor",	F_COLOR,	&TitlebarActiveColor},
   {"TitlebarActiveColor2",	F_COLOR,	&TitlebarActiveColor2},
   {"TitleStringColor",		F_COLOR,	&TitleStringColor},
   {"TitleStringActiveColor",	F_COLOR,	&TitleStringActiveColor},
   {"IconForeColor",		F_COLOR,	&IconForeColor},
   {"IconBackColor",		F_COLOR,	&IconBackColor},
   {"IconStringColor",		F_COLOR,	&IconStringColor},
   {"IconStringActiveColor",	F_COLOR,	&IconStringActiveColor},
   {"FrameColor",		F_COLOR,	&FrameColor},
   {"FrameActiveColor",		F_COLOR,	&FrameActiveColor},
   {"PagerColor",		F_COLOR,	&PagerColor},
   {"PagerActiveColor",		F_COLOR,	&PagerActiveColor},
   {"MiniatureColor",		F_COLOR,	&MiniatureColor},
   {"MiniatureActiveColor",	F_COLOR,	&MiniatureActiveColor},
   {"MenuColor",		F_COLOR,	&MenuColor},
   {"MenuActiveColor",		F_COLOR,	&MenuActiveColor},
   {"MenuStringColor",		F_COLOR,	&MenuStringColor},
   {"MenuStringActiveColor",	F_COLOR,	&MenuStringActiveColor},
   {"SwitcherColor",		F_COLOR,	&SwitcherColor},
   {"SwitcherActiveColor",	F_COLOR,	&SwitcherActiveColor},
   {"SwitcherStringColor",	F_COLOR,	&SwitcherStringColor},
   {"DialogColor",		F_COLOR,	&DialogColor},
   {"DialogStringColor",	F_COLOR,	&DialogStringColor},
   {"ButtonColor",		F_COLOR,	&ButtonColor},
   {"ButtonActiveColor",	F_COLOR,	&ButtonActiveColor},
   {"ButtonStringColor",	F_COLOR,	&ButtonStringColor},
   {"ButtonStringActiveColor",	F_COLOR,	&ButtonStringActiveColor},
   {"TaskbarColor",		F_COLOR,	&TaskbarColor},
   {"ClockStringColor",		F_COLOR,	&ClockStringColor},
   {"DesktopColor",		F_COLOR,	&DesktopColor},
   {"DesktopActiveColor",	F_COLOR,	&DesktopActiveColor},
   {"StartMenuLogoColor",	F_COLOR,	&StartMenuLogoColor},
   {"CursorColor",		F_COLOR,	&CursorColor},
   {"TooltipColor",		F_COLOR,	&TooltipColor},
   {"TooltipStringColor",	F_COLOR,	&TooltipStringColor},
   {"DefaultFont",		F_STRING,	&DefaultFont},
   {"TitleFont",		F_STRING,	&TitleFont},
   {"TaskbarFont",		F_STRING,	&TaskbarFont},
   {"TaskbarBoldFont",		F_STRING,	&TaskbarBoldFont},
   {"CascadeMenuFont",		F_STRING,	&CascadeMenuFont},
   {"CtrlMenuFont",		F_STRING,	&CtrlMenuFont},
   {"StartMenuFont",		F_STRING,	&StartMenuFont},
   {"IconFont",			F_STRING,	&IconFont},
   {"DialogFont",		F_STRING,	&DialogFont},
   /* for backward compatibility */
   {"PixmapPath",		F_STRING,	&ImagePath},
   {"IconPath",			F_STRING,	&ImagePath}};

/*
 * Table for fontset variable and font name.
 */
FsNameSet fsSet[] =
  {{&fsTitle,       &TitleFont},
   {&fsTaskbar,     &TaskbarFont},
   {&fsCascadeMenu, &CascadeMenuFont},
   {&fsCtrlMenu,    &CtrlMenuFont},
   {&fsStartMenu,   &StartMenuFont},
   {&fsIcon,        &IconFont},
   {&fsDialog,      &DialogFont},
   {&fsBoldTaskbar, &TaskbarBoldFont}};  // bold font must be the last entry

AttrNameSet attrSet[] =
  {{"STICKY",		STICKY,			True},
   {"NO_FOCUS",		NO_FOCUS,		True},
   {"NO_TITLE",		TITLE,			False},
   {"NO_BORDER",	BORDER | BORDER_EDGE,	False},
   {"NO_BORDER_EDGE",	BORDER_EDGE,		False},
   {"NO_TBUTTON",	NO_TBUTTON,		True},
   {"NO_CTRLBTN",	CTRL_MENU,		False},
   {"NO_BUTTON1",	BUTTON1,		False},
   {"NO_BUTTON2",	BUTTON2,		False},
   {"NO_BUTTON3",	BUTTON3,		False},
   {"SMALL_IMG",	SMALL_IMG,		True},
   {"LARGE_IMG",	LARGE_IMG,		True},
   {"CLOSE_SOON",	CLOSE_SOON,		True},
   {"ONTOP",		ONTOP,			True},
   {"INIT_MAXIMIZE",	INIT_MAXIMIZE,		True},
   {"INIT_MINIMIZE",	INIT_MINIMIZE,		True},
   {"FOCUS_ON_CLICK",	FOCUS_ON_CLICK,		True},
   /* for backward compatibility */
   {"SMALL_PIX",	SMALL_IMG,		True},
   {"LARGE_PIX",	LARGE_IMG,		True},
   {"NOFOCUS",		NO_FOCUS,		True},
   {"NOTITLE",		TITLE,			False},
   {"NOBORDER",		BORDER | BORDER_EDGE,	False},
   {"NOBORDEREDGE",	BORDER_EDGE,		False},
   {"NOTBUTTON",	NO_TBUTTON,		True},
   {"SMALLPIX",		SMALL_IMG,		True},
   {"LARGEPIX",		LARGE_IMG,		True},
   {"CLOSESOON",	CLOSE_SOON,		True},
   {"INITMAX",		INIT_MAXIMIZE,		True},
   {"INITMIN",		INIT_MINIMIZE,		True}
};

KeyMod keyMod[] =
  {{"Shift", ShiftMask},
   {"Ctrl",  ControlMask},
   {"Alt",   Mod1Mask},  // Mod1Mask is default
   {"Meta",  Mod1Mask},  // Mod1Mask is default
   {"Mod1",  Mod1Mask},
   {"Mod2",  Mod2Mask},
   {"Mod3",  Mod3Mask},
   {"Mod4",  Mod4Mask},
   {"Mod5",  Mod5Mask},
   {"None",  0}};

const int FuncTableSize = sizeof(funcTable) / sizeof(FuncTable);
const int VarTableSize = sizeof(varTable) / sizeof(VariableTable);
const int FsNum = sizeof(fsSet) / sizeof(FsNameSet);
const int AttrNum = sizeof(attrSet) / sizeof(AttrNameSet);
const int KeyModNum = sizeof(keyMod) / sizeof(KeyMod);
const int MenuKeyNum = sizeof(scMenuKey) / sizeof(SCKeyEntry);
