


#define Nspecies 2
#define MaxDecChain 4

#define BEAM_RATE {2,10} // particle per sec, {target RI, BG}
#define BEAM_PROFILE {12,12} // mm, sigma

#define BETA_POS_DEV 3 // mm, sigma
double halflife[Nspecies][MaxDecChain]= // Fill -1 for stable isotope.
{
	{1.17, 12.2, 59.7, 2.8*3600}, // 93Pd
	{6, 3.5*60, 4.23*60, -1} // 92Rh
}; // sec

/// stopper size, mm ///
#define STOPPERX 100
#define STOPPERY 100

/// DAQ run, sec /// 
#define ts_lowlimit 0
#define ts_upplimit 12600
// Beam up time should be modified in MC_generator.C


/// Analysis /// 
#define CORREL_LENGTH 3*2.355/2 // mm
#define TIME_SCALE 1 // sec
