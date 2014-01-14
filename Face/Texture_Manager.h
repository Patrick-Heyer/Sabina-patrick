/***************************************************************************
 *   Copyright (C) 2005 by Patrick Heyer                                   *
 *   patrick@localhost.localdomain                                         *
 *                                                                         *
 *   This program is not free software; you can't redistribute it and/or   *
 *   modify it under any circumstances                                     *
 *                                                                         *
 ***************************************************************************/
#pragma once
#include <list>
#include <string.h>
#include <iostream>
#include <cstdlib>
// ==============================================
// CTexture - texture class object.
// ==============================================
class CTexture
{
public:
	// constructors/destructor
	CTexture( void ) { }
	CTexture( const char *texname ) { m_name = texname; }
	CTexture( unsigned int texid, const char *texname, int instance )	{ m_id = texid; m_name = texname; instances=instance; }
	~CTexture( void ) { glDeleteTextures( 1, &m_id ); m_name.erase(); }

	unsigned int	GetTexId( void ) { return m_id; }
	const char		*GetName( void ) { return m_name.c_str(); }

	// members variables
	unsigned int	m_id;			// texture id
	int 			instances;
	std::string		m_name;			// texture name
};

// ==============================================
// CTextureManager - OpenGL texture manager.
// ==============================================
class CTextureManager
{
protected:
	// constructor/destructor
	CTextureManager( void ) { Initialize(); }
	virtual ~CTextureManager( void ) { CleanAllTextures(); }

public:
	// singleton functions
	static CTextureManager	*getInstance( void );
	static void				FreeInstance( void );

	// functions
	void			Initialize( void );
	unsigned int	LoadTexture( const char *filename );
	void			DeleteTexture( unsigned int id );
	void			DeleteTexture( const char *filename );
	void			ForceDeleteTexture( unsigned int id );
	void			ForceDeleteTexture( const char *filename );
	void			CleanAllTextures( void );

private:
	// linked texture list
	typedef std::list<CTexture *>	TextureList;
	typedef TextureList::iterator	TListItor;

	TextureList						m_texlist;

	// singleton
	static CTextureManager			*m_singleton;
};

// global function using the texture manager
inline unsigned int LoadTexture( const char *filename )
{
	return CTextureManager::getInstance()->LoadTexture( filename );
}


