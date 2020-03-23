Moire : MultiOutUGen {

    *ar { arg /* in, */freq = 110, max_delay = 2, harmonics = 0.5, detune = 1.01;
        /*
        if(in.size != 2) {
            Error(
                "Input signal must be stereo!"
            ).throw;
        };
        */
        ^this.multiNew('audio', /*in[0], in[1], */freq, max_delay, harmonics, detune);
    }

    init {
        arg ... theInputs;
        inputs = theInputs;
        ^this.initOutputs(2, rate);
    }

    //checkInputs { ^this.checkNInputs(0) }

}
