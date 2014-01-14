/***************************************************************************
 *   Copyright (C) 2005 by Patrick Heyer                                   *
 *   patrick@localhost.localdomain                                         *
 *                                                                         *
 *   This program is not free software; you can't redistribute it and/or   *
 *   modify it under any circumstances                                     *
 *                                                                         *
 ***************************************************************************/

#pragma once

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

/* OpenGL texture info */
struct gl_texture_t
{
    GLsizei width;
    GLsizei height;

    GLenum format;
    GLint internalFormat;
    GLuint id;

    GLubyte *texels;
};

extern GLuint loadPNGTexture (const char *filename);
