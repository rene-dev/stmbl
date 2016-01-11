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

#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

	private:
		QOpenGLShaderProgram* m_shader;
		QOpenGLBuffer m_vbo;
        QOpenGLVertexArrayObject m_vao;

        QMatrix4x4 m_matrix;
        QVector3D m_translation;
        float m_angle;
        QVector3D m_rotation;
        QVector3D m_scalation;

        bool m_translating;
        bool m_scaling;

        QPoint m_transpos;
        QPoint m_scalepos;

        void updateMatrix();

	protected:
		void initializeGL() override;
		void paintGL() override;
        void resizeGL(int w, int h) override;
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);

	public:
		explicit GLWidget(QWidget* parent = 0);
};

#endif // GLWIDGET_HPP
