
#include "scop.h"
#include <pthread.h>

void	key_handle(t_app *e)
{

	if (glfwGetKey(e->win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(e->win, GL_TRUE);

	if (glfwGetKey(e->win, GLFW_KEY_W) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.z += fabs(e->min.z) / 10;

	if (glfwGetKey(e->win, GLFW_KEY_S) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.z -= fabs(e->min.z) / 10;

	if (glfwGetKey(e->win, GLFW_KEY_A) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.x += e->max.x / 10;

	if (glfwGetKey(e->win, GLFW_KEY_D) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.x -= e->max.x / 10;

	if (glfwGetKey(e->win, GLFW_KEY_R) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.y += e->max.y / 10;

	if (glfwGetKey(e->win, GLFW_KEY_F) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.y -= e->max.y / 10;


	if (glfwGetKey(e->win, GLFW_KEY_Q) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.y += 0.2;

	if (glfwGetKey(e->win, GLFW_KEY_E) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.y -= 0.2;

	if (glfwGetKey(e->win, GLFW_KEY_T) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.x += 0.2;

	if (glfwGetKey(e->win, GLFW_KEY_G) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.x -= 0.2;

	if (glfwGetKey(e->win, GLFW_KEY_Y) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.z += 0.2;

	if (glfwGetKey(e->win, GLFW_KEY_H) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.z -= 0.2;

	if (glfwGetKey(e->win, GLFW_KEY_N) == GLFW_PRESS ? 1 : 0)
		e->scale += 1;
	if (glfwGetKey(e->win, GLFW_KEY_M) == GLFW_PRESS ? 1 : 0)
		e->scale -= 1;

		
	if (glfwGetKey(e->win, GLFW_KEY_P) == GLFW_PRESS ? 1 : 0)
	{
		(e->display_mode & 2) ? (e->display_mode ^= 2) : (e->display_mode |= 2);
		glPolygonMode(GL_FRONT_AND_BACK, e->display_mode & 2 ? GL_LINE : GL_FILL);
	}
}

void	mainit(t_app *e)
{
	e->cam_pos 		= (t_vec3f){0, 0, 0};
	e->cam_rot 		= (t_vec3f){0, 0, 0};
	e->display_mode 	|= 0;

	if (!glfwInit())
		exit (1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	e->win = glfwCreateWindow(1600, 900, "Scop", NULL, NULL);
	if (!e->win)
		exit (1);

	glfwMakeContextCurrent(e->win);
	glfwSetInputMode(e->win, GLFW_STICKY_KEYS, 1);
	glfwSwapInterval(1);

}

void	ogl_options()
{
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}




void	*display_thread(t_app *e)
{
	glfwMakeContextCurrent(e->win);

	mat4f	mvp;
	mat4f	out;

	GLuint glMatTransform 	= glGetUniformLocation(e->program, "Transform");
	GLuint persp 			= glGetUniformLocation(e->program, "Persp");
	GLuint glMatModel 		= glGetUniformLocation(e->program, "Model");

	mat4f mat_persp;
	mat4f matmodel;
	mat4f mattransform;

	mat4_init(matmodel);
	mat4_init(mattransform);
	mat4_perspective(mat_persp, 1.0472, 1600.0f / 900.0f, 0.00001f, 1000.0f);
	glUniformMatrix4fv(persp, 1, GL_FALSE, mat_persp[0]);
	mat4f	mat_scale;
	

	//mat4_scale(mat_scale, (t_vec3f){150, 150, 150});
  // mat4_mult(mat4_model, mat4_model, mat_scale);

	glUniformMatrix4fv(glMatTransform, 1, GL_FALSE, mattransform[0]);


	GLint loc = glGetUniformLocation(e->program, "kd");
	float color[] = {1, 0, 1};

	glUniform3f(loc, .5, .5, .5);

	mat4f	mat4_model;
	int i;
	int o;

	e->scale = 1;
	while (!glfwWindowShouldClose(e->win))
	{
        key_handle(e);
		glClearColor(0.1f, 0.1f, 0.1f, 0.f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
		matmat(mat4_model, e->cam_pos, e->cam_rot, e->center, e->scale);

        glUniformMatrix4fv(glMatModel, 1, GL_FALSE, mat4_model[0]);
		
		i = -1;
		o = 0;
		while (++i < e->draw_buffer_count)
		{
			t_vec3f kd = e->draw_buffer[i].kd;
			if (i > 0)
				o += e->draw_buffer[i - 1].count;

			glUniform3f(loc, kd.x, kd.y, kd.z);
			glDrawArrays(GL_TRIANGLES, 3 * o, (3 * e->draw_buffer[i].count));
		}
		glfwSwapBuffers(e->win);
        glfwPollEvents();
	}
	glfwDestroyWindow(e->win);
}


t_vec3f	calculate_normal(float *vertices)
{
	const t_vec3f	pt0 = (t_vec3f){vertices[0], vertices[1], vertices[2]};
	const t_vec3f	pt1 = (t_vec3f){vertices[6], vertices[7], vertices[8]};
	const t_vec3f	pt2 = (t_vec3f){vertices[12], vertices[13], vertices[14]};
	const t_vec3f	u = vec_op(pt1, '-', pt0);
	const t_vec3f	v = vec_op(pt2, '-', pt0);
	t_vec3f			n;

	n = vec_op(u, '*', v);
	return (n);
}

void    addTriangle(t_app *e, float *vertices, uint8_t f_type)
{
	t_vec3f	normal;

	if (f_type & (SINGLE | WITH_TEXCOORDS))
	{
		normal = calculate_normal(vertices);
		vertices[3] = normal.x;
		vertices[4] = normal.y;
		vertices[5] = normal.z;
		vertices[9] = normal.x;
		vertices[10] = normal.y;
		vertices[11] = normal.z;
		vertices[15] = normal.x;
		vertices[16] = normal.y;
		vertices[17] = normal.z;
	}
	glBufferSubData(GL_ARRAY_BUFFER, e->offset, 72, vertices);
    e->offset += 72;
	ogl_geterror();
}

void	*parsing_thread(t_app *e, char *argc)
{
    t_obj_parse	    buff;
    t_obj		    obj;

    e->offset = 0;

    file_firstpass(e, argc, &buff, &obj);

    glfwMakeContextCurrent(e->win);
	ogl_geterror();
	glBindBuffer(GL_ARRAY_BUFFER, e->ogl.vbo);
	ogl_geterror();
	glBindVertexArray(0);
	ogl_geterror();
	glBindVertexArray(e->ogl.vao);
	ogl_geterror();

	unsigned long long int si = ((unsigned long long int)e->nb_tri) * (36 + 36);
	glBufferData(GL_ARRAY_BUFFER, si, NULL, GL_DYNAMIC_DRAW);
	ogl_geterror();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void *)0);
	ogl_geterror();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void *)(3 * sizeof(float)));
	ogl_geterror();

    glEnableVertexAttribArray(0);
    ogl_geterror();
    glEnableVertexAttribArray(1);
    ogl_geterror();

    file_secondpass(e, &buff, &obj);
	//e->parsing_done = 1;
	sleep(1);
}

int main(int argv, char *argc[])
{
	t_app		*e;
	pthread_t	thread[2];

	if (!(e = malloc(sizeof(t_app))))
		exit (1);

	//e->parsing_done = 0;
	mainit(e);
	printf("heee\n");
	glfwHideWindow(e->win);

	if (gl3wInit())
	{
	    fprintf(stderr, "failed to initialize OpenGL\n");
	    return (-1);
	}

	e->program = create_shader_program();
   	glUseProgram(e->program);
	ogl_options();
	glGenBuffers(1, &e->ogl.vbo);  
	glGenVertexArrays(1, &e->ogl.vao);  
	glBindVertexArray(e->ogl.vao);
	e->nb_tri = 0;
	
	parsing_thread(e, argc[1]);

	printf("Min: (%2f, %2f, %2f)\n", e->min.x, e->min.y, e->min.z);
	printf("Max: (%2f, %2f, %2f)\n", e->max.x, e->max.y, e->max.z);


	e->center = vec_multf(vec_op(e->min, '+', e->max), .5);

	e->cam_pos.z = (e->min.z - 10);
	e->cam_pos.y = (-e->center.y);
	e->cam_pos.x = (-e->center.x);

	e->cam_rot.x = 0;
	e->cam_rot.y = 0;
	e->cam_rot.z = 0;
	printf("Center: (%2f, %2f, %2f)\n", e->center.x, e->center.y, e->center.z);
	glfwShowWindow(e->win);
	display_thread(e);

	exit (1);

	/*while (1)
	{
		sleep(1);
	}*/
	free(e);
	return (false);
}



