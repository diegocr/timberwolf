/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Sun Microsystems,
 * Inc. Portions created by Sun are
 * Copyright (C) 1999 Sun Microsystems, Inc. All
 * Rights Reserved.
 *
 * Contributor(s): 
 */
#ifndef JNIEnvTests_h___
#define JNIEnvTests_h___

#ifdef XP_WIN
#include <windows.h>
#endif
#include "nsIServiceManager.h"
#include "nsIJVMManager.h"
#include "nsJVMManager.h"

//#include "jni.h"

#include "ojiapitests.h"

#define SecENV TRUE

static NS_DEFINE_CID(kJVMManagerCID,NS_JVMMANAGER_CID);
static NS_DEFINE_IID(kIJVMManagerIID, NS_IJVMMANAGER_IID);

extern nsresult GetJNI(JNIEnv **env);

typedef unsigned char byte;

#endif //JNIEnvTests_h___
