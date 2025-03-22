#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: $0 your_script.py [args...]"
  exit 1
fi

export CMS_PATH=/cvmfs/cms.cern.ch
export SCRAM_ARCH=el9_amd64_gcc12
export cmsswrel='cmssw/CMSSW_15_0_1'

source $CMS_PATH/cmsset_default.sh
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/src
echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
echo "@@@@ cmsswrel = "$cmsswrel
echo "@@@@ scram..."
eval `scramv1 runtime -sh`
cd -
source /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/external/$SCRAM_ARCH/bin/thisroot.sh

export Vcb_Post_Analysis_WD="/data6/Users/isyoon/Vcb_Post_Analysis"
export Vcb_Post_Analysis_Sample_Dir="/gv0/Users/isyoon/Vcb_Post_Analysis/Sample/"

python3 "$@"
