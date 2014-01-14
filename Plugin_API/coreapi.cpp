
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
