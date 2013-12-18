/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H

char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
GLuint create_program(const char* vertexfile, const char *fragmentfile);
GLuint create_gs_program(const char* vertexfile, const char *geometryfile, const char *fragmentfile, GLint input, GLint output, GLint vertices);
GLint get_attrib(GLuint program, const char *name);
GLint get_uniform(GLuint program, const char *name);
#endif
