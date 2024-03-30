include .env
export

NAME		= scop

INC_DIR		= include/
SRC_DIR		= source/
OBJ_DIR		= object/

CC			= c++

CFLAGS		= -Wall -Wextra -Werror -Wshadow -Wconversion -std=c++98

# **************************************************************************** #
#                                                                              #
#   -I   | Chemin du dossier où trouver un .h								   #
#   -L   | Chemin du dossier où trouver un .a								   #
#   -l   | Nom du .a sans le préfixe "lib"									   #
#                                                                              #
# **************************************************************************** #

I_HEADERS	= -I $(INC_DIR) -I $(VULKAN_SDK)/include

LDFLAGS		= -L $(VULKAN_SDK)/lib
LDLIBS		= -l vulkan

HEADERS		= $(shell find $(INC_DIR) -type f -name '*.hpp')

NAME_FILE   = $(shell find $(SRC_DIR) -type f -name '*.cpp')

OBJ         = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(NAME_FILE))
DEPENDS     = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.d, $(NAME_FILE))

# **************************************************************************** #
#                                                                              #
#   $@   | Le nom de la cible												   #
#   $<   | Le nom de la première dépendance									   #
#   $^   | La liste des dépendances											   #
#   $?   | La liste des dépendances plus récentes que la cible				   #
#   $*   | Le nom du fichier sans suffixe									   #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#   Relink si les headers ou le Makfile sont changés                           #
#                                                                              #
# *****************************vvvvvvvvvvvvvvvvvvv**************************** #

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADERS) Makefile
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(I_HEADERS) -MMD -MP -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(I_HEADERS) $(LDFLAGS) $(LDLIBS) -o $(NAME) -Wl,-rpath,$(VULKAN_SDK)/lib

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

# **************************************************************************** #
#   Linux                                                                      #
# **************************************************************************** #

valgrind_memcheck_fd: $(OBJ)
	valgrind --tool=memcheck --track-origins=yes --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

valgrind_memcheck: $(OBJ)
	valgrind --tool=memcheck --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(NAME)

valgrind_helgrind: $(OBJ)
	valgrind --tool=helgrind --history-level=full ./$(NAME)

# **************************************************************************** #
#   MacOs                                                                      #
# **************************************************************************** #

leaks: $(NAME)
	@leaks --atExit -- ./$(NAME)

# **************************************************************************** #

.PHONY: all clean fclean re valgrind_memcheck_fd valgrind_memcheck valgrind_helgrind leaks

-include $(DEPENDS)
