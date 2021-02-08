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


void 	mat4_mult(mat4f res, mat4f a, mat4f b)
{
	int i, j, k; 
    for (i = 0; i < 4; i++) 
    { 
        for (j = 0; j < 4; j++) 
        { 
            res[i][j] = 0; 
            for (k = 0; k < 4; k++) 
                res[i][j] += a[i][k] * b[k][j]; 
        } 
    } 
}
void	matmat(mat4f res, t_vec3f t, t_vec3f r, t_vec3f center, int scale)
{
	mat4f	tt;
	mat4f	tr;
	mat4f	ts;
	mat4f	tmp;

	mat4_rotate_around(tr, vec_multf(center, scale), r);
	mat4_scale(ts, (t_vec3f){scale, scale, scale});
	mat4_translate(tt, t);
	mat4_mult(tmp, ts, tr);
	mat4_mult(res, tmp, tt);
}

void	printmat(mat4f mat)
{
	int			i;
	i = -1;
	while (++i < 4)
	{
		printf("%.2f %.2f %.2f %.2f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
	}
	printf("\n");

}