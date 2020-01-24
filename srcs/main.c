
#include "wfc.h"







char	**create_tile(Data *data, int i) {
	char	**new;
	
	new = malloc(sizeof(char*) * TILE_SIZE);
	for (int y = 0; y < TILE_SIZE; y++) {
		new[y] = malloc(sizeof(char) * TILE_SIZE);
		for (int x = 0; x < TILE_SIZE; x++) {
			new[y][x] = data->base_map[y+(i/(data->b_W-(TILE_SIZE-1)))][x+(i%(data->b_W-(TILE_SIZE-1)))];
		}
	}
	return (new);
}

char	***create_all_tiles(Data *data, int *nb_tiles) {
	char	***tiles_lst;
	
	*nb_tiles = (data->b_H-(TILE_SIZE-1))*(data->b_W-(TILE_SIZE-1));
	tiles_lst = malloc(sizeof(char**) * (*nb_tiles));
	for (int i = 0; i < (*nb_tiles); i++) {
		tiles_lst[i] = create_tile(data, i);
	}
	return (tiles_lst);
}

Tile	*t_new(char ***tiles_lst, int nb_tiles, int x, int y) {
	Tile	*new;
	
	new = malloc(sizeof(Tile));
	new->lst_size = nb_tiles;
	new->lst_max_size = nb_tiles;
	new->x = x;
	new->y = y;
	new->tiles_lst = malloc(sizeof(char**) * nb_tiles);
	for (int i = 0; i < nb_tiles; i++) {
		new->tiles_lst[i] = tiles_lst[i];
	}	
	return (new);
}

void	init_tiles_map(Data *data, char ***tiles_lst, int nb_tiles) {
	data->tiles_map = malloc(sizeof(Tile**) * MAP_RESULT_H);
	for (int i = 0; i < MAP_RESULT_H; i++) {
		data->tiles_map[i] = malloc(sizeof(Tile*) * MAP_RESULT_W);
		for (int j = 0; j < MAP_RESULT_W; j++) {
			data->tiles_map[i][j] = t_new(tiles_lst, nb_tiles, j, i);
		}
	}
}

void	init_res_map(Data *data) {
	data->res_map = malloc(sizeof(char*) * (MAP_RESULT_H+(TILE_SIZE-1)));
	for (int i = 0; i < (MAP_RESULT_H+(TILE_SIZE-1)); i++) {
		data->res_map[i] = malloc(sizeof(char) * (MAP_RESULT_W+(TILE_SIZE-1)));
		memset(data->res_map[i], 0, sizeof(char) * (MAP_RESULT_W+(TILE_SIZE-1)));
	}
}

void	update_final_map(Data *data, Tile *actu) {
	int		random;
	int		i;
	
	random = rand()%actu->lst_size;
	i = 0;
	while (1) {
		if (actu->tiles_lst[i]) {
			random--;
		}
		if (random < 0) {
			break;
		}
		i++;
	}
	
	for (int y = 0; y < TILE_SIZE; y++) {
		for (int x = 0; x < TILE_SIZE; x++) {
			if (data->res_map[y+actu->y][x+actu->x] == 0) {
				data->res_map[y+actu->y][x+actu->x] = actu->tiles_lst[i][y][x];
				//return ;
			}
		}
	}
	return ;
}

int		check_map(Data *data, int x, int y, int X) {
	int check;
	
	check = 0;
	for (int i = 0; i < TILE_SIZE; i++) {
		for (int j = 0; j < TILE_SIZE; j++) {
			if (data->res_map[y+i][x+j] == 0) {
				check++;
			}
			if (data->res_map[y+i][x+j] != 0
			&& data->res_map[y+i][x+j] != data->tiles_map[y][x]->tiles_lst[X][i][j]) {
				return (0);
			}
		}
	}
	return (check);
}

void	check_tiles_lst(Data *data, int xx, int yy) {
	for (int i = yy-(TILE_SIZE-1); i <= yy+(TILE_SIZE-1); i++) {
		if (i < 0 || i >= MAP_RESULT_H) {
			continue;
		}
		for (int j = xx-(TILE_SIZE-1); j <= xx+(TILE_SIZE-1); j++) {
			if (j >= 0 && j < MAP_RESULT_W && data->tiles_map[i][j]) {
				for (int x = 0; x < data->tiles_map[i][j]->lst_max_size; x++) {
					if (data->tiles_map[i][j]->tiles_lst[x] && 
					!check_map(data, j, i, x)) {
						data->tiles_map[i][j]->tiles_lst[x] = 0;
						data->tiles_map[i][j]->lst_size--;
					}
				}
				if (data->tiles_map[i][j]->lst_size <= 0) {
					free(data->tiles_map[i][j]->tiles_lst);
					free(data->tiles_map[i][j]);
					data->tiles_map[i][j] = 0;
				}
			}
		}
	}
}

Tile	*choose_tile(Data *data) {
	Tile	*save;
	int		best;
	
	best = 999999999;
	save = NULL;
	for (int i = 0; i < MAP_RESULT_H; i++) {
		for (int j = 0; j < MAP_RESULT_W; j++) {
			if (data->tiles_map[i][j] && data->tiles_map[i][j]->lst_size < best) {
				best = data->tiles_map[i][j]->lst_size;
				save = data->tiles_map[i][j];
			}
		}
	}
	return (save);
}

void	print_map(Data *data) {
	printf("\e[1;1H\e[2J");
	for (int i = 0; i < MAP_RESULT_H+(TILE_SIZE-1); i++) {
		for (int j = 0; j < MAP_RESULT_W+(TILE_SIZE-1); j++) {
			if (data->res_map[i][j] == 0) {
				printf("?");
			} else {
				printf("%c", data->res_map[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
}

int main(int ac, char **av) {
	char	***tiles_lst;
	int		nb_tiles;
	Data	*data;
	
	if (ac < 2) {
		fprintf(stderr, "Need bmp file as 1st arg\n");
		return (1);
	}
	srand(time(NULL));
	data = init(av[1]);
	
	tiles_lst = create_all_tiles(data, &nb_tiles);
	init_tiles_map(data, tiles_lst, nb_tiles);
	init_res_map(data);
	
	//Tile	*actu = data->tiles_map[MAP_RESULT_H/2][MAP_RESULT_W/2];
	Tile	*actu = data->tiles_map[0][0];
	while (actu) {
		update_final_map(data, actu);
		//print_map(data);
		check_tiles_lst(data, actu->x, actu->y);
		actu = choose_tile(data);
	}
	print_map(data);
	save_bmp(data);	
	return (0);
}




















