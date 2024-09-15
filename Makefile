NAME=minishell
CC=gcc
CFLAGS=-Wall -Wextra -Werror -g3
LIB_PATH=libft/
LIBFT= ${LIB_PATH}/libft.a
INCLUDES=-I./includes -I./${SRC_PATH} -I./${LIB_PATH}
LINK = -L${LIB_PATH} -lft -lreadline

SRC_PATH = sources/
SRC_FILES = ast.c \
			main.c	\
			interpret.c \
			command.c \
			parser.c \
			parser_utils.c \
			hashtable.c \
			hashtable_utils.c \
			free.c \
			env_variable.c \
			string.c \
			redir.c
SRC_OBJ = ${addprefix ${SRC_PATH}, ${SRC_FILES}}

OBJ_PATH = obj/
OBJ_FILES = ${SRC_FILES:.c=.o} 
OBJECTS = ${addprefix ${OBJ_PATH}, ${OBJ_FILES}}

all : ${NAME}

${NAME} : ${OBJECTS} ${LIBFT}
	${CC} -DDEBUG ${CFLAGS} ${INCLUDES}	${OBJECTS} ${LINK} -o $@

${LIBFT} :
	make -C ${LIB_PATH}

${OBJ_PATH}%.o : ${SRC_PATH}%.c | ${OBJ_PATH}
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

${OBJ_PATH} :
	mkdir -p ${OBJ_PATH}

clean :
	make clean -C ${LIB_PATH}
	rm -rf ${OBJ_PATH}

fclean : clean
	make fclean -C ${LIB_PATH}
	rm -f ${NAME}

re : fclean all

tempo :
	gcc -g3 -lreadline mog.c

debug : ${NAME}
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --leak-resolution=high --trace-children=yes ./${NAME}
