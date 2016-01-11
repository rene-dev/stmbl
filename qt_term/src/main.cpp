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

#include <QApplication>

#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "SystemLocation: " << info.systemLocation();
        
        /* STMBL:
        Name :  "cu.usbmodem1D1441"
        Description :  "STMBL Virtual ComPort" <--- darauf matchen
        Manufacturer:  "STMicroelectronics"
        SystemLocation:  "/dev/cu.usbmodem1D1441"
        */
        
        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
            serial.close();
    }

	MainWindow w;
	w.show();

	return a.exec();
}
