#include "scop.h"

void 		mat4_init(mat4f out)
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

void		mat4_cpy(mat4f dest, mat4f src)
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

void		mat4_empty(mat4f mat)
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

void		mat4_mult(mat4f result, mat4f a, mat4f b)
{
	int			i;
	int			j;
	int			k;
	mat4f		cpa;
	mat4f		cpb;

	mat4_cpy(cpa, a);
	mat4_cpy(cpb, b);
	mat4_empty(result);
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
