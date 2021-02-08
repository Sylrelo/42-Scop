#include "scop.h"

void		mat4_rotate(mat4f result, t_vec3f rot)
{
	mat4f	rot_x;
	mat4f	rot_y;
	mat4f	rot_z;
	mat4f	rot_xy;

	mat4_init(rot_x);
	mat4_init(rot_y);
	mat4_init(rot_z);
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
	mat4_init(result);
	result[0][0] = scale.x;
	result[1][1] = scale.y;
	result[2][2] = scale.z;
}

void		mat4_translate(mat4f result, t_vec3f pos)
{
	mat4_init(result);
	result[3][0] = pos.x;
	result[3][1] = pos.y;
	result[3][2] = pos.z;
}


void		mat4_perspective(mat4f result, float fov, float aspect, float near_plane, float far_plane)
{
	float	tan_half_fov;
	
	mat4_init(result);
	tan_half_fov = tanf(fov / 2.0f);
	result[0][0] = 1.0f / (aspect * tan_half_fov);
	result[1][1] = 1.0f / (tan_half_fov);
	result[2][2] = (far_plane + near_plane) / (far_plane - near_plane) * -1;
	result[2][3] = -1.0f;
	result[3][2] = (2.0f * far_plane * near_plane) / (far_plane - near_plane) * -1;
}

void		mat4_rotate_around(mat4f result, t_vec3f center, t_vec3f rot)
{
	mat4f	trans;
	mat4f	trans_back;
	mat4f	rotation;
	mat4f	tmp;

	mat4_translate(trans, vec_multf(center, -1));
	mat4_rotate(rotation, rot);
	mat4_translate(trans_back, center);
	mat4_mult(tmp, trans, rotation);
	mat4_mult(result, tmp, trans_back);
}