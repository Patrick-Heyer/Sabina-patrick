/***************************************************************************
 *   Copyright (C) 2005 by Patrick Heyer                                   *
 *   patrick@localhost.localdomain                                         *
 *                                                                         *
 *   This program is not free software; you can't redistribute it and/or   *
 *   modify it under any circumstances                                     *
 *                                                                         *
 ***************************************************************************/

#pragma once

/* OpenGL texture info */
typedef struct
{
    GLsizei width;
    GLsizei height;

    GLenum 	format;
    GLint	internalFormat;
    GLuint 	id;

    GLubyte *texels;

}
gl_texture_tga;

#pragma pack(push, 1)
/* tga header */
typedef struct
{
    GLubyte id_lenght;          /* size of image id */
    GLubyte colormap_type;      /* 1 is has a colormap */
    GLubyte image_type;         /* compression type */

    short	cm_first_entry;       /* colormap origin */
    short	cm_length;            /* colormap length */
    GLubyte cm_size;            /* colormap size */

    short	x_origin;             /* bottom left x coord origin */
    short	y_origin;             /* bottom left y coord origin */

    short	width;                /* picture width (in pixels) */
    short	height;               /* picture height (in pixels) */

    GLubyte pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
    GLubyte image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */

}
tga_header_t;
#pragma pack(pop)

GLuint loadTGATexture (const char *filename);
