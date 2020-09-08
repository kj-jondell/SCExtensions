/**
 *
 * TODO: FIX STABILITY ISSUES (freeing/creating more than 1 instance..)
 * Make a MoireFX aswell... (remainders...)
 * clean and comments..
 *
 */
#include "SC_PlugIn.hpp"
#include <stdlib.h>

static InterfaceTable *ft;

struct Moire : public SCUnit {

public:
  Moire() {
    // TODO: calc size of noise and data (delay..)
    // generate noise if no input given...
    // maxdelay
    maxDelay = in0(1);
    bufSize = NEXTPOWEROFTWO(sampleRate() * maxDelay);

    delaybufL =
        (float *)RTAlloc(mWorld, bufSize * sizeof(float)); // Cyclic buffer
    delaybufR =
        (float *)RTAlloc(mWorld, bufSize * sizeof(float)); // Cyclic buffer

    if (delaybufL == NULL || delaybufR == NULL) {
      ClearUnitOutputs(this, 1);
      Print("Failed to allocate memory for Moire ugen.\n");
      return;
    }

    memset(delaybufL, 0, bufSize * sizeof(float));
    memset(delaybufR, 0, bufSize * sizeof(float));

    set_calc_function<Moire, &Moire::next>();
  }

  ~Moire() {
    if (delaybufL != nullptr)
      RTFree(mWorld, delaybufL);
    if (delaybufR != nullptr)
      RTFree(mWorld, delaybufR);
  }

private:
  float *delaybufL = nullptr, *delaybufR = nullptr;
  int bufSize = 0, phase = 0, maxDelay = 0;

  void next(int inNumSamples) {
    float *outLeft = out(0);
    float *outRight = out(1);

    float freq = in0(0);

    float harmonics = in0(2);
    float detune = in0(3);
    float mul = in0(4);

    float noiseL, noiseR;
    float compositeL = 0, compositeR = 0;

    for (int i = 0; i < inNumSamples; i++) {
      noiseL = (-1 + ((float)rand() / RAND_MAX) * 2); // inLeft
      noiseR = (-1 + ((float)rand() / RAND_MAX) * 2); // inRight

      compositeL = 0, compositeR = 0;
      for (int tap = 1; tap < 16; tap++) {
        compositeL +=
            harmonics / ((float)tap) *
            delaybufL[mod((phase - (tap) * (int)((float)sampleRate() / (freq))),
                          bufSize)];
        compositeR +=
            harmonics / ((float)tap) *
            delaybufR[mod((phase - (tap) * (int)((float)sampleRate() /
                                                 (tap * freq * detune))),
                          bufSize)];
      }

      float leftOut = mul * (noiseL + compositeL);
      float rightOut = mul * (noiseR + compositeR);

      outLeft[i] = abs(leftOut) > 1 ? sgn(leftOut) : leftOut;
      outRight[i] = abs(rightOut) > 1 ? sgn(rightOut) : rightOut;

      delaybufL[phase] = outLeft[i] * 0.8;
      delaybufR[phase] = outRight[i] * 0.8;

      phase = (phase + 1) % bufSize;
    }
  }

  /**
   * int a is modulated, if negative carry over
   * int b is modulo, total size
   */
  int sgn(int a) {
    if (a >= 0)
      return 1;
    else
      return -1;
  }

  int mod(int a, int b) {
    int modulo = a % b;
    return modulo >= 0 ? modulo : b + modulo;
  }
};

PluginLoad(MoireUGens) {
  ft = inTable;
  registerUnit<Moire>(ft, "Moire");
}
