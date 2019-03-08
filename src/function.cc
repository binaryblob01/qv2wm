/*
 * function.cc
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
#include <string.h>
#include <unistd.h>
#ifdef __EMX__
#include <process.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include "main.h"
#include "misc.h"
#include "qvwm.h"
#include "menu.h"
#include "startmenu.h"
#include "fbutton.h"
#include "parse.h"
#include "event.h"
#include "switcher.h"
#include "qvwmrc.h"
#include "exit_dialog.h"
#include "confirm_dialog.h"
#include "paging.h"
#include "pager.h"
#include "icon.h"
#include "focus_mgr.h"
#include "desktop.h"
#include "gnome.h"

/* XXX */
KeyCode swCode;
unsigned int swMod;

/*
 * ExecFunction --
 *   Execute qvwm function.
 */
void Menu::ExecFunction(FuncNumber fn, int num)
{
  Point ptFrame, pt, ptJunk;
  int pFocus;
  int borderWidth, topBorder, titleHeight, titleEdge;
  Rect rect;
  Window junkRoot, junkChild;
  unsigned int mask;
  int gDir;

  switch (fn) {
  /*
   * Do nothing.
   */
  case Q_NONE:
    break;

  /*
   * Extract the menu.
   */
  case Q_DIR:
    ASSERT(num >= 0 && num < nitems);

    // delayed menu creation
    if (next[num] == NULL) {
      ASSERT(childItem[num]);
      next[num] = new Menu(childItem[num], fs, this, qvWm,
			   leftMargin, nameMargin, hiMargin,
			   imgParentFolder, imgParentIcon);
    }

    switch (fDir) {
    /*
     * To right side.
     */
    case RIGHT:
      ptFrame.x = rc.x + rc.width - 7;
      if (ptFrame.x + next[num]->rc.width > rcScreen.width-1) {
	ptFrame.x = rc.x - next[num]->rc.width + MenuFrameWidth;
	fDir = LEFT;
	gDir = GD_LEFT;
      }
      else
	gDir = GD_RIGHT;
      break;

    /*
     * To left side.
     */
    case LEFT:
      ptFrame.x = rc.x - next[num]->rc.width+MenuFrameWidth;
      if (ptFrame.x + next[num]->rc.width < 0) {
	ptFrame.x = rc.x + rc.width - 7;
	fDir = RIGHT;
	gDir = GD_RIGHT;
      }
      else
	gDir = GD_LEFT;
      break;
    }

    ptFrame.y = rc.y + CalcItemYPos(num) - MenuFrameWidth;
    if (ptFrame.y + next[num]->rc.height > rcScreen.height - 1) {
      ptFrame.y = ptFrame.y + itemHeight - next[num]->rc.height +
	MenuFrameWidth * 2;
      gDir |= GD_UP;
    }
    else
      gDir |= GD_DOWN;

    child = next[num];
    child->SetQvwm(qvWm);
    child->MapMenu(ptFrame.x, ptFrame.y, gDir);
    break;

  /*
   * Execute the external command.
   */
  case Q_EXEC:
    ASSERT(num >= 0 && num < nitems);
    ExecCommand(exec[num]);
    break;

  /*
   * Restore the window pos & size.
   */
  case Q_RESTORE:
    ASSERT(qvWm);
    if (qvWm->CheckStatus(MAXIMIZE_WINDOW) &&
	!qvWm->CheckStatus(MINIMIZE_WINDOW)) {
      ASSERT(qvWm->fButton[1]);
      qvWm->fButton[1]->ChangeImage(FrameButton::MAXIMIZE);
    }
    qvWm->RestoreWindow();

    /*
     * After mapped, give the focus to qvWm.
     */
    rootQvwm->SetFocus();
    qvWm->SetFocus();
    break;

  /*
   * Move the window.
   */
  case Q_MOVE:
    ASSERT(qvWm);
    rect = qvWm->GetRect();
    XDefineCursor(display, root, cursor[MOVE]);
    XQueryPointer(display, root, &junkRoot, &junkChild, &pt.x, &pt.y,
		  &ptJunk.x, &ptJunk.y, &mask);
    qvWm->SetStatus(PRESS_FRAME);
    qvWm->MoveWindow(pt, False);
    XDefineCursor(display, root, cursor[SYS]);
    break;

  /*
   * Resize the window.
   */
  case Q_RESIZE:
    ASSERT(qvWm);
    rect = qvWm->GetRect();
    XDefineCursor(display, root, cursor[MOVE]);
    XQueryPointer(display, root, &junkRoot, &junkChild, &pt.x, &pt.y,
		  &ptJunk.x, &ptJunk.y, &mask);
    qvWm->SetStatus(PRESS_FRAME);
    qvWm->ResizeWindow(0, False);
    XDefineCursor(display, root, cursor[SYS]);
    break;

  /*
   * Minimize the window.
   */
  case Q_MINIMIZE:
    ASSERT(qvWm);
    qvWm->MinimizeWindow();
    break;

  /*
   * Maximize the window.
   */
  case Q_MAXIMIZE:
    ASSERT(qvWm);
    ASSERT(qvWm->fButton[1]);
    qvWm->fButton[1]->ChangeImage(FrameButton::RESTORE);
    qvWm->MaximizeWindow();
    rootQvwm->SetFocus();
    qvWm->SetFocus();
    break;

  /*
   * Expand the window "in place" without obscuring other windows
   */
  case Q_EXPAND:
    ASSERT(qvWm);
    qvWm->ExpandWindow(EXPAND_LEFT | EXPAND_RIGHT | EXPAND_UP | EXPAND_DOWN);
    rootQvwm->SetFocus();
    qvWm->SetFocus();
    break;

  case Q_EXPAND_LEFT:
    ASSERT(qvWm);
    qvWm->ExpandWindow(EXPAND_LEFT);
    rootQvwm->SetFocus();
    qvWm->SetFocus();
    break;

  case Q_EXPAND_RIGHT:
    ASSERT(qvWm);
    qvWm->ExpandWindow(EXPAND_RIGHT);
    rootQvwm->SetFocus();
    qvWm->SetFocus();
    break;

  case Q_EXPAND_UP:
    ASSERT(qvWm);
    qvWm->ExpandWindow(EXPAND_UP);
    rootQvwm->SetFocus();
    qvWm->SetFocus();
    break;

  case Q_EXPAND_DOWN:
    ASSERT(qvWm);
    qvWm->ExpandWindow(EXPAND_DOWN);
    rootQvwm->SetFocus();
    qvWm->SetFocus();
    break;

  /*
   * Close the window.
   */
  case Q_CLOSE:
    if (qvWm == rootQvwm) {
      ExecFunction(Q_EXIT, 0);
      return;
    }
    ASSERT(qvWm);
    qvWm->FetchWMProtocols();
    if (qvWm->CheckFlags(WM_DELETE_WINDOW))
      qvWm->CloseWindow();
    else
      qvWm->KillClient();
    break;

  /*
   * Kill the application.
   */
  case Q_KILL:
    if (qvWm != rootQvwm) {
      ASSERT(qvWm);
      qvWm->KillClient();
    }
    break;

  /*
   * Exit qvwm.
   */
  case Q_EXIT:
    if (!UseExitDialog) {
      if (UseConfirmDialog) {
	if (confirmDlg == NULL)
	  confirmDlg = new ConfirmDialog();
	confirmDlg->MapDialog();
	confirmDlg->ProcessDialog();
      }
      else
	FinishQvwm();
    }
    else {
      if (exitDlg == NULL)
	exitDlg = new ExitDialog();
      exitDlg->MapDialog();
      exitDlg->ProcessDialog();
    }
    break;

  /*
   * Move taskbar to the bottom of screen.
   */
  case Q_BOTTOM:
    if (UseTaskbar)
      taskBar->MoveTaskbar(Taskbar::BOTTOM);
    break;

  /*
   * Move taskbar to the top of screen.
   */
  case Q_TOP:
    if (UseTaskbar)
      taskBar->MoveTaskbar(Taskbar::TOP);
    break;

  /*
   * Move taskbar to the left of screen.
   */
  case Q_LEFT:
    if (UseTaskbar)
      taskBar->MoveTaskbar(Taskbar::LEFT);
    break;

  /*
   * Move taskbar to the right of screen.
   */
  case Q_RIGHT:
    if (UseTaskbar)
      taskBar->MoveTaskbar(Taskbar::RIGHT);
    break;

  /*
   * Restart qvwm.
   */
  case Q_RESTART:
    RestartQvwm();
    break;

  /*
   * Switch the window to next window in focus stack.
   */
  case Q_CHANGE_WINDOW:
    focusMgr.RollFocus(True);
    break;

  case Q_CHANGE_WINDOW_BACK:
    focusMgr.RollFocus(False);
    break;

  case Q_CHANGE_WINDOW_INSCR:
    focusMgr.RollFocusWithinScreen(True);
    break;

  case Q_CHANGE_WINDOW_BACK_INSCR:
    focusMgr.RollFocusWithinScreen(False);
    break;

  /*
   * Switch the task to another task with the task switcher.
   */
  case Q_SWITCH_TASK:
    if (taskSwitcher == NULL)
      taskSwitcher = new TaskSwitcher();
    taskSwitcher->SwitchTask(True, swCode, swMod);
    break;

  /*
   * Switch back the task to another task with the task switcher.
   */
  case Q_SWITCH_TASK_BACK:
    if (taskSwitcher == NULL)
      taskSwitcher = new TaskSwitcher();
    taskSwitcher->SwitchTask(False, swCode, swMod);
    break;

  /*
   * Popup the start menu.
   */
  case Q_POPUP_START_MENU:
    if (startMenu == NULL)
      startMenu = new StartMenu(StartMenuItem);

    if (!startMenu->CheckMapped()) {
      rootQvwm->SetFocus();
      startMenu->MapMenu();
    }
    break;

  /*
   * Popup the desktop menu.
   */
  case Q_POPUP_DESKTOP_MENU:
    rootQvwm->SetFocus();
    rootQvwm->ctrlMenu->ExecFunction(Q_POPUP_MENU, 0);
    break;

  /*
   * Popup the menu.
   */
  case Q_POPUP_MENU:
    if (!CheckMapped()) {
      ASSERT(qvWm);
      if (qvWm == rootQvwm) {
	Window junkRoot, junkChild;
	Point ptRoot, ptJunk;
	unsigned int mask;

	XQueryPointer(display, root, &junkRoot, &junkChild,
		      &ptRoot.x, &ptRoot.y, &ptJunk.x, &ptJunk.y, &mask);
	MapMenu(ptRoot.x, ptRoot.y);
      }
      else {
	qvWm->GetBorderAndTitle(borderWidth, topBorder, titleHeight,
				titleEdge);
	rect = qvWm->GetRect();
	MapMenu(rect.x + topBorder, rect.y + topBorder + titleHeight);
      }
    }
    break;

  /*
   * Popdown one menu.
   */
  case Q_POPDOWN_MENU:
    // Erasemenu only one menu.
    UnmapMenu();
    break;

  /*
   * Popdown all menu.
   */
  case Q_POPDOWN_ALL_MENU:
    UnmapAllMenus();
    break;

  /*
   * Move the focus of menu up.
   */
  case Q_UP_FOCUS:
    if (iFocus == -1) {
      iFocus = nitems - 1;
      while (func[iFocus] == Q_SEPARATOR)
	if (--iFocus == -1)
	  return;
    }
    else {
      ASSERT(iFocus >= 0 && iFocus < nitems);
      pFocus = iFocus;

      do {
	if (--iFocus == -1)
	  iFocus = nitems - 1;
      } while (func[iFocus] == Q_SEPARATOR);

      SetMenuFocus(pFocus);
    }
    ASSERT(iFocus >= 0 && iFocus < nitems);
    SetMenuFocus(iFocus);
    break;
    
  /*
   * Move the focus of menu down.
   */
  case Q_DOWN_FOCUS:
    if (iFocus == -1) {
      iFocus = 0;
      while (func[iFocus] == Q_SEPARATOR)
	if (++iFocus == nitems)
	  return;
    }
    else {
      ASSERT(iFocus >= 0 && iFocus < nitems);
      pFocus = iFocus;

      do {
	if (++iFocus == nitems)
	  iFocus = 0;
      } while (func[iFocus] == Q_SEPARATOR);

      SetMenuFocus(pFocus);
    }
    ASSERT(iFocus >= 0 && iFocus < nitems);
    SetMenuFocus(iFocus);
    break;

  /*
   * Move the focus of menu right.
   */
  case Q_RIGHT_FOCUS:
    if (fDir == RIGHT) {
      if (iFocus == -1)
	break;
      ASSERT(iFocus >= 0 && iFocus < nitems);
      if (func[iFocus] == Q_DIR) {
	ExecFunction(Q_DIR, iFocus);
	ASSERT(child);
	child->ExecFunction(Q_DOWN_FOCUS, 0);
      }
    }
    else {
      ASSERT(fDir == LEFT);
      if (parent != NULL)
	UnmapMenu();
    }
    break;

  /*
   * Move the focus of menu left.
   */
  case Q_LEFT_FOCUS:
    if (fDir == LEFT) {
      if (iFocus == -1)
	break;
      ASSERT(iFocus >= 0 && iFocus < nitems);
      if (func[iFocus] == Q_DIR) {
	ExecFunction(Q_DIR, iFocus);
	ASSERT(child);
	child->ExecFunction(Q_DOWN_FOCUS, 0);
      }
    }
    else {
      ASSERT(fDir == RIGHT);
      if (parent != NULL)
	UnmapMenu();
    }
    break;
    
  /*
   * Execute the function of the menu item.
   */
  case Q_ACTION:
    ASSERT(iFocus >= 0 && iFocus < nitems);
    if (IsSelectable(func[iFocus])) {
      pFocus = iFocus;
      if (func[iFocus] != Q_DIR)
	UnmapAllMenus();

      ExecFunction(func[pFocus], pFocus);
    }
    break;

  /*
   * Switch to left virtual page.
   */
  case Q_LEFT_PAGING:
    {
      if (Menu::CheckAnyMenusMapped())
	return;

      Rect rcRoot = rootQvwm->GetRect();
      Point oldOrigin = paging->origin;

      ASSERT(paging);

      paging->origin.x -= rcRoot.width;
      paging->PagingAllWindows(oldOrigin);

      desktop.ChangeFocusToCursor();
    }
    break;

  /*
   * Switch to right virtual page.
   */
  case Q_RIGHT_PAGING:
    {
      if (Menu::CheckAnyMenusMapped())
	return;

      Rect rcRoot = rootQvwm->GetRect();
      Point oldOrigin = paging->origin;

      ASSERT(paging);

      paging->origin.x += rcRoot.width;
      paging->PagingAllWindows(oldOrigin);

      desktop.ChangeFocusToCursor();
    }
    break;

  /*
   * Switch to up virtual page.
   */
  case Q_UP_PAGING:
    {
      if (Menu::CheckAnyMenusMapped())
	return;

      Rect rcRoot = rootQvwm->GetRect();
      Point oldOrigin = paging->origin;

      ASSERT(paging);

      paging->origin.y -= rcRoot.height;
      paging->PagingAllWindows(oldOrigin);

      desktop.ChangeFocusToCursor();
    }
    break;

  /*
   * Switch to down virtual page.
   */
  case Q_DOWN_PAGING:
    {
      if (Menu::CheckAnyMenusMapped())
	return;

      Rect rcRoot = rootQvwm->GetRect();
      Point oldOrigin = paging->origin;

      ASSERT(paging);

      paging->origin.y += rcRoot.height;
      paging->PagingAllWindows(oldOrigin);

      desktop.ChangeFocusToCursor();
    }
    break;

  /*
   * Line up desktop icons
   */ 
  case Q_LINEUP_ICON:
    desktop.LineUpAllIcons();
    break;

  case Q_OVERLAP:
    rect = Rect(paging->origin.x, paging->origin.y,
		rcScreen.width, rcScreen.height);
    desktop.Overlap(rect);
    break;

  case Q_TILE_HORZ:
    rect = Rect(paging->origin.x, paging->origin.y,
		rcScreen.width, rcScreen.height);
    desktop.TileHorz(rect);
    break;

  case Q_TILE_VERT:
    rect = Rect(paging->origin.x, paging->origin.y,
		rcScreen.width, rcScreen.height);
    desktop.TileVert(rect);
    break;

  case Q_MINIMIZE_ALL:
    rect = Rect(paging->origin.x, paging->origin.y,
		rcScreen.width, rcScreen.height);
    desktop.MinimizeAll(rect);
    break;

  /*
   * Raise a window
   */
  case Q_RAISE:
    ASSERT(qvWm);
    qvWm->RaiseWindow(True);
    break;
    
  /*
   * Lower a window
   */
  case Q_LOWER:
    ASSERT(qvWm);
    qvWm->LowerWindow();
    break;

  /*
   * Toggle switch of an ONTOP attribute
   */
  case Q_TOGGLE_ONTOP:
    if (qvWm != rootQvwm) {
      ASSERT(qvWm);
      if (qvWm->CheckFlags(ONTOP)) {
	qvWm->ResetFlags(ONTOP);
	desktop.GetOnTopList().Remove(qvWm);
	Gnome::ChangeLayer(qvWm, WIN_LAYER_NORMAL);
      }
      else {
	qvWm->SetFlags(ONTOP);
	desktop.GetOnTopList().InsertTail(qvWm);
	Gnome::ChangeLayer(qvWm, WIN_LAYER_ONTOP);
      }
    }
    break;

  case Q_TOGGLE_STICKY:
    if (qvWm != rootQvwm) {
      if (qvWm->CheckFlags(STICKY)) {
	qvWm->ResetFlags(STICKY);
	qvWm->ConfigureNewRect(qvWm->GetRect());
	Gnome::ResetState(qvWm, WIN_STATE_STICKY);
      }
      else {
	qvWm->SetFlags(STICKY);
	qvWm->ConfigureNewRect(qvWm->GetRect());
	Gnome::SetState(qvWm, WIN_STATE_STICKY);
      }
    }
    break;

  case Q_TOGGLE_AUTOHIDE:
    if (!UseTaskbar)
      break;

    if (TaskbarAutoHide) {
      TaskbarAutoHide = False;
      if (taskBar->IsHiding())
	taskBar->ShowTaskbar();
      rcScreen = taskBar->GetScreenRectOnShowing();
      if (UsePager) {
	ASSERT(pager);
	pager->RecalcPager();
      }
    }
    else {
      TaskbarAutoHide = True;
      rcScreen = taskBar->GetScreenRectOnHiding();
      taskBar->HideTaskbar();
      if (UsePager) {
	ASSERT(pager);
	pager->RecalcPager();
      }
    }
    break;

  case Q_DESKTOP_FOCUS:
    rootQvwm->SetFocus();
    break;

  case Q_SHOW_TASKBAR:
    if (UseTaskbar)
      if (TaskbarAutoHide && taskBar->IsHiding())
	taskBar->ShowTaskbar();
    break;

  case Q_HIDE_TASKBAR:
    if (UseTaskbar)
      if (TaskbarAutoHide && !taskBar->IsHiding())
	taskBar->HideTaskbar();
    break;
  }
}

