/*
 * rearrange.cc
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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main.h"
#include "misc.h"
#include "qvwm.h"
#include "util.h"
#include "paging.h"
#include "pager.h"
#include "mini.h"
#include "taskbar.h"
#include "qvwmrc.h"
#include "desktop.h"

/*
 * Overlap --
 *   Rearrange windows in this screen in the overlapping form.
 */
void Desktop::Overlap(const Rect& vt)
{
  List<Qvwm>::Iterator i(&qvwmList);
  Point topLeft(rcScreen.x, rcScreen.y);

  for (Qvwm* qvWm = i.GetHead(); qvWm; qvWm = i.GetNext()) {
    if (!qvWm->CheckFlags(STICKY) && qvWm->CheckMapped() &&
	Intersect(qvWm->GetRect(), vt)) {
      XMoveWindow(display, qvWm->GetFrameWin(), topLeft.x, topLeft.y);
      qvWm->RaiseWindow(True);

      Rect rect(topLeft.x + vt.x, topLeft.y + vt.y,
		qvWm->GetRect().width, qvWm->GetRect().height);
      qvWm->ConfigureNewRect(rect);

      if (UsePager) {
	Point pt(rect.x, rect.y);
	ASSERT(qvWm->mini);
	ASSERT(pager);
	qvWm->mini->MoveMiniature(pager->ConvertToPagerPos(pt));
      }

      topLeft += Point(Qvwm::TITLE_HEIGHT + Qvwm::TOP_BORDER,
		       Qvwm::TITLE_HEIGHT + Qvwm::TOP_BORDER);
    }
  }

  if (UseTaskbar && OnTopTaskbar)
    taskBar->RaiseTaskbar();
  if (UsePager && OnTopPager)
    pager->RaisePager();
}

/*
 * TileHorz --
 *   Rearrange windows in this screen in the horizontally-tiling form.
 */
void Desktop::TileHorz(const Rect& vt)
{
  List<Qvwm>::Iterator i(&qvwmList);
  Qvwm* qvWm;
  Point topLeft(vt.x + rcScreen.x, vt.y + rcScreen.y);
  int num = 0;

  // count non-sticky window in this screen
  for (qvWm = i.GetHead(); qvWm; qvWm = i.GetNext())
    if (!qvWm->CheckFlags(STICKY) && qvWm->CheckMapped() &&
	Intersect(qvWm->GetRect(), vt))
      num++;

  for (qvWm = i.GetHead(); qvWm; qvWm = i.GetNext()) {
    if (!qvWm->CheckFlags(STICKY) && qvWm->CheckMapped() &&
	Intersect(qvWm->GetRect(), vt)) {
      Rect rect(topLeft.x, topLeft.y, vt.width / num, vt.height);
      XSizeHints hints = qvWm->GetSizeHints();
      Rect rcFix = qvWm->GetFixSize(rect, hints.max_width, hints.max_height,
				    hints.width_inc, hints.height_inc,
				    hints.base_width, hints.base_height);
      qvWm->ConfigureNewRect(rcFix);
      qvWm->RedrawWindow();
      qvWm->RaiseWindow(True);

      topLeft.x += rcFix.width;
    }
  }

  if (UseTaskbar && OnTopTaskbar)
    taskBar->RaiseTaskbar();
}

/*
 * TileVert --
 *   Rearrange windows in this screen in the vertically-tiling form.
 */
void Desktop::TileVert(const Rect& vt)
{
  List<Qvwm>::Iterator i(&qvwmList);
  Qvwm* qvWm;
  Point topLeft(vt.x + rcScreen.x, vt.y + rcScreen.y);
  int num = 0;

  // count non-sticky window in this screen
  for (qvWm = i.GetHead(); qvWm; qvWm = i.GetNext())
    if (!qvWm->CheckFlags(STICKY) && qvWm->CheckMapped() &&
	Intersect(qvWm->GetRect(), vt))
      num++;

  for (qvWm = i.GetHead(); qvWm; qvWm = i.GetNext()) {
    if (!qvWm->CheckFlags(STICKY) && qvWm->CheckMapped() &&
	Intersect(qvWm->GetRect(), vt)) {
      Rect rect(topLeft.x, topLeft.y, vt.width, vt.height / num);
      XSizeHints hints = qvWm->GetSizeHints();
      Rect rcFix = qvWm->GetFixSize(rect, hints.max_width, hints.max_height,
				    hints.width_inc, hints.height_inc,
				    hints.base_width, hints.base_height);
      qvWm->ConfigureNewRect(rcFix);
      qvWm->RedrawWindow();
      qvWm->RaiseWindow(True);

      topLeft.y += rcFix.height;
    }
  }

  if (UseTaskbar && OnTopTaskbar)
    taskBar->RaiseTaskbar();
}

void Desktop::MinimizeAll(const Rect& vt)
{
  List<Qvwm>::Iterator i(&qvwmList);

  rootQvwm->SetFocus();

  for (Qvwm* qvWm = i.GetHead(); qvWm; qvWm = i.GetNext()) {
    if (!qvWm->CheckFlags(NO_TBUTTON | STICKY) && qvWm->CheckMapped() &&
	Intersect(qvWm->GetRect(), vt))
      qvWm->MinimizeWindow(False, False);
  }
}
