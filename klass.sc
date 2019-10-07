Namn {
	var in_freq;

    *new { arg freq;
		^super.new.init(freq)
	}

	init { arg freq;
		in_freq = freq;
	}

	get_freq{
		^in_freq; //returnera mha ^ (carot)
	}

}

Efternamn : Namn {
}

//polymorphism