/*
 * IsSelectable --
 *   Return True if the menu item is selectable.
 */
Bool Menu::IsSelectable(FuncNumber fn)
{
  ASSERT(qvWm);

  switch (fn) {
  case Q_NONE:
    return False;

  case Q_RESTORE:
    if (qvWm->CheckStatus(MINIMIZE_WINDOW | MAXIMIZE_WINDOW))
      return True;
    return False;

  case Q_MOVE:
  case Q_RESIZE:
    if (qvWm->CheckStatus(MINIMIZE_WINDOW | MAXIMIZE_WINDOW))
      return False;
    return True;

  case Q_MINIMIZE:
    if (qvWm->CheckStatus(MINIMIZE_WINDOW) || qvWm->CheckFlags(NO_TBUTTON))
      return False;
    return True;

  case Q_MAXIMIZE:
    if (qvWm->CheckStatus(MAXIMIZE_WINDOW) &&
	!qvWm->CheckStatus(MINIMIZE_WINDOW))
      return False;
    return True;

  case Q_SEPARATOR:
    return False;

  case Q_BOTTOM:
  case Q_TOP:
  case Q_LEFT:
  case Q_RIGHT:
    if (UseTaskbar)
      return True;
    else
      return False;

  case Q_LEFT_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.x > rcVirt.x * rcRoot.width)
	return True;
      else
	return False;
    }

  case Q_RIGHT_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.x < (rcVirt.x + rcVirt.width - 1) * rcRoot.width)
	return True;
      else
	return False;
    }

  case Q_UP_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.y > rcVirt.y * rcRoot.height)
	return True;
      else
	return False;
    }
    
  case Q_DOWN_PAGING:
    {
      Rect rcRoot = rootQvwm->GetRect();
      Rect rcVirt = paging->GetVirtRect();
      if (paging->origin.y < (rcVirt.y + rcVirt.height - 1) * rcRoot.height)
	return True;
      else
	return False;
    }

  default:
    return True;
  }
}
