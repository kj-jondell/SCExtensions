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
        Moire(){
            //TODO: calc size of noise and data (delay..)
            // generate noise if no input given...
            // maxdelay
            max_delay = in0(1);
            buf_size = NEXTPOWEROFTWO(sampleRate()*max_delay);

            noise = (float*)RTAlloc(mWorld, buf_size*sizeof(float));
            delaybuf_l = (float*)RTAlloc(mWorld, buf_size*sizeof(float));//Cyclic buffer
            delaybuf_r = (float*)RTAlloc(mWorld, buf_size*sizeof(float));//Cyclic buffer

            if(noise == NULL || delaybuf_l == NULL || delaybuf_r == NULL){
                ClearUnitOutputs(this, 1);
                Print("Failed to allocate memory for Moire ugen.\n");
                return ;
            }

            memset(noise, 0, buf_size*sizeof(float));
            memset(delaybuf_l, 0, buf_size*sizeof(float));
            memset(delaybuf_r, 0, buf_size*sizeof(float));

            set_calc_function<Moire,&Moire::next>();
        }

        ~Moire(){
            //RTFree(mWorld, noise);
            if(delaybuf_l != nullptr)
                RTFree(mWorld, delaybuf_l);
            if(delaybuf_r != nullptr)
                RTFree(mWorld, delaybuf_r);
            if(noise != nullptr)
                RTFree(mWorld, noise);
            //ClearUnitOutputs(this, 1);
        }

    private:
        float *noise = nullptr, *delaybuf_l = nullptr, *delaybuf_r = nullptr;
        int buf_size = 0, phase = 0, max_delay = 0;

        void next(int inNumSamples){
            float *outLeft = out(0);
            float *outRight = out(1);
            //const float *inLeft = in(0);
            //const float *inRight = in(1);

            float freq = in0(0); 
            float harmonics = in0(2);
            float detune = in0(3);

            int delay_amt_l, delay_amt_r; 
            float noise_l, noise_r; 

            //Print("%i\n",buf_size);
            for (int i = 0; i<inNumSamples; i++)
            {
                delay_amt_l = (phase-(int)(sampleRate()/freq))%buf_size;
                delay_amt_r = (phase-(int)(sampleRate()/(detune*freq)))%buf_size;

                noise_l = (-1+((float)rand()/RAND_MAX)*2); //inLeft
                noise_r = (-1+((float)rand()/RAND_MAX)*2); //inRight

                outLeft[i] = zapgremlins(noise_l + harmonics*delaybuf_l[delay_amt_l>= 0 ? delay_amt_l : buf_size+delay_amt_l]);
                outRight[i] = zapgremlins(noise_r + harmonics*delaybuf_r[delay_amt_l>= 0 ? delay_amt_l : buf_size+delay_amt_l]);

                delaybuf_l[phase] = outLeft[i];
                delaybuf_r[phase] = outRight[i];

                phase = (phase+1) % buf_size;
            }

            //Print("delaybuf: %f\n", delaybuf_l[i]);

        }

};

PluginLoad(MoireUGens) {
    ft = inTable; 
    registerUnit<Moire>(ft, "Moire");
}

