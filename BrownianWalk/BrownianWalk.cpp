#include "SC_PlugIn.hpp"
#include <stdlib.h> 

static InterfaceTable *ft;

struct BrownianWalk : public SCUnit {

    public:
        BrownianWalk(){
            if (isAudioRateIn(0)) {
                set_calc_function<BrownianWalk,&BrownianWalk::next_a>();
            } else {    
                set_calc_function<BrownianWalk,&BrownianWalk::next_k>();
            }   

            rate = in0(1);

            if (isAudioRateIn(0)) {
                next_a(1);
            } else {
                next_k(1);
            }
        }

    private:
        float rate;

        void next_a(int inNumSamples){
            float *outBuf = out(0);
            float incr = in0(0);
            float l_rate = rate;

            for (int i=0; i < inNumSamples; ++i)
            {
                outBuf[i] = l_rate;
                l_rate += incr*(-1+((float)rand()/RAND_MAX)*2);
            }

            rate = l_rate;
        }

        void next_k(int inNumSamples){
            float *outBuf = out(0);
            float incr = in0(0);
            float l_rate = rate;

            for (int i=0; i < inNumSamples; ++i)
            {
                outBuf[i] = l_rate;
                l_rate += incr*(-1+((float)rand()/RAND_MAX)*2);
            }

            rate = l_rate;
        }

};

PluginLoad(BrownianWalkUGens) {
    ft = inTable; 
    registerUnit<BrownianWalk>(ft, "BrownianWalk");
}

