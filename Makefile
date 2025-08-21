include .env
export

NAME		= scop

INC_DIR		= include/
SRC_DIR		= source/
OBJ_DIR		= object/

CXX			= c++

# **************************************************************************** #
#                                                                              #
#   CXXFLAGS += -DNDEBUG       | Désactive les validation layers               #
#   CXXFLAGS += -Wshadow       | Avertit si une variable masque une autre      #
#   CXXFLAGS += -Wconversion   | Avertit sur les conversions implicites        #
#                                                                              #
# **************************************************************************** #

CXXFLAGS	= -Wall -Wextra -Werror -Wshadow -std=c++17

# **************************************************************************** #
#                                                                              #
#   Dépendance GLFW (téléchargement + build auto)                              #
#                                                                              #
# **************************************************************************** #

# GLFW_VERSION	> ./.env

GLFW_URL		= https://github.com/glfw/glfw/archive/refs/tags/$(GLFW_VERSION).tar.gz
GLFW_TARBALL	= library/glfw-$(GLFW_VERSION).tar.gz
GLFW_SRC_DIR	= library/glfw-$(GLFW_VERSION)
GLFW_BUILD_DIR	= $(GLFW_SRC_DIR)/build
GLFW_LIB		= $(GLFW_BUILD_DIR)/src/libglfw3.a

# **************************************************************************** #
#                                                                              #
#   GLM                                                                        #
#                                                                              #
# **************************************************************************** #

# GLM_VERSION	> 1.0.1

GLM_URL			= https://github.com/g-truc/glm/archive/refs/tags/$(GLM_VERSION).tar.gz
GLM_SRC_DIR		= library/glm-$(GLM_VERSION)
GLM_DIR			= library/glm-master

# **************************************************************************** #
#                                                                              #
#   stb_image.h                                                                #
#                                                                              #
# **************************************************************************** #

STB_URL      = https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
STB_DIR      = library/stb
STB_HEADER   = $(STB_DIR)/stb_image.h

# **************************************************************************** #
#                                                                              #
#   tiny_obj_loader.h                                                          #
#                                                                              #
# **************************************************************************** #

TINYOBJ_URL    = https://raw.githubusercontent.com/tinyobjloader/tinyobjloader/release/tiny_obj_loader.h
TINYOBJ_DIR    = library/tinyobj
TINYOBJ_HEADER = $(TINYOBJ_DIR)/tiny_obj_loader.h

# **************************************************************************** #
#                                                                              #
#   -I   | Chemin du dossier où trouver un .h								   #
#   -L   | Chemin du dossier où trouver un .a								   #
#   -l   | Nom du .a sans le préfixe "lib"									   #
#                                                                              #
# **************************************************************************** #

# VULKAN_SDK	> ./.env

I_HEADERS		= -I $(INC_DIR)					\
				  -I $(VULKAN_SDK)/include		\
				  -I $(GLFW_SRC_DIR)/include	\
				  -I $(GLM_DIR)					\
				  -I $(STB_DIR)					\
				  -I $(TINYOBJ_DIR)

LDFLAGS			= -L $(VULKAN_SDK)/lib			\
				  -L $(GLFW_BUILD_DIR)/src		\
				  -Wl,-rpath,$(VULKAN_SDK)/lib

# **************************************************************************** #
#                                                                              #
#   Addapation à l'OS                                                          #
#                                                                              #
# **************************************************************************** #

UNAME							= $(shell uname)

ifeq ($(shell uname), Darwin)				# macOS
#   MACOSX_DEPLOYMENT_TARGET	> ./.env

	ARCH						= $(shell uname -m)

	CXXFLAGS				   += -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET) -arch $(ARCH)
	LDFLAGS					   += -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET) -arch $(ARCH)

	GLFW_CMAKE_FLAGS			= -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release		\
	                			  -DCMAKE_OSX_DEPLOYMENT_TARGET=$(MACOSX_DEPLOYMENT_TARGET)	\
	                			  -DCMAKE_OSX_ARCHITECTURES=$(ARCH)

	LDLIBS						= -l vulkan					\
								  -l glfw3					\
								  -framework Cocoa			\
								  -framework IOKit			\
								  -framework CoreVideo		\
								  -framework CoreFoundation
