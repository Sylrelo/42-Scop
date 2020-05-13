#include "scop.h"

typedef struct	s_obj_parse
{
	FILE		*fp;
	size_t		read;
	size_t		len;
	char		*line;
	short int	percent_current;
	short int	percent_old;
	int			nb_v;
	int			nb_vt;
	int			nb_vn;
	int			nb_f;
	off_t		filesize;
	off_t		readsize;
}				t_obj_parse;

off_t		get_filesize(char *file)
{
	struct stat		st;

	stat(file, &st);
	return (st.st_size);
}

static void show_progress(t_obj_parse *buff)
{	
	/*buff->percent_current = (buff->readsize * 100.f) / buff->filesize;
	if (buff->percent_old != buff->percent_current 
		&& buff->percent_current % 2 == 0)
	{		
		printf("\x1b[48;2;%d;%d;%dm \x1b[0m", 
			200, 
			200, 
			200);
		buff->percent_old = buff->percent_current;
	}
	buff->readsize += buff->read;*/
	
	buff->percent_current = (buff->readsize * 100.f) / buff->filesize;
	if (buff->percent_old != buff->percent_current 
		&& buff->percent_current % 2 == 0)
	{		
		printf("-");
		buff->percent_old = buff->percent_current;
	}
	buff->readsize += buff->read;
}

static void	prepare(t_obj_parse *buff)
{
	buff->percent_old = -1;
	while ((buff->read = getline(&buff->line, &buff->len, buff->fp)) != -1)
	{
		if (buff->line[0] == 'v' && buff->line[1] == ' ')
			buff->nb_v++;
		if (buff->line[0] == 'v' && buff->line[1] == 't')
			buff->nb_vt++;
		if (buff->line[0] == 'v' && buff->line[1] == 'n')
			buff->nb_vn++;
		if (buff->line[0] == 'f' && buff->line[1] == ' ')
			buff->nb_f++;
		show_progress(buff);
	}
	rewind(buff->fp);
}

static void show_infos(t_obj_parse *buff)
{
	printf("\n\n");
	printf("Number of vertices            : %d\n", buff->nb_v);
	printf("Number of texture coordinates : %d\n", buff->nb_vt);
	printf("Number of vertex normals      : %d\n", buff->nb_vn);
	printf("Number of polygons            : %d\n\n", buff->nb_f);
}

int			allocate_arrays(t_app *e, t_obj *obj, t_obj_parse *buff)
{
	if (!(e->vertices = malloc(((buff->nb_f * 3) * 8) * sizeof(float))))
		return (false);
	if (!(e->indexes = malloc((buff->nb_f * 3) * sizeof(unsigned int))))
		return (false);
	if (!(obj->vertices = malloc(sizeof(t_vec3f) * buff->nb_v)))
		return (false);
	if (!(obj->texture_coords = malloc(sizeof(t_vec2f) * buff->nb_vt)))
		return (false);
	if (!(obj->normals = malloc(sizeof(t_vec3f) * buff->nb_vn)))
		return (false);
	obj->nb_vertices = 0;
	obj->nb_normals = 0;
	obj->nb_texture_coords = 0;
	e->nb_vertices = 0;
	e->nb_indexes = 0;
	return (true);
}

void		fill_arrays(t_app *e, t_obj *obj, t_obj_parse *buff)
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
			obj_parse_face(e, obj, buff->line);
		show_progress(buff);
	}
}

void		open_file(t_app *e, char *file)
{
	t_obj_parse	buff;
	t_obj		obj;

	buff.line = NULL;
	buff.nb_f = 0;
	buff.nb_v = 0;
	buff.nb_vn = 0;
	buff.nb_vt = 0;
	buff.filesize = get_filesize(file);
	if ((buff.fp = fopen(file, "r")) == NULL)
	{
		perror("Error while opening the file\n");
		exit(EXIT_FAILURE);
	}
	printf("Reading %s...\n", file);
	prepare(&buff);
	show_infos(&buff);
	if (!allocate_arrays(e, &obj, &buff))
	{
		printf("Allocating failure\n");
		exit(EXIT_FAILURE);
	}
	printf("Creating triangles...\n", file);
	fill_arrays(e, &obj, &buff);
	//exit(EXIT_SUCCESS);
}