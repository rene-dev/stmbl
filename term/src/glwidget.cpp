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

GLWidget::GLWidget(QWidget * parent) :
	QOpenGLWidget(parent)
{

}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_vao.create();
	m_vao.bind();

	QVector<GLfloat> data;

	for(int i = -2000; i <= 2000; i++) {
		data.append(i);
		data.append(sin(i));
	}

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(data.constData(), data.count() * sizeof(GLfloat));

	//todo: find better method for shaderpathhandling
	QOpenGLShader* vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
	vs->compileSourceFile("../shader/main.vs");
	QOpenGLShader* fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
	fs->compileSourceFile("../shader/main.fs");

	m_shader = new QOpenGLShaderProgram();
	m_shader->addShader(vs);
	m_shader->addShader(fs);
	m_shader->link();
	m_shader->bind();
	m_shader->enableAttributeArray("vertex");
	m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 2, 0);

	m_shader->release();
	m_vao.release();
}

void GLWidget::paintGL()
{
	glClearColor(0.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_vao.bind();
	m_shader->bind();
	m_vbo.bind();

	QMatrix4x4 m;
	m.ortho(-100.5f, +100.5f, +100.5f, -100.5f, 0.0f, 15.0f);
	m.translate(0.0f, 0.0f, -10.0f);

	m_shader->setUniformValue("mvp", m);
	m_shader->enableAttributeArray("vertex");
	m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 2, 0);

	glDrawArrays(GL_LINE_STRIP, 0, 4000);

	m_shader->release();
	m_vao.release();
}

void GLWidget::resizeGL(int w, int h)
{
	int side = qMin(w, h);
	glViewport((w - side) / 2, (h - side) / 2, side, side);
}
