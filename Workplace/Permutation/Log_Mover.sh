#!/bin/bash

for index in {0..11}
do
    n_jet=`expr $index % 4 + 4`
    pre_cut=`expr $index / 4`
    final_kin=`expr $index / 8`
    
    if [ $final_kin -eq 1 ]; then
	pre_cut=0
    fi

    echo $n_jet $pre_cut $final_kin
    
    if [ $pre_cut -eq 0 ] && [ $final_kin -eq 0 ]
    then
	mv job_${NTREE}_${CHANNEL}_${index}.* ${ERA}/${CHANNEL}/nTree_${NTREE}/PrePermutation_${n_jet}Jets
    elif [ $pre_cut -eq 1 ]; then
	mv job_${NTREE}_${CHANNEL}_${index}.* ${ERA}/${CHANNEL}/nTree_${NTREE}/PreCut_${n_jet}Jets
    elif [ $final_kin -eq 1 ]; then
	mv job_${NTREE}_${CHANNEL}_${index}.* ${ERA}/${CHANNEL}/nTree_${NTREE}/Permutation_${n_jet}Jets
    fi
done

