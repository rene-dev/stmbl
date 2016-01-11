/*
LICENSE

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.

FEEDBACK & QUESTIONS

For feedback and questions about Reunion please e-mail one of the authors named in
the AUTHORS file.
*/

#include <include/mainwindow.hpp>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
    this->setupUi(this);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if( (event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {

    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    //stub
}
