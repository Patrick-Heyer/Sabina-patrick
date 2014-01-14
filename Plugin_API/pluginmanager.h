/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   pluginmanager.h
/// \author Martin Reddy
/// \brief  A Plugin Manager singleton
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "defines.h"

#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include "plugin.h"

///
/// An object to represent a single plugin in the system
///
class CORE_API PluginInstance
{
public:
    explicit PluginInstance(const std::string &name);
    ~PluginInstance();

    /// Load the plugin
    bool Load();
    /// Unload the plugin
    bool Unload();
    /// Return true if the plugin is loaded
    bool IsLoaded();

    bool running;

    /// Return the path to the plugin file on disk
    std::string GetFileName();
    /// Return the display name for the plugin
    std::string GetDisplayName();
    IPlugin *actual;

private:
    // Make this object be noncopyable as it holds a pointer
    PluginInstance(const PluginInstance &);
    const PluginInstance &operator =(const PluginInstance &);

    class Impl;
    Impl *mImpl;

};

///
/// A manger for all plugins in the Core API
///
class CORE_API PluginManager
{
public:
    /// Return the single instance of the plugin manager
    static PluginManager &getInstance();


    /// Load a single plugin by name
    bool Load(const std::string &name);

    /// Execute a single plugin by name
    bool Execute(const std::string &name);

    /// Execute all plugins in mPlugins
    bool ExecuteAll();

    /// Unload all plugins
    bool UnloadAll();
    /// Unload a single plugin by name
    bool Unload(const std::string &name);

    /// Return a list of all plugins
    bool LoadFromFile(const std::string &filename);


    typedef std::list<PluginInstance *>   PluginList;
    typedef PluginList::iterator   TListItor;
    PluginList                     m_pluginlist;
private:
    PluginManager();
    ~PluginManager();
    PluginManager(const PluginManager &);
    const PluginManager &operator =(const PluginManager &);






    // singleton
    static PluginManager          *mPlugins;

    //std::vector<PluginInstance *> mPlugins;
};


#endif
