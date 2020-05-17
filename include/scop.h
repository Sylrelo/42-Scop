










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

/*
** Object parsing utils
*/

typedef struct	s_obj_parse
{
	FILE		*fp;
	size_t		read;
	size_t		len;
	char		*line;
	short int	percent_current;
	short int	percent_old;
	int			nb_v;
	int			nb_vt;
	int			nb_vn;
	int			nb_f;
	int			nb_mats;
	off_t		filesize;
	off_t		readsize;
	char		*mtl_name;
}				t_obj_parse;

typedef enum	e_parse_enum
{
	SINGLE = 2,
	WITH_NORMAL = 4,
	WITH_TEXCOORDS = 8,
	COMPLETE = 16
}				t_parse_enum;

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

typedef struct		s_mtl
{
	char			*name;
	char			*texture_file;
	t_vec3f			color_ambiant;
	t_vec3f			color_diffuse;
	t_vec3f			color_specular;
}					t_mtl;

typedef struct		s_obj
{
	t_vec3f		*vertices;
	int			nb_vertices;
	t_vec2f		*texture_coords;
	int			nb_texture_coords;
	t_vec3f		*normals;
	int			nb_normals;
	t_mtl		*materials;
	int			nb_mats;
}					t_obj;

typedef struct	s_ogl
{
	unsigned int	vao;
	unsigned int	vbo;
	unsigned int	ebo;

}				t_ogl;

typedef struct	s_app
{
	GLFWwindow		*win;
	int				program;

	t_vec3f			cam_pos;
	t_vec3f			cam_rot;

	t_vec3f			obj_center;
	
	uint8_t			display_mode;
	t_ogl			ogl;

	int				nb_data;
	float			*vertices;
	int				nb_vertices;

	unsigned int	*indexes;
	int				nb_indexes;
	
}				t_app;

int			create_shader_program();



/*
** BMP Parser
*/
unsigned int	*bmp_parse(char *file);

/*
** Matrices
*/
void 		mat4_init(mat4f out);
void		mat4_rotate(mat4f result, t_vec3f rot);
void		mat4_scale(mat4f result, t_vec3f scale);
void		mat4_translate(mat4f result, t_vec3f pos);
void		mat4_mult(mat4f result, mat4f a, mat4f b);
void		mat4_perspective(mat4f out, float fov, float aspect, float near_plane, float far_plane);

/* 
** Object Parsing
*/
void		obj_open_file(t_app *e, char *file);
void		open_file(t_app *e, char *file);
void		obj_parse_face(t_app *e, t_obj *obj, char *line);
void 		obj_parse_show_progress(t_obj_parse *buff);
void 		obj_parse_show_infos(t_obj_parse *buff);


/*
** Utils
*/
uint8_t		face_type(char *line);
int			chrcount(char *line, char c);
int			chrat(char *line, char c);
char		*strafterocc(char *line, char c);


/*
** OpenGL
*/
void		ogl_geterror();
void		ogl_attrib(t_app *e);
void		ogl_init_buffers(t_app *e);

#endif