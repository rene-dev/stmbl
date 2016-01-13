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

#include <QVector>

using namespace std;

#define VBO_SIZE 20000

FunctionGraph::FunctionGraph() :
	m_xpos(0),
	m_datasize(0)
{
}

void FunctionGraph::initializeGL()
{
	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(VBO_SIZE * sizeof(GLfloat));

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	if(f) {
		f->glEnableVertexAttribArray(0);
		f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	} else {
		qWarning("couldn't get function context");
	}

	m_vao.release();
	m_vbo.release();
}

void FunctionGraph::paintGL()
{
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	if(f && m_datasize) {
		f->glDrawArrays(GL_LINE_STRIP, 0, m_datasize / 2);
	}

	m_vao.release();
}

void FunctionGraph::addPoint(float y)
{
	m_vbo.bind();
	QVector<GLfloat> data;

	data.append(m_xpos++);
	data.append(y);

	m_vbo.write(m_datasize * sizeof(GLfloat), data.constData(), data.count() * sizeof(GLfloat));
	m_datasize += data.count();
	m_vbo.release();
}

void FunctionGraph::restart()
{
	m_xpos = 0;
	m_datasize = 0;
}
