#include "gsNetPoll_g++.h"

/*
#ifdef __cplusplus
#extern "C" {
#endif
*/

#ifdef WIN32
void bzero(char *s, int n)
{
    memset(s, 0, n);
}

void bcopy(const void *src, void *dst, int n)
{
    memcpy(dst, src, n);
}
#else
void poll_wait(int fd, int events)
{
    int n;
    struct pollfd pollfds[1];
    memset((char *) &pollfds, 0, sizeof(pollfds));

    pollfds[0].fd = fd;
    pollfds[0].events = events;

    n = poll(pollfds, 1, -1);
    if (n < 0)
    {
        perror("poll()");
        errx(1, "Poll failed");
    }
}

size_t
readmore(int sock, char *buf, size_t n)
{

    fd_set rfds;
    int ret, bytes;



    poll_wait(sock, POLLERR | POLLIN );
    bytes = readall(sock, buf, n);

    if (0 == bytes)
    {
        perror("Connection closed");
        errx(1, "Readmore Connection closure");
        /* NOT REACHED */
    }

    return bytes;
}

size_t
readall(int sock, char *buf, size_t n)
{
    size_t pos = 0;
    ssize_t res;

    while (n > pos)
    {
        res = read (sock, buf + pos, n - pos);
        switch ((int)res)
        {
        case -1:
            if (errno == EINTR || errno == EAGAIN)
                continue;
            return 0;
        case 0:
            errno = EPIPE;
            return pos;
        default:
            pos += (size_t)res;
        }
    }
    return (pos);
}


size_t
writenw(int fd, char *buf, size_t n)
{
    size_t pos = 0;
    ssize_t res;
    while (n > pos)
    {
        poll_wait(fd, POLLOUT | POLLERR);
        res = write (fd, buf + pos, n - pos);
        switch ((int)res)
        {
        case -1:
            if (errno == EINTR || errno == EAGAIN)
                continue;
            return 0;
        case 0:
            errno = EPIPE;
            return pos;
        default:
            pos += (size_t)res;
        }
    }
    return (pos);

}
#endif



GSNPS *gsNetPollOpen(char *hostname, int port, int buffersize, char *mode, char *buffer)
{
    GSNPS *result;              /* what we're building        */
    /* locals necessary to build it:    */
    int status;

    result = (GSNPS *) malloc(sizeof(GSNPS));
    // printf("Estoy en gsNetPollOpen\n");
    if (result == 0)
    {
        perror("allocating gsNetPollStruct structure in gsNetPollOpen");
        return(NULL);
    }
    result->blocking = 0;
    /* Fill in what we already know from our args */
    if (strcmp(mode, "r") == 0)
        result->mode = GSNPS_READ;
    else if (strcmp(mode, "w") == 0)
        result->mode = GSNPS_WRITE;
    else if (strcmp(mode, "b") == 0)
        result->mode = GSNPS_BIDI;
    else
    {
        perror("gsNetPollOpen: invalid mode, must be \"r\" or \"w\" or \"b\"");
        free(result);
        return(NULL);
    }

    strcpy(result->address, hostname);
    result->port = port;

    status = gsNetPollOpenSockets(result, hostname, port);
    if(status == -1)
        return NULL;

    result->buffersize = buffersize;
    /* Create socket to work from. */
    //printf("Creando el socket\n");

    result->read_buffer  = 0;
    result->write_buffer  = 0;
    result->read_buffer = (char *) malloc(result->buffersize);

    if (result->read_buffer == 0)
    {
        fprintf(stderr, "Tried to allocate %d bytes; got ptr to %d.\n",
                result->buffersize, result->read_buffer);
        perror("gsNetPollOpen: allocating buffer space");
        free(result);
        return(NULL);
    }

    result->write_buffer = (char *) malloc(result->buffersize);

    if (result->write_buffer == 0)
    {
        fprintf(stderr, "Tried to allocated %d bytes; got ptr to %d.\n",
                result->buffersize, result->write_buffer);
        perror("gsNetPollOpen: allocating buffer space");
        free(result);
        return(NULL);
    }

    if (buffer)                  /* if the user provided a dflt    */
        bcopy(buffer, result->write_buffer, result->buffersize);
    else                        /* else init to zero            */
        bzero(result->write_buffer, result->buffersize);

    bzero((char *) &result->sin, sizeof(result->sin));
    return(result);        /* finally, a happily built structure */
}


