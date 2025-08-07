#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-a', dest='Analyzer', default="Modelling_Patch")
parser.add_argument('-e', dest='Era', default="2017")
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

#import shutil
import subprocess
import os

mc_list = ["TTLJ_WtoCB_powheg",
           "TTLJ_powheg",
           "TTLL_powheg",
           #"TTJJ_powheg",
           "TTbb_4f_TTLJ",
           "TTbb_4f_TTLL",
           "TTLJ_bbDPS",
           "TTLL_bbDPS",
           "SingleTop_sch_Lep",
           "SingleTop_tch_antitop_Incl",
           "SingleTop_tch_top_Incl",
           "SingleTop_tW_antitop_NoFullyHad",
           "SingleTop_tW_top_NoFullyHad",
           "DYJets_MG",
           #"DYJets",
           #"WJets_MG",
           "WJets_HT100to200",
           "WJets_HT200to400",
           "WJets_HT400to600",
           "WJets_HT600to800",
           "WJets_HT800to1200",
           "WJets_HT1200to2500",
           "WJets_HT2500toInf",
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
    #src = f"/gv0/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_{args.Analyzer}/{args.Era}/Vcb_{args.Analyzer}_{mc}.root"
    src = f"/gv0/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_{args.Analyzer}/{args.Era}/Vcb_{args.Analyzer}_{mc}.root"
    dst = f"/gv0/Users/isyoon/Vcb_Post_Analysis/Sample/{args.Era}/{args.Analyzer}/Vcb_{args.Analyzer}_{mc}.root"
    print(src, "to", dst)
   
    dst_dir = os.path.dirname(dst)
    os.makedirs(dst_dir, exist_ok=True)

    if os.path.exists(dst):
        os.remove(dst)
            
    #shutil.copy2(src, dst)
    #subprocess.run(["rsync", "-avW", "--info=progress2", src, dst], check=True)
    #subprocess.run(["rsync", "-avW", "--dry-run", "--info=progress2", src, dst], check=True)
    #subprocess.run(["cp", "-a", src, dst], check=True)
    #subprocess.run(["xrdcp", "-fP", src, dst], check=True)
