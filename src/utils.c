/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42-lyon.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 00:13:57 by slopez            #+#    #+#             */
/*   Updated: 2020/05/18 00:13:57 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

uint8_t		face_type(char *line)
{
	int		i;
	int		c;

	i = -1;
	c = 0;
	while (line[++i])
	{
		if (line[i] == '/' && line[i + 1] && line[i + 1] == '/')
			return (WITH_NORMAL);
		if (line[i] == '/')
			c++;
		if (line[i] == ' ')
			break ;
	}
	if (c == 2)
		return (COMPLETE);
	if (c == 1)
		return (WITH_TEXCOORDS);
	return (SINGLE);
}

int			chrcount(char *line, char c)
{
	const int 	len = strlen(line);
	int			i;
	int			count;
	
	i = -1;
	count = 0;
	while (line[++i])
		if (line[i] == c && (line[i + 1] && line[i + 1] != '\n' 
			&& line[i + 1] != '\0' && line[i + 1] != ' '))
			count++;
	return (count);
}

int			chrat(char *line, char c)
{
	int		i;

	i = -1;
	while (line[++i])
		if (line[i] == c)
			return (i);
	return (-1);
}

char		*strafterocc(char *line, char c)
{
	int		i;

	i = -1;
	while (line[++i])
		if (line[i] == c)
			return (line + i + 1);
	return (NULL);
}

float		rgbfloat(float r, float g, float b) 
{
	float re = (float)((r) + ((g) / 256.0) + ( (b) / (256.0 * 256.0)));

	float ar = 65536 * r + 256 * g + b;

	return (ar);
}