/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#ifndef nsDeviceContextGTK_h___
#define nsDeviceContextGTK_h___

#include "nsIDeviceContext.h"
#include "nsUnitConversion.h"
#include "nsIFontCache.h"
#include "nsIWidget.h"
#include "nsIView.h"
#include "nsIRenderingContext.h"

#include "nsDrawingSurfaceGTK.h"
#include "nsRenderingContextGTK.h"
#include <gtk/gtk.h>

class nsDeviceContextGTK : public nsIDeviceContext
{
public:
  nsDeviceContextGTK();

  NS_DECL_ISUPPORTS

  virtual nsresult Init();

  virtual nsIRenderingContext * CreateRenderingContext(nsIView *aView);
  virtual void InitRenderingContext(nsIRenderingContext *aContext, nsIWidget *aWidget);

  virtual float GetTwipsToDevUnits() const;
  virtual float GetDevUnitsToTwips() const;

  virtual void SetAppUnitsToDevUnits(float aAppUnits);
  virtual void SetDevUnitsToAppUnits(float aDevUnits);

  virtual float GetAppUnitsToDevUnits() const;
  virtual float GetDevUnitsToAppUnits() const;

  virtual float GetScrollBarWidth() const;
  virtual float GetScrollBarHeight() const;

  virtual nsIFontCache * GetFontCache();
  virtual void FlushFontCache();

  virtual nsIFontMetrics* GetMetricsFor(const nsFont& aFont);

  virtual void SetZoom(float aZoom);
  virtual float GetZoom() const;

  virtual nsDrawingSurface GetDrawingSurface(nsIRenderingContext &aContext);

  //functions for handling gamma correction of output device
  virtual float GetGamma(void);
  virtual void SetGamma(float aGamma);

  //XXX the return from this really needs to be ref counted somehow. MMP
  virtual PRUint8 * GetGammaTable(void);

protected:
  ~nsDeviceContextGTK();
  nsresult CreateFontCache();

  nsIFontCache         *mFontCache;
  float                 mGammaValue;
  nsDrawingSurfaceGTK *mSurface;

};

#endif /* nsDeviceContextGTK_h___ */
