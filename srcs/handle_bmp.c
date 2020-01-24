
#include "wfc.h"
# include "cbmp.h"





Pix_to_char *pix_to_char(Data *data, char a) {
	int		i;
	
	i = -1;
	while (data->table[++i]) {
		if (data->table[i]->ascii == a) {
			return (data->table[i]);
		}
	}
	return (data->table[0]);
}

void	save_bmp(Data *data) {
	Pix			**img;
	Pix_to_char	*tmp;
	
	img = malloc(sizeof(Pix*) * (MAP_RESULT_H+(TILE_SIZE-1)));
	for (int y = 0; y < MAP_RESULT_H+(TILE_SIZE-1); y++) {
		img[y] = malloc(sizeof(Pix) * (MAP_RESULT_W+(TILE_SIZE-1)));
		for (int x = 0; x < MAP_RESULT_W+(TILE_SIZE-1); x++) {
			tmp = pix_to_char(data, data->res_map[y][x]);
			img[y][x].R = tmp->R;
			img[y][x].G = tmp->G;
			img[y][x].B = tmp->B;
		}
	}
	bmp_save(MAP_RESULT_H+(TILE_SIZE-1), MAP_RESULT_W+(TILE_SIZE-1), img, "data/res.bmp");
	
	for (int y = 0; y < MAP_RESULT_H+(TILE_SIZE-1); y++) {
		free(img[y]);
	}
	free(img);
}



char	pix_to_ascii(Pix *pix, Pix_to_char **table) {
	int	i;
	
	i = -1;
	while (table[++i]) {
		if (table[i]->R == pix->R
		 && table[i]->G == pix->G
		 && table[i]->B == pix->B) {
			return (table[i]->ascii);
		 }
	}
	table[i] = malloc(sizeof(Pix_to_char));
	table[i]->R = pix->R;
	table[i]->G = pix->G;
	table[i]->B = pix->B;
	if (i == 0) {
		table[i]->ascii = 'A';
	} else {
		table[i]->ascii = table[i-1]->ascii+1;
	}
	return (table[i]->ascii);
}

int bmp_to_data(Data *data, Bmp *file) {
	data->b_H = file->H;
	data->b_W = file->W;
	data->base_map = malloc(sizeof(char*) * data->b_H);
	for (int y = 0; y < data->b_H; y++) {
		data->base_map[y] = malloc(sizeof(char) * data->b_W);
		for (int x = 0; x < data->b_W; x++) {
			data->base_map[y][x] = pix_to_ascii(&(file->img[y][x]), data->table);
		}
	}
	return (1);
}

Data	*init(const char *file_name) {
	Data	*data;
	Bmp		*file;
	
	data = malloc(sizeof(Data));
	data->table = malloc(sizeof(Pix_to_char*) * 64);
	memset(data->table, 0, sizeof(Pix_to_char*) * 64);
	if ((file = bmp_load(file_name)) == NULL) {
		exit (0);
	}
	bmp_to_data(data, file);
	bmp_delete(file);
	return (data);
}