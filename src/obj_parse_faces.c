#include "scop.h"

void	obj_parse_push_data(t_app *e, t_obj *obj, t_3i index)
{
	t_vec3f		value_v;
	t_vec3f		value_vn;
	t_vec2f		value_vt;

	value_v = (t_vec3f){0.f, 0.f, 0.f};
	value_vn = (t_vec3f){0.f, 0.f, 0.f};
	value_vt = (t_vec2f){0.f, 0.f};
	if (index.a != -1 && index.a - 1 < obj->nb_vertices)
		value_v = obj->vertices[index.a - 1];
	if (index.b != -1 && index.b - 1 < obj->nb_texture_coords)
		value_vt = obj->texture_coords[index.b - 1];
	if (index.c != -1 && index.c - 1 < obj->nb_normals)
		value_vn = obj->normals[index.c - 1];
	e->vertices[(e->nb_vertices++)] = value_v.x;
	e->vertices[(e->nb_vertices++)] = value_v.y;
	e->vertices[(e->nb_vertices++)] = value_v.z;
	e->vertices[(e->nb_vertices++)] = value_vn.x;
	e->vertices[(e->nb_vertices++)] = value_vn.y;
	e->vertices[(e->nb_vertices++)] = value_vn.z;
	e->vertices[(e->nb_vertices++)] = value_vt.x;
	e->vertices[(e->nb_vertices++)] = value_vt.y;


	srand(e->nb_vertices);
	e->vertices[(e->nb_vertices++)] = rgbfloat(rand() % 255, rand() % 255, rand() % 255);
	e->vertices[(e->nb_vertices++)] = 0;

	/*e->vertices[(e->nb_vertices++)] = 0;
	e->vertices[(e->nb_vertices++)] = 155	;
	e->vertices[(e->nb_vertices++)] = 255;
	e->vertices[(e->nb_vertices++)] = 255;
	*/
	//e->vertices|(e->nb_vertices++)] = (texture_id & 0xFF << 16) | (face_type & 0xFF << 8);
}

t_3i	obj_parse_handle_face(char *line)
{
	const uint8_t	f_type = face_type(line);
	t_3i			value;

	value = (t_3i){-1, -1, -1};
	if (f_type & SINGLE)
		sscanf(line, "%d", &value.a);
	else if (f_type & WITH_TEXCOORDS)
		sscanf(line, "%d/%d", &value.a, &value.b);
	else if (f_type & WITH_NORMAL)
		sscanf(line, "%d//%d", &value.a, &value.c);
	else if (f_type & COMPLETE)
		sscanf(line, "%d/%d/%d", &value.a, &value.b, &value.c);
	return (value);
}

void	obj_push_indexes(t_app *e)
{
	int		i;

	i = -1;
	while (++i < 3)
	{	
		e->indexes[e->nb_indexes] = e->nb_indexes;
		e->nb_indexes++;
	}
}

void	obj_parse_face_triangulate(t_app *e, t_obj *obj, int count, char *line)
{
	char		*token;
	t_3i		*data;
	int			i;
	int			base_index;

	base_index = 0;
	i = 0;
	if (!(data = calloc(count, sizeof(t_3i))))
		return ;
	token = strtok(line, " ");
	while(token != NULL && i < count)
	{
		data[i] = obj_parse_handle_face(token);
		i++;
		token = strtok(NULL, " ");
	}
	i = -1;
	while (++i < count - 2)
	{
		obj_parse_push_data(e, obj, data[0]);
		obj_parse_push_data(e, obj, data[i + 1]);
		obj_parse_push_data(e, obj, data[i + 2]);
		obj_push_indexes(e);
	}
	free(data);
}

void	obj_parse_face(t_app *e, t_obj *obj, char *line)
{
	const int	v_count = chrcount(line, ' ') + 1;
	char		*token;
	t_3i		tmp;

	if (v_count > 3)
	{
		obj_parse_face_triangulate(e, obj, v_count, line);
		return ;
	}
	token = strtok(line, " ");
	while(token != NULL)
	{
		tmp = obj_parse_handle_face(token);
		obj_parse_push_data(e, obj, tmp);
		token = strtok(NULL, " ");
	}
	obj_push_indexes(e);
}
