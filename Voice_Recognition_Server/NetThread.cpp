#include "NetThread.h"
#include <iostream>
#include <sstream>


//SharedMemory *sharedMemory;

NetThread::NetThread()
{
    buffersize = 1024;
    g_in = 0;
    g_out = 0;
    incoming = 0;
    reply = 0;
#ifdef WIN32
    gsNetPollInit();
#endif

}

NetThread::~NetThread()
{
#ifdef WIN32
    gsNetPollShutdown();
#endif
}

void NetThread::SetOutputStream(FILE *stream)
{
    output = stream;
}

int NetThread::OpenReadPort(int port, int buffersize )
{
    const int OK = 1;
    const int FAIL = 0;
    fprintf(output, "Listening connections from port %d\n", port);
    g_in = gsNetPollOpen("localhost", port, buffersize, "r", NULL);
    assert(g_in != NULL);
    if (!g_in)
    {
        return 0;
    }
    else
    {
        incoming = (char *)calloc(buffersize, sizeof(char));
        reply = (char *)calloc(buffersize, sizeof(char));
        return 1;
    }
}

int NetThread::OpenOutputAddress(char *address, int port, int buffersize)
{
    fprintf(output, "Directing output to %s:%d\n", address, port);
    g_out = gsNetPollOpen(address, port, buffersize, "w", NULL);
    g_in=g_out;
    if (!g_out)
    {
        return 0;
    }
    else
    {
        incoming = (char *)calloc(buffersize, sizeof(char));
        reply = (char *)calloc(buffersize, sizeof(char));
        return 1;
    }
}

void NetThread::SetMessage(const char *s)
{
    sprintf(reply, "%s", s);
}

void NetThread::Write()
{
    if (!g_out)
    {
        g_out = g_in;
    }
//     fputs("Sending reply...", output);
//     fputs(reply, output);
    gsNetPollWrite(g_out, reply);
}

void NetThread::Write(const char *s)
{
    SetMessage(s);
    Write();
}

void NetThread::Read()
{
    bzero(incoming, strlen(incoming) + 1);
    strcpy(incoming, gsNetPollRead(g_in));
    if (g_in->status != -1)
    {
        if(g_in->status > 0)
        {
            incoming[g_in->status] = '\0';
// 	  fprintf(output, "Got message: %s\n", incoming);
            std::stringstream ss;
            ss  << incoming ;
            std::string temp=ss.str();
           // std::cout <<temp << std::endl;

        }
    }

}

char* NetThread::GetIncoming()
{
    return incoming;
}

void NetThread::Close()
{
    if (g_out != g_in)
    {
        gsNetPollClose(g_out);
    }
    gsNetPollClose(g_in);
    free(incoming);
    free(reply);
}

void NetThread::Accept()
{
    gsNetPollAccept(g_in);
}

int NetThread::GetStatus()
{
    return g_in->status;

}

