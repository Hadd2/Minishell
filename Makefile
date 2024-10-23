NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror
LIB_PATH=libft/
LIBFT= ${LIB_PATH}/libft.a
INCLUDES=-I./includes -I./${SRC_PATH} -I./${LIB_PATH}
LINK = -L${LIB_PATH} -lft -lreadline

SRC_PATH = sources/
SRC_FILES = ast.c \
			b_bubble.c \
			b_call.c \
			b_cd.c \
			b_echo.c \
			b_env.c \
			b_exit.c \
			b_export.c \
			b_pwd.c \
			b_unset.c \
			b_utils.c \
			main.c	\
			interpret.c \
			command.c \
			parser.c \
			parser_utils.c \
			hashtable.c \
			hashtable_utils.c \
			free.c \
			env_variable.c \
			env_variable_utils.c \
			string.c \
			init.c \
			signal.c \
			file.c \
			trie.c \
			wildcard.c \
			tab.c \
			quotes.c \
			token.c \
			token_utils.c \
			debug.c \
			redir.c
SRC_OBJ = ${addprefix ${SRC_PATH}, ${SRC_FILES}}

OBJ_PATH = obj/
OBJ_FILES = ${SRC_FILES:.c=.o}
OBJECTS = ${addprefix ${OBJ_PATH}, ${OBJ_FILES}}

RED=\033[1;31m
YELLOW=\033[1;33m
BLUE=\033[1;34m
GREEN=\033[1;32m
RESET=\033[0m

all : ${NAME}

${NAME} : ${OBJECTS} ${LIBFT}
	@echo "${GREEN}${NAME}${RESET}"
	@${CC} -DDEBUG ${CFLAGS} ${INCLUDES}	${OBJECTS} ${LINK} -o $@

${LIBFT} :
	@echo "${BLUE}libft${RESET}"
	@make -C ${LIB_PATH} --no-print-directory

${OBJ_PATH}%.o : ${SRC_PATH}%.c | ${OBJ_PATH}
	@echo "${YELLOW}compiling $<${RESET}"
	@${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

${OBJ_PATH} :
	@mkdir -p ${OBJ_PATH}

clean :
	@echo "${RED}clean${RESET}"
	@make clean -C ${LIB_PATH} --no-print-directory
	@rm -rf ${OBJ_PATH}

fclean : clean
	@echo "${RED}fclean${RESET}"
	@make fclean -C ${LIB_PATH} --no-print-directory
	@rm -f ${NAME}

re : fclean all

tempo :
	@gcc -g3 -lreadline mog.c

debug : ${NAME}
	@echo "${GREEN}Valgrind${RESET}"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	--track-fds=yes --leak-resolution=high --trace-children=yes ./${NAME}
