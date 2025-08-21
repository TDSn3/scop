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