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

            delaybuf_l = (float*)RTAlloc(mWorld, buf_size*sizeof(float));//Cyclic buffer
            delaybuf_r = (float*)RTAlloc(mWorld, buf_size*sizeof(float));//Cyclic buffer

            if(delaybuf_l == NULL || delaybuf_r == NULL){
                ClearUnitOutputs(this, 1);
                Print("Failed to allocate memory for Moire ugen.\n");
                return ;
            }

            memset(delaybuf_l, 0, buf_size*sizeof(float));
            memset(delaybuf_r, 0, buf_size*sizeof(float));

            set_calc_function<Moire,&Moire::next>();
        }

        ~Moire(){
            if(delaybuf_l != nullptr)
                RTFree(mWorld, delaybuf_l);
            if(delaybuf_r != nullptr)
                RTFree(mWorld, delaybuf_r);
        }

    private:
        float *delaybuf_l = nullptr, *delaybuf_r = nullptr;
        int buf_size = 0, phase = 0, max_delay = 0;

        void next(int inNumSamples){
            float *outLeft = out(0);
            float *outRight = out(1);

            float freq = in0(0); 
            
            float harmonics = in0(2);
            float detune = in0(3);
            float mul = in0(4);

            float noise_l, noise_r; 
            float composite_l = 0, composite_r = 0;

            //Print("%i\n",buf_size);
            for (int i = 0; i<inNumSamples; i++)
            {
                noise_l = (-1+((float)rand()/RAND_MAX)*2); //inLeft
                noise_r = (-1+((float)rand()/RAND_MAX)*2); //inRight


                composite_l = 0, composite_r = 0;
                for(int tap = 1; tap<16; tap++){                    
                    composite_l += harmonics/((float)tap)*delaybuf_l[mod((phase-(tap)*(int)((float)sampleRate()/(freq))), buf_size)];
                    composite_r += harmonics/((float)tap)*delaybuf_r[mod((phase-(tap)*(int)((float)sampleRate()/(tap*freq*detune))), buf_size)];
                }

                float left_out = mul*(noise_l + composite_l);
                float right_out = mul*(noise_r + composite_r);

                outLeft[i] = abs(left_out) > 1 ? sgn(left_out) : left_out;
                outRight[i] = abs(right_out) > 1 ? sgn(right_out) : right_out;

                delaybuf_l[phase] = outLeft[i]*0.8;
                delaybuf_r[phase] = outRight[i]*0.8;

                phase = (phase+1) % buf_size;
            }

            //Print("delaybuf: %f\n", delaybuf_l[i]);

        }

        /**
         * int a is modulated, if negative carry over
         * int b is modulo, total size
         */
        int sgn(int a){
            if (a>=0)
                return 1;
            else
                return -1;
        }

        int mod(int a, int b){
            int modulo = a%b;
            return modulo >= 0 ? modulo : b + modulo;
        }

};

PluginLoad(MoireUGens) {
    ft = inTable; 
    registerUnit<Moire>(ft, "Moire");
}

