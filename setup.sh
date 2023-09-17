#### use cvmfs for root ####
#export CMS_PATH=/cvmfs/cms.cern.ch
#source $CMS_PATH/cmsset_default.sh
#export SCRAM_ARCH=slc7_amd64_gcc900
#export cmsswrel='cmssw/CMSSW_11_3_0'
#cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/src
#echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
#echo "@@@@ cmsswrel = "$cmsswrel
#echo "@@@@ scram..."
#eval `scramv1 runtime -sh`
#cd -
#source /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/external/$SCRAM_ARCH/bin/thisroot.sh

export Vcb_Post_Analysis_WD=`pwd`

