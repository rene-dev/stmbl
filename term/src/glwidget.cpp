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
#include <QMouseEvent>

GLWidget::GLWidget(QWidget * parent) :
        QOpenGLWidget(parent)
{
    m_translating = false;
    m_scaling = false;

    m_translation.setX(0.0f);
    m_translation.setY(0.0f);
    m_translation.setZ(-10.0f);
    m_angle = 0.0f;
    m_rotation.setX(0.0f);
    m_rotation.setY(0.0f);
    m_rotation.setZ(0.0f);
    m_scalation.setX(1.0f);
    m_scalation.setY(1.0f);
    m_scalation.setZ(1.0f);

    updateMatrix();
}

void GLWidget::updateMatrix()
{
    m_matrix.setToIdentity();
    m_matrix.ortho(+width()/2, -width()/2, +height()/2, -height()/2, 0.0f, 15.0f);
    m_matrix.translate(m_translation);
    m_matrix.rotate(m_angle, m_rotation);
    m_matrix.scale(m_scalation);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    QVector<GLfloat> data;

    for(int i = -10000; i <= 10000; i++) {
        float f = i;
        data.append(f);
        data.append(sin(f/10)*10);
    }

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(data.constData(), data.count() * sizeof(GLfloat));

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

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
    m_vbo.release();
}

void GLWidget::paintGL()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_shader->bind();
    m_vbo.bind();

    updateMatrix();
    m_shader->setUniformValue("mvp", m_matrix);
    m_shader->enableAttributeArray("vertex");
    m_shader->setAttributeBuffer("vertex", GL_FLOAT, 0, 2, 0);

    glDrawArrays(GL_LINE_STRIP, 0, 20000);

    m_shader->release();
    m_vao.release();
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

        pos.setY(-pos.y());

        m_translation += QVector3D(pos);

        m_transpos = event->pos();
        repaint();
    }

    if(m_scaling && event->buttons() == Qt::RightButton) {
        QPoint pos = m_scalepos - event->pos();

        //todo

        m_scalepos = event->pos();
        repaint();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
        m_translating = false;

    if(event->buttons() != Qt::RightButton)
        m_scaling = false;
}
