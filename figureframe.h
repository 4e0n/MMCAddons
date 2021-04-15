/*
MMCUtil - Moodle Multiple Choice Exam Question Set Arranger Utility
   Copyright (C) 2021 Barkin Ilhan
   N.E.U. Meram Medical Faculty Biophysics Department
   Computational Cognitive Neuroscience Laboratory
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If no:t, see <https://www.gnu.org/licenses/>.
 Contact info:
 E-Mail:  barkin@unrlabs.org
 Website: http://icon.unrlabs.org/staff/barkin/
 Repo:    https://github.com/4e0n/
*/

#ifndef FIGUREFRAME_H
#define FIGUREFRAME_H

#include <QtGui>

class FigureFrame : public QFrame {
 Q_OBJECT
 public:
  FigureFrame(QWidget *p,int x,int y,int w,int h) : QFrame(p) {
   frameWidth=w; frameHeight=h;
   setGeometry(x,y,w,h); setContentsMargins(1,1,1,1);
   x1=contentsRect().left(); y1=contentsRect().top();
   x2=contentsRect().right(); y2=contentsRect().bottom();
   dx=x2-x1; dy=y2-y1;
   imageVisible=false;
   show();
  }
  void setImage(QImage img) {
   image=img; imageVisible=true; repaint();
  }
  void unsetImage() {
   imageVisible=false; repaint();
  }
  
 public slots:
 
 protected:
  virtual void paintEvent(QPaintEvent* p) {
   painter.begin(this);
   if (imageVisible) {
	QPixmap kPix(frameWidth,frameHeight);
	kPix.convertFromImage(image.scaled(frameWidth,frameHeight,Qt::KeepAspectRatio));
	painter.drawPixmap(0,0,kPix);
   } else {
	QPixmap kPix(frameWidth,frameHeight); kPix.fill(Qt::white);
    painter.setPen(Qt::red); painter.setBrush(Qt::green);
    painter.drawRect(0,0,frameWidth-1,frameHeight-1);
	painter.drawLine(0,0,frameWidth-1,frameHeight-1);
	painter.drawLine(frameWidth-1,0,0,frameHeight-1);
   }
   painter.end();
  }
 private:
  QPainter painter; bool imageVisible; QImage image;
  int frameWidth,frameHeight,x1,y1,x2,y2,dx,dy;
  QString dummyString;
};

#endif
