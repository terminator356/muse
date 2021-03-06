//=========================================================
//  MusE
//  Linux Music Editor
//    $Id: swidget.h,v 1.1.1.1 2003/10/27 18:54:49 wschweer Exp $
//  (C) Copyright 1999 Werner Schweer (ws@seh.de)
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; version 2 of
//  the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//=========================================================

#ifndef __SWIDGET_H__
#define __SWIDGET_H__

#include <QWidget>

namespace MusEGui {

//---------------------------------------------------------
//   SWidget
//    a simple widget which emits a heighChanged signal
//    on received ResizeEventīs
//---------------------------------------------------------

class SWidget : public QWidget {
      Q_OBJECT
      virtual void resizeEvent(QResizeEvent*);
      

   signals:
      void heightChanged(int);

   public:
      SWidget(QWidget* parent) : QWidget(parent) {}
      };

} // namespace MusEGui

#endif

