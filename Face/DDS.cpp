/***************************************************************************
 *   Copyright (C) 2006 by Patrick Heyer                                   *
 *   patrick@localhost.localdomain                                         *
 *                                                                         *
 *   This program is not free software; you can't redistribute it and/or   *
 *   modify it under any circumstances                                     *
 *                                                                         *
 ***************************************************************************/

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DDS.h"

gl_texture_dds *ReadDDSFile (const char *filename)
{
    DDSurfaceDesc ddsd;
    gl_texture_dds *texinfo;
    FILE *fp;
    char magic[4];
    int mipmapFactor;
    long bufferSize, curr, end;

    /* open the file */
    fp = fopen (filename, "rb");
    if (!fp)
    {
        fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
        return NULL;
    }

    /* read magic number and check if valid .dds file */
    fread (&magic, sizeof (char), 4, fp);

    if (strncmp (magic, "DDS ", 4) != 0)
    {
        fprintf (stderr, "the file \"%s\" doesn't appear to be"
                 "a valid .dds file!\n", filename);
        fclose (fp);
        return NULL;
    }

    /* get the surface descriptor */
    fread (&ddsd, sizeof (ddsd), 1, fp);

    texinfo = (gl_texture_dds *)malloc (sizeof (gl_texture_dds));
    memset (texinfo, 0, sizeof (gl_texture_dds));
    texinfo->width = ddsd.width;
    texinfo->height = ddsd.height;
    texinfo->numMipmaps = ddsd.mipMapLevels;

    switch (ddsd.format.fourCC)
    {
    case FOURCC_DXT1:
        /* DXT1's compression ratio is 8:1 */
        texinfo->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        texinfo->internalFormat = 3;
        mipmapFactor = 2;
        break;

    case FOURCC_DXT3:
        /* DXT3's compression ratio is 4:1 */
        texinfo->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        texinfo->internalFormat = 4;
        mipmapFactor = 4;
        break;

    case FOURCC_DXT5:
        /* DXT5's compression ratio is 4:1 */
        texinfo->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        texinfo->internalFormat = 4;
        mipmapFactor = 4;
        break;

    default:
        /* bad fourCC, unsupported or bad format */
        fprintf (stderr, "the file \"%s\" doesn't appear to be"
                 "compressed using DXT1, DXT3, or DXT5! [%i]\n",
                 filename, ddsd.format.fourCC);
        free (texinfo);
        fclose (fp);
        return NULL;
    }

    /* calculate pixel data size */
    curr = ftell (fp);
    fseek (fp, 0, SEEK_END);
    end = ftell (fp);
    fseek (fp, curr, SEEK_SET);
    bufferSize = end - curr;

    /* read pixel data with mipmaps */
    texinfo->texels = (GLubyte *)malloc (bufferSize * sizeof (GLubyte));
    fread (texinfo->texels, sizeof (GLubyte), bufferSize, fp);

    /* close the file */
    fclose (fp);
    return texinfo;
}

GLuint loadDDSTexture (const char *filename)
{
    gl_texture_dds *compressed_texture = NULL;
    GLsizei mipWidth, mipHeight, mipSize;
    int blockSize, offset;
    GLuint tex_id = 0;
    GLint mip;

    /* read texture from file */
    compressed_texture = ReadDDSFile (filename);

    if (compressed_texture && compressed_texture->texels)
    {
        /* generate new texture */
        glGenTextures (1, &compressed_texture->id);
        glBindTexture (GL_TEXTURE_2D, compressed_texture->id);

        /* setup some parameters for texture filters and mipmapping */
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        mipWidth = compressed_texture->width;
        mipHeight = compressed_texture->height;
        blockSize = (compressed_texture->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
        offset = 0;

        /* upload mipmaps to video memory */
        for (mip = 0; mip < compressed_texture->numMipmaps; ++mip)
        {
            mipSize = ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4) * blockSize;

            glCompressedTexImage2D (GL_TEXTURE_2D, mip, compressed_texture->format,
                                    mipWidth, mipHeight, 0, mipSize,
                                    compressed_texture->texels + offset);

            mipWidth = max (mipWidth >> 1, 1);
            mipHeight = max (mipHeight >> 1, 1);

            offset += mipSize;
        }

        tex_id = compressed_texture->id;

        /* opengl has its own copy of pixels */
        free (compressed_texture->texels);
        free (compressed_texture);
    }


    return tex_id;
}
