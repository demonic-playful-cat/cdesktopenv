/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these libraries and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/*
 * $XConsortium: Atomizer.hh /main/3 1996/06/11 16:17:39 cde-hal $
 *
 * Copyright (c) 1992 HaL Computer Systems, Inc.  All rights reserved.
 * UNPUBLISHED -- rights reserved under the Copyright Laws of the United
 * States.  Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 * 
 * This software contains confidential information and trade secrets of HaL
 * Computer Systems, Inc.  Use, disclosure, or reproduction is prohibited
 * without the prior express written permission of HaL Computer Systems, Inc.
 * 
 *                         RESTRICTED RIGHTS LEGEND
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(l)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 *                        HaL Computer Systems, Inc. 
 *                  1315 Dell Avenue, Campbell, CA  95008
 * 
 */
/* This is just temporary code... */

/* The idea is that you construct atomizers (Atom was taken by X11) with
   a string.  Atomizers constructed with the same initial string will
   return the same value. */

#ifndef _Atomizer_hh
#define _Atomizer_hh

class Atomizer : public FolioObject
{
public: // functions
  Atomizer (const char *);

// both of the following routines try to return
// a char * as an int.  hopefully, no one calls
// them and they can be eliminated.

  size_t value() { return ((size_t) f_value); }
  operator int () 
    { return ((size_t) f_value); }

  operator const char * ()
    { return (f_value); }
  const char *string() { return (f_value); }

protected: // variables
  static List f_pool;
  char *f_value;
};

#endif /* _Atomizer_hh */
/* DO NOT ADD ANY LINES AFTER THIS #endif */
