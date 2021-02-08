#include "scop.h"

void		mtidx_init(t_obj_parse *buff, char *line)
{
	if (!(buff->mat_indexes = (t_matindex *) calloc(1, sizeof(t_matindex) * 1)))
		exit (1);
	strcpy((buff->mat_indexes)[0].name, buff->previous_mat);
	buff->mat_indexes[0].count = 1;
	buff->mat_indexes[0].index = 0;
	buff->mat_count = 1;
}

void			progressbar(int total, int *current)
{
	static int old = 0;
	int percent;

	percent = (*current * 100.f) / total;
	if (old != percent && percent % 2 == 0)
	{		
		printf(PRGS_CHAR);
		fflush(0);
		old = percent;
	}
	(*current)++;
}

void			mtidx_iterate(t_app *e, t_obj *obj, t_obj_parse *buff)
{
	int i;
	int j;
	int	t;
	int c;

	c = 0;
	i = -1;
	t = 0;
	while (++i < buff->mat_count)
		t += buff->mat_indexes[i].count;
	i = -1;
	printf("\nCreating triangles...\n");
	while (++i < buff->mat_count)
	{
		e->draw_buffer[i].count = 0;
		e->draw_buffer[i].kd = (t_vec3f){ .7, .7, .7};
		j = -1;
		while (++j < buff->mat_indexes[i].count)
		{
			obj_parse_face(e, obj, buff->faces[i][j] + 2, i);
			progressbar(t, &c);
		}
	}
	printf(" done.\n");
}

void		mtidx_update(t_obj_parse *buff, char *line)
{
	int i;
	int f;

	i = -1;
	f = -1;
	while (f == -1 && ++i < buff->mat_count)
		if (!strcmp((buff->mat_indexes)[i].name, buff->previous_mat))
			f = i;
	if (f == -1)
	{
		if (!(buff->mat_indexes = (t_matindex *) realloc(buff->mat_indexes,	\
								sizeof(t_matindex) * (buff->mat_count + 1))))
			exit (1);
		strcpy((buff->mat_indexes)[buff->mat_count].name, buff->previous_mat);
		buff->mat_indexes[buff->mat_count].count = 1;
		buff->mat_indexes[buff->mat_count].index = 0;
		buff->mat_count++;
	}
	else
		buff->mat_indexes[f].count++;
}

void		mtidx_populate(t_obj_parse *buff)
{
	rewind(buff->fp);
	bzero(buff->previous_mat, 256);
	while ((buff->read = getline(&buff->line, &buff->len, buff->fp)) != -1)
	{
		if (buff->line[0] == 'u' && strstr(buff->line, "usemtl ") != NULL)
			strcpy(buff->previous_mat, trim(buff->line + 7));
		else if (buff->line[0] == 'f' && buff->line[1] == ' ')
			mtidx_addface(buff);
	}
	free(buff->line);
	buff->line = NULL;
}

void		mtidx_allocfaces(t_obj_parse *buff)
{
	int		i;

	printf("\nAllocating faces per materials...");
	i = -1;
	if (!(buff->faces = (char ***) calloc(sizeof(char ***), buff->mat_count)))
		exit (1);
	while (++i < buff->mat_count)
	{
		if (!(buff->faces[i] = (char **) calloc(sizeof(char **),	\
											buff->mat_indexes[i].count)))
			exit (1);
	}
	printf(" done.");
}
void		mtidx_addface(t_obj_parse *buff)
{
	int		i;
	int		f;
	int		index;
	
	f = -1;
	i = -1;
	while (f == -1 && ++i < buff->mat_count)
	{
		if (!strcmp(buff->mat_indexes[i].name, buff->previous_mat))
			f = i;
	}
	index = buff->mat_indexes[f].index;
	if (f >= 0 && index < buff->mat_indexes[f].count)
	{
		if (!(buff->faces[f][index] = strdup(buff->line)))
			exit (1);
		strcpy(buff->faces[f][index], trim(buff->line));
		buff->mat_indexes[f].index++;
	}
}

void		materials_indexing(t_obj_parse *buff, char *line)
{
	if (!buff->mat_count && strlen(buff->previous_mat))
		mtidx_init(buff, line);
	else
		mtidx_update(buff, line);
}
