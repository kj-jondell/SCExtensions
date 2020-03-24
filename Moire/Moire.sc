Moire : MultiOutUGen {

    *ar { arg freq = 110, max_delay = 2, harmonics = 0.5, detune = 1.01, mul = 0.7;
        ^this.multiNew('audio', freq, max_delay, harmonics, detune, mul);
    }

    init {
        arg ... theInputs;
        inputs = theInputs;
        ^this.initOutputs(2, rate);
    }

}
