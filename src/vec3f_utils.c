#include "scop.h"

t_vec3f		vec_op(t_vec3f u, char c, t_vec3f v)
{
	t_vec3f		res;

	if (c == '+')
	{
		res.x = u.x + v.x;
		res.y = u.y + v.y;
		res.z = u.z + v.z;
	}
	if (c == '-')
	{
		res.x = u.x - v.x;
		res.y = u.y - v.y;
		res.z = u.z - v.z;
	}
	if (c == '*')
	{
		res.x = u.y * v.z - u.z * v.y;
		res.y = u.z * v.x - u.x * v.z;
		res.z = u.x * v.y - u.y * v.x;
	}
	return (res);
}

float		vec_norm(t_vec3f u)
{
	return (sqrtf(u.x * u.x + u.y * u.y + u.z * u.z));
}

t_vec3f		vec_normalize(t_vec3f u)
{
	float	l;

	l = 1 / vec_norm(u);
	u.x *= l;
	u.y *= l;
	u.z *= l;
	return (u);
}

float		vec_dot(t_vec3f u, t_vec3f v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

t_vec3f		vec_multf(t_vec3f u, float f)
{
	u.x *= f;
	u.y *= f;
	u.z *= f;
	return (u);
}