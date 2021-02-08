/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_verbose.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42-lyon.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 00:21:02 by slopez            #+#    #+#             */
/*   Updated: 2020/05/18 00:21:02 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <locale.h>

void obj_parse_show_progress(t_obj_parse *buff)
{	
	buff->percent_current = (buff->readsize * 100.f) / buff->filesize;
	if (buff->percent_old != buff->percent_current 
		&& buff->percent_current % 2 == 0)
	{		
		printf(PRGS_CHAR);
		buff->percent_old = buff->percent_current;
		fflush(0);
	}
	buff->readsize += buff->read;
}

void obj_parse_show_infos(t_obj_parse *buff)
{
	//setlocale(LC_NUMERIC, "");
	printf("\n\n");
	printf("Number of vertices            : %d\n", buff->nb_v);
	printf("Number of texture coordinates : %d\n", buff->nb_vt);
	printf("Number of vertex normals      : %d\n", buff->nb_vn);
	printf("Number of polygons            : %d\n", buff->nb_f);
	printf("Number of materials           : %d\n", buff->mat_count);
}