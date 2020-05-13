#include "scop.h"

typedef enum	e_parse_enum
{
	SINGLE = 2,
	WITH_NORMAL = 4,
	WITH_TEXCOORDS = 8,
	COMPLETE = 16
}				t_parse_enum;

uint8_t		face_type(char *line)
{
	int		i;
	int		c;

	i = 1;
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
	int		i;
	int		count;

	i = -1;
	count = 0;
	while (line[++i])
		if (line[i] == c)
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

void	obj_parse_get_v(t_app *e, t_obj *obj, int index)
{
	const t_vec3f value = obj->vertices[index - 1];

	e->vertices[(e->nb_vertices++)] = value.x;
	e->vertices[(e->nb_vertices++)] = value.y;
	e->vertices[(e->nb_vertices++)] = value.z;
}

void	obj_parse_get_vn(t_app *e, t_obj *obj, int index)
{
	const t_vec3f value = obj->normals[index - 1];

	e->vertices[(e->nb_vertices++)] = value.x;
	e->vertices[(e->nb_vertices++)] = value.y;
	e->vertices[(e->nb_vertices++)] = value.z;
}

void	obj_parse_get_vt(t_app *e, t_obj *obj, int index)
{
	t_vec2f value;

	if (index == -1)
		value = (t_vec2f) {0, 0};
	else
		value = obj->texture_coords[index - 1];
	e->vertices[(e->nb_vertices++)] = value.x;
	e->vertices[(e->nb_vertices++)] = value.y;
}

void	obj_parse_face_complete(t_app *e, t_obj *obj, char *line)
{
	t_3i	tmp_v;
	t_3i	tmp_vt;
	t_3i	tmp_vn;

	sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
		&tmp_v.a, &tmp_vt.a, &tmp_vn.a,
		&tmp_v.b, &tmp_vt.b, &tmp_vn.b,
		&tmp_v.c, &tmp_vt.c, &tmp_vn.c
	);
	obj_parse_get_v(e, obj, tmp_v.a);
	obj_parse_get_vn(e, obj, tmp_vn.a);
	obj_parse_get_vt(e, obj, tmp_vt.a);

	obj_parse_get_v(e, obj, tmp_v.b);
	obj_parse_get_vn(e, obj, tmp_vn.b);
	obj_parse_get_vt(e, obj, tmp_vt.b);
	
	obj_parse_get_v(e, obj, tmp_v.c);
	obj_parse_get_vn(e, obj, tmp_vn.c);
	obj_parse_get_vt(e, obj, tmp_vt.c);

	e->indexes[e->nb_indexes] = e->nb_indexes;
	e->nb_indexes++;
	e->indexes[e->nb_indexes] = e->nb_indexes;
	e->nb_indexes++;
	e->indexes[e->nb_indexes] = e->nb_indexes;
	e->nb_indexes++;
}

void	obj_parse_face_with_normals(t_app *e, t_obj *obj, char *line)
{
	t_3i	tmp_v;
	t_3i	tmp_vn;

	int		count = chrcount(line, ' ');
	if (count > 3)
	{	
		printf("Not handling not-triangle");
		return ;
	}

	sscanf(line, "f %d//%d %d//%d %d//%d",
		&tmp_v.a, &tmp_vn.a,
		&tmp_v.b, &tmp_vn.b,
		&tmp_v.c, &tmp_vn.c
	);
	obj_parse_get_v(e, obj, tmp_v.a);
	obj_parse_get_vn(e, obj, tmp_vn.a);
	obj_parse_get_vt(e, obj, -1);

	obj_parse_get_v(e, obj, tmp_v.b);
	obj_parse_get_vn(e, obj, tmp_vn.b);
	obj_parse_get_vt(e, obj, -1);

	obj_parse_get_v(e, obj, tmp_v.c);
	obj_parse_get_vn(e, obj, tmp_vn.c);
	obj_parse_get_vt(e, obj, -1);

	e->indexes[e->nb_indexes] = e->nb_indexes;
	e->nb_indexes++;
	e->indexes[e->nb_indexes] = e->nb_indexes;
	e->nb_indexes++;
	e->indexes[e->nb_indexes] = e->nb_indexes;
	e->nb_indexes++;
}

void	obj_parse_face(t_app *e, t_obj *obj, char *line)
{
	const uint8_t type = face_type(line);

	if (type & COMPLETE)
		obj_parse_face_complete(e, obj, line);
	else if (type & WITH_NORMAL)
		obj_parse_face_with_normals(e, obj, line);
}