/***************************************************************************
 *   Copyright (C) 2005 by Patrick Heyer                                   *
 *   patrick@localhost.localdomain                                         *
 *                                                                         *
 *   This program is not free software; you can't redistribute it and/or   *
 *   modify it under any circumstances                                     *
 *                                                                         *
 ***************************************************************************/

#include <GL/glut.h>

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <map>

#include "Texture_Manager.h"
#include "TGA.h"
#include "DDS.h"
#include "PNG.h"

using namespace std;

// initialize the singleton
CTextureManager	*CTextureManager::m_singleton = 0;

// ----------------------------------------------
// getInstance() - get a pointer to the unique
// CTextureManager instance (the singleton)
// ----------------------------------------------
CTextureManager *CTextureManager::getInstance( void )
{
    if( m_singleton == 0 )
        m_singleton = new CTextureManager;

    return ((CTextureManager *)m_singleton);
}

// ----------------------------------------------
// FreeInstance() - delete the unique CTextureManager
// instance.
// ----------------------------------------------
void CTextureManager::FreeInstance( void )
{
    if( m_singleton != 0 )
    {
        delete m_singleton;
        m_singleton = 0;
    }
}

// ----------------------------------------------
// Initialize() - create a checker for the default
// texture.
// ----------------------------------------------
void CTextureManager::Initialize( void )
{
    // this is the first texture loaded. If a texture
    // can't be loaded, we use this instead

    // initialize only once!
    if( m_texlist.empty() )
    {
        CTexture *tex = new CTexture( "default" );

        // create and initialize the texture
        glGenTextures( 1, &tex->m_id );
        glBindTexture( GL_TEXTURE_2D, tex->m_id );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        m_texlist.push_back( tex );

        // create a checker for the default texture
        int i, j, c;				// temporary variable
        unsigned char *checker;		// texture data

        checker = new unsigned char[ 64 * 64 * 4 ];

        for( i = 0; i < 64; i++ )
        {
            for( j = 0; j < 64; j++ )
            {
                c = ( !(i & 8) ^ !(j & 8)) * 255;

                checker[ (i * 256) + (j * 4) + 0 ] = (unsigned char)c;
                checker[ (i * 256) + (j * 4) + 1 ] = (unsigned char)c;
                checker[ (i * 256) + (j * 4) + 2 ] = (unsigned char)c;
                checker[ (i * 256) + (j * 4) + 3 ] = (unsigned char)255;

            }
        }

        glTexImage2D( GL_TEXTURE_2D, 0, 4, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker );

        delete [] checker;
    }
}

