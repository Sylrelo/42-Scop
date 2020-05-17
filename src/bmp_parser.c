#include "scop.h"


typedef struct 	s_header_bmp
{
	unsigned short		reserved0;
	unsigned short		reserved1;
	unsigned int		offset;
}				t_header_bmp;

typedef struct	s_header_img
{
	unsigned short type;
	int			size;
	int			offset;

	int			width;
	int			height;
	int			bpp;
}				t_header_img;

const float c_precision		= 128.0;
const float c_precisionp1	= c_precision + 1.0;
float color2float(t_vec3f vec) 
{	//vec = clamp(vec, 0.0, 1.0);
	return (floor(vec.x * c_precision + 0.5)
		+ (floor(vec.y * c_precision + 0.5) * c_precisionp1)
		+ (floor(vec.z * c_precision + 0.5) * c_precisionp1 * c_precisionp1));
}

unsigned int	*bmp_parse(char *file)
{
	FILE		*fp;
	size_t		read;
	size_t		len;

	if ((fp = fopen(file, "r")) == NULL)
	{
		perror("Error while opening the file\n");
		exit(EXIT_FAILURE);
	}


	unsigned char		header[54];

	//fread(header, 1, 54, fp);

	printf ("%f\n", color2float((t_vec3f){ 1024.58251828, 512.4565155115, 256.95626}));

	t_header_bmp		bmp_header;

	fread(&bmp_header, 1, sizeof(t_header_bmp), fp);

	char		type[3];
	unsigned short int bpp;
    fseek(fp, 0, 0);
    fread(type, 1, 2, fp);
    type[2] = '\0';
 
    fseek(fp,28,0);
    fread(&bpp,1,2,fp);
 
    if (strcmp(type,"BM") || (bpp != 24)){
        printf("\nThe file is not BMP format or is not 24 bits\n");
            exit(0);
    }
	t_header_img		img_header;

	fseek(fp, 18, SEEK_SET);
	fread(&img_header.width, 4, 1, fp);
	fread(&img_header.height, 4, 1, fp);
	fseek(fp, 2, SEEK_CUR);
	fread(&img_header.bpp, 2, 1, fp);

	printf("%d %d : %d\n", img_header.width, img_header.height, img_header.bpp);
	exit (1);
}