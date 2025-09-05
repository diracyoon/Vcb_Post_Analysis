#export CMS_PATH=/cvmfs/cms.cern.ch
#source $CMS_PATH/cmsset_default.sh
#export SCRAM_ARCH=slc7_amd64_gcc11
#export cmsswrel='cmssw/CMSSW_12_3_0'
#cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/src
#echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
#echo "@@@@ cmsswrel = "$cmsswrel
#echo "@@@@ scram..."
#eval `scramv1 runtime -sh`
#cd -
#source /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/external/$SCRAM_ARCH/bin/thisroot.sh

source /cvmfs/sft.cern.ch/lcg/views/LCG_108/x86_64-el9-gcc15-opt/setup.sh
ROOTLIB="$ROOTSYS/lib"

export Vcb_Post_Analysis_WD=`pwd`
export Vcb_Post_Analysis_Sample_Dir="/gv0/Users/isyoon/Vcb_Post_Analysis/Sample/"

export ROOT_DIR=$(root-config --prefix)
export CMAKE_PREFIX_PATH=$ROOT_DIR:$CMAKE_PREFIX_PATH
