/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Netscape Portable Runtime (NSPR).
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998-2000
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "prbit.h"
#include "prsystem.h"

#ifdef XP_UNIX
#include <unistd.h>
#endif
#ifdef SUNOS4
#include "md/sunos4.h"
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef XP_BEOS
#include <OS.h>
#endif
#ifdef XP_AMIGAOS
#include <exec/types.h>
#include <exec/exectags.h>
#include <proto/exec.h>
#endif

PRInt32 _pr_pageShift;
PRInt32 _pr_pageSize;

/*
** Get system page size
*/
static void GetPageSize(void)
{
	PRInt32 pageSize;

    /* Get page size */
#ifdef XP_UNIX
#if defined SUNOS4 || defined BSDI || defined AIX \
        || defined LINUX || defined __GNU__ || defined __GLIBC__ \
        || defined FREEBSD || defined NETBSD || defined OPENBSD \
        || defined DARWIN || defined NEXTSTEP || defined SYMBIAN
    _pr_pageSize = getpagesize();
#elif defined(HPUX)
    /* I have no idea. Don't get me started. --Rob */
    _pr_pageSize = sysconf(_SC_PAGE_SIZE);
#else
    _pr_pageSize = sysconf(_SC_PAGESIZE);
#endif
#endif /* XP_UNIX */

#ifdef XP_BEOS
    _pr_pageSize = B_PAGE_SIZE;
#endif

#ifdef XP_PC
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    _pr_pageSize = info.dwPageSize;
#else
    _pr_pageSize = 4096;
#endif
#endif /* XP_PC */
#ifdef XP_AMIGAOS
	uint32 pmask;
	IExec->GetCPUInfoTags(
		GCIT_CPUPageSize, &pmask,
	TAG_DONE);
	if (pmask & 0x1000) _pr_pageSize = 4096;
	else
	{
		int i;
		int mask = 1;
		for (i=0; i<32; i++)
		{
			if (pmask & mask)
			{
				_pr_pageSize = pmask & mask;
				break;
			}
		}
	}
#endif

	pageSize = _pr_pageSize;
	PR_CEILING_LOG2(_pr_pageShift, pageSize);
}

PR_IMPLEMENT(PRInt32) PR_GetPageShift(void)
{
    if (!_pr_pageSize) {
	GetPageSize();
    }
    return _pr_pageShift;
}

PR_IMPLEMENT(PRInt32) PR_GetPageSize(void)
{
    if (!_pr_pageSize) {
	GetPageSize();
    }
    return _pr_pageSize;
}
