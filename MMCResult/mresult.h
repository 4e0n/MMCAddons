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

#ifndef MRESULT_H
#define MRESULT_H

#include <stdio.h>
#include <QString>
#include <QStringList>

struct Student {
 QString surname,name,email,stat,start,end,dur;
 float net,net2;
 QVector<int> response;
};

class MResult {
 public:
  MResult() {}

  void load(QString meResultStr) {
   QStringList sl=meResultStr.split("],[",QString::SkipEmptyParts);
   sl.removeLast();
   exam.resize(sl.size());
   for (int i=0;i<sl.size();i++) {
    QStringList sll=sl[i].split('\n',QString::SkipEmptyParts);
    if (i==0) sll.removeFirst(); //if (i==sl.size()-1) sll.removeLast();
    // --
    for (int j=0;j<sll.size();j++)
     sll[j]=sll[j].remove(sll[j].size()-1,1) \
                  .remove(QChar('\"')).replace(QChar(','),QChar('.'));
    (exam[i].response).resize(sll.size()-8);
    exam[i].surname=sll[0].remove(0,4).toUpper();
    exam[i].name=sll[1].remove(0,4).toUpper();
    exam[i].email=sll[2].remove(0,4);
    exam[i].stat=sll[3].remove(0,4);
    exam[i].start=sll[4].remove(0,4);
    exam[i].end=sll[5].remove(0,4);
    exam[i].dur=sll[6].remove(0,4);
    exam[i].net=sll[7].remove(0,4).toFloat();
    // --
    for (int j=8;j<sll.size();j++)
     exam[i].response[j-8]=(sll[j].toFloat()>0) ? 1 : 0;
    float sum=0;
    for (int j=0;j<(exam[i].response).size();j++)
     if (exam[i].response[j]==1) sum+=1.0;
    exam[i].net2=sum;
   }
//   for (int i=0;i<exam.size();i++) {
//    qDebug("%s %s",exam[i].name.toAscii().data(),exam[i].surname.toAscii().data());

//    qDebug("%2.4f %2.4f",exam[i].net,exam[i].net2);
//    for (int j=0;j<(exam[i].response).size();j++)
//     printf("%d ",exam[i].response[j]);
//    printf("\n");
//   }
  }

  void outStr(QTextStream* ts) {
   QString line;
   line.append(QTextCodec::codecForName("UTF-8")->toUnicode( \
               "ÖĞRENCİ NO   AD SOYAD"));
   do { line.append(" "); } while (line.size()<50); // align grade
   line.append(QTextCodec::codecForName("UTF-8")->toUnicode("SONUÇ\n"));
   *ts << line;
   for (int j=0;j<70;j++) *ts << "-"; *ts << "\n";
   for (int i=0;i<exam.size();i++) {
    line="";
    line.append((exam[i].email).split("@")[0]); line.append("  ");
    line.append(exam[i].name); line.append(" "); line.append(exam[i].surname);
    do { line.append(" "); } while (line.size()<50); // align grade
    line.append(QString::number(exam[i].net2)); line.append("\n");
    *ts << line;
   }
  }

  QVector<float> diffIndex() {
   float sum=0.0; QVector<float> result((exam[0].response).size());
   for (int i=0;i<result.size();i++) {
    sum=0.0; for (int j=0;j<exam.size();j++) sum+=(float)(exam[j].response[i]);
    result[i]=sum/(float)(exam.size());
   }
   return result;
  }

 private:
  QVector<Student> exam;
};

#endif
