/*!
 * \section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>

#include "assets/assetpaths.h"
#include "ui/mainwindow.h"

void extractEmbeddedCrosswordPuzzles()
{
    QDir destDir(assets::getExternalCrosswordsFolderPath());

    if (!destDir.exists()) {
        destDir.mkpath(".");
    }

    QDirIterator it(assets::getEmbeddedCrosswordsFolderPath(), QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        QFile::copy(it.filePath(), destDir.absolutePath() + "/" + it.fileName());
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    extractEmbeddedCrosswordPuzzles();

    ui::MainWindow w;
    w.show();
    return a.exec();
}
