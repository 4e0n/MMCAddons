
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

#ifndef MMCUTIL_H
#define MMCUTIL_H

#include <QtGui>
#include "figureframe.h"

const int APP_WIDTH=1000;
const int APP_HEIGHT=550;

struct Question {
 int dept;
 QString tag,body;
 QVector<QString> choice;
 int correctAnswer;   // 0..4
 bool hasBodyFigure;
 QString bodyFigure;  // base64
 QImage bodyImage;    // For thumbnail
 QString bodyImageFN;
 QString xml;
};

class MMCUtil : public QWidget {
 Q_OBJECT
 public:
  MMCUtil(QWidget *p=0) : QWidget(0) {
   setGeometry(5,30,APP_WIDTH,APP_HEIGHT);
   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
   
   QStringList qMax; bool ok;
   for (int i=0;i<200;i++) qMax << QString::number(i+1);
   QString item=QInputDialog::getItem(this,tr("QInputDialog::getItem()"),
                                      tr("Soru sayısı:"),qMax,0,false,&ok);

   if (ok && !item.isEmpty()) questionCount=item.toInt();
   else close();

   question.resize(questionCount); // # of questions
   for (int i=0;i<question.size();i++) { question[i].choice.resize(5); resetQuestion(i); }

   populateDepts();
	  
   // -----
   
   setWindowTitle(tr("Moodle XML soru setí oluşturma aracı v0.5 - (c) 2021 GPLv3 Barkın İlhan (barkin@unrlabs.org) - NEÜ MTF Biyofizik AD"));
   statusBar=new QStatusBar(this); statusBar->setGeometry(0,APP_HEIGHT-20,APP_WIDTH,20);
   statusBar->show(); statusBar->showMessage(tr("Hazır."));
   
   QLabel* qIndexLabel=new QLabel(tr("Soru#:"),this);
   qIndexLabel->setGeometry(20,20,50,20); qIndexLabel->show();
   qSelectCB=new QComboBox(this);
   for (int i=0;i<questionCount;i++) qSelectCB->addItem(QString::number(i+1));
   qSelectCB->setGeometry(70,20,70,20); qSelectCB->show();
   connect(qSelectCB,SIGNAL(currentIndexChanged(int)),this,SLOT(selectQuestionSlot(int)));
   
   QLabel* deptLabel=new QLabel(tr("Anabilim Dalı:"),this);
   deptLabel->setGeometry(160,20,90,20); deptLabel->show();
   deptCB=new QComboBox(this);
   for (int i=0;i<deptNames.size();i++) deptCB->addItem(deptNames[i]);
   deptCB->setGeometry(250,20,150,20); deptCB->show();
   
   QLabel* tagLabel=new QLabel(tr("Rumuz:"),this);
   tagLabel->setGeometry(420,20,50,20); tagLabel->show();
   tagLE=new QLineEdit(this); tagLE->setGeometry(470,20,150,20); tagLE->show();
   
   QLabel* bodyLabel=new QLabel(tr("Soru gövdesi:"),this);
   bodyLabel->setGeometry(10,60,80,20); bodyLabel->show();
   bodyTE=new QTextEdit(this); bodyTE->setGeometry(10,90,510,100); bodyTE->show();
   
   bodyFigLoadButton=new QPushButton(tr("Şekil\n(PNG/JPG)\nyükle..."),this);
   bodyFigLoadButton->setGeometry(530,80,90,50);
   connect(bodyFigLoadButton,SIGNAL(clicked()),this,SLOT(bodyFigLoadSlot()));
   
   figFrame=new FigureFrame(this,530,140,90,90); figFrame->show();
   
   QTextEdit *choiceTE; choiceBG=new QButtonGroup(this); 
   QLabel* choicesLabel=new QLabel(tr("Seçenekler:"),this);
   choicesLabel->setGeometry(10,210,60,20); choicesLabel->show();
   choiceTE=new QTextEdit(this); choiceTE->setGeometry(50,240,560,40); choiceTE->show(); choices.append(choiceTE);
   choiceRB=new QRadioButton(this); choiceRB->setGeometry(30,250,20,20); choiceBG->addButton(choiceRB,1);
   choiceTE=new QTextEdit(this); choiceTE->setGeometry(50,290,560,40); choiceTE->show(); choices.append(choiceTE);
   choiceRB=new QRadioButton(this); choiceRB->setGeometry(30,300,20,20); choiceBG->addButton(choiceRB,2);
   choiceTE=new QTextEdit(this); choiceTE->setGeometry(50,340,560,40); choiceTE->show(); choices.append(choiceTE);
   choiceRB=new QRadioButton(this); choiceRB->setGeometry(30,350,20,20); choiceBG->addButton(choiceRB,3);
   choiceTE=new QTextEdit(this); choiceTE->setGeometry(50,390,560,40); choiceTE->show(); choices.append(choiceTE);
   choiceRB=new QRadioButton(this); choiceRB->setGeometry(30,400,20,20); choiceBG->addButton(choiceRB,4);
   choiceTE=new QTextEdit(this); choiceTE->setGeometry(50,440,560,40); choiceTE->show(); choices.append(choiceTE);
   choiceRB=new QRadioButton(this); choiceRB->setGeometry(30,450,20,20); choiceBG->addButton(choiceRB,5);
   
   QLabel* xmlLabel=new QLabel(tr("Moodle XML Çıktısı:"),this);
   xmlLabel->setGeometry(630,10,140,20); xmlLabel->show();
   xmlTE=new QTextEdit(this); xmlTE->setGeometry(630,40,350,450); xmlTE->show();
   
   // -----
   
   prevIndex=-1; qSelectCB->setCurrentIndex(0);
   
   
   
   clearButton=new QPushButton(tr("Temizle"),this);
   clearButton->setGeometry(140,APP_HEIGHT-54,120,40);
   connect(clearButton,SIGNAL(clicked()),this,SLOT(clearSlot()));
   
   generateButton=new QPushButton(tr("XML oluştur"),this);
   generateButton->setGeometry(280,APP_HEIGHT-54,120,40);
   connect(generateButton,SIGNAL(clicked()),this,SLOT(xmlSlot()));
   
   xmlSaveButton=new QPushButton(tr("Final XML kaydet..."),this);
   xmlSaveButton->setGeometry(APP_WIDTH-370,APP_HEIGHT-54,120,40);
   connect(xmlSaveButton,SIGNAL(clicked()),this,SLOT(xmlSaveSlot()));
   
   quitButton=new QPushButton(tr("Çıkış"),this);
   quitButton->setGeometry(APP_WIDTH-140,APP_HEIGHT-54,120,40);
   connect(quitButton,SIGNAL(clicked()),this,SLOT(quitSlot()));
   
   selectQuestionSlot(0);

   show();
  }

