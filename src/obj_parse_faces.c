#include "scop.h"

void	store_minmax(t_app *e, t_vec3f pt)
{
	if (e->min.x > pt.x)
		e->min.x = pt.x;
	if (e->min.y > pt.y)
		e->min.y = pt.y;
	if (e->min.z > pt.z)
		e->min.z = pt.z;
	if (e->max.x < pt.x)
		e->max.x = pt.x;
	if (e->max.y < pt.y)
		e->max.y = pt.y;
	if (e->max.z < pt.z)
		e->max.z = pt.z;
}

void	obj_parse_push_data(t_app *e, t_obj *obj, t_3i index, t_fdata *data)
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
	store_minmax(e, value_v);
    data->pos[0 + data->idx] = value_v.x;
    data->pos[1 + data->idx] = value_v.y;
    data->pos[2 + data->idx] = value_v.z;
    data->pos[3 + data->idx] = value_vn.x;
    data->pos[4 + data->idx] = value_vn.y;
    data->pos[5 + data->idx] = value_vn.z;
    //data->pos[6 + data->idx] = value_vt.x;
    //data->pos[7 + data->idx] = value_vt.y;
    //e->nb_poly++;
    data->idx += 6;
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

typedef struct s_face_data
{
	float		rgb;
	float		face_type;
	float		face_textureid;
}				t_face_data;

void	obj_parse_face_triangulate(t_app *e, t_obj *obj, char *line, int index)
{
	const int		count = chrcount(line, ' ') + 1;
	const uint8_t	f_type = face_type(line);
	char		*token;
	t_3i		*data;
	int			i;
	t_fdata     data0;

	i = -1;
	if (!(data = calloc(count, sizeof(t_3i))))
		exit (1);
	token = strtok(line, " ");
	while(token != NULL && ++i < count)
	{
		data[i] = obj_parse_handle_face(token);
		token = strtok(NULL, " ");
	}
    i = -1;
    while (++i < count - 2)
    {
        data0.idx = 0;
        obj_parse_push_data(e, obj, data[0], &data0);
        obj_parse_push_data(e, obj, data[i + 1], &data0);
        obj_parse_push_data(e, obj, data[i + 2], &data0);
        addTriangle(e, data0.pos, f_type);
		e->draw_buffer[index].count++;
    }
    free(data);
}

void	obj_parse_face(t_app *e, t_obj *obj, char *line, int index)
{
	const int		v_count = chrcount(line, ' ') + 1;
	const uint8_t	f_type = face_type(line);
    t_fdata     data;
	char		*token;
	t_3i		tmp;

	if (v_count > 3)
	{
		obj_parse_face_triangulate(e, obj, line, index);
	        return;
	}
	token = strtok(line, " ");
    data.idx = 0;
	while(token != NULL)
	{
		tmp = obj_parse_handle_face(token);
		obj_parse_push_data(e, obj, tmp, &data);
		token = strtok(NULL, " ");
	}
    addTriangle(e, data.pos, f_type);
	e->draw_buffer[index].count++;
}
