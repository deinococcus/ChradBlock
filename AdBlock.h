/*
    Copyright (C) 2010 ruth(f.c.s) (deinococcus.radiophilus@gmail.com)
    partially based on GPL code by Jonah Sherman <sherman.jonah@gmail.com>
    that code was based on:
    AdBlockPlus by Wladimir Palant <trev@adblockplus.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef AdBlock_h
#define AdBlock_h

#include "config.h"
#include "CachedResource.h"
#include <wtf/text/CString.h>

#include <RegularExpression.h>
#include <pcre.h>
#include <iostream>

#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>

#include <vector>
namespace WebCore {
	bool shouldbeBlocked(const KURL& url);
}
#endif