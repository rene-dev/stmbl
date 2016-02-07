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

#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLContext>

#include <chrono>

typedef std::chrono::steady_clock monoclock;

class FunctionGraph
{
	private:
		QOpenGLBuffer m_vbo;
		QOpenGLVertexArrayObject m_vao;

		size_t m_xpos;
		size_t m_data_mid;
		size_t m_data_size;

		bool m_filled;

	public:
		FunctionGraph();
		void initializeGL();
		void paintGL();
		void addPoint(float y);
		void restart();
};
