










#ifndef SCOP_H
# define SCOP_H

#include <stdio.h>
#include <stdlib.h>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <string.h>

typedef GLfloat	mat4f[4][4];

typedef float	mat4[16];

typedef enum 	s_bool
{
	false,
	true
}				t_bool;

typedef struct	s_vec3f
{
	float		x;
	float		y;
	float		z;
}				t_vec3f;

typedef struct	s_3i
{
	int			a;
	int			b;
	int			c;
}				t_3i;

typedef struct	s_vec2f
{
	float		x;
	float		y;
}				t_vec2f;

typedef struct	s_vec4f
{
	float		x;
	float		y;
	float		z;
	float		w;
}				t_vec4f;

typedef struct		s_obj
{
	t_vec3f		*vertices;
	int			nb_vertices;
	t_vec2f		*texture_coords;
	int			nb_texture_coords;
	t_vec3f		*normals;
	int			nb_normals;
}					t_obj;

typedef struct	s_ogl
{
	unsigned int	vao;
	unsigned int	vbo;
	unsigned int	ebo;

}				t_ogl;

typedef struct	s_app
{
	GLFWwindow	*win;
	int			program;

	t_vec3f		cam_pos;
	t_vec3f		cam_rot;

	uint8_t		display_mode;
	t_ogl		ogl;

	int				nb_data;
	float			*vertices;
	int				nb_vertices;

	unsigned int	*indexes;
	int				nb_indexes;
	
}				t_app;

int			create_shader_program();



void 		mat_init(mat4f out);
void		mat_translate(mat4f mat, t_vec3f pt);

void		mat4_print(mat4f in);

void			mat_rotx(mat4f mat, double vcos, double vsin);
void			mat_roty(mat4f mat, double vcos, double vsin);
void			mat_rotz(mat4f mat, double vcos, double vsin);
void			mat_mult(mat4f orig, mat4f new);

void			mat_rotation(mat4f in, mat4f out, float radx, float rady, float radz);


void		mat_perspective(mat4f out, float fov, float aspect, float near_plane, float far_plane);

void			mat_scale(mat4f in_out, float scale_x, float scale_y, float scale_z);


/*
** Matrices
*/
void		mat4_rotate(mat4f result, t_vec3f rot);
void		mat4_scale(mat4f result, t_vec3f scale);
void		mat4_translate(mat4f result, t_vec3f pos);
void		mat4_mult(mat4f result, mat4f a, mat4f b);

/* 
** Object Parsing
*/
void		obj_open_file(t_app *e, char *file);
void		open_file(t_app *e, char *file);
void		obj_parse_face(t_app *e, t_obj *obj, char *line);

/*
** OpenGL
*/
void		ogl_geterror();
void		ogl_attrib(t_app *e);
void		ogl_init_buffers(t_app *e);

#endif