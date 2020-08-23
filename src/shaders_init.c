#include "scop.h"

char *load_glsl_file(char *file, int *size)
{
	char		*data;
	char 		*line;
	FILE 		*fp;
	size_t		read;
	size_t		len;

	*size = 0;
	len = 0;
	data = NULL;
	line = NULL;
	if ((fp = fopen(file, "r")) == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	fseek(fp, 0, SEEK_END);
	*size = ftell(fp);
	rewind(fp);
	if (!(data = calloc(*size + 1, sizeof(char))))
	{
		printf("[Shader] Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	while ((read = getline(&line, &len, fp)) != -1) {
		strcat(data, line);
	}
	free(line);
	return (data);
}

int load_shader(char *file, unsigned int type)
{
	int			len;
	const char	*source = load_glsl_file(file, &len);
	int			shader;
	int			success;
	char		log[512];

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("[Shader] Compilation failed %d\n%s\n", type, log);
		exit(EXIT_FAILURE);
	}
	free(source);
	return (shader);
}

int create_shader_program()
{
	int			program;
	int			success;
	char		log[512];
	const int 	vertex = load_shader("./vertex.glsl", GL_VERTEX_SHADER);
	const int 	fragment = load_shader("./fragment.glsl", GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, log);
		printf("[Program] Linking failed\n%s\n", log);
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return (program);
}