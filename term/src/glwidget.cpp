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

#include <include/glwidget.hpp>

#include <QVector>

#include <conf/config.h>

GLWidget::GLWidget(QWidget * parent) :
	QOpenGLWidget(parent)
{

}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	QVector<GLfloat> data;

	for(int i = 0; i <= 2000; i++) {
		data.append(sin(i * 10.0) / (1.0 + i * i));
	}

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(data.constData(), data.count() * sizeof(GLfloat));

	QOpenGLShader* vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
	vs->compileSourceFile("shader/main.vs");
	QOpenGLShader* fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
	fs->compileSourceFile("shader/main.fs");

	m_shader = new QOpenGLShaderProgram();
	m_shader->addShader(vs);
	m_shader->addShader(fs);
	m_shader->bindAttributeLocation("vertex", 0);
	m_shader->link();
	m_shader->bind();

}

void GLWidget::paintGL()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_POINTS, 0, 2000);
}
