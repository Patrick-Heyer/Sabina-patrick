/*! \file
 *
 */
#ifndef _NETTHREAD_H_
#define _NETTHREAD_H_

#include "gsNetPoll_g++.h"
#include "Thread.h"
#include <cstdlib>
#include <cstring>
#include <deque>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;
static char **args;


class NetThread: public Thread {
private:
    GSNPS *g_in;
    GSNPS *g_out;
    int buffersize;
    char *incoming;
    char *reply;
    FILE *output;
public:
    deque<string> messages;

    /**! \brief Creates a new NetThread
     *
     *
     */
    NetThread();
    ~NetThread();
    void SetOutputStream(FILE *stream = stdout);

    /**! \brief Opens the port
     *
     * Opens a port
     * @param port Defines the port to open
     * @param buffersize Size of the buffer used default=1024
     */
    int OpenReadPort(int port, int buffersize = 1024);

    /**! \brief Opens the port
    *
    * Opens a port
    * @param port Defines the port to open
    * @param buffersize Size of the buffer used default=1024
    */
    int OpenOutputAddress(char *address, int port, int buffersize = 1024);
    void SetMessage(const char *s);

    /**! \brief Writes a message to the port
     * @param s content to write
     */
    void Write();
    void Write(const char *s);

    /**! \brief Reads the conection port
     */
    void Read();
    /**! \brief Reads in the message from the port
     */
    char* GetIncoming();
    /**! \brief Closes the conection
     */
    void Close();
    /**! \brief Accepts a conection
     */
    void Accept();
    /**! \brief Gets the status (if status > 0 there is a message)
     */
    int GetStatus();



};

#endif
