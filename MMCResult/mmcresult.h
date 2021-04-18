/*
MMCResult - Moodle Multiple Choice Exam Result Analysis Utility
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

#ifndef MMCRESULT_H
#define MMCRESULT_H

#include <QtGui>
#include "mresult.h"

const int APP_WIDTH=600;
const int APP_HEIGHT=150;

class MMCResult : public QWidget {
 Q_OBJECT
 public:
  MMCResult(QWidget *p=0) : QWidget(0) {
   setGeometry(5,30,APP_WIDTH,APP_HEIGHT);
   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
   
   setWindowTitle(tr("Moodle Sınav Sonuç Analiz Aracı v0.5 - (c) 2021 GPLv3 Barkın İlhan (barkin@unrlabs.org) - NEÜ MTF Biyofizik AD"));
   statusBar=new QStatusBar(this); statusBar->setGeometry(0,APP_HEIGHT-20,APP_WIDTH,20);
   statusBar->show(); statusBar->showMessage(tr("Hazır."));
   
   jsonLoadButton=new QPushButton(tr("Dosya\n(JSON)\nyükle..."),this);
   jsonLoadButton->setGeometry(10,10,90,100);
   connect(jsonLoadButton,SIGNAL(clicked()),this,SLOT(jsonLoadSlot()));
   
   saveButton=new QPushButton(tr("Sonuç\nkaydet..."),this);
   saveButton->setGeometry(110,10,90,100);
   connect(saveButton,SIGNAL(clicked()),this,SLOT(saveSlot()));

   quitButton=new QPushButton(tr("Çıkış"),this);
   quitButton->setGeometry(APP_WIDTH-90,APP_HEIGHT-140,80,100);
   connect(quitButton,SIGNAL(clicked()),this,SLOT(quitSlot()));
   
   mr=new MResult();

   show();
  }

 public slots:
  void jsonLoadSlot() {
   QString fileName=QFileDialog::getOpenFileName(this,tr("Dosya aç..."),
                                                 "./", tr(" Files (*.json)"));
   QFile jsonFile(fileName);
   if (jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QByteArray ba=jsonFile.readAll(); jsonFile.close();
    QString str=QTextCodec::codecForName("UTF-8")->toUnicode(ba);
//    QByteArray ba2=str.toUtf8();
    mr->load(str);
    statusBar->showMessage(tr("JSON dosyası okundu.."),5000);
   } else {
    QMessageBox msgBox; msgBox.setText(tr("JSON dosyası okunamadı!")); msgBox.exec(); 
   }
  }

  void saveSlot() {
   QString fileName=QFileDialog::getSaveFileName(this,tr("TXT kaydet..."), \
                                                 "./sonuc.txt",
						 tr("TXT Files (*.txt)"));
   QFile txtFile(fileName);
   if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream txtStream(&txtFile);
    txtStream.setCodec("UTF-8");
    mr->outStr(&txtStream);
    QVector<float> di=mr->diffIndex();
    txtStream << "\n";
    for (int i=0;i<di.size();i++) {
     txtStream << "Soru " << QString::number(i+1) << "  " \
	                  << QString::number(di[i],'g',2) << " ";
     if (di[i]<0.3 || di[i]>0.7) txtStream << "*";
     txtStream << "\n";
    }
    txtFile.close();
    statusBar->showMessage("Kaydedildi..",5000);
   } else {
    QMessageBox msgBox; msgBox.setText(tr("Dosya kaydedilemedi!"));
    msgBox.exec(); 
   }
  }
  
  void quitSlot() {
   close();
  }

 private:
  QPushButton *jsonLoadButton,*saveButton,*quitButton;
  QStatusBar *statusBar;

  MResult* mr;
};

#endif
