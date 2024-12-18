
nparallel=30

set -e

root -b -q MC_generator.C+
root -b -q Sort.C+
bash para_Analysis.sh ${nparallel}
root -b -q Analysis_dec.C+
root fit_h1_dTdecTimp.c
