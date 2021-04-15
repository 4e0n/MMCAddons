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

#include <QApplication>
#include <QTextCodec>
#include <QStringList>
#include "mmcutil.h"

int main(int argc,char *argv[]) {
 QApplication app(argc,argv);
 MMCUtil mmcUtil;
 return app.exec();
}
