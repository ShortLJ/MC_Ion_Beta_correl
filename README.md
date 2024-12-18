# MC_Ion_Beta_correl

MC tool for ion-beta correlation

## files to modify

### Global.h
parameters of beam, stopper size, decay information, etc.

### MC_generator.C
To change beam duration, you can change the line with "beam_up.push_back(make_pair(A,B));".
Also you can check the effect of beam-down-time with this.

### RunMC_Analysis.sh
Ion-beta correlating takes quite long time. So I prefer running it with parallelism "&". 
Set "nparallel" variable to a proper number for your computer.

## How to run
bash RunMC_Analysis.sh
