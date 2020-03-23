BrownianWalk : UGen {

    *ar { arg incr = 0.003, init_rate = 1.0;
        ^this.multiNew('audio', incr, init_rate);
    }

    *kr { arg incr = 0.003, init_rate = 1.0;
        ^this.multiNew('control', incr, init_rate);
    }

}
