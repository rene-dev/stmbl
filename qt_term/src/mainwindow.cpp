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

For feedback and questions about stmbl please e-mail one of the authors named in
the AUTHORS file.
*/

#include <include/mainwindow.hpp>
#include <QKeyEvent>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		historypos(0)
{
	setupUi(this);
	lineEdit->setFocus();
    lineEdit->installEventFilter(this);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(pollTimerEvent()));
	timer->setInterval(50);
    timer->setTimerType(Qt::PreciseTimer);
	timer->start();
}

bool MainWindow::eventFilter(QObject* obj, QEvent *event){
    if (obj == lineEdit){
        if (event->type() == QEvent::KeyPress){
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            
            if( (keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) {
                if((history.size()==0 || history.back() != lineEdit->text()) && !lineEdit->text().isEmpty()){
                    history.push_back(lineEdit->text());
                }
                historypos = history.size();
                
                textEdit->append(lineEdit->text());
                lineEdit->clear();
                return true;
            }
            
            if(keyEvent->key() == Qt::Key_Up) {
                if(historypos > 0 && history.size()>0){
                    lineEdit->setText(history.at(--historypos));
                    //textinput->SetInsertionPointEnd();
                }
                return true;
            }

            if(keyEvent->key() == Qt::Key_Down) {
                if(historypos < history.size()-1 && history.size()>0){
                    lineEdit->setText(history.at(++historypos));
                    //textinput->SetInsertionPointEnd();
                }else if(historypos < history.size()){
                    historypos++;
                    lineEdit->clear();
                }
                return true;
            }
        }
        return false;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_actionReset_triggered()
{
	openGLWidget->resetMatrix();
}

void MainWindow::on_actionResetMatrix_triggered()
{
	openGLWidget->resetMatrix();
}

void MainWindow::pollTimerEvent()
{
    static int i = 0;
	openGLWidget->m_function1.addPoint(20.0*sin(i++/10.0));
}

void MainWindow::on_actionExit_triggered()
{
	close();
}
