
#include "scop.h"

void	key_handle(t_app *e)
{
	if (glfwGetKey(e->win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(e->win, GL_TRUE);

	if (glfwGetKey(e->win, GLFW_KEY_W) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.z += .5f;

	if (glfwGetKey(e->win, GLFW_KEY_S) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.z -= .5f;

	if (glfwGetKey(e->win, GLFW_KEY_A) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.x += .5f;

	if (glfwGetKey(e->win, GLFW_KEY_D) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.x -= .5f;

	if (glfwGetKey(e->win, GLFW_KEY_R) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.y += .5f;

	if (glfwGetKey(e->win, GLFW_KEY_F) == GLFW_PRESS ? 1 : 0)
		e->cam_pos.y -= .5f;


	if (glfwGetKey(e->win, GLFW_KEY_LEFT) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.y += 0.05;

	if (glfwGetKey(e->win, GLFW_KEY_RIGHT) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.y -= 0.05;

	if (glfwGetKey(e->win, GLFW_KEY_UP) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.x += 0.05;

	if (glfwGetKey(e->win, GLFW_KEY_DOWN) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.x -= 0.05;

	if (glfwGetKey(e->win, GLFW_KEY_KP_4) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.z += 0.05;

	if (glfwGetKey(e->win, GLFW_KEY_KP_6) == GLFW_PRESS ? 1 : 0)
		e->cam_rot.z -= 0.05;

		
	if (glfwGetKey(e->win, GLFW_KEY_P) == GLFW_PRESS ? 1 : 0)
	{
		(e->display_mode & 2) ? (e->display_mode ^= 2) : (e->display_mode |= 2);
		glPolygonMode(GL_FRONT_AND_BACK, e->display_mode & 2 ? GL_LINE : GL_FILL);
	}
}

void	_init(t_app *e)
{
	e->cam_pos 		= (t_vec3f){0, 0, 0};
	e->cam_rot 		= (t_vec3f){0, 0, 0};
	e->display_mode 	|= 0;

	if (!glfwInit())
		exit (1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	e->win = glfwCreateWindow(1600, 900, "Scop", NULL, NULL);
	if (!e->win)
		exit (1);

	glfwMakeContextCurrent(e->win);
	glfwSetInputMode(e->win, GLFW_STICKY_KEYS, 1);
	glfwSwapInterval(1);
}

#include <windows.h>

#include <imagehlp.h>

LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS * ExceptionInfo)
{
  switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
  {
    case EXCEPTION_ACCESS_VIOLATION:
      fputs("Error: EXCEPTION_ACCESS_VIOLATION\n", stderr);
      break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
      fputs("Error: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n", stderr);
      break;
    case EXCEPTION_BREAKPOINT:
      fputs("Error: EXCEPTION_BREAKPOINT\n", stderr);
      break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
      fputs("Error: EXCEPTION_DATATYPE_MISALIGNMENT\n", stderr);
      break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
      fputs("Error: EXCEPTION_FLT_DENORMAL_OPERAND\n", stderr);
      break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
      fputs("Error: EXCEPTION_FLT_DIVIDE_BY_ZERO\n", stderr);
      break;
    case EXCEPTION_FLT_INEXACT_RESULT:
      fputs("Error: EXCEPTION_FLT_INEXACT_RESULT\n", stderr);
      break;
    case EXCEPTION_FLT_INVALID_OPERATION:
      fputs("Error: EXCEPTION_FLT_INVALID_OPERATION\n", stderr);
      break;
    case EXCEPTION_FLT_OVERFLOW:
      fputs("Error: EXCEPTION_FLT_OVERFLOW\n", stderr);
      break;
    case EXCEPTION_FLT_STACK_CHECK:
      fputs("Error: EXCEPTION_FLT_STACK_CHECK\n", stderr);
      break;
    case EXCEPTION_FLT_UNDERFLOW:
      fputs("Error: EXCEPTION_FLT_UNDERFLOW\n", stderr);
      break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
      fputs("Error: EXCEPTION_ILLEGAL_INSTRUCTION\n", stderr);
      break;
    case EXCEPTION_IN_PAGE_ERROR:
      fputs("Error: EXCEPTION_IN_PAGE_ERROR\n", stderr);
      break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
      fputs("Error: EXCEPTION_INT_DIVIDE_BY_ZERO\n", stderr);
      break;
    case EXCEPTION_INT_OVERFLOW:
      fputs("Error: EXCEPTION_INT_OVERFLOW\n", stderr);
      break;
    case EXCEPTION_INVALID_DISPOSITION:
      fputs("Error: EXCEPTION_INVALID_DISPOSITION\n", stderr);
      break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
      fputs("Error: EXCEPTION_NONCONTINUABLE_EXCEPTION\n", stderr);
      break;
    case EXCEPTION_PRIV_INSTRUCTION:
      fputs("Error: EXCEPTION_PRIV_INSTRUCTION\n", stderr);
      break;
    case EXCEPTION_SINGLE_STEP:
      fputs("Error: EXCEPTION_SINGLE_STEP\n", stderr);
      break;
    case EXCEPTION_STACK_OVERFLOW:
      fputs("Error: EXCEPTION_STACK_OVERFLOW\n", stderr);
      break;
    default:
      fputs("Error: Unrecognized Exception\n", stderr);
      break;
  }
  fflush(stderr);
  return EXCEPTION_EXECUTE_HANDLER;
}
 
void set_signal_handler()
{
    SetUnhandledExceptionFilter(windows_exception_handler);
}

void	ogl_options()
{
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

int main()
{
	t_app		*e;
	
	e = malloc(sizeof(t_app));
	set_signal_handler();
	e->nb_data = 8;
	open_file(e, "Ressources/untitled5.obj");
	_init(e);
	if (gl3wInit())
	{
	    fprintf(stderr, "failed to initialize OpenGL\n");
	    return (-1);
	}

	e->program = create_shader_program();
	glViewport(0, 0, 1600, 900);
	ogl_init_buffers(e);
	ogl_attrib(e);

	glPolygonMode(GL_FRONT_AND_BACK, e->display_mode & 2 ? GL_LINE : GL_FILL);

	GLuint MatrixID;

	mat4f	mvp;
	mat4f	out;
	mat4_init(mvp);
	float a = 0.001;
    glUseProgram(e->program);
	MatrixID = glGetUniformLocation(e->program, "transform");
	GLuint persp = glGetUniformLocation(e->program, "persp");
	GLuint Model = glGetUniformLocation(e->program, "Model");
	
	mat4f mat_persp;
	mat4f matmodel;
	mat4_init(matmodel);

	mat4_perspective(mat_persp, 1.0472, 1600.0f / 900.0f, 0.00001f, 1000.0f);
	
	glUniformMatrix4fv(persp, 1, GL_FALSE, mat_persp[0]);
	glUniformMatrix4fv(Model, 1, GL_FALSE, matmodel[0]);

	ogl_options();


	mat4f	mat4_model;
	mat4f	mat_trans;
	mat4f	mat_rot;
	mat4f	mat_scale;

	while (!glfwWindowShouldClose(e->win))
	{
		key_handle(e);
		glClearColor(0.1f, 0.1f, 0.1f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		mat4_rotate(mat_rot, e->cam_rot);
		mat4_translate(mat_trans, e->cam_pos);
		//mat4_scale(mat_scale, (t_vec3f){5, 5, 5});
		mat4_mult(mat4_model, mat_rot, mat_trans);
		//mat4_mult(mat4_model, mat4_model, mat_scale);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, mat4_model[0]);

        glBindVertexArray(e->ogl.vao);
        glDrawElements(GL_TRIANGLES, e->nb_vertices / e->nb_data, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(e->win);
        glfwPollEvents();
	}



	return (false);
}
