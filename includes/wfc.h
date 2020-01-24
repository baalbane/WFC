#ifndef WFC_H_
# define WFC_H_


# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <time.h>
# include <string.h>

# define			MAP_RESULT_H	100
# define			MAP_RESULT_W	200

# define			TILE_SIZE		7

typedef struct		t_pix_to_char {
	unsigned char	R;	
	unsigned char	G;	
	unsigned char	B;
	char			ascii;
}					Pix_to_char;


typedef struct		t_tile {
	int				x;
	int				y;
	int				lst_size;
	int				lst_max_size;
	char			***tiles_lst;
}					Tile;


typedef struct		t_data {
	char			**base_map;
	int				b_H;
	int				b_W;
	Tile			***tiles_map;
	char			**res_map;
	Pix_to_char		**table;
}					Data;



Data	*init(const char *file_name);
void	save_bmp(Data *data);








#endif