// ----------------------------------------------
// LoadTexture() - load a texture from file. Verify
// if it has not already been loaded before. If yes,
// return stored texture's id, otherwise, load it
// and return the new texture id generated.
// ----------------------------------------------
unsigned int CTextureManager::LoadTexture( const char *filename )
{
    unsigned int	id = 0;
    GLuint tre;


    for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
    {
        if( strcmp( (*itor)->GetName(), filename ) == 0 )
        {
            (*itor)->instances++;
            cout << "Instances of "<< filename << " loaded " << (*itor)->instances << endl;
            return (*itor)->GetTexId();
        }
    }

    //if( strstr( filename, ".bmp" ) || strstr( filename, ".BMP" ) )
    //success = LoadFileBMP( filename, &texels, &width, &height, true );

    if( strstr( filename, ".tga" ) || strstr( filename, ".TGA" ) )
    {
        tre = loadTGATexture( filename );

        if( tre>0 )
        {
            // create and initialize new texture
            id=tre;
            CTexture *tex = new CTexture( id, filename, 1 );
            m_texlist.push_back( tex );
            cout << "Instances of "<< filename << " loaded 1" << endl;

        }
        else
        {
            // can't load the texture, use default texture
            cout << "error loading TGA texture " << filename << " using default" << endl;
            id = (*m_texlist.begin())->GetTexId();
        }
    }
    else if( strstr( filename, ".dds" ) || strstr( filename, ".DDS" ) )
    {
        tre = loadDDSTexture( filename );
        if( tre>0 )
        {
            // create and initialize new texture
            id=tre;
            CTexture *tex = new CTexture( id, filename, 1);
            m_texlist.push_back( tex );
            cout << filename << " loaded " << endl;
        }
        else
        {
            // can't load the texture, use default texture
            cout << "error loading DDS texture " << filename << " using default" << endl;
            id = (*m_texlist.begin())->GetTexId();
        }
    }

    else if( strstr( filename, ".png" ) || strstr( filename, ".PNG" ) )
    {
        tre = loadPNGTexture( filename );
        if( tre>0 )
        {
            // create and initialize new texture
            id=tre;
            CTexture *tex = new CTexture( id, filename, 1);
            m_texlist.push_back( tex );
            cout << filename << " loaded " << endl;
        }
        else
        {
            // can't load the texture, use default texture
            cout << "error loading PNG texture " << filename << " using default" << endl;
            id = (*m_texlist.begin())->GetTexId();
        }
    }
    else
    {
        // can't load the texture, use default texture
        cout << "error loading texture " << filename << " Unknown format using default" << endl;
        id = (*m_texlist.begin())->GetTexId();
    }

    return id;
}

// ----------------------------------------------
// DeleteTexture() - delete texture.
// ----------------------------------------------
void CTextureManager::DeleteTexture( unsigned int id )
{
    for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
    {
        if( (*itor)->GetTexId() == id )
        {
            if ((*itor)->instances>1)
            {
                (*itor)->instances--;
                cout << "Instances of "<< (*itor)->m_name << " left " << (*itor)->instances << endl;
            }
            else
            {
                glDeleteTextures(1, &id);
                cout << "Deleted " << (*itor)->m_name << endl;
                delete (*itor);
                itor = m_texlist.erase( itor );
            }
        }
    }
}

void CTextureManager::DeleteTexture( const char *filename )
{
    for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
    {
        if( strcmp( (*itor)->GetName(), filename ) == 0 )
        {

            if ((*itor)->instances>1)
            {
                (*itor)->instances--;
                cout << "Instances of "<< (*itor)->m_name << " left " << (*itor)->instances << endl;
            }
            else
            {
                GLuint var=(*itor)->GetTexId();
                glDeleteTextures(1, &var);
                cout << "Deleted " << (*itor)->m_name << endl;
                delete (*itor);
                itor = m_texlist.erase( itor );
            }
        }
    }
}

// ----------------------------------------------
// DeleteTexture() -Force delete texture.
// ----------------------------------------------
void CTextureManager::ForceDeleteTexture( unsigned int id )
{
    for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
    {
        if( (*itor)->GetTexId() == id )
        {
            glDeleteTextures(1, &id);
            cout << "Force deleted " << (*itor)->m_name << endl;
            delete (*itor);
            itor = m_texlist.erase( itor );
        }
    }
}

void CTextureManager::ForceDeleteTexture( const char *filename )
{
    for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
    {
        if( strcmp( (*itor)->GetName(), filename ) == 0 )
        {
            GLuint var=(*itor)->GetTexId();
            glDeleteTextures(1, &var);
            cout << "Force deleted " << (*itor)->m_name << endl;
            delete (*itor);
            itor = m_texlist.erase( itor );

        }
    }
}

// ----------------------------------------------
// CleanAllTextures() - delete all textures stored
// in the linked list.
// ----------------------------------------------
void CTextureManager::CleanAllTextures( void )
{
    GLuint var;
    for( TListItor itor = m_texlist.begin(); itor != m_texlist.end(); ++itor )
    {
        var=(*itor)->GetTexId();
        glDeleteTextures(1, &var);
        delete (*itor);
        itor = m_texlist.erase( itor );
    }
}
