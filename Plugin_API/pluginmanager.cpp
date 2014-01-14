
#include "pluginmanager.h"
#include "coreapi.h"
#include <string>
#include <map>
#include <set>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>

#if _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

class PluginInstance::Impl
{
public:
	std::string mFileName;
	std::string mDisplayName;
#if _WIN32
	// Plugin loading routines for Windows
	HINSTANCE handle;

	typedef FARPROC PluginFunc;

	bool Load()
	{
		handle = LoadLibrary(mFileName.c_str());
		return (handle != NULL);
	}

	bool Unload()
	{
		if (!handle)
			return true;
		return FreeLibrary(handle) != 0;
	}

	PluginFunc GetFunction(const std::string &name)
	{
		PluginFunc fptr = GetProcAddress(handle, name.c_str());
		if (! fptr)
			return NULL;
		return fptr;
	}
#else
	// Plugin loading routines for Mac & Linux
	void *handle;

	typedef void(*PluginFunc)();

	bool Load()
	{
		handle = dlopen(mFileName.c_str(), RTLD_LOCAL | RTLD_LAZY);
		return (handle != NULL);
	}

	bool Unload()
	{
		if (!handle)
			return true;
		return dlclose(handle) == 0;
	}

	PluginFunc GetFunction(const std::string &name)
	{
		dlerror();
		PluginFunc fptr = (PluginFunc) dlsym(handle, name.c_str());
		if (dlerror())
			return NULL;
		return fptr;
	}
#endif


};

PluginInstance::PluginInstance(const std::string &name)
{
	mImpl = new Impl;
	mImpl->mDisplayName = name;

#if _WIN32
	mImpl->mFileName = name + ".dll";
#elif __APPLE__
	mImpl->mFileName = "lib" + name + ".dylib";
#else
	mImpl->mFileName = name;
#endif
}

PluginInstance::~PluginInstance()
{
	delete mImpl;
}

bool PluginInstance::Load()
{
	if (! mImpl->Load())
		return false;

	Impl::PluginFunc init_func = mImpl->GetFunction("PluginInit");
	if (!init_func)
		return false;

	(*init_func)();

	Impl::PluginFunc name_string = mImpl->GetFunction("PluginDisplayName");
	if (name_string)
	{
		const char **ptr = (const char **) name_string;
		mImpl->mDisplayName = *ptr;
	}

	return true;
}

bool PluginInstance::Unload()
{
	return mImpl->Unload();
}

bool PluginInstance::IsLoaded()
{
	return (mImpl->handle != NULL);
}

std::string PluginInstance::GetFileName()
{
	return mImpl->mFileName;
}

std::string PluginInstance::GetDisplayName()
{
	return mImpl->mDisplayName;
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

PluginManager &PluginManager::getInstance()
{
	static PluginManager mgr;
	return mgr;
}

bool PluginManager::Load(const std::string &filename)
{
    std::string temp;
    for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
    {
        if((*itor)->GetFileName().find_last_of(".") != std::string::npos)
        {
            temp=(*itor)->GetFileName().substr(0,(*itor)->GetFileName().find_last_of("."));
        }
        
        if( strcmp( temp.substr(3).c_str(), filename.c_str() ) == 0 )
        {
            std::cout << temp.substr(3) << " Plugin alredy loaded" << std::endl;
            return true;
        }
    }
    


	// try to load this plugin
	PluginInstance *pi = new PluginInstance(filename);
	if (! pi->Load())
	{
		delete pi;
		return false;
	}

	// success! add the plugin to our manager
	//mPlugins.push_back(pi);
	m_pluginlist.push_back(pi);
    std::cout << pi->GetDisplayName() << " Plugin loaded" << std::endl;

	return true;
}

bool PluginManager::Execute(const std::string& name)
{
    
    for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
    {
        if( strcmp( (*itor)->GetDisplayName().c_str(), name.c_str() ) == 0 )
        {
            (*itor)->actual=PluginFactory::CreatePlugin(name.c_str());
            (*itor)->actual->run();
            return true;
        }
    }
    
}

bool PluginManager::ExecuteAll()
{
    for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
    {
 
        (*itor)->actual=PluginFactory::CreatePlugin((*itor)->GetDisplayName().c_str());
            (*itor)->actual->run();
    }
    return true;
}



bool PluginManager::UnloadAll()
{
    for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
    {
		(*itor)->actual->stop();
		
	}
	return false;
}

bool PluginManager::Unload(const std::string &name)
{
  
    for( TListItor itor = m_pluginlist.begin(); itor != m_pluginlist.end(); ++itor )
    {
        if( strcmp( (*itor)->GetDisplayName().c_str(), name.c_str() ) != 0 )
        {
            (*itor)->actual->stop();
            return true;
        }
    }
}

bool PluginManager::LoadFromFile(const std::string &filename)
{
	// If we had an external metadata file that described all of the
	// plugins, this would be the point where we load that file -
	// without loading the actual plugins.  Without that external
	// metadata, we need to load all plugins.
	std::ifstream inFile;
    std::string plugin_name;
    
    inFile.open(filename.c_str());
    if (!inFile) {
        std::cout << "Unable to open file" <<std::endl;
        return false;
    }
    
    while (inFile >> plugin_name) {
        Load(plugin_name);
    }
    
    inFile.close();
    
	return true;
}