 public slots:
  void clearSlot() {
   resetQuestion(qSelectCB->currentIndex());
   selectQuestionSlot(qSelectCB->currentIndex());
  }
  
  void selectQuestionSlot(int q) {
   if (prevIndex>=0 && prevIndex!=q) { // Write old
    question[prevIndex].dept=deptCB->currentIndex();
    question[prevIndex].tag=tagLE->text();
    question[prevIndex].body=bodyTE->toPlainText();
    question[prevIndex].choice[0]=choices[0]->toPlainText();
    question[prevIndex].choice[1]=choices[1]->toPlainText();
    question[prevIndex].choice[2]=choices[2]->toPlainText();
    question[prevIndex].choice[3]=choices[3]->toPlainText();
    question[prevIndex].choice[4]=choices[4]->toPlainText();
    if (choiceBG->checkedId()>0) question[prevIndex].correctAnswer=choiceBG->checkedId()-1;
    question[prevIndex].xml=xmlTE->toPlainText();
   }
   // Set new
   deptCB->setCurrentIndex(question[q].dept);
   tagLE->setText(question[q].tag);
   bodyTE->clear(); bodyTE->insertPlainText(question[q].body);
   choices[0]->clear(); choices[0]->insertPlainText(question[q].choice[0]);
   choices[1]->clear(); choices[1]->insertPlainText(question[q].choice[1]);
   choices[2]->clear(); choices[2]->insertPlainText(question[q].choice[2]);
   choices[3]->clear(); choices[3]->insertPlainText(question[q].choice[3]);
   choices[4]->clear(); choices[4]->insertPlainText(question[q].choice[4]);
   if (choiceBG->checkedId()>0 && choiceBG->button(question[q].correctAnswer+1)>0)
	(choiceBG->button(question[q].correctAnswer+1))->setChecked(true);
   xmlTE->clear(); xmlTE->insertPlainText(question[q].xml);
   if (question[qSelectCB->currentIndex()].hasBodyFigure)
	figFrame->setImage(question[qSelectCB->currentIndex()].bodyImage);
   else figFrame->unsetImage();
   prevIndex=q;
  }
  
