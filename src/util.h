/*
 * util.h
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

#ifndef _UTIL_H_
#define _UTIL_H_

#include "misc.h"
#include "image.h"

/*
 * Qvwm function number.
 */
enum FuncNumber {
  Q_NONE,
  Q_SEPARATOR,
  Q_DIR,
  Q_EXEC,
  Q_RESTORE,
  Q_MOVE,
  Q_RESIZE,
  Q_MINIMIZE,
  Q_MAXIMIZE,
  Q_EXPAND,
  Q_EXPAND_LEFT,
  Q_EXPAND_RIGHT,
  Q_EXPAND_UP,
  Q_EXPAND_DOWN,
  Q_CLOSE,
  Q_KILL,
  Q_EXIT,
  Q_BOTTOM,
  Q_TOP,
  Q_LEFT,
  Q_RIGHT,
  Q_RESTART,
  Q_CHANGE_WINDOW,
  Q_CHANGE_WINDOW_BACK,
  Q_CHANGE_WINDOW_INSCR,
  Q_CHANGE_WINDOW_BACK_INSCR,
  Q_SWITCH_TASK,
  Q_SWITCH_TASK_BACK,
  Q_POPUP_START_MENU,
  Q_POPUP_DESKTOP_MENU,
  Q_POPUP_MENU,
  Q_POPDOWN_MENU,
  Q_POPDOWN_ALL_MENU,
  Q_UP_FOCUS,
  Q_DOWN_FOCUS,
  Q_RIGHT_FOCUS,
  Q_LEFT_FOCUS,
  Q_ACTION,
  Q_LEFT_PAGING,
  Q_RIGHT_PAGING,
  Q_UP_PAGING,
  Q_DOWN_PAGING,
  Q_LINEUP_ICON,
  Q_OVERLAP,
  Q_TILE_HORZ,
  Q_TILE_VERT,
  Q_MINIMIZE_ALL,
  Q_EXEC_ICON,
  Q_DELETE_ICON,
  Q_RAISE,
  Q_LOWER,
  Q_TOGGLE_ONTOP,
  Q_TOGGLE_STICKY,
  Q_TOGGLE_AUTOHIDE,
  Q_DESKTOP_FOCUS,
  Q_SHOW_TASKBAR,
  Q_HIDE_TASKBAR
};

/*
 * The origin of menu.
 */
struct MenuElem {
  char* name;
  char* file;
  FuncNumber func;
  char* exec;
  int x, y;
  MenuElem* next;
  MenuElem* child;
};

/*
 * Fontset and fontname.
 */
struct FsNameSet {
  XFontSet* fs;
  char** fontname;
};

/*
 * Attribute bit and the name.
 */
struct AttrNameSet {
  char* name;
  unsigned long flag;
  Bool act;
};

class AppAttr {
public:
  unsigned long flags;
  char* small_file;
  QvImage* small_img;
  char* large_file;
  QvImage* large_img;

  AppAttr() : flags(0), small_file(NULL), small_img(NULL),
              large_file(NULL), large_img(NULL) {}
  ~AppAttr() {
    if (small_img != NULL)
      QvImage::Destroy(small_img);
    if (large_img != NULL)
      QvImage::Destroy(large_img);
  }
};

/*
 * Key modifier
 */
struct KeyMod
{
  char* str;
  unsigned int mask;
};

/*
 * Size and bitmask from GetGeometry().
 */
class InternGeom {
public:
  Rect rc;
  Point gravity;

public:
  InternGeom() {}
  InternGeom(int x, int y, unsigned int width, unsigned int height,
	     const Point& grav)
    : rc(Rect(x, y, width, height)), gravity(grav) {}
};

/*
 * Offset
 */
const int CENTER = -1;
const int EAST = -2;
const int WEST = 0;
const int NORTH = 0;
const int SOUTH = -2;

extern Point GravityOffset[];

/*
 * inline function
 */
inline int Max(int x, int y)
{
  return ((x > y) ? x : y);
}

inline int Min(int x, int y)
{
  return ((x < y) ? x : y);
}

inline int RoundDown(int x, int y)
{
  return x / y * y;
}

inline int RoundUp(int x, int y)
{
  return (x + y - 1) / y * y;
}

inline int RoundOff(int x, int y)
{
  int rdx = x / y * y;

  return (x - rdx > y / 2) ? (rdx + y) : rdx;
}

class Menu;
class QvImage;
class Timer;

extern Bool InRect(const Point& pt, const Rect& rc);
extern Bool Intersect(const Rect& rc1, const Rect& rc2);
extern Bool IsDoubleClick(Time prevTime, Time clickTime, const Point& ptPrev,
			  const Point& ptClick);
extern int GetMenuItemNum(const MenuElem* mItem);
extern QvImage* CreateImageFromFile(char* file, Timer* timer);
extern QvImage* CreateImage(char* filename, Timer* timer);
extern char* GetFixName(XFontSet& fs, const char* name, int width);
extern Bool MappedNotOverride(Window w, long* state);
extern GC CreateTileGC(Drawable d);
extern void DarkenScreen();
extern void RefreshScreen();
extern pid_t ExecCommand(char* exec);
extern void RestoreCursor();
extern char* ExtractPathName(char* name);
extern char* GetNextDelim(char* path, char* name, int maxSize);

#ifndef HAVE_USLEEP
#ifdef __EMX__
#include <emx/syscalls.h>
#define usleep __sleep2
#else
extern void usleep(unsigned long);
#endif
#endif // HAVE_USLEEP

extern int GetRealWidth(XFontSet fs, char* str);
extern void DrawRealString(Window w, XFontSet fs, GC gc, int x, int y,
			   char* str);
extern void PlaySound(char* file, Bool sync = False);
extern Bool IsPointerInWindow(const Point& ptRoot);
extern int CreateColor(unsigned short red, unsigned short green,
		       unsigned short blue, XColor* color);

#endif // _UTIL_H_
