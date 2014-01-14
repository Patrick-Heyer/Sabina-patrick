#include <math.h>
#include <stdio.h>

#if defined(__WXMSW__) && !defined(__CYGWIN__)
#include <float.h>
#define finite(x) _finite(x)
#endif

#ifdef __MACOSX__
#include <math.h>
#endif

#ifdef __MACOS9__
#include <fp.h>
#define finite(x) isfinite(x)
#endif

#include <stdlib.h>
#include "NoiseRemoval.h"
#include "FFT.h"

int windowSize = 16384;			    // ajustado al mismo valor del buffer de sphinx. Su valor original era de 2048
float *noiseGate;
float *sum;
float *sumsqz;
int *profileCount;
float *smoothing;
int level = 4;

// Esta funcion se llama desde Sphinx; GetProfile(K, bufferSphinx), donde k es la cantidad de datos leidos por sphinx
void GetProfile (int len, float *buffer) {

    noiseGate = (float*)calloc(windowSize, sizeof(float));  //Reservando espacio para las variables
    sum = (float*)calloc(windowSize, sizeof(float));
    sumsqz = (float*)calloc(windowSize, sizeof(float));
    profileCount = (int*)calloc(windowSize, sizeof(int));
    smoothing = (float*)calloc(windowSize, sizeof(float));

    int i;

    for(i=0; i<windowSize; i++) {                   // Limpiando contenido del profile anterior
        sum[i] = (float)0.0;
        sumsqz[i] = (float)0.0;
        profileCount[i] = 0;
    }

    float *in;
    float *out;
    in = (float*)calloc(len, sizeof(float));
    out = (float*)calloc(len, sizeof(float));

    for(i=0; i<len; i++)
        in[i] = buffer[i];

    // Apply FFT
    PowerSpectrum(len, in, out);

    for(i=0; i<=len/2; i++) {
        float value = log(out[i]);

        if (finite(value)) {
            sum[i] += value;
            sumsqz[i] += value*value;
            profileCount[i]++;
        }
    }

    for(i=0; i<=windowSize/2; i++) {
        noiseGate[i] = sum[i] / profileCount[i]; // average
    }

    free ((float*)in);
    free ((float*)out);
}

// Esta funcion se llama desde Sphinx; RemoveNoise(K, bufferSphinx), donde K es la cantidad de datos leidos por sphinx.

void RemoveNoise(int len, float *buffer) {

    int lastpos = 0;
    float *inr;
    float *ini;
    float *outr;
    float *outi;
    float *power;
    float *plog;
    inr = (float*)calloc(windowSize, sizeof(float));
    ini = (float*)calloc(windowSize, sizeof(float));
    outr = (float*)calloc(windowSize, sizeof(float));
    outi = (float*)calloc(windowSize, sizeof(float));
    power = (float*)calloc(windowSize, sizeof(float));
    plog = (float*)calloc(windowSize, sizeof(float));

    int i;

    for (i=0; i<windowSize; i++)
        smoothing[i] = (float)0.0;

    lastpos = 0;

    for (;;) {

        for(i=0; i<windowSize; i++)
            inr[i] = buffer[i + lastpos];

        // Apply FFT
        FFT(windowSize, 0, inr, NULL, outr, outi);	// No existe el tipo bool en C, reemplazado por 0 y 1 para falso y verdadero respectivamente

        for(i=0; i<windowSize; i++)
            inr[i] = buffer[i + lastpos];

//       WindowFunc(2, windowSize, inr);         // Haning window
        PowerSpectrum(windowSize, inr, power);

        for(i=0; i<=windowSize/2; i++)
            plog[i] = log(power[i]);

        int half = windowSize/2;

        for(i=0; i<=half; i++) {
            float smooth;
            if (plog[i] < noiseGate[i] + (level/2.0))
                smooth = (float)0.0;
            else
                smooth = (float)1.0;

            smoothing[i] = smooth * 0.5 + smoothing[i] * 0.5;
        }

        /* try to eliminate tinkle bells */
        for(i=2; i<=half-2; i++) {
            if (smoothing[i]>=0.5 &&
                    smoothing[i]<=0.55 &&
                    smoothing[i-1]<0.1 &&
                    smoothing[i-2]<0.1 &&
                    smoothing[i+1]<0.1 &&
                    smoothing[i+2]<0.1)
                smoothing[i] = (float)0.0;
        }

        outr[0] *= smoothing[0];
        outi[0] *= smoothing[0];
        outr[half] *= smoothing[half];
        outi[half] *= smoothing[half];

        for(i=1; i<half; i++) {
            int j = windowSize - i;
            float smooth = smoothing[i];

            outr[i] *= smooth;
            outi[i] *= smooth;
            outr[j] *= smooth;
            outi[j] *= smooth;
        }

        // Inverse FFT and normalization
        FFT(windowSize, 1, outr, outi, inr, ini);             // No existe el tipo bool en C, reemplazado por 0 y 1 para falso y verdadero rspectivamente
//       WindowFunc(2, windowSize, inr); /* Hanning window */

        for(i=0; i<windowSize; i++)
            buffer[i + lastpos] = inr[i];
        lastpos += windowSize;

        if (lastpos > len)
            break;
    }

    free ((float*)inr);
    free ((float*)ini);
    free ((float*)outr);
    free ((float*)outi);
    free ((float*)power);
    free ((float*)plog);
}

int Amplify(float *buffer, int len)
{
    int i;
    int showbuff = 1;
    int ampfact = 9999999;
    int max_amplify = 15;	     // Amplificacion maxima
    short samplevel = 0;

    /* cal. new ampl. factor */
    for (i=0; i < len; i++) {
        if (buffer[i] != (float)0.0) {

            samplevel = (int)fabs((float)32767.0 / buffer[i]);	// 32767 es el tamaño maximo de short(int16)

            if (ampfact > samplevel)
                ampfact = samplevel;

            if (showbuff < ceil(buffer[i]))
                showbuff = ceil(buffer[i]);
        }
    }

    if (ampfact > max_amplify)
        ampfact = max_amplify;

    fprintf(stdout, "Nivel del buffer antes de amplificar = %d\n", showbuff);
    fflush(stdout);
    fprintf(stdout, "Factor de amplificacion: %d\n", ampfact);
    fflush (stdout);
    showbuff = 0;
    if (ampfact<3) ampfact=3;
    for (i = 0; i < len; i++)
        buffer[i] = (buffer[i] * ampfact);

    return 1;
}
