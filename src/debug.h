/*
 * debug.h
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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef DEBUG
#include <stdio.h>
# define ASSERT(ex) {						\
  if(!(ex)) {							\
    fprintf(stderr,"Assertion failed: file \"%s\", line %d\n",	\
	    __FILE__, __LINE__);				\
    ::FinishQvwm();						\
    abort();							\
  }								\
}
#else
# define ASSERT(ex)
#endif

class Debug {
private:
  static Bool stackTraceDone;

public:
  void TraceStack(char* debugger, char* program);

  static void TraceStackSigChld(int sig);
};

#endif // _DEBUG_H_
