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


void		mat_cpy(mat4f dest, mat4f src)
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

void		mat_empty(mat4f mat)
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

void		mat4_mult(mat4f result, mat4f a, mat4f b)
{
	int			i;
	int			j;
	int			k;
	mat4f		cpa;
	mat4f		cpb;

	mat_cpy(cpa, a);
	mat_cpy(cpb, b);
	mat_empty(result);
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			k = -1;
			while (++k < 4)
				result[i][j] += cpa[i][k] * cpb[k][j];
		}
	}
}

void		mat4_rotate(mat4f result, t_vec3f rot)
{
	mat4f	rot_x;
	mat4f	rot_y;
	mat4f	rot_z;
	mat4f	rot_xy;

	mat_init(rot_x);
	mat_init(rot_y);
	mat_init(rot_z);
	rot_x[1][1] = cos(rot.x);
	rot_x[1][2] = -sin(rot.x);
	rot_x[2][1] = sin(rot.x);
	rot_x[2][2] = cos(rot.x);
	rot_y[0][0] = cos(rot.y);
	rot_y[0][2] = sin(rot.y);
	rot_y[2][0] = -sin(rot.y);
	rot_y[2][2] = cos(rot.y);
	rot_z[0][0] = cos(rot.z);
	rot_z[1][0] = -sin(rot.z);
	rot_z[0][1] = sin(rot.z);
	rot_z[1][1] = cos(rot.z);
	mat4_mult(rot_xy, rot_x, rot_y);
	mat4_mult(result, rot_xy, rot_z);
}

void		mat4_scale(mat4f result, t_vec3f scale)
{
	mat_init(result);
	result[0][0] = scale.x;
	result[1][1] = scale.y;
	result[2][2] = scale.z;
}

void		mat4_translate(mat4f result, t_vec3f pos)
{
	mat_init(result);
	result[3][0] = pos.x;
	result[3][1] = pos.y;
	result[3][2] = pos.z;
}


void		mat_perspective(mat4f result, float fov, float aspect, float near_plane, float far_plane)
{
	float	tan_half_fov;
	
	mat_init(result);
	tan_half_fov = tanf(fov / 2.0f);
	result[0][0] = 1.0f / (aspect * tan_half_fov);
	result[1][1] = 1.0f / (tan_half_fov);
	result[2][2] = (far_plane + near_plane) / (far_plane - near_plane) * -1;
	result[2][3] = -1.0f;
	result[3][2] = (2.0f * far_plane * near_plane) / (far_plane - near_plane) * -1;
}