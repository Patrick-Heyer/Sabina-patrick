#ifndef _GSNETPOLL_H_
#define _GSNETPOLL_H_

#define INVALID_SOCKET 0xFFFFFFFF

#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <err.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#endif
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

/*
 * The gsNetPoll... suite establish a UDP (Who Cares About Packets?)
 *    link between two processes. The sender can update the "state" of
 *    a certain buffer; the recipient can "poll" the state; always
 *    learning the most recent state of the buffer that's available
 *    with nary a block.
 *
 * Interface:
 *    GSNPS *gsNetPollOpen(char *hostname, int port,
 *            int buffersize, char *mode, char *buffer)
 *     =>    You provide hostname/port number to work with, the size of the
 *        buffer, and the mode ("r" == read, "w" == write).
 *     => You may optionally provide default data for the buffer if opened
 *        in READ mode; use a NULL pointer if you want it initialized to zero.
 *     *    NOTE: In read mode, the hostname field is ignored.
 *     <=    In return you get a gsNetPollStruct to use later.
 *     *  Returns NULL and prints a handy error message if it fails.
 *     *    Possible trouble (DEBUG): if buffersize > MTU, what will read
 *        read? Probably it'll be okay (if IP fragmentation is all-powerful),
 *        and for our application, we probably don't care, but don't say
 *        I didn't mention it.
 *
 *    char *gsNetPollRead(GSNPS *g)
 *     =>    You provide a previously opened gsNetPollStruct and a buffer
 *        as big as buffersize (defined when the gsNetPollStruct was
 *        opened). (Obviously, the gsNPS must be opened for reading.)
 *     <= Returns a pointer to a buffer. Be nice and don't write to it,
 *        okay?
 *
 *    gsNetPollWrite(GSNPS *g, char *buffer)
 *     => You provide a gsNPS opened for writing and a buffer of
 *        buffersize bytes.
 *     <= Your buffer gets sent (probably) to the receiver for his next Poll.
 *
 *    gsNetPollClose(GSNPS *g)
 *     => You're being courteous to the operating system. How nice. :v)
 *
 */

/*
enum MODE {
        GSNPS_READ,
        GSNPS_WRITE
        };
*/

enum MODE {
    GSNPS_READ,
    GSNPS_WRITE,
    GSNPS_BIDI
} ;


struct gsNetPollStruct {
    MODE mode;

    /* mode we're open in, as requested        */
#ifdef WIN32
    SOCKET listen_sock;                /* socket we'll open                    */
    SOCKET connect_sock;
#else
    int listen_sock;                /* socket we'll open                    */
    int connect_sock;
#endif
    struct sockaddr_in sin;    /* address of socket                    */
    int    buffersize;            /* our data buffer size, as requested    */
    char *write_buffer;            /* the data we're updating. This gets malloced if reading, ignored if writing. */
    char *read_buffer;
    int status;
    char address[16];
    int port;
    int blocking;
    int fl_in;
    int fl_out;
};

typedef struct gsNetPollStruct GSNPS;

GSNPS *gsNetPollOpen(char *hostname, int port,
                     int buffersize, char *mode, char *buffer);
int gsNetPollOpenSockets(GSNPS *g, char *hostname, int port);
char *gsNetPollRead(GSNPS *g);
void gsNetPollWrite(GSNPS *g, char *buffer);
void gsNetPollClose(GSNPS *g);
void gsNetPollCloseSockets(GSNPS *g);
int gsNetPollAccept(GSNPS *g);
void gsNetPollClose(GSNPS *g);
int gsNetPollReconnect(GSNPS *g);
int gsNetPollInit();
void gsNetPollShutdown();

#ifdef WIN32
void bzero(char *s, int n);
void bcopy(const void *src, void *dst, int n);
#else
void poll_wait(int fd, int events);
size_t readmore(int sock, char *buf, size_t n) ;
size_t readall(int sock, char *buf, size_t n) ;
size_t writenw(int fd, char *buf, size_t n);
#endif

/*
#ifdef __cplusplus
}
#endif

#endif
*/
#endif