else										# Linux and others
	GLFW_CMAKE_FLAGS			= -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release
	
	LDLIBS						= -l vulkan					\
								  -l glfw					\
								  -l X11					\
								  -l pthread				\
								  -l dl						\
								  -l Xrandr					\
								  -l Xi
endif

HEADERS		= $(shell find $(INC_DIR) -type f -name '*.hpp')

NAME_FILE	= $(shell find $(SRC_DIR) -type f -name '*.cpp')

OBJ			= $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(NAME_FILE))
DEPENDS		= $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.d, $(NAME_FILE))

# **************************************************************************** #
#                                                                              #
#   Shaders                                                                    #
#                                                                              #
# **************************************************************************** #

SHADERS_DIR   	= shaders
SHADERS_SCRIPT	= $(SHADERS_DIR)/compile.sh

VERT_SRC		= $(SHADERS_DIR)/shader.vert
FRAG_SRC		= $(SHADERS_DIR)/shader.frag
VERT_SPV		= $(SHADERS_DIR)/vert.spv
FRAG_SPV		= $(SHADERS_DIR)/frag.spv

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

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADERS) Makefile | $(GLM_DIR) $(STB_HEADER) $(TINYOBJ_HEADER)
	@ mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(I_HEADERS) -MMD -MP -c $< -o $@

all: $(NAME)

$(NAME): $(GLFW_LIB) $(OBJ) | $(VERT_SPV) $(FRAG_SPV)
	$(CXX) $(OBJ) $(I_HEADERS) $(LDFLAGS) $(LDLIBS) -o $(NAME)

# **************************************************************************** #
#                                                                              #
#   Téléchargement + extraction GLFW                                           #
#                                                                              #
# **************************************************************************** #

$(GLFW_TARBALL):
	@ mkdir -p library
	@ echo ">> Téléchargement GLFW $(GLFW_VERSION)"
	@ if command -v curl >/dev/null 2>&1; then		\
		curl -L "$(GLFW_URL)" -o "$(GLFW_TARBALL)";	\
	else											\
		wget -O "$(GLFW_TARBALL)" "$(GLFW_URL)";	\
	fi

$(GLFW_SRC_DIR): $(GLFW_TARBALL)
	@ echo ">> Extraction GLFW -> $(GLFW_SRC_DIR)"
	@ tar -xzf "$(GLFW_TARBALL)" -C library
	@ [ -d "$(GLFW_SRC_DIR)" ] || (echo "Extraction GLFW échouée"; exit 1)

# **************************************************************************** #
#                                                                              #
#   Build GLFW                                                                 #
#                                                                              #
# **************************************************************************** #

$(GLFW_LIB): $(GLFW_SRC_DIR)
	@ echo ">> Build GLFW dans $(GLFW_BUILD_DIR)"
	@ env -u CC -u CFLAGS -u CXX -u CXXFLAGS cmake -S "$(GLFW_SRC_DIR)" -B "$(GLFW_BUILD_DIR)" $(GLFW_CMAKE_FLAGS)
	@ env -u CC -u CFLAGS -u CXX -u CXXFLAGS cmake --build "$(GLFW_BUILD_DIR)" --config Release -- -j
	@ $(MAKE) check-glfw

# **************************************************************************** #
#                                                                              #
#   Téléchargement + extraction GLFM                                           #
#                                                                              #
# **************************************************************************** #

$(GLM_DIR):
	@ mkdir -p library
	@ if [ ! -d "$(GLM_DIR)" ]; then						\
		echo ">> Fetch + extract GLM $(GLM_VERSION)";		\
		if command -v curl >/dev/null 2>&1; then			\
			curl -L "$(GLM_URL)" | tar -xz -C library;		\
		else												\
			wget -qO- "$(GLM_URL)" | tar -xz -C library;	\
		fi;													\
		mv "$(GLM_SRC_DIR)" "$(GLM_DIR)";					\
	fi

# **************************************************************************** #
#                                                                              #
#   Téléchargement stb_image.h                                                 #
#                                                                              #
# **************************************************************************** #

