#ifndef _NET_H_
#define _NET_H_

#include "gsNetPoll_g++.h"
#include <cstdlib>
#include <cstring>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;
static char **args;

class Net {
private:
    GSNPS *g_in;
    GSNPS *g_out;
    int buffersize;
    char *incoming;
    char *reply;
    FILE *output;
public:
    Thread_Net() {
        buffersize = 1024;
        g_in = 0;
        g_out = 0;
        incoming = 0;
        reply = 0;

#ifdef WIN32
        gsNetPollInit();
#endif

    }

    ~Thread_Net() {
#ifdef WIN32
        gsNetPollShutdown();
#endif
    }

    void SetOutputStream(FILE *stream = stdout)
    {
        output = stream;
    }

    int OpenReadPort(int port, int buffersize = 1024) {
        const int OK = 1;
        const int FAIL = 0;
        fprintf(output, "Listening connections from port %d\n", port);
        g_in = gsNetPollOpen("localhost", port, buffersize, "r", NULL);
        assert(g_in != NULL);
        if (!g_in) {
            return 0;
        } else {
            incoming = (char *)calloc(buffersize, sizeof(char));
            reply = (char *)calloc(buffersize, sizeof(char));
            return 1;
        }
    }

    int OpenOutputAddress(char *address, int port, int buffersize = 1024) {
        fprintf(output, "Directing output to %s:%d\n", address, port);
        g_out = gsNetPollOpen(address, port, buffersize, "w", NULL);
        if (!g_out) {
            return 0;
        } else {
            incoming = (char *)calloc(buffersize, sizeof(char));
            reply = (char *)calloc(buffersize, sizeof(char));
            return 1;
        }
    }

    void SetMessage(const char *s) {
        sprintf(reply, "%s", s);
    }

    void Write() {
// 		fputs("Sending reply...", output);
// 		fputs(reply, output);
        gsNetPollWrite(g_out, reply);
    }

    void Write(const char *s) {
        SetMessage(s)
        Write();
    }

    void Read()
    {
        bzero(incoming, strlen(incoming) + 1);
        strcpy(incoming, gsNetPollRead(g_in));
        if (g_in->status != -1)
        {
            incoming[g_in->status] = '\0';
            fprintf(output, "Got message: %s\n", incoming);
        }

    }

    char* GetIncoming()
    {
        return incoming;
    }

    void Close()
    {
        if (g_out != g_in)
        {
            gsNetPollClose(g_out);
        }
        gsNetPollClose(g_in);
        free(incoming);
        free(reply);
    }
};

#endif
