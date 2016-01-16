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

#include <include/glwidget.hpp>

#include <QVector>
#include <QMouseEvent>
#include <math.h>

#include <QKeyEvent>

static const char *vertexShaderSource =
				"#version 120\n"
				"uniform mat4 mvp;\n"
				"void main(void)\n"
				"{\n"
				"    gl_Position = mvp * gl_Vertex;\n"
				"}\n";

static const char *fragmentShaderSource =
				"#version 120\n"
				"void main(void)\n"
				"{\n"
				"    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
				"}\n";

GLWidget::GLWidget(QWidget * parent) :
		QOpenGLWidget(parent)
{
	m_translating = false;
	m_scaling = false;

	m_translation.setX(0.0f);
	m_translation.setY(0.0f);
	m_translation.setZ(-10.0f);
	m_scalation.setX(1.0f);
	m_scalation.setY(1.0f);
	m_scalation.setZ(1.0f);

	updateMatrix();
}

void GLWidget::resetMatrix()
{
	m_translation.setX(0.0f);
	m_translation.setY(0.0f);
	m_translation.setZ(-10.0f);
	m_scalation.setX(1.0f);
	m_scalation.setY(1.0f);
	m_scalation.setZ(1.0f);

	repaint();
}

void GLWidget::updateMatrix()
{
	m_matrix.setToIdentity();
	m_matrix.ortho(-width()/2, +width()/2, +height()/2, -height()/2, 0.0f, 15.0f);
	m_matrix.translate(m_translation);
	m_matrix.scale(m_scalation);
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	m_vbo.create();
	m_vbo.bind();

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	if(f) {
		f->glEnableVertexAttribArray(0);
		f->glEnableVertexAttribArray(1);
		f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	} else {
		qWarning("couldn't get function context");
	}

	QVector<GLfloat> data;

	data.append(-10000000.0);
	data.append(0.0);
	data.append(10000000.0);
	data.append(0.0);

	data.append(0.0);
	data.append(-10000000.0);
	data.append(0.0);
	data.append(10000000.0);

	m_vbo.allocate(data.constData(), data.count() * sizeof(GLfloat));

	m_vao.release();
	m_vbo.release();

	m_function1.initializeGL();

	for(int i = 0; i < 1000; i++) {
		m_function1.addPoint(sin(i));
	}

	m_shader = new QOpenGLShaderProgram();
	m_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_shader->link();
	m_shader->bind();
	m_shader->enableAttributeArray("vertex");
	m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 2, 0);

	m_shader->release();
}

void GLWidget::paintGL()
{
	//monoclock::time_point t = monoclock::now();

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader->bind();

	updateMatrix();

	m_shader->setUniformValue("mvp", m_matrix);
	m_shader->enableAttributeArray("vertex");
	m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 2, 0);

	m_function1.paintGL();

	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	if(f) {
		f->glDrawArrays(GL_LINES, 0, 4);
	}

	m_vao.release();

	m_shader->release();

	update();

	//monoclock::time_point t2 = monoclock::now();
	//auto seconds_passed = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1> > >(t2 - t).count();
	//qDebug() << 1 / seconds_passed;
}

void GLWidget::resizeGL(int w, int h)
{
	int side = qMin(w, h);
	glViewport((w - side) / 2, (h - side) / 2, side, side);
	updateMatrix();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton) {
		m_translating = true;
		m_transpos = event->pos();
	}
	if(event->buttons() == Qt::RightButton) {
		m_scaling = true;
		m_scalepos = event->pos();
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(m_translating && event->buttons() == Qt::LeftButton) {
		QPoint pos = m_transpos - event->pos();

		pos.setX(-pos.x());
		pos.setY(-pos.y());

		m_translation += QVector3D(pos);

		m_transpos = event->pos();
	}

	if(m_scaling && event->buttons() == Qt::RightButton) {
		QPoint pos = m_scalepos - event->pos();

		if(pos.y() != 0) {
			m_scalation.setX(m_scalation.x() + (pos.y() / 5));
			m_scalation.setY(m_scalation.y() + (pos.y() / 5));

			if(m_scalation.x() <= 0.0f) {
				m_scalation.setX(0.05f);
				m_scalation.setY(0.05f);
				m_scalation.setZ(1.0f);
			}
		}

		m_scalepos = event->pos();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->buttons() != Qt::LeftButton)
		m_translating = false;

	if(event->buttons() != Qt::RightButton)
		m_scaling = false;
}
