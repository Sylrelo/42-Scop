/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_mtl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42-lyon.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 00:33:12 by slopez            #+#    #+#             */
/*   Updated: 2020/05/18 00:33:12 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

t_vec3f	mtl_parse_float3(char *line)
{
	t_vec3f		result;

	sscanf(line, "%f %f %f", &result.x, &result.y, &result.z);
	return (result);
}	

int		mtl_getbufferid(t_app *e, t_obj_parse *buff, char mat[256])
{
	int		i;
	int		f;

	f = -1;
	i = -1;
	while (f == -1 && ++i < buff->mat_count)
	{
		if (!strcmp(buff->mat_indexes[i].name, mat))
			f = i;
	}
	return (f);
}

void	mtl_readfile(t_app *e, t_obj_parse *buff)
{
	FILE		*fp;
	char		path[1024];

	bzero(path, 1024);
	strcpy(path, "res/");
	strcat(path, buff->mtl_name);
	if (!strlen(buff->mtl_name))
		return ;
	printf("\nOpening %s...\n", path);
	if ((fp = fopen(path, "r")) == NULL)
    {
        perror("Error while opening the MTL file.\n");
        return ;
    }
	size_t		read;
	char		*line;
	size_t		len;
	char		current_material[256];
	int			idx;
	t_vec3f		value;

	bzero(current_material, 256);
    line = NULL;
	while ((read = getline(&line, &len, fp)) != -1)
	{
		if (line[0] == 'n' && strstr(line, "newmtl ") != NULL)
		{
			strcpy(current_material, trim(line + 7));
			idx = mtl_getbufferid(e, buff, current_material);
		}
		else if (line[0] == 'K' && line[1] == 'd' && idx != -1)
		{
			e->draw_buffer[idx].kd = mtl_parse_float3(trim(line + 3));
		}
	}
	free(line);
	line = NULL;
	fclose(fp);
}