int gsNetPollOpenSockets(GSNPS *result, char *hostname, int port)
{
    struct hostent *hp;         /* host address structure            */
#ifdef WIN32
    char *yes = "1";           /* fcntl argument                    */
    u_long iMode = 1;
#else
    int yes = 1;           /* fcntl argument                    */
#endif

#ifdef DEBUG
    puts("gsNetPollOpenSockets: init socket");
#endif
    if (result->mode == GSNPS_READ)
    {
        result->listen_sock = socket(AF_INET, SOCK_STREAM,  IPPROTO_TCP);
        if (result->listen_sock < 0)
        {
            perror("gsNetPollOpen: opening stream socket");
            return(-1);
        }
#ifdef WIN32
        /*
        if (setsockopt(result->listen_sock, SOL_SOCKET, SO_REUSEADDR, yes, sizeof(int)) == -1) {
        	perror("SO_REUSEADDR");
        	exit(1);
        }

        if (setsockopt(result->listen_sock, SOL_SOCKET, SO_KEEPALIVE, yes, sizeof(int)) == -1) {
        	perror("SO_KEEPALIVE");
        	exit(1);
        }
        */
#else
        if (setsockopt(result->listen_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (setsockopt(result->listen_sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }
#endif
    }
    else
    {
        result->connect_sock = socket(AF_INET, SOCK_STREAM,  IPPROTO_TCP);
        if (result->connect_sock < 0)
        {
            perror("gsNetPollOpen: opening stream socket");
            return(-1);
        }
    }

#ifdef DEBUG
    puts("gsNetPollOpenSockets: setting addr");
#endif
    result->sin.sin_family = AF_INET;        /* address family            */
    if (result->mode == GSNPS_READ)          /* address hostaddr            */
        result->sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* who cares who sends it?*/
    else
    {
        hp = gethostbyname(hostname);        /* convert hostname to IP    */
        if (!hp)
        {
#ifdef WIN32
            fprintf(stderr, "%s\n", hostname);
#else
            herror(hostname);                /* write message if we can't*/
#endif
            return(-1);
        }
        bcopy(hp->h_addr, &(result->sin.sin_addr.s_addr), hp->h_length);
    }
    result->sin.sin_port = htons(port);        /* address port                */


    /* bind the socket to the address */
    if (result->mode == GSNPS_READ)          /* only bind on read        */
    {
#ifdef DEBUG
        puts("gsNetPollOpenSockets: binding");
#endif
        //printf("Estoy uniendo el socket con bind de .c\n");
        if (bind(result->listen_sock, (const sockaddr *)&result->sin, sizeof(result->sin)) < 0)
        {
#ifdef WIN32
            fprintf(stderr, "Error code: %d\n", WSAGetLastError());
#endif
            perror("binding name to datagram socket");
            return(-1);
        }
        /*Escuchando valores por medio del listen */
#ifdef DEBUG
        puts("gsNetPollOpenSockets: listening");
#endif
        if (listen(result->listen_sock, 100) == -1)
        {
            perror("Error en el listen, llego a su maximo valor \n");
            exit(1);
        }
        /* also only need bfr on read */
#ifdef WIN32
        ioctlsocket(result->listen_sock,FIONBIO,&iMode);
#endif
        puts("gsNetPollOpenSockets: connection complete");
    }

    if (result->mode == GSNPS_WRITE)           /* only connect on write        */
    {
        if ( connect(result->connect_sock, (struct sockaddr *)&result->sin, sizeof(result->sin)) == -1 )
        {
            //perror("Error al llamar el metodo connect");
#ifdef WIN32
            closesocket(result->connect_sock);
#else
            close(result->connect_sock);
#endif
            return(-1);

            //exit(1);
        }
#ifdef WIN32
        ioctlsocket(result->connect_sock,FIONBIO,&iMode);
#endif
    }
    return 0;
}



char *gsNetPollRead(GSNPS *g)
{
    int size, lastsize;
    int nError  = 0;

    size = g->buffersize;


    lastsize = size;  /* record for posterity */
    bzero(g->read_buffer, g->buffersize);
#ifdef WIN32
    size = recv(g->connect_sock, g->read_buffer, g->buffersize - 1 , 0);
    g->status  = size;
    nError = WSAGetLastError();
#ifdef DEBUG
    fprintf(stderr, "Error code: %d\n", nError);
#endif
    if(size == 0)
    {
        puts("Connection has been severed...");
        perror("gsNetPollRead: receiving stream message; Trying reconnection");
        perror("NonBlocking Read");
        gsNetPollReconnect(g);
        bzero(g->read_buffer, g->buffersize);
        g->status  = 0;
    }
    else
    {
        if(nError!=WSAEWOULDBLOCK&&nError!=0)
        {
            puts("Connection has been severed...");
            perror("gsNetPollRead: receiving stream message; Trying reconnection");
            perror("NonBlocking Read");
            gsNetPollReconnect(g);
            bzero(g->read_buffer, g->buffersize);
            g->status  = 0;
        }
        else if(nError == WSAEWOULDBLOCK)
        {
#ifdef DEBUG
            puts("Waiting for message...");
#endif
            g->status  = 0;
        }
#ifdef DEBUG
        else if (nError == 0)
        {
            puts("AOK");
        }
#endif
    }
#else
    size = recv(g->connect_sock, g->read_buffer, g->buffersize - 1 , MSG_DONTWAIT);
    g->status  = size;
    if (size == -1)
    {
        switch (errno)
        {
        case EAGAIN:
#ifdef DEBUG
            puts("Waiting for message...");
#endif
            g->status  = 0;
            break;

        case EPIPE:
            perror("gsNetPollRead: receiving stream message; Trying reconnection");
            perror("NonBlocking Read");
            gsNetPollReconnect(g);
            bzero(g->read_buffer, g->buffersize);
            g->status  = 0;
            break;
        }

    }
    else if(size == 0)
    {
        puts("Connection has been severed...");
        perror("gsNetPollRead: receiving stream message; Trying reconnection");
        perror("NonBlocking Read");
        gsNetPollReconnect(g);
        bzero(g->read_buffer, g->buffersize);
        g->status  = 0;
    }
    else
    {
#ifdef DEBUG
        g->read_buffer[size] = '\0';
        printf("Read %s(%d)\n", g->read_buffer, size);
#endif
    }
#endif
    return(g->read_buffer);
}

void gsNetPollWrite(GSNPS *g, char *buffer)
{
    int size;
    int nError = 0;

    bzero(g->write_buffer, g->buffersize);
    strcpy(g->write_buffer, buffer);
#ifdef DEBUG
    printf("Buffer: %s(%d)\n", buffer, strlen(buffer));
    printf("Sending %s(%d)\n", g->write_buffer, strlen(g->write_buffer));
#endif

    g->write_buffer[strlen(g->write_buffer)] = '\0';
#ifdef WIN32
    size = send(g->connect_sock, g->write_buffer, strlen(g->write_buffer), 0);
#ifdef DEBUG
    printf("Sent %d bytes\n", size);
#endif
    g->status  = size;
    nError = WSAGetLastError();
#ifdef DEBUG
    fprintf(stderr, "Error code: %d\n", nError);
#endif
    if(size == 0)
    {
        puts("Connection has been severed...");
        perror("gsNetPollRead: receiving stream message; Trying reconnection");
        perror("NonBlocking Read");
        gsNetPollReconnect(g);
        bzero(g->read_buffer, g->buffersize);
        g->status  = 0;
    }
    else
    {
        if(nError!=WSAEWOULDBLOCK&&nError!=0)
        {
            puts("Connection has been severed...");
            perror("gsNetPollRead: receiving stream message; Trying reconnection");
            perror("NonBlocking Read");
            gsNetPollReconnect(g);
            bzero(g->read_buffer, g->buffersize);
            g->status  = 0;
        }
        else if(nError == WSAEWOULDBLOCK)
        {
#ifdef DEBUG
            puts("Waiting for message...");
#endif
            g->status  = 0;
        }
#ifdef DEBUG
        else if (nError == 0)
        {
            puts("AOK");
        }
#endif
    }
#else
    size = send(g->connect_sock, g->write_buffer, strlen(g->write_buffer), MSG_DONTWAIT);
#ifdef DEBUG
    printf("Sent %d bytes\n", size);
#endif
    g->status = size;
    if (size == -1)
    {
        switch (errno)
        {
        case EAGAIN:
#ifdef DEBUG
            puts("Waiting for message...");
#endif
            g->status  = 0;
            break;

        case EPIPE:
            perror("gsNetPollRead: receiving stream message; Trying reconnection");
            perror("NonBlocking Read");
            gsNetPollReconnect(g);
            g->status  = 0;
            break;
        }
    }
#endif
}

void gsNetPollCloseSockets(GSNPS *g)
{
#ifdef WIN32
    closesocket(g->listen_sock);
    closesocket(g->connect_sock);

#else
    close(g->listen_sock);
    close(g->connect_sock);
#endif
}

void gsNetPollClose(GSNPS *g)
{
    if (g->mode == GSNPS_READ)
    {
#ifdef WIN32
        closesocket(g->listen_sock);
#else
        close(g->listen_sock);
#endif

    }
    else
    {
#ifdef WIN32
        closesocket(g->connect_sock);
#else
        close(g->connect_sock);
#endif
    }
    free(g->read_buffer);
    free(g->write_buffer);
    free(g);
}

int gsNetPollAccept(GSNPS *g)
{
    int status = 0;
    if (g->mode == GSNPS_READ)
    {
        //set listen_sock as non blocking
        g->connect_sock = accept(g->listen_sock, NULL, NULL);
#ifndef DEBUG
        printf("### %d, %d\n", g->listen_sock, g->connect_sock );
#endif
        g->status = status;
        return status;
    }
    else
    {
        perror("gsNetPollAccept: Write-mode sockets cannot accept incoming connections");
        g->status = -1;
        return -1;
    }
}

int gsNetPollReconnect(GSNPS *g)
{
    struct hostent *hp;         /* host address structure            */
#ifdef WIN32
    char *yes = "1";
#else
    int yes = 1;
#endif
    int status = 0;

#ifdef WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
    printf("Reconnecting to %s %d\n", g->address, g->port);

    if(status != -1)
    {
        if(g->mode == GSNPS_READ)
        {
            status = gsNetPollAccept(g);
            if(status != INVALID_SOCKET)
            {
                puts("gsNetPollReconnect: connection complete");
            }
        }
    }
    else
    {
        perror("gsNetPollReconnect");
    }

    return status;
}


int gsNetPollInit()
{
#ifdef WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)   /* Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        return -1;
    }
    return 1;
#else
    return 1;
#endif
}

void gsNetPollShutdown()
{
#ifdef WIN32
    WSACleanup();
#endif
}


/*
#ifdef __cplusplus
}
#endif
*/

