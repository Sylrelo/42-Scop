#include "scop.h"

void 		mat_init(mat4f out)
{
	out[0][0] = 1.f;
	out[0][1] = 0.f;
	out[0][2] = 0.f;
	out[0][3] = 0.f;
	out[1][0] = 0.f;
	out[1][1] = 1.f;
	out[1][2] = 0.f;
	out[1][3] = 0.f;
	out[2][0] = 0.f;
	out[2][1] = 0.f;
	out[2][2] = 1.f;
	out[2][3] = 0.f;
	out[3][0] = 0.f;
	out[3][1] = 0.f;
	out[3][2] = 0.f;
	out[3][3] = 1.f;
}
/*
void		mat_translate(mat4f in, mat4f out, t_vec3f vec)
{
	out[0][0] = in[0][0];
	out[0][1] = in[0][1];
	out[0][2] = in[0][2];
	out[0][3] = in[0][3];
	out[1][0] = in[1][0];
	out[1][1] = in[1][1];
	out[1][2] = in[1][2];
	out[1][3] = in[1][3];
	out[2][0] = in[2][0];
	out[2][1] = in[2][1];
	out[2][2] = in[2][2];
	out[2][3] = in[2][3];
	out[3][0] = vec.x;
	out[3][1] = vec.y;
	out[3][2] = vec.z;
	out[3][3] = in[3][3];
}*/

void			mat_cpy(mat4f dest, mat4f src)
{
	int	i;
	int	j;

	if (!dest || !src)
		return ;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			dest[i][j] = src[i][j];
	}
}

void			mat_empty(mat4f mat)
{
	int		i;
	int		j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			mat[i][j] = 0.0;
	}
}

void		mat4_print(mat4f in)
{
	for(int i = 0; i < 4; i++)
	{
		printf("%+0.3f %+0.3f %+0.3f %+0.3f\n", in[i][0], in[i][1], in[i][2], in[i][3]);
	}
}

void			mat_mult(mat4f orig, mat4f new)
{
	mat4f		tmp;
	int			i;
	int			j;
	int			k;

	mat_cpy(tmp, orig);
	mat_empty(orig);
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			k = -1;
			while (++k < 4)
				orig[i][j] += tmp[i][k] * new[k][j];
		}
	}
}

void			mat_rotx(mat4f mat, double vcos, double vsin)
{
	mat4f		tmp;

	mat_init(tmp);
	tmp[1][1] = vcos;
	tmp[1][2] = -vsin;
	tmp[2][1] = vsin;
	tmp[2][2] = vcos;
	mat_mult(mat, tmp);
}

void			mat_roty(mat4f mat, double vcos, double vsin)
{
	mat4f		tmp;

	mat_init(tmp);
	tmp[0][0] = vcos;
	tmp[0][2] = vsin;
	tmp[2][0] = -vsin;
	tmp[2][2] = vcos;
	mat_mult(mat, tmp);
}

void			mat_rotz(mat4f mat, double vcos, double vsin)
{
	mat4f		tmp;

	mat_init(tmp);
	tmp[0][0] = vcos;
	tmp[1][0] = -vsin;
	tmp[0][1] = vsin;
	tmp[1][1] = vcos;
	mat_mult(mat, tmp);
}

void			mat_rotation(mat4f in, mat4f out, float radx, float rady, float radz)
{
	mat4f rot_x;
	mat4f rot_y;
	mat4f rot_z;

	mat_init(rot_x);
	rot_x[1][1] = cos(radx);
	rot_x[1][2] = -sin(radx);
	rot_x[2][1] = sin(radx);
	rot_x[2][2] = cos(radx);
	mat_init(rot_y);
	rot_y[0][0] = cos(rady);
	rot_y[0][2] = sin(rady);
	rot_y[2][0] = -sin(rady);
	rot_y[2][2] = cos(rady);
	mat_init(rot_z);
	rot_z[0][0] = cos(radz);
	rot_z[1][0] = -sin(radz);
	rot_z[0][1] = sin(radz);
	rot_z[1][1] = cos(radz);
	mat_mult(rot_x, rot_y);
	mat_mult(rot_x, rot_z);
	mat_cpy(out, rot_x);
}

void			mat_scale(mat4f in_out, float scale_x, float scale_y, float scale_z)
{
	mat4f scale;

	mat_init(scale);
	scale[0][0] = scale_x;
	scale[1][1] = scale_y;
	scale[2][2] = scale_z;
	mat_mult(in_out, scale);
}

void			mat_translate(mat4f mat, t_vec3f pt)
{
	mat4f		translate;

	mat_init(translate);
	translate[3][0] = pt.x;
	translate[3][1] = pt.y;
	translate[3][2] = pt.z;
	mat_mult(mat, translate);
}


void		mat_perspective(mat4f out, float fov, float aspect, float near_plane, float far_plane)
{
	float	tan_half_fov;
	
	mat_init(out);
	tan_half_fov = tanf(fov / 2.0f);
	out[0][0] = 1.0f / (aspect * tan_half_fov);
	out[1][1] = 1.0f / (tan_half_fov);
	out[2][2] = (far_plane + near_plane) / (far_plane - near_plane) * -1;
	out[2][3] = -1.0f;
	out[3][2] = (2.0f * far_plane * near_plane) / (far_plane - near_plane) * -1;
}