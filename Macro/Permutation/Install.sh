#!/bin/bash

path_fitter_data="/data6/Users/isyoon/SKFlatAnalyzer/external/KinematicFitter/data"

mkdir -p ${path_fitter_data}/${ERA}/${CHANNEL} && cp -vR ${ERA}/${CHANNEL}/PreCut_?Jets ${path_fitter_data}/${ERA}/${CHANNEL}
mkdir -p ${path_fitter_data}/${ERA}/${CHANNEL} && cp -vR ${ERA}/${CHANNEL}/PrePermutation_?Jets ${path_fitter_data}/${ERA}/${CHANNEL}

path_swapper_data="/data6/Users/isyoon/SKFlatAnalyzer/data/Run2UltraLegacy_v3"

mkdir -p ${path_swapper_data}/${ERA}/Swapper/${CHANNEL} &&  cp -vR ${ERA}/${CHANNEL}/Permutation_?Jets ${path_swapper_data}/${ERA}/Swapper/${CHANNEL}