  void bodyFigLoadSlot() {
   QString fileName=QFileDialog::getOpenFileName(this,tr("Şekil aç..."),
                                                 "./", tr(" Files (*.png *.jpg)"));
   QImage img(fileName);
   QFile imgFile(fileName);
   if (imgFile.open(QIODevice::ReadOnly)) {
	QByteArray imageBin=imgFile.readAll();
	question[qSelectCB->currentIndex()].hasBodyFigure=true;
	question[qSelectCB->currentIndex()].bodyFigure=QString(imageBin.toBase64());
	question[qSelectCB->currentIndex()].bodyImage=img;
	question[qSelectCB->currentIndex()].bodyImageFN=QString::number(qSelectCB->currentIndex()+1).append(".png");
	imgFile.close();
    figFrame->setImage(question[qSelectCB->currentIndex()].bodyImage);
   } else {
	QMessageBox msgBox; msgBox.setText(tr("Dosya açılamadı!")); msgBox.exec(); 
   }
  }
  
  void xmlSaveSlot() {
   QString fileName=QFileDialog::getSaveFileName(this,tr("XML kaydet..."),
                                                 "./sorular.xml", tr("XML Files (*.xml)"));
   QFile xmlFile(fileName);
   if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
	QTextStream xmlStream(&xmlFile);
	xmlStream.setCodec("UTF-8");
	xmlAllOutput(&xmlStream);
	xmlFile.close();
	statusBar->showMessage("Kaydedildi..",5000);
   } else {
	QMessageBox msgBox; msgBox.setText(tr("Dosya kaydedilemedi!")); msgBox.exec(); 
   }
  }

  void xmlSlot() {
   if (choiceBG->checkedId()==-1) {
	QMessageBox msgBox; msgBox.setText(tr("Doğru şık seçimi yapmadınız!")); msgBox.exec(); 
   } else {	   
	QString output;
    output.append("<!-- ========= SORU ");
    output.append(QString::number(qSelectCB->currentIndex()+1));
    output.append(" ========= -->\n<question type=\"multichoice\"><name><text>\n");
    output.append(deptNames[deptCB->currentIndex()]);
    output.append("::"); output.append(tagLE->text());
    output.append("\n</text></name><questiontext format=\"html\"><text><![CDATA[\n");
    output.append(bodyTE->toPlainText());
    output.append("\n]]></text></questiontext>\n\n");
    for (int i=0;i<5;i++) {
     output.append("<answer fraction=\"");
     if (i==choiceBG->checkedId()-1) output.append("100"); else output.append("0");
     output.append("\"><text>\n");
     output.append(choices[i]->toPlainText());
     output.append("\n</text></answer>\n\n");
    }
    output.append("<shuffleanswers>1</shuffleanswers>\n");
    output.append("<single>true</single>\n");
    output.append("<answernumbering>none</answernumbering>\n");
    if (question[qSelectCB->currentIndex()].hasBodyFigure) {
	 output.append("\n<image>");
	 output.append(question[qSelectCB->currentIndex()].bodyImageFN);
     output.append("</image>\n<image_base64>\n");
	 output.append(question[qSelectCB->currentIndex()].bodyFigure);
	 output.append("\n</image_base64>\n");
    }
    output.append("</question>\n");
	xmlTE->setText(output);
   }
  }
  
  void quitSlot() {
   close();
  }

 private:
  void xmlAllOutput(QTextStream* s) {
   *s << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<quiz>\n\n";
   for (int i=0;i<question.size();i++) {
	*s << "<!-- ========= SORU ";
	*s << QString::number(i+1);
	*s << " ========= -->\n<question type=\"multichoice\"><name><text>\n";
	*s << deptNames[question[i].dept];
	*s << "::";
	*s << question[i].tag;
	*s << "\n</text></name><questiontext format=\"html\"><text><![CDATA[\n";
	*s << question[i].body;
	*s << "<p>\n]]></text></questiontext>\n\n";
	for (int j=0;j<5;j++) {
	 *s << "<answer fraction=\"";
	 if (j==question[i].correctAnswer) *s << "100"; else *s << "0";
	 *s << "\" format=\"html\"><text><![CDATA[\n";
	 *s << question[i].choice[j];
	 *s << "\n]]></text></answer>\n\n";
    }
    *s << "<single>true</single>\n";
    *s << "<shuffleanswers>1</shuffleanswers>\n";
    *s << "<answernumbering>abc</answernumbering>\n"; // none
	*s << "<showstandardinstruction>0</showstandardinstruction>\n";
    if (question[i].hasBodyFigure) {
     *s << "\n<image>";
     *s << question[i].bodyImageFN;
     *s << "</image>\n<image_base64>\n";
	 *s << question[i].bodyFigure;
	 *s << "\n</image_base64>\n";
    }
	*s << "</question>\n\n";
   }	
   *s << "</quiz>\n";
  }
 
  void resetQuestion(int i) {
   question[i].dept=0;
   question[i].tag=tr("Soru rumuzu"); question[i].body=tr("Soru gövdesini giriniz.");
   question[i].choice[0]=tr("İlk şıkkı giriniz.");
   question[i].choice[1]=tr("İkinci şıkkı giriniz.");
   question[i].choice[2]=tr("Üçüncü şıkkı giriniz.");
   question[i].choice[3]=tr("Dördüncü şıkkı giriniz.");
   question[i].choice[4]=tr("Beşinci şıkkı giriniz.");
   question[i].correctAnswer=-1;
   question[i].hasBodyFigure=false;
   question[i].bodyFigure.clear();
   question[i].bodyImageFN.clear();
   question[i].xml=tr("\"XML oluştur\" butonuna bastıktan sonra burada oluşan kodu kullanablirsiniz.");
  }

  void populateDepts() {
   deptNames.append(tr("Akademik Okuryazarlık"));
   deptNames.append(tr("Anatomi"));
   deptNames.append(tr("Biyofizik"));
   deptNames.append(tr("Biyokimya"));
   deptNames.append(tr("Biyomedikal Fizik"));
   deptNames.append(tr("Çocuk Sağlığı ve Hastalıkları"));
   deptNames.append(tr("Fizyoloji"));
   deptNames.append(tr("Genel Cerrahi"));
   deptNames.append(tr("Histoloji"));
   deptNames.append(tr("İç Hastalıkları"));
   deptNames.append(tr("Kadın Hastalıkları ve Doğum"));
   deptNames.append(tr("Nükleer Tıp"));
   deptNames.append(tr("Radyasyon Onkolojisi"));
   deptNames.append(tr("Radyoloji"));
   deptNames.append(tr("Semptomdan Ayırıcı Tanıya Çaplı Düşünme"));
   deptNames.append(tr("Tıbbi Biyoloji"));
   deptNames.append(tr("Tıbbi Biyokimya"));
   deptNames.append(tr("Tıbbi Farmakoloji"));
   deptNames.append(tr("Tıbbi Genetik"));
   deptNames.append(tr("Tıbbi İstatistik"));
   deptNames.append(tr("Tıbbi Mikrobiyoloji"));
   deptNames.append(tr("Tıbbi Patoloji"));
   deptNames.append(tr("Tıp Eğitimi ve Bilişimi"));
   deptNames.append(tr("Tıp Tarihi ve Etik"));
  }

  QVector<QString> deptNames;
  QVector<Question> question; int prevIndex,questionCount;
  
  QComboBox *qSelectCB,*deptCB; QLineEdit *tagLE; QButtonGroup *choiceBG; 
  QTextEdit *bodyTE,*xmlTE;
  
  FigureFrame* figFrame;
  
  QRadioButton *choiceRB; QVector<QTextEdit*> choices;
  
  QPushButton *xmlSaveButton,*clearButton,*bodyFigLoadButton,*generateButton,*quitButton;
  QStatusBar *statusBar;
};

#endif
