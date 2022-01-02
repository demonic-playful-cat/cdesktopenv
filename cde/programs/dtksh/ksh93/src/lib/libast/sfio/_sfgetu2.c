/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*          Copyright (c) 1985-2011 AT&T Intellectual Property          *
*          Copyright (c) 2020-2021 Contributors to ksh 93u+m           *
*                      and is licensed under the                       *
*                 Eclipse Public License, Version 1.0                  *
*                    by AT&T Intellectual Property                     *
*                                                                      *
*                A copy of the License is available at                 *
*          http://www.eclipse.org/org/documents/epl-v10.html           *
*         (with md5 checksum b35adb5213ca9657e911e9befb180842)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                   Phong Vo <kpv@research.att.com>                    *
*                                                                      *
***********************************************************************/
/* OBSOLETE 19961031 -- for shared library compatibility */

#include	"sfhdr.h"

#undef	_sfgetu2

#if _BLD_sfio && defined(__EXPORT__)
#define extern	__EXPORT__
#endif

extern long	_sfgetu2(Sfio_t*, long);

#undef	extern

long _sfgetu2(reg Sfio_t* f, long v)
{
	if (v < 0)
		return -1;
	sfungetc(f, v);
	return sfgetu(f);
}
