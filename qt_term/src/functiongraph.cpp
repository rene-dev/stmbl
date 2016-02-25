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

#include <include/functiongraph.hpp>

#include <QtOpenGLExtensions/qopenglextensions.h>

#include <QVector>

using namespace std;

#define VBO_SIZE 100

FunctionGraph::FunctionGraph() :
	m_xpos(0),
	m_data_mid(0),
	m_data_size(0),
    m_filled(false),
    m_color(0.0f,0.0f,0.0f)
{
}

void FunctionGraph::initializeGL()
{
	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(VBO_SIZE * 2 * sizeof(GLfloat));
	QVector<GLfloat> data;
	data.append(0);
	data.append(0);
	m_vbo.write(0, data.constData(), data.count() * sizeof(GLfloat));

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	if(f) {
		f->glEnableVertexAttribArray(0);
#ifdef __APPLE__
		//wat
		//http://stackoverflow.com/questions/28156524/meaning-of-index-parameter-in-glenablevertexattribarray-and-possibly-a-bug-i
		f->glEnableVertexAttribArray(1);
#endif
		f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	} else {
		qWarning("couldn't get function context");
	}

	m_vao.release();
	m_vbo.release();
}

void FunctionGraph::paintGL(QOpenGLShaderProgram* shader)
{
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if(f && shader) {
        if(m_data_size > 0) {
            shader->setUniformValue("color", m_color);
			if(!m_filled) {
				f->glDrawArrays(GL_LINE_STRIP, 0, m_data_size);
			} else {
				f->glDrawArrays(GL_LINE_STRIP, 0, m_data_mid);
				if(m_data_mid < m_data_size)
					f->glDrawArrays(GL_LINE_STRIP, m_data_mid + 1, m_data_size - m_data_mid - 1);
			}
		}
    }

	m_vao.release();
}

void FunctionGraph::addPoint(float y)
{
	m_vbo.bind();

	size_t offset = 0;
	QVector<GLfloat> data;

	if(!m_filled) {
		if(m_data_size < VBO_SIZE) {
			offset = m_data_size * 2;
			m_data_size++;
		} else {
			m_xpos = 0;
			m_data_mid = 2;
			m_filled = true;
			//keep the two front zeros
			offset = 2;
		}
	} else {
		if(m_data_mid < VBO_SIZE) {
			offset = m_data_mid * 2;
			m_data_mid++;
		} else {
			m_xpos = 0;
			m_data_mid = 2;
			offset = 2;
		}
	}

	data.append(m_xpos++);
	data.append(y);

	m_vbo.write(offset * sizeof(GLfloat), data.constData(), data.count() * sizeof(GLfloat));

    m_vbo.release();
}

void FunctionGraph::setColor(float r, float g, float b)
{
    m_color.setX(r);
    m_color.setY(g);
    m_color.setZ(b);
}

void FunctionGraph::restart()
{
	m_xpos = 0;
	m_data_size = 0;
	m_filled = false;
}
