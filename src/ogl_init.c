#include "scop.h"

void	ogl_geterror()
{
	const int err = glGetError();

	if (err)
	{
		printf("[OpenGL] [Error] %d\n", err);
		exit (EXIT_FAILURE);
	}
}

void	ogl_attrib(t_app *e)
{
   	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
   	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
	   	e->nb_data * sizeof(float), (void *)0);
	ogl_geterror();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		e->nb_data * sizeof(float), (void *)(3 * sizeof(float)));
	ogl_geterror();
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		e->nb_data * sizeof(float), (void *)(6 * sizeof(float)));
	ogl_geterror();
   	glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void	ogl_init_buffers(t_app *e)
{
	glGenVertexArrays(1, &e->ogl.vao);
	ogl_geterror();
	glGenBuffers(1, &e->ogl.vbo);
	ogl_geterror();
	glGenBuffers(1, &e->ogl.ebo);	
	ogl_geterror();
	glBindVertexArray(e->ogl.vao);	
	ogl_geterror();
	glBindBuffer(GL_ARRAY_BUFFER, e->ogl.vbo);
	ogl_geterror();
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * (e->nb_vertices), e->vertices, GL_STATIC_DRAW);
	ogl_geterror();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e->ogl.ebo);
	ogl_geterror();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(unsigned int) * (e->nb_indexes), e->indexes, GL_STATIC_DRAW);
	ogl_geterror();
}
