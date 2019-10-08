Pstepper : Pattern {
    //Code based on Pseries
    //Sequencer for PianoChopper
    var <>length;

    *new{
        arg length = inf;
        ^super.newCopyArgs(length);
    }

    storeArgs{
        ^[length];
    }

    embedInStream{ 
        arg inval;
        var counter = 0;
        while{counter<length}
        {
            inval = counter.yield;
            counter = counter + 1;
        }
       ^inval;  
    }

    printOn{
        arg stream;
        stream << "Step Sequencer of length " << length; 
    }

}

