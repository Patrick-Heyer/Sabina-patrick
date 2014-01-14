/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   coreapi.h
/// \author Martin Reddy
/// \brief  A factory object to create Renderer instances.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef COREAPI_H
#define COREAPI_H

#include "defines.h"
#include "plugin.h"

#include <string>
#include <map>

///
/// A factory object in the Core API
///
class CORE_API PluginFactory
{
public:
    typedef IPlugin *(*CreateCallback)();

    static void RegisterPlugin(const std::string &type,
                               CreateCallback cb);
    static void UnregisterPlugin(const std::string &type);

    static IPlugin *CreatePlugin(const std::string &type);

private:
    typedef std::map<std::string, CreateCallback> CallbackMap;
    static CallbackMap mPlugins;
};

#endif
