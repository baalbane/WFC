
NAME = wfc

CC = gcc
OPTFLAG= -O3 -lm -g


SRCS_PATH = srcs/
SRCS_NAME =		handle_bmp.c
				
SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))


OBJS_PATH = objs/
OBJS_NAME = $(SRCS_NAME:.c=.o)
OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))


INCLUDES_PATH = includes/
INCLUDES = $(addprefix -I , $(INCLUDES_PATH))

all: $(NAME) 

$(NAME): obj
	$(CC) srcs/main.c $(OBJS) $(OPTFLAG) -o $(NAME) $(INCLUDES)
	
obj :
	@mkdir objs
	$(CC) $(OPTFLAG) -c $(SRCS) $(INCLUDES)
	@mv *.o objs/
	
clean:
	rm -rf $(OBJS)
	
fclean: clean
	rm -rf $(NAME)
	
re: clean all

install:
	./scripts/install_cbmp.sh