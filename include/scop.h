










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
#include <ctype.h>

#define PRGS_CHAR "#"

typedef GLfloat	mat4f[4][4];

/*
** Object parsing utils
*/

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

typedef struct 	s_matindex
{
	char			name[256];
	int				count;
	int				index;
}				t_matindex;

typedef struct 	s_fpm
{
	char			**name;
	int				count;
	char			**faces;
}				t_fpm;

typedef struct	s_draw
{
	t_vec3f		kd;
	int			count;
	int			texture;
	int			obj_index;
}				t_draw;


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
	char		mtl_name[256];

	char		previous_mat[256];

	char			***faces;
	int				mat_count;
	t_matindex		*mat_indexes;


}				t_obj_parse;

typedef enum	e_parse_enum
{
	SINGLE = 2,
	WITH_NORMAL = 4,
	WITH_TEXCOORDS = 8,
	COMPLETE = 16
}				t_parse_enum;

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
}					t_obj;

typedef struct	s_ogl
{
	unsigned int	vao;
	unsigned int	vbo;
	unsigned int	ebo;

}				t_ogl;

typedef struct  s_fdata
{
    float           pos[9];
    float           normals[9];
    float           texcoords[6];
    unsigned int    color;
    unsigned int    idx;
}               t_fdata;


typedef struct	s_app
{
	GLFWwindow		*win;
	int				program;

	t_vec3f			cam_pos;
	t_vec3f			cam_rot;
	int				scale;
	
	t_vec3f			obj_center;
	
	uint8_t			display_mode;
	t_ogl			ogl;

	int				nb_data;

	t_vec3f			min;
	t_vec3f			max;
	t_vec3f			center;

	t_draw			*draw_buffer;
	int				draw_buffer_count;
	//volatile int	parsing_done;

    //int             nb_poly;

    int             nb_tri;

	//float			*vertices;
	//int				nb_vertices;

	//unsigned int	*indexes;
	//int				nb_indexes;

	int             offset;
	//int				test;
}				t_app;

int			create_shader_program();



/*
** BMP Parser
*/
unsigned int	*bmp_parse(char *file);

/*
**
*/
t_vec3f		vec_op(t_vec3f u, char c, t_vec3f v);
float		vec_norm(t_vec3f u);
t_vec3f		vec_normalize(t_vec3f u);
float		vec_dot(t_vec3f u, t_vec3f v);
t_vec3f		vec_multf(t_vec3f u, float f);


/*
** Matrices
*/
void 		mat4_init(mat4f out);
void		mat4_rotate(mat4f result, t_vec3f rot);
void		mat4_scale(mat4f result, t_vec3f scale);
void		mat4_translate(mat4f result, t_vec3f pos);
void		mat4_mult(mat4f result, mat4f a, mat4f b);
void		mat4_perspective(mat4f out, float fov, float aspect, float near_plane, float far_plane);

void		mat4_rotate_around(mat4f result, t_vec3f center, t_vec3f rot);

/* 
** Object Parsing
*/
void		obj_open_file(t_app *e, char *file);
void		open_file(t_app *e, char *file);
void		obj_parse_face(t_app *e, t_obj *obj, char *line, int index);
void 		obj_parse_show_progress(t_obj_parse *buff);
void 		obj_parse_show_infos(t_obj_parse *buff);
off_t		get_filesize(char *file);
void		prepare(t_obj_parse *buff);
void		allocate_arrays(t_app *e, t_obj *obj, t_obj_parse *buff);

void        file_firstpass(t_app *e, char *file, t_obj_parse *buff, t_obj *obj);
void        file_secondpass(t_app *e, t_obj_parse *buff, t_obj *obj);

void		materials_indexing(t_obj_parse *buff, char *line);
void		mtidx_allocfaces(t_obj_parse *buff);
void		mtidx_addface(t_obj_parse *buff);
void		mtidx_populate(t_obj_parse *buff);
void		mtidx_iterate(t_app *e, t_obj *obj, t_obj_parse *buff);

void		mtl_readfile(t_app *e, t_obj_parse *buff);

/*
** Utils
*/
uint8_t		face_type(char *line);
int			chrcount(char *line, char c);
int			chrat(char *line, char c);
char		*strafterocc(char *line, char c);

float		rgbfloat(float r, float g, float b);
char *trim(char *s);

/*
** OpenGL
*/
void		ogl_geterror();
void		ogl_attrib(t_app *e);
void		ogl_init_buffers(t_app *e);

void    addTriangle(t_app *e, float *vertices, uint8_t f_type);


void	printmat(mat4f mat);

void	matmat(mat4f res, t_vec3f t, t_vec3f r, t_vec3f center, int scale);

#endif