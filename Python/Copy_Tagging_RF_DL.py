#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-a', dest='Analyzer', default="Tagging_RF")
parser.add_argument('-e', dest='Era', default="2017")
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

args.Analyzer = "Tagging_RF_DL"

#import shutil
import subprocess
import os

mc_list = ["TTLJ_WtoCB_powheg",
           "TTLJ_WtoCB_powheg_CP5Down",
           "TTLJ_WtoCB_powheg_CP5Up",
           "TTLJ_WtoCB_powheg_hdampDown",
           "TTLJ_WtoCB_powheg_hdampUp",
           "TTLJ_WtoCB_powheg_mtop171p5",
           "TTLJ_WtoCB_powheg_mtop173p5",
           "TTLJ_powheg",
           "TTLL_powheg",
           #"TTBB",
           "SingleTop_sch_Lep",
           "SingleTop_tch_antitop_Incl",
           "SingleTop_tch_top_Incl",
           "SingleTop_tW_antitop_NoFullyHad",
           "SingleTop_tW_top_NoFullyHad",
           "DYJets_MG",
           "WJets_MG",
           #"DYJets",
           #"WJets_Sherpa",
           "QCD_bEnriched_HT100to200",
           "QCD_bEnriched_HT200to300",
           "QCD_bEnriched_HT300to500",
           "QCD_bEnriched_HT500to700",
           "QCD_bEnriched_HT700to1000",
           "QCD_bEnriched_HT1000to1500",
           "QCD_bEnriched_HT1500to2000",
           "QCD_bEnriched_HT2000toInf",
           "ttWToLNu",
           "ttWToQQ",
           "ttZToLLNuNu",
           "ttZToQQ",
           "ttZToQQ_ll",
           "ttHToNonbb",
           "ttHTobb",
           "WW_pythia",
           "WZ_pythia",
           "ZZ_pythia",
           "TTLJ_powheg_CP5Down",
           "TTLJ_powheg_CP5Up",
           "TTLJ_powheg_hdampDown",
           "TTLJ_powheg_hdampUp",
           "TTLJ_powheg_mtop171p5",
           "TTLJ_powheg_mtop173p5",
           "TTLL_powheg_CP5Down",
           "TTLL_powheg_CP5Up",
           "TTLL_powheg_hdampDown",
           "TTLL_powheg_hdampUp",
           "TTLL_powheg_mtop171p5",
           "TTLL_powheg_mtop173p5",
           "TTLJ_WtoCB_powheg_CP5Down",
           "TTLJ_WtoCB_powheg_CP5Up",
           "TTLJ_WtoCB_powheg_hdampDown",
           "TTLJ_WtoCB_powheg_hdampUp",
           "TTLJ_WtoCB_powheg_mtop171p5",
           "TTLJ_WtoCB_powheg_mtop173p5",
           ]

for mc in mc_list:
    src = f"/gv0/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_{args.Analyzer}/{args.Era}/Vcb_{args.Analyzer}_{mc}.root"
    dst = f"/gv0/Users/isyoon/Vcb_Post_Analysis/Sample/{args.Era}/{args.Analyzer}/Vcb_{args.Analyzer}_{mc}.root"
    print(src, "to", dst)
   
    dst_dir = os.path.dirname(dst)
    os.makedirs(dst_dir, exist_ok=True)

    if os.path.exists(dst):
       os.remove(dst)
            
    #shutil.copy2(src, dst)
    subprocess.run(["rsync", "-a", "-t", "--info=progress2", src, dst], check=True)
    #subprocess.run(["cp", "-a", src, dst], check=True)
