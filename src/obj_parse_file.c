#include "scop.h"

off_t			get_filesize(char *file)
{
	struct stat		st;

	stat(file, &st);
	return (st.st_size);
}

static void		prepare(t_obj_parse *buff)
{
	buff->percent_old = -1;
	int		nb;
	char	*ptr_mtl;
	char	btest[256];

	while ((buff->read = getline(&buff->line, &buff->len, buff->fp)) != -1)
	{
		if (buff->line[buff->read - 1] == '\n')
			buff->line[buff->read - 1] = '\0';
		if (buff->line[0] == 'v' && buff->line[1] == ' ')
			buff->nb_v++;
		else if (buff->line[0] == 'v' && buff->line[1] == 't')
			buff->nb_vt++;
		else if (buff->line[0] == 'v' && buff->line[1] == 'n')
			buff->nb_vn++;
		else if (buff->line[0] == 'f' && buff->line[1] == ' ')
			buff->nb_f += chrcount(buff->line, ' ') - 2;
		else if (buff->line[0] == 'm' && strstr(buff->line, "mtllib ") != NULL)
		{
			memset(btest, '\0', 256);
			memcpy(btest, &buff->line[7], strlen(buff->line) - 8);
			if (!(buff->mtl_name = calloc(strlen(btest), sizeof(char))))
			{	
				printf("\n[Parsing] MTL File name calloc failed\n");
				continue ;
			}
			else
				memcpy(btest, btest, strlen(btest));
		}
		else if (buff->line[0] == 'u' && strstr(buff->line, "usemtl ") != NULL)
		{
			buff->nb_mats++;
		}
		obj_parse_show_progress(buff);
	}
	rewind(buff->fp);
}

static int		allocate_arrays(t_app *e, t_obj *obj, t_obj_parse *buff)
{
	if (!(e->vertices = calloc(((buff->nb_f * 3) * 11), sizeof(float))))
		return (false);
	if (!(e->indexes = calloc((buff->nb_f * 3), sizeof(unsigned int))))
		return (false);
	if (!(obj->vertices = calloc(sizeof(t_vec3f), buff->nb_v)))
		return (false);
	if (!(obj->texture_coords = calloc(sizeof(t_vec2f), buff->nb_vt)))
		return (false);
	if (!(obj->normals = calloc(sizeof(t_vec3f), buff->nb_vn)))
		return (false);
	if (!(obj->materials = calloc(sizeof(t_mtl), buff->nb_mats)))
		return (false);
	obj->nb_vertices = 0;
	obj->nb_normals = 0;
	obj->nb_texture_coords = 0;
	obj->nb_mats = 0;
	e->nb_vertices = 0;
	e->nb_indexes = 0;
	return (true);
}

static void		fill_arrays(t_app *e, t_obj *obj, t_obj_parse *buff)
{
	t_vec3f		tmp;

	buff->percent_old = -1;
	buff->readsize = 0;
	buff->read = 0;
	while ((buff->read = getline(&buff->line, &buff->len, buff->fp)) != -1)
	{
		if (sscanf(buff->line, "v %f %f %f", &tmp.x, &tmp.y, &tmp.z))
			obj->vertices[obj->nb_vertices++] = (t_vec3f){tmp.x, tmp.y, tmp.z};
		else if (sscanf(buff->line, "vn %f %f %f", &tmp.x, &tmp.y, &tmp.z))
			obj->normals[obj->nb_normals++] = (t_vec3f){tmp.x, tmp.y, tmp.z};
		else if (sscanf(buff->line, "vt %f %f", &tmp.x, &tmp.y))
			obj->texture_coords[obj->nb_texture_coords++] = (t_vec2f){tmp.x, tmp.y};
		else if (buff->line[0] == 'f')
		{	
			obj_parse_face(e, obj, buff->line + 2);
		}
		obj_parse_show_progress(buff);
	}
}

void			open_file(t_app *e, char *file)
{
	t_obj_parse	buff;
	t_obj		obj;

	buff.line = NULL;
	buff.nb_f = 0;
	buff.nb_v = 0;
	buff.nb_vn = 0;
	buff.nb_vt = 0;
	buff.nb_mats = 0;
	buff.filesize = get_filesize(file);
	if ((buff.fp = fopen(file, "r")) == NULL)
	{
		perror("Error while opening the file\n");
		exit(EXIT_FAILURE);
	}
	printf("Reading %s...\n", file);
	prepare(&buff);
	obj_parse_show_infos(&buff);
	if (!allocate_arrays(e, &obj, &buff))
	{
		printf("Allocating failure\n");
		exit(EXIT_FAILURE);
	}
	printf("Creating triangles...\n", file);
	fill_arrays(e, &obj, &buff);
	free(obj.vertices);
	free(obj.normals);
	free(obj.texture_coords);
	free(obj.materials);
	fclose(buff.fp);
}