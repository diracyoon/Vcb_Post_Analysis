#!/bin/bash

if [ $1 -gt 11 ]; then
    echo "Out of range"
    exit
fi

channel="El"
n_jet=`expr $1 % 4 + 4`
pre_cut=`expr $1 / 4`
final_kin=`expr $1 / 8` 

if [ $final_kin -eq 1 ]; then
    pre_cut=0
fi

echo $n_jet $pre_cut $final_kin

dir="/data6/Users/isyoon/Vcb_Post_Analysis/Macro/Permutation"

${dir}/Permutation.py -e 2018 -ch ${channel} -njet ${n_jet} -pre_cut $pre_cut -final_kin $final_kin

#ls dataset/weights/TMVAClassification*

if [ $pre_cut -eq 0 ] && [ $final_kin -eq 0 ]
then 
    mkdir -p ${dir}/${channel}/PrePermutation_${n_jet}Jets/weights/
    
    mv dataset/weights/TMVAClassification_* ${dir}/${channel}/PrePermutation_${n_jet}Jets/weights/
    mv Vcb_PrePermutation_TTLJ_WtoCB_${n_jet}Jets.root ${dir}/${channel}/PrePermutation_${n_jet}Jets
    #mv job_$1.??? ${dir}/PrePermutation_${n_jet}Jets

elif [ $pre_cut -eq 1 ]; then
    mkdir -p ${dir}/${channel}/PreCut_${n_jet}Jets/weights/

    mv dataset/weights/TMVAClassification_* ${dir}/${channel}/PreCut_${n_jet}Jets/weights/
    mv Vcb_PreCut_TTLJ_WtoCB_${n_jet}Jets.root ${dir}/${channel}/PreCut_${n_jet}Jets
    #mv job_$1.??? ${dir}/PreCut_${n_jet}Jets

elif [ $final_kin -eq 1 ]; then
    mkdir -p ${dir}/${channel}/Permutation_${n_jet}Jets/weights/
 
    mv dataset/weights/TMVAClassification_* ${dir}/${channel}/Permutation_${n_jet}Jets/weights/
    mv Vcb_Permutation_TTLJ_WtoCB_${n_jet}Jets.root ${dir}/${channel}/Permutation_${n_jet}Jets
    #mv job_$1.??? ${dir}/Permutation_${n_jet}Jets
fi
