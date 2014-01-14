

#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>
#include <iostream>
#include <pthread.h>

///
/// An abstract interface for plugins to create new renderers.
///
class IPlugin
{
public:
	pthread_t thread_id;
    virtual void Main() = 0;
	virtual void run()=0;
	static void* IncWrapper(void* thisPtr) {
		((IPlugin*) thisPtr)->Main();
		return NULL;
	}

};

#endif

