#!/bin/bash

if [ $1 -gt 5 ]; then
    echo "Out of range"
    exit
fi

n_jet=`expr $1 / 2 + 4`
#chk_prekin_cut=`expr $1 % 4 / 2`
chk_permutation_pre=`expr $1 % 2` 

type_name=""
if [ $chk_permutation_pre -eq 1 ]; then
    type_name="Permutation_Pre"
else
    type_name="Permutation"
fi

echo $n_jet $chk_permutation_pre $type_name

dir_python=${Vcb_Post_Analysis_WD}"/Python"
dir_permutation=${Vcb_Post_Analysis_WD}"/Workplace/Permutation"

${dir_python}/Permutation.py -e ${ERA} -ch ${CHANNEL} -njet ${n_jet} -permutation_pre $chk_permutation_pre

mkdir -p ${dir_permutation}/${ERA}/${CHANNEL}/nTree_${NTREE}/${type_name}_${n_jet}Jets/weights/

mv dataset/weights/TMVAClassification_* ${dir_permutation}/${ERA}/${CHANNEL}/nTree_${NTREE}/${type_name}_${n_jet}Jets/weights/
mv Vcb_${type_name}_TTLJ_WtoCB_${n_jet}Jets.root ${dir_permutation}/${ERA}/${CHANNEL}/nTree_${NTREE}/${type_name}_${n_jet}Jets
