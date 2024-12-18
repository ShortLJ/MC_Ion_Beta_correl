#!/bin/bash

set -x
set -e


#mkdir -p /dev/shm/nocomp

#if [ -e /dev/shm/nocomp/tmp_tree_imp_dec.root ]
#then 
#	echo '/dev/shm/nocomp/tmp_tree_imp_dec.root already exist'
#else 
#	hadd -f0 /dev/shm/nocomp/tmp_tree_imp_dec.root tmp_tree_imp_dec.root
#fi


npart=$1
#npart=10
ipart=-1
root -b -q Analysis_correl.C+\(${npart},${ipart}\)
for ((ipart=0 ; ipart < npart ; ipart++))
do
	root -l -b -q Analysis_correl.C+\(${npart},${ipart}\) &
	sleep 0.05
done
wait

hadd -f -j 10 hists.root tmp_hists_*.root
rm tmp_hists_*.root
echo done
