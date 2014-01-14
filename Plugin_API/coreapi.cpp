/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   coreapi.cpp
/// \author Martin Reddy
/// \brief  A factory object to create Renderer instances.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "coreapi.h"
#include <iostream>

// instantiate the static variable in PluginFactory
PluginFactory::CallbackMap PluginFactory::mPlugins;

void PluginFactory::RegisterPlugin(const std::string &type,
									   CreateCallback cb)
{
	mPlugins[type] = cb;
}

void PluginFactory::UnregisterPlugin(const std::string &type)
{
	mPlugins.erase(type);
}

IPlugin *PluginFactory::CreatePlugin(const std::string &type)
{
	CallbackMap::iterator it = mPlugins.find(type);
	if (it != mPlugins.end())
	{
		// call the creation callback to construct this derived type
		return (it->second)();
	}

    return NULL;
}
