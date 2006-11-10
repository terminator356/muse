//=============================================================================
//  MusE
//  Linux Music Editor
//  $Id:$
//
//  Copyright (C) 2006 by Werner Schweer
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

#include "splitlayergui.h"
#include "splitlayer.h"
#include "gui.h"

//---------------------------------------------------------
//   SplitLayerGui
//---------------------------------------------------------

SplitLayerGui::SplitLayerGui(SplitLayer* f, QWidget* parent)
  : QWidget(parent)
      {
      sl = f;
      QGridLayout* grid = new QGridLayout;
      grid->setSpacing(1);
      setLayout(grid);
      QSignalMapper* m1 = new QSignalMapper(this);
      QSignalMapper* m2 = new QSignalMapper(this);
      QSignalMapper* m3 = new QSignalMapper(this);
      connect(m1, SIGNAL(mapped(int)), SLOT(startPitchChanged(int)));
      connect(m2, SIGNAL(mapped(int)), SLOT(endPitchChanged(int)));
      connect(m3, SIGNAL(mapped(int)), SLOT(pitchOffsetChanged(int)));
      for (int i = 0; i < MIDI_CHANNELS; ++i) {
            QLabel* l = new QLabel(QString("Ch %1").arg(i+1));
            grid->addWidget(l, i, 0);

            p1[i] = new Awl::PitchEdit(0);
            p1[i]->setToolTip(tr("start pitch for split"));
            connect(p1[i], SIGNAL(valueChanged(int)), m1, SLOT(map()));
            m1->setMapping(p1[i], i);

            a1[i] = new QAction(this);
            a1[i]->setToolTip(tr("enable learn mode for start pitch"));
            a1[i]->setCheckable(true);
            QIcon icon;
            icon.addFile(":/xpm/recordOn.svg",  ICON_SIZE, QIcon::Normal, QIcon::On);
            icon.addFile(":/xpm/recordOff.svg", ICON_SIZE, QIcon::Normal, QIcon::Off);
            a1[i]->setIcon(icon);
            a1[i]->setData(i);
            QToolButton* rb1  = new QToolButton;
            rb1->setDefaultAction(a1[i]);
            connect(rb1, SIGNAL(triggered(QAction*)), SLOT(learnStartPitch(QAction*)));

            p2[i] = new Awl::PitchEdit(0);
            p1[i]->setToolTip(tr("end pitch for split"));
            connect(p2[i], SIGNAL(valueChanged(int)), m2, SLOT(map()));
            m2->setMapping(p2[i], i);

            a2[i] = new QAction(this);
            a2[i]->setToolTip(tr("enable learn mode for end pitch"));
            a2[i]->setCheckable(true);
            a2[i]->setIcon(icon);
            a2[i]->setData(i);
            QToolButton* rb2  = new QToolButton;
            rb2->setDefaultAction(a2[i]);
            connect(rb2, SIGNAL(triggered(QAction*)), SLOT(learnEndPitch(QAction*)));

            p3[i] = new Awl::PitchEdit(0);
            p1[i]->setToolTip(tr("pitch offset for split"));
            p3[i]->setDeltaMode(true);
            connect(p3[i], SIGNAL(valueChanged(int)), m3, SLOT(map()));
            m3->setMapping(p3[i], i);

            grid->addWidget(p1[i], i, 1);
            grid->addWidget(rb1, i, 2);
            grid->addWidget(p2[i], i, 3);
            grid->addWidget(rb2, i, 4);
            grid->addWidget(p3[i], i, 5);
            }
      int filedes[2];         // 0 - reading   1 - writing
      if (pipe(filedes) == -1) {
            perror("SplitLayerGui:creating pipe");
            exit(-1);
            }
      fd2 = filedes[0];
      fd1 = filedes[1];
      
      QSocketNotifier* socket = new QSocketNotifier(fd2, 
         QSocketNotifier::Read, this);
      connect(socket, SIGNAL(activated(int)), SLOT(resetLearnMode(int)));
      init();
      }

//---------------------------------------------------------
//   init
//---------------------------------------------------------

void SplitLayerGui::init()
      {
      for (int i = 0; i < MIDI_CHANNELS; ++i) {
            p1[i]->setValue(sl->data.startPitch[i]);
            p2[i]->setValue(sl->data.endPitch[i]);
            p3[i]->setValue(sl->data.pitchOffset[i]);
            }
      }

//---------------------------------------------------------
//   learStartPitch
//---------------------------------------------------------

void SplitLayerGui::learnStartPitch(QAction* a)
      {
      sl->learnChannel = a->data().toInt();
      sl->learnStartPitch = false;
      sl->learnMode = true;
      for (int i = 0; i < MIDI_CHANNELS; ++i) {
            if (a != a1[i])
                  a1[i]->setChecked(false);
            if (a != a2[i])
                  a2[i]->setChecked(false);
            }
      }

//---------------------------------------------------------
//   learnEndPitch
//---------------------------------------------------------

void SplitLayerGui::learnEndPitch(QAction* a)
      {
      sl->learnChannel = a->data().toInt();
      sl->learnStartPitch = false;
      sl->learnMode = true;
      for (int i = 0; i < MIDI_CHANNELS; ++i) {
            if (a != a1[i])
                  a1[i]->setChecked(false);
            if (a != a2[i])
                  a2[i]->setChecked(false);
            }
      }

//---------------------------------------------------------
//   startPitchChanged
//---------------------------------------------------------

void SplitLayerGui::startPitchChanged(int n)
      {
      sl->data.startPitch[n] = p1[n]->value();
      }

//---------------------------------------------------------
//   endPitchChanged
//---------------------------------------------------------

void SplitLayerGui::endPitchChanged(int n)
      {
      sl->data.endPitch[n] = p2[n]->value();
      }

//---------------------------------------------------------
//   pitchOffsetChanged
//---------------------------------------------------------

void SplitLayerGui::pitchOffsetChanged(int n)
      {
      sl->data.pitchOffset[n] = p3[n]->value();
      }

//---------------------------------------------------------
//   resetLearnMode
//---------------------------------------------------------

void SplitLayerGui::resetLearnMode(int fd)
      {
      char buffer[16];
      read(fd, buffer, 16);

      for (int i = 0; i < MIDI_CHANNELS; ++i) {
            a1[i]->setChecked(false);
            a2[i]->setChecked(false);
            }
      init();
      }

//---------------------------------------------------------
//   sendResetLearnMode
//---------------------------------------------------------

void SplitLayerGui::sendResetLearnMode()
      {
      write(fd1, "X", 1);
      }

