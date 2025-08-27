#include <header.hpp>

/* ************************************************************************** */
/*                                                                            */
/*   CONSTRUCTEUR															  */
/*                                                                            */
/* ************************************************************************** */
ScopApplication::ScopApplication(const int argc, const char **argv) {
    if (argc != 2 && argc != 3)
        throw invalid_argument(COLOR_BOLD_RED "\n usage: ./scop <models/model_name.obj>\n or\n.       ./scop <models/model_name.obj> <textures/texture_name.jpg>" COLOR_RESET);

    this->_modelPath = argv[1];
    if (argc == 3)
        this->_texturePath = argv[2];

#ifdef USE_GLM
    cout << COLOR_ITALIC_BLUE<< "glm used insted of mathft" << COLOR_RESET << endl;
#else
    cout << COLOR_ITALIC_BLUE << "mathft used insted of glm" << COLOR_RESET << endl;
#endif

#ifdef USE_LOADLIB
    cout << COLOR_ITALIC_BLUE<< "stb_image and tiny_obj_loader used insted of loaderft" << COLOR_RESET << endl;
#else
    cout << COLOR_ITALIC_BLUE << "loaderft used insted of stb_image and tiny_obj_loader" << COLOR_RESET << endl;
#endif
}

/* ************************************************************************** */
/*                                                                            */
/*   COPY CONSTRUCTEUR					    								  */
/*                                                                            */
/* ************************************************************************** */
ScopApplication::ScopApplication(const ScopApplication &src) {
	(void)src;
}

/* ************************************************************************** */
/*                                                                            */
/*   DESTRUCTEUR															  */
/*                                                                            */
/* ************************************************************************** */
ScopApplication::~ScopApplication(void) {}