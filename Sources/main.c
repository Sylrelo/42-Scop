/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slopez <slopez@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 11:50:11 by slopez            #+#    #+#             */
/*   Updated: 2021/06/19 16:57:11 by slopez           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Scop.h"
#include <time.h>
#include <sys/stat.h>
#include <float.h>
#include <math.h>
#include "stb_image.h"

static void	auto_object_position(t_scop *scop)
{
	t_objects	*obj;
	t_objects	*prev_obj;
	t_vec3f		min;
	t_vec3f		max;
	t_vec3f		center;
	float		max_component;

	max	= (t_vec3f){ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	min	= (t_vec3f){ FLT_MAX, FLT_MAX, FLT_MAX };
	max_component = -FLT_MAX;
	
	for (size_t obji = 0; obji < scop->objects_count; obji++)
	{
		obj = &scop->objects[obji];
		obj->pos = (t_vec3f) {-obj->center.x, -obj->min.y, -obj->center.z};
		obj->rot = (t_vec3f) {0, 0, 0};
		if (obji > 0)
		{
			prev_obj = &scop->objects[obji - 1];
			obj->pos.x += prev_obj->pos.x + prev_obj->max.x + obj->max.x + obj->center.x;
		}
		max.x = fmax(max.x, obj->pos.x);
		max.y = fmax(max.y, obj->pos.y);
		max.z = fmax(max.z, obj->pos.z);
		min.x = fmin(min.x, obj->pos.x);
		min.y = fmin(min.y, obj->pos.y);
		min.z = fmin(min.z, obj->pos.z);
		if (scop->objects_count == 1)
		{
			obj->pos = (t_vec3f) {-obj->center.x, -obj->center.y / 2, -obj->center.z};
			max.x = fmax(max.x, obj->max.x);
			max.y = fmax(max.y, obj->max.y);
			max.z = fmax(max.z, obj->max.z);
			min.x = fmin(min.x, obj->min.x);
			min.y = fmin(min.y, obj->min.y);
			min.z = fmin(min.z, obj->min.z);
		}
	}
	
	center = vec_multf(vec_add(min, max), .5);

	max_component = fmax(max_component, max.x);
	max_component = fmax(max_component, max.y);
	max_component = fmax(max_component, max.z);

	if (scop->objects_count > 1)
	{
		scop->cam_pos.x -= center.x;
		scop->cam_pos.y -= center.y;
	}
	scop->cam_pos.z -= max_component * 1.5;
	scop->multiplier = fmin(max_component * 0.125, 30);
}

static void	parse_basic_texture(t_scop *scop, char *file)
{
    unsigned char   *image;
    int             width;
    int             height;
    int             channels;

 	if (!(image = stbi_load(file, &width, &height, &channels, 0)))
    {
        return ;
    }
	generate_gl_texture(scop, width, height, image, "CHECKER", GL_UNSIGNED_BYTE);
	free(image);
	image = NULL;
}

static void	start_parser(t_scop *scop, int argc, char *argv[])
{
	struct stat		st;
	int				st_result;
	float			start		= glfwGetTime();
	float			diff 		= start;
	size_t			buffer_size = 0;
	size_t			total_mats	= 0;
	uint8_t			basic_texture_parsed = 0;

	start -= diff;

	for (int i = 1; i < argc; i++)
	{
		if (!strncmp(argv[i], "-t", 2))
		{
			if ((argc - 1) - i >= 1 && basic_texture_parsed)
			{
				printf("\033[0;31m- [-t] Basic texture already parsed. Skipping\033[0m\n\n");
				i++;
				continue ;
			}
			else if ((argc - 1) - i >= 1)
			{
				parse_basic_texture(scop, argv[i + 1]);
				basic_texture_parsed = 1;
				i++;
				continue ;
			}
			else
			{
				printf("\033[0;31m- [-t] Missing texture path.\033[0m\n\n");
				continue ;
			}
		}
		st_result = stat(argv[i], &st);
		
		if (st_result)
		{
			printf("\033[0;31m- File \033[1m%s\033[0m \033[0;31mnot found, skipping.\033[0m\n\n", argv[i]);
			continue ;
		}
		if (!strstr(argv[i], ".obj"))
		{
			printf("\033[0;31m- File \033[1m%s\033[0m \033[0;31mextension is invalid, skipping.\033[0m\n\n", argv[i]);
			continue ;
		}
		if (st.st_mode & S_IFDIR)
		{
			printf("\033[0;31m- File \033[1m%s\033[0m \033[0;31mis a directory, skipping.\033[0m\n\n", argv[i]);
			continue ;
		}
		printf("+ Loading \033[1m%s\033[0m... (%.2f Mb)\n", argv[i], st.st_size * 0.0000009765625);
		parser_init(scop, argv[i]);
		printf("\n");
		get_total_buffer_size(scop, &buffer_size, &total_mats);
		if (buffer_size / BUFFER_COMPONENT / 3 > 17500000 || (((buffer_size * sizeof(float)) * 0.000001) * 1.5) >= 4000 || total_mats >= 300)
		{
			printf("\033[0;31m- Maximum triangles, materials or memory exceeded. For safety, parsing has been stopped\033[0m\n\n");
			break ;
		}
	}
	printf("Time taken : %.2f\n", (glfwGetTime() - diff) - start);
	if (!(scop->objects = realloc(scop->objects, scop->objects_count * sizeof(t_objects))))
		die ("Realloc object failed");
}

int 		main(int argc, char *argv[])
{
	t_scop	*scop;

	if (!(scop = calloc(1, sizeof(t_scop))))
		die ("Could not start Scop");

	srand(time(NULL));

	scop->textures_count 	= 0;
	scop->textures 			= NULL;
	scop->cam_pos			= (t_vec3f) {0, 0, 0};
	scop->cam_rot			= (t_vec3f) {0, 0, 0};
	scop->width	 			= 1920;
	scop->height 			= 1080;
	scop->multiplier		= 1;
	scop->ogl.s_texturing	= 0;
	scop->objects_count		= 0;
	memset(scop->keys, 0, sizeof(uint32_t) * 348);
		
	printf("[Scop] Starting OpenGL initialization\n");
	init_window(&scop->window, scop->width, scop->height);

	if (!(scop->objects = calloc(argc, sizeof(t_objects))))
	{
		printf("Error calloc objects\n");
		free(scop);
		exit (0);
	}

	scop->ogl.p_render = create_shader_program("Shaders/simple/vertex.glsl", "Shaders/simple/fragment.glsl", NULL);
	// scop->ogl.p_render = create_shader_program("Shaders/vertex_classic.glsl", "Shaders/fragment_classic.glsl", "Shaders/geometry_classic.glsl");

	printf("[Scop] Starting parser\n\n");
	start_parser(scop, argc, argv);
	
	printf("[Scop] Starting OpenGL Buffer initialization\n");
	init_opengl_buffer_multi(scop);

	printf("[Scop] Opening Window\n");
	glfwShowWindow(scop->window);

	glUseProgram(scop->ogl.p_render);

	printf("[Scop] Ready\n");

	auto_object_position(scop);

	display_loop(scop);
	clean_exit(scop);
}