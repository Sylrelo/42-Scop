/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:53 by slopez            #+#    #+#             */
/*   Updated: 2021/06/04 12:37:21 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H


#include <stdlib.h>


typedef struct s_vec3i
{
	int		x;
	int		y;
	int		z;
}				t_vec3i;

typedef struct s_vec3f
{
	float	x;
	float	y;
	float	z;
}				t_vec3f;

typedef struct s_vec2f
{
	float	x;
	float	y;
}				t_vec2f;

typedef struct s_parser
{
	size_t		v_count;
	size_t		vn_count;
	size_t		vt_count;
	t_vec3f		*v;
	t_vec3f		*vn;
	t_vec2f		*vt;
}				t_parser;

typedef struct	s_mat
{
	char		material_name[256];
	uint16_t	tex_id;
	t_vec3f		kd;
	t_vec3f		ks;
	t_vec3f		tf;
	float		*gl_buffer;
	size_t		gl_buffer_size;
}				t_mat;

typedef struct s_scop
{
	size_t		nb_mats;
	t_mat		*materials;
}				t_scop;

#endif