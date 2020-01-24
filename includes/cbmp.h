#ifndef CBMP_H_
# define CBMP_H_

# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>

typedef struct		t_pix {
	unsigned char	R;	
	unsigned char	G;	
	unsigned char	B;
}					Pix;

typedef struct		t_bmp {
	Pix				**img;
	unsigned char	*data;
	long			pad;
    unsigned long	size;
	unsigned long	H;
	unsigned long	W;
}					Bmp;

#endif


void	bmp_delete(Bmp *x) {
	if (x->data) {
		free(x->data);
	}
	if (x->img) {
		for (int i = 0; i < x->H; i++) {
			free(x->img[i]);
		}
		free(x->img);
	}
	free(x);
}

Bmp *bmp_init_save(unsigned long H, unsigned long W) {
	Bmp		*file;
	
	file = malloc(sizeof(Bmp));
	file->img = NULL;
	file->pad = (W * -3UL) & 3;
	file->size = H * (W * 3 + file->pad) + 14 + 40;
	file->data = malloc(file->size);
	memset(file->data, 0, file->size);
	file->data[0] = 0x42;
	file->data[1] = 0x4D;
	file->data[2] = file->size >>  0;
	file->data[3] = file->size >>  8;
	file->data[4] = file->size >>  16;
	file->data[5] = file->size >>  24;
	file->data[10] = 0x36;
	file->data[14] = 0x28;
	file->data[18] = W >> 0;
	file->data[19] = W >> 8;
	file->data[20] = W >> 16;
	file->data[21] = W >> 24;
	file->data[22] = H >> 0;
	file->data[23] = H >> 8;
	file->data[24] = H >> 16;
	file->data[25] = H >> 24;
	file->data[26] = 0x01;
	file->data[28] = 0x18;
	return (file);
}

int		bmp_save(unsigned long H, unsigned long W, Pix **img, const char *file_name) {
	Bmp		*file;
	int		fd;
	
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd < 1) {
		fprintf(stderr, "cbmp error : can't open file");
		return (0);
	}
	file = bmp_init_save(H, W);
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			unsigned char *p;
			p = file->data + 14 + 40 + y * (W * 3 + file->pad) + x * 3;
			p[0] = img[H-(y+1)][x].B;
			p[1] = img[H-(y+1)][x].G;
			p[2] = img[H-(y+1)][x].R;
		}
	}
	if (write(fd, file->data, file->size) != file->size) {
		fprintf(stderr, "cbmp error: can't write to file");
	}
	close(fd);
	bmp_delete(file);
	return (1);
}

Bmp		*bmp_init_load(unsigned char *header) {
	Bmp		*new;
	
	new = malloc(sizeof(Bmp));
	new->H = *(int*)&header[22];
	new->W = *(int*)&header[18];
	new->img = malloc(sizeof(Pix*) * new->H);
	for (int i = 0; i < new->H; i++) {
		new->img[i] = malloc(sizeof(Pix) * new->W);
		memset(new->img[i], 0, sizeof(Pix) * new->H);
	}
	new->data = NULL;
	return (new);
}

void	set_pix(Pix *pix, unsigned char R, unsigned char G, unsigned char B) {
	pix->R = R;
	pix->G = G;
	pix->B = B;
}

Bmp		*bmp_load(const char *file_name) {
	Bmp				*file;
	int				fd;
	unsigned char	header[54];
	unsigned char*	line;
	unsigned long	line_size;
	
	fd = open(file_name, O_RDONLY);
	if (fd < 2) {
		fprintf(stderr, "cbmp error: can't open \"%s\"\n", file_name);
		return (NULL);
	}
	if (read(fd, header, 54) != 54) {
		fprintf(stderr, "cbmp read error : wrong header\n");
		return (NULL);
	}
	file = bmp_init_load(header);
	line_size = (file->W*3 + 3) & (~3);
	line = malloc(line_size);
	for(int y = file->H-1; y >= 0; y--) {
		if (read(fd, line, line_size) != line_size) {
			fprintf(stderr, "cbmp read error : wrong file\n");
			bmp_delete(file);
			free(line);
			return (NULL);
		}
        for(int x = 0; x < file->W; x++) {
			set_pix(&(file->img[y][x]), line[x*3+2], line[x*3+1], line[x*3]);
        }
    }
	close(fd);
	free(line);
	return (file);
}