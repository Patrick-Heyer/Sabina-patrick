/**********************************************************************

  Audacity: A Digital Audio Editor

  NoiseRemoval.h

  Dominic Mazzoni
  Vaughan Johnson (Preview)

**********************************************************************/

#ifndef __AUDACITY_EFFECT_NOISE_REMOVAL__
#define __AUDACITY_EFFECT_NOISE_REMOVAL__

// #include <wx/bitmap.h>
// #include <wx/button.h>
// #include <wx/panel.h>
// #include <wx/sizer.h>
// #include <wx/slider.h>
// #include <wx/stattext.h>
// #include <wx/intl.h>


   void GetProfile(int len,
                   float *buffer);
   void RemoveNoise(int len,
                   float *buffer); 

   int Amplify(float *buffer,
                   int len);

   extern int       windowSize;
   extern float    *noiseGate;
   extern float    *sum;
   extern float    *sumsqz;
   extern float    *smoothing;
   extern int      *profileCount;
   extern int       level;

#endif
