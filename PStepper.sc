Pstepper : Pattern {
    //Code based on Pwhite
    //Sequencer for PianoChopper
    var <>size, <>length, <counter, >reset;

    *new{
        arg size = 1, length = inf;
        ^super.newCopyArgs(size, length);
    }

    storeArgs{
        ^[size,length];
    }

    embedInStream{ arg inval;
		var counterStr = counter.asStream;
		var counterVal;
		length.value(inval).do({
			counterVal = counterStr.next(inval);
			if(counterVal.isNil) { ^inval };
			inval = counterVal.yield;
            counter = counter + 1;
		});
       ^inval;  
    }

    printOn{
        arg stream;
        stream << counter; 
    }

}

