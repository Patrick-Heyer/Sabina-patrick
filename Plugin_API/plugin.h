

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

    /**
     * @brief Main funcction of the plugin
     *
     * Started in a new thread by the Run() funcction is the most importatn funcction of the plugin and generaly is divided in two parts.
     * -# Initialization. Declares local variable, registers with the GUI, makes conections to hardware if needed.
     * -# Infinite loop (generaly a for(;;) function that runs for the whole test.). At each iteration it checks the action to be preformed (Action variable of the shared memory) and reacts acordingly.
     * @return void
     **/
    virtual void Main() = 0;


    /**
     * @brief Execute plugin
     *
     * Starts a new thread and executes the plugins main() funcction
     * @return void
     **/
    virtual void run()=0;
    virtual void stop()=0;
    static void* IncWrapper(void* thisPtr) {
        ((IPlugin*) thisPtr)->Main();
        return NULL;
    }

};

#endif

