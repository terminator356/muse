//=============================================================================
//  Awl
//  Audio Widget Library
//  $Id:$
//
//  Copyright (C) 2007 by Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#ifndef __AWLDRAWBAR_H__
#define __AWLDRAWBAR_H__

#include "slider.h"

namespace Awl {

//---------------------------------------------------------
//   Drawbar
//---------------------------------------------------------

class Drawbar : public Slider {
      Q_OBJECT

      Q_PROPERTY(QColor sliderColor READ sliderColor WRITE setSliderColor)

      virtual void paintEvent(QPaintEvent*);

   protected:
      QColor _sliderColor;

   public:
      Drawbar(QWidget* parent = 0);
      ~Drawbar();

      //! return current slider color
      QColor sliderColor() const      { return _sliderColor; }
      void setSliderColor(const QColor&);
      };

}

#endif

