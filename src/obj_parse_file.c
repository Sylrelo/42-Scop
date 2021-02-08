#include "scop.h"

off_t		get_filesize(char *file)
{
	struct stat		st;

	stat(file, &st);
	return (st.st_size);
}

void		obj_file_firstpass(t_obj_parse *buff)
{
	int		nb;

	buff->percent_old = -1;
	buff->mat_count = 0;
	buff->mat_indexes = NULL;
	buff->readsize = 0;
	buff->read = 0;
	bzero(buff->previous_mat, 256);
	bzero(buff->mtl_name, 256);
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
		else if (buff->line[0] == 'm' && strstr(buff->line, "mtllib ") != NULL)
		{
			printf("Found mtl : [%s]\n", trim(buff->line + 7));
			strcpy(buff->mtl_name, trim(buff->line + 7));
			//memset(btest, '\0', 256);
			//memcpy(btest, &buff->line[7], strlen(buff->line) - 8);
			//if (!(buff->mtl_name = calloc(strlen(btest), sizeof(char))))
			//{	
			//	printf("\n[Parsing] MTL File name calloc failed\n");
			//	continue ;
			//}
			//else
			//	memcpy(btest, btest, strlen(btest));
		}
		
		else if (buff->line[0] == 'u' && strstr(buff->line, "usemtl ") != NULL)
		{
			strcpy(buff->previous_mat, trim(buff->line + 7));
			buff->nb_mats++;
		}
		else if (buff->line[0] == 'f' && buff->line[1] == ' ')
		{
			materials_indexing(buff, buff->line);
			buff->nb_f += chrcount(buff->line, ' ') - 2;
		}
		obj_parse_show_progress(buff);
	}
	printf(" done.\n");
	mtidx_allocfaces(buff);
	mtidx_populate(buff);
	free(buff->line);
	buff->line = NULL;
}

void			arrays_allocate(t_app *e, t_obj *obj, t_obj_parse *buff)
{
	if (!(obj->vertices = calloc(sizeof(t_vec3f), buff->nb_v)))
		exit (1);
	if (!(obj->texture_coords = calloc(sizeof(t_vec2f), buff->nb_vt)))
		exit (1);
	if (!(obj->normals = calloc(sizeof(t_vec3f), buff->nb_vn)))
		exit (1);
	obj->nb_vertices = 0;
	obj->nb_normals = 0;
	obj->nb_texture_coords = 0;
	//e->nb_vertices = 0;
	//e->nb_indexes = 0;
}

static void		array_fill(t_app *e, t_obj *obj, t_obj_parse *buff)
{
	t_vec3f		tmp;

	buff->percent_old = -1;
	buff->readsize = 0;
	buff->read = 0;
	printf("\nIndexing data...\n");
	while ((buff->read = getline(&buff->line, &buff->len, buff->fp)) != -1)
	{
		if (sscanf(buff->line, "v %f %f %f", &tmp.x, &tmp.y, &tmp.z))
			obj->vertices[obj->nb_vertices++] = (t_vec3f){tmp.x, tmp.y, tmp.z};
		else if (sscanf(buff->line, "vn %f %f %f", &tmp.x, &tmp.y, &tmp.z))
			obj->normals[obj->nb_normals++] = (t_vec3f){tmp.x, tmp.y, tmp.z};
		else if (sscanf(buff->line, "vt %f %f", &tmp.x, &tmp.y))
			obj->texture_coords[obj->nb_texture_coords++] = (t_vec2f){tmp.x, tmp.y};
		obj_parse_show_progress(buff);
	}
	printf(" done.");
	free(buff->line);
	buff->line = NULL;
}

void            file_secondpass(t_app *e, t_obj_parse *buff, t_obj *obj)
{
	int			i;
	int			j;

	i = -1;
	rewind(buff->fp);
    array_fill(e, obj, buff);
	mtidx_iterate(e, obj, buff);
	mtl_readfile(e, buff);
    free(obj->vertices);
    free(obj->normals);
    free(obj->texture_coords);
	// free ***faces
	while (++i < buff->mat_count)
	{
		j = -1;
		while (++j < buff->mat_indexes[i].count)
			free(buff->faces[i][j]);
		free(buff->faces[i]);
	}
	free(buff->faces);
	free(buff->mat_indexes);
    fclose(buff->fp);
}

void            file_firstpass(t_app *e, char *file, t_obj_parse *buff, t_obj *obj)
{
    //e->nb_poly = 0;
    buff->line = NULL;
    buff->nb_f = 0;
    buff->nb_v = 0;
    buff->nb_vn = 0;
    buff->nb_vt = 0;
    buff->nb_mats = 0;
    buff->filesize = get_filesize(file);
    if ((buff->fp = fopen(file, "r")) == NULL)
    {
        perror("Error while opening the file\n");
        exit(EXIT_FAILURE);
    }
    printf("Reading %s...\n", file);
    obj_file_firstpass(buff);
	if (!(e->draw_buffer = calloc(sizeof(t_draw), buff->mat_count)))
		exit (1);
	e->draw_buffer_count = buff->mat_count;
    obj_parse_show_infos(buff);
    arrays_allocate(e, obj, buff);
    e->nb_tri = buff->nb_f;
}