$(STB_HEADER):
	@ mkdir -p $(STB_DIR)
	@ echo ">> Téléchargement stb_image.h"
	@ if command -v curl >/dev/null 2>&1; then		\
		curl -fsSL "$(STB_URL)" -o "$(STB_HEADER)"; \
	else											\
		wget -qO "$(STB_HEADER)" "$(STB_URL)";		\
	fi
	@ [ -s "$(STB_HEADER)" ] || (echo "Téléchargement de stb_image.h échoué"; rm -f "$(STB_HEADER)"; exit 1)

# **************************************************************************** #
#                                                                              #
#   Téléchargement tiny_obj_loader.h                                           #
#                                                                              #
# **************************************************************************** #

$(TINYOBJ_HEADER):
	@ mkdir -p $(TINYOBJ_DIR)
	@ echo ">> Téléchargement tiny_obj_loader.h"
	@ if command -v curl >/dev/null 2>&1; then		\
		curl -fsSL "$(TINYOBJ_URL)" -o "$(TINYOBJ_HEADER)"; \
	else											\
		wget -qO "$(TINYOBJ_HEADER)" "$(TINYOBJ_URL)";	\
	fi
	@ [ -s "$(TINYOBJ_HEADER)" ] || (echo "Téléchargement de tiny_obj_loader.h échoué"; rm -f "$(TINYOBJ_HEADER)"; exit 1)


# **************************************************************************** #
#                                                                              #
#   Compilation des shaders                                                    #
#                                                                              #
# **************************************************************************** #

$(VERT_SPV): $(VERT_SRC) $(FRAG_SRC) $(SHADERS_SCRIPT)
	@ echo ">> Compiling shaders"
	@ chmod +x $(SHADERS_SCRIPT) || true
	@ cd $(SHADERS_DIR) && ./$(notdir $(SHADERS_SCRIPT))

$(FRAG_SPV): $(VERT_SPV) ;

# **************************************************************************** #

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

libclean:
	rm -rf ./library

shader: $(VERT_SPV) $(FRAG_SPV)

shaderclean:
	rm -f $(VERT_SPV) $(FRAG_SPV)

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
#   MacOS                                                                      #
# **************************************************************************** #

leaks: $(NAME)
	@ leaks --atExit -- ./$(NAME)

check-glfw:
ifeq ($(UNAME), Darwin)
	@ obj=$$(find "$(GLFW_BUILD_DIR)/src/CMakeFiles/glfw.dir" -name '*.o' | head -n 1);		\
	if [ -z "$$obj" ]; then echo "Aucun objet GLFW pour vérif."; exit 1; fi;				\
	echo ">> Vérif macOS : minOS & arch sur $$obj";											\
	minos=$$(otool -l "$$obj" | awk '/LC_BUILD_VERSION/{f=1} f&&/minos/{print $$2; exit}');	\
	echo "   minOS détecté: $$minos — attendu: $(MACOSX_DEPLOYMENT_TARGET)";				\
	test "$$minos" = "$(MACOSX_DEPLOYMENT_TARGET)" || (echo "   ERREUR: mismatch de cible macOS"; exit 1)
else
	@ obj=$$(find "$(GLFW_BUILD_DIR)/src/CMakeFiles/glfw.dir" -name '*.o' | head -n 1);	\
	if [ -z "$$obj" ]; then echo "Aucun objet GLFW pour vérif."; exit 1; fi;			\
	echo ">> Vérif Linux : arch objet vs arch système";									\
	echo -n "   Objet: "; file "$$obj" | sed 's/.*: //';								\
	echo "   Système: $(shell uname -m)"
endif

# **************************************************************************** #

opti: CXXFLAGS += -O3 -march=native -DNDEBUG
opti: CXXFLAGS := $(filter-out -Werror,$(CXXFLAGS))
opti: re

debug: CXXFLAGS += -g -O0
debug: re

# **************************************************************************** #

.PHONY: all clean fclean libclean shader shaderclean re valgrind_memcheck_fd valgrind_memcheck valgrind_helgrind leaks check-glfw opti

-include $(DEPENDS)
