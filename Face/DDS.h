/***************************************************************************
 *   Copyright (C) 2006 by Patrick Heyer                                   *
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

    GLenum  format;
    GLint 	internalFormat;
    GLuint  id;

    GLubyte *texels;

    GLint numMipmaps;

}
gl_texture_dds;


/* DirectDraw's structures */
typedef struct
{
    GLuint size;
    GLuint flags;
    GLuint fourCC;
    GLuint bpp;
    GLuint redMask;
    GLuint greenMask;
    GLuint blueMask;
    GLuint alphaMask;

}
DDPixelFormat;

typedef struct
{
    GLuint caps;
    GLuint caps2;
    GLuint caps3;
    GLuint caps4;

}
DDSCaps;

typedef struct
{
    GLuint lowVal;
    GLuint highVal;

}
DDColorKey;

typedef struct
{
    GLuint size;
    GLuint flags;
    GLuint height;
    GLuint width;
    GLuint pitch;
    GLuint depth;
    GLuint mipMapLevels;
    GLuint alphaBitDepth;
    GLuint reserved;
    GLuint surface;

    DDColorKey ckDestOverlay;
    DDColorKey ckDestBlt;
    DDColorKey ckSrcOverlay;
    DDColorKey ckSrcBlt;

    DDPixelFormat format;
    DDSCaps caps;

    GLuint textureStage;

}
DDSurfaceDesc;

#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
  (GLuint)( \
    (((GLuint)(GLubyte)(ch3) << 24) & 0xFF000000) | \
    (((GLuint)(GLubyte)(ch2) << 16) & 0x00FF0000) | \
    (((GLuint)(GLubyte)(ch1) <<  8) & 0x0000FF00) | \
     ((GLuint)(GLubyte)(ch0)        & 0x000000FF) )

#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')

#ifndef max
static int
max (int a, int b)
{
    return ((a > b) ? a : b);
}
#endif

GLuint loadDDSTexture (const char *filename);
