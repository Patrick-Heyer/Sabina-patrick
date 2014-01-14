

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

#include <sys/types.h>
#include <sys/time.h>

#include <err.h>

#ifdef __cplusplus
extern "C" {
#include <sphinx2/s2types.h>
#include <sphinx2/ad.h>
#include <sphinx2/cont_ad.h>
#include <sphinx2/fbs.h>
}
#endif
#include <iostream>

#include "../Gui/console.h"

#include <stdlib.h>
#include <math.h>


#define SAMPLE_RATE   16000


float *noiseGate;
float *sum;
float *sumsqz;
int *profileCount;
float *smoothing;

static ad_rec_t *ad;

/* sleep_msec for specified msec */

static void sleep_msec (int32 ms)
{
#ifdef WIN32
    sleep_msec(ms);
#else
    // ------------------- Unix ------------------
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms*1000;

    select(0, NULL, NULL, NULL, &tmo);
#endif
}


/*
 * Main utterance processing loop:
 *     for (;;) {
 * 	   wait for start of next utterance;
 * 	   decode utterance until silence of at least 1 sec observed;
 * 	   print utterance result;
 *     }
 */
/* HHAA 14-05-2007 */
char *sphinx2_listen(Console *cons)
{
    int16 adbuf[4096];
    int32 k, fr, ts, rem;
    int i;
    char *hyp;
    cont_ad_t *cont;
    int threshold = 20;

    struct timeval t_start, t_finish;
    double elapsed=0;

    /* Initialize continuous listening module */
    if ((cont = cont_ad_init (ad, ad_read)) == NULL)
        std::cout << "cont_ad_init failed" << std::endl;
    if (ad_start_rec (ad) < 0)
        std::cout << "ad_start_rec failed" << std::endl;
    if (cont_ad_calib (cont) < 0)
        std::cout << "cont_ad_calib failed" << std::endl;

    /* Indicate listening for next utterance */
    printf ("READY..\n");
    fflush (stdout);
    fflush (stderr);
    print(cons,"READY..\n");

    hyp = NULL;
    gettimeofday(&t_start, NULL);
    elapsed = 0;
    /* Await data for next utterance */
    /* interact y time  para controlar la duración de la escucha antes de volver a listening */
    // cont_ad_reset() para vaciar el buffer patrick
    cont_ad_reset(cont);

    while (((k = cont_ad_read (cont, adbuf, 4096)) <= 0)  && elapsed < threshold) { //version original == en vez de <= 0 patrick
        sleep_msec(200);
        gettimeofday(&t_finish, NULL);
        elapsed = t_finish.tv_sec - t_start.tv_sec + (t_finish.tv_usec - t_start.tv_usec) / 1.e6;
    }
    fprintf(stdout, "El valor de k es %d\n", k);
    fflush(stdout);

    if (elapsed >= threshold) {
        fprintf(stdout, "Elapsed time: %lf\n", elapsed);
        ad_stop_rec (ad);
        cont_ad_close (cont);
        return hyp;
    }

    if (k < 0)
        std::cout << "cont_ad_read line 162 failed" << std::endl;

    /*
     * Non-zero amount of data received; start recognition of new utterance.
     * NULL argument to uttproccont_ad_reset()_begin_utt => automatic generation of utterance-id.
     */
    if (uttproc_begin_utt (NULL) < 0)
        std::cout << "uttproc_begin_utt() failed" << std::endl;

    uttproc_rawdata (adbuf, k, 0);
    printf ("Listening...\n");
    fflush (stdout);
    print(cons,"Listening...\n");
    /* Note timestamp for this first block of data */
    ts = cont->read_ts;

    /* Decode utterance until end (marked by a "long" silence, > 1sec) */
    for (;;) {
        /* Read non-silence audio data, if any, from continuous listening module */
        if ((k = cont_ad_read (cont, adbuf, 4096)) < 0)
            std::cout << "cont_ad_read 181 failed" << std::endl;
        if (k == 0) {
            /*
             * No speech data available; check current timestamp with most recent
             * speech to see if more than 1 sec elapsed.  If so, end of utterance.
             */
            if ((cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
                break;
        } else {
            /* New speech data received; note current timestamp */
            ts = cont->read_ts;
        }

        rem = uttproc_rawdata(adbuf, k, 0);		// Decodificar bloque
        if ((rem == 0) && (k == 0))
            sleep_msec(20);

    }

    /*
     * Utterance ended; flush any accumulated, unprocessed A/D data and stop
     * listening until current utterance completely decoded
     */
    ad_stop_rec (ad);
    while (ad_read (ad, adbuf, 4096) >= 0);
    cont_ad_reset (cont);

    printf ("Stopped listening, please wait...\n");
    fflush (stdout);
    print(cons,"Stopped listening, please wait...\n");

#if 0
    /* Power histogram dump (FYI) */
    cont_ad_powhist_dump (stdout, cont);
#endif
    /* Finish decoding, obtain and print result */
    uttproc_end_utt ();
    if (uttproc_result (&fr, &hyp, 1) < 0)
        std::cout << "uttproc_result failed" << std::endl;
    printf ("%d: %s\n", fr, hyp);
    fflush (stdout);

    cont_ad_close (cont);

    /* Siempre que se haya escuchado algo, dejar de escuchar */

    return hyp;
}

static jmp_buf jbuf;
static void sighandler(int signo)
{
    longjmp(jbuf, 1);
}

int sphinx2_init (int argc, char *argv[])
{

    /* Make sure we exit cleanly (needed for profiling among other things) */
    signal(SIGINT, &sighandler);

    fbs_init (argc, argv);

    if ((ad = ad_open_sps (SAMPLE_RATE)) == NULL) {
        std::cout << "ad_open_sps failed" << std::endl;
        return 0;
    }

    //E_INFO("%s COMPILED ON: %s, AT: %s\n\n", argv[0], __DATE__, __TIME__);

    return 1;
}


void sphinx2_close (void) {

    fbs_end ();
    ad_close (ad);

}
