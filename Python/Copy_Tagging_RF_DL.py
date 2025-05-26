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
           "TTLJ_powheg",
           "TTLL_powheg",
           "TTJJ_powheg",
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
            "QCD_Pt_15to30",
           "QCD_Pt_30to50",
           "QCD_Pt_50to80",
           "QCD_Pt_80to120",
           "QCD_Pt_120to170",
           "QCD_Pt_170to300",
           "QCD_Pt_300to470",
           "QCD_Pt_470to600",
           "QCD_Pt_600to800",
           "QCD_Pt_800to1000",
           "QCD_Pt_1000to1400",
           "QCD_Pt_1400to1800",
           "QCD_Pt_1800to2400",
           "QCD_Pt_2400to3200",
           "QCD_Pt_3200toInf",
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
           "TTLJ_WtoCB_powheg_CP5Down",
           "TTLJ_WtoCB_powheg_CP5Up",
           "TTLJ_WtoCB_powheg_hdampDown",
           "TTLJ_WtoCB_powheg_hdampUp",
           "TTLJ_WtoCB_powheg_mtop171p5",
           "TTLJ_WtoCB_powheg_mtop173p5",
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
           #TTJJ_powheg_CP5Down",
           #TTJJ_powheg_CP5Up",
           #TTJJ_powheg_hdampDown",
           #TTJJ_powheg_hdampUp",
           #TTJJ_powheg_mtop171p5",
           #TTJJ_powheg_mtop173p5",
           ]

for mc in mc_list:
    if "TTJJ" not in mc:
        continue

    #src = f"/gv0/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_{args.Analyzer}/{args.Era}/Vcb_{args.Analyzer}_{mc}.root"
    src = f"/gv0/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_{args.Analyzer}/{args.Era}/RunNoJESBreakdown__/Vcb_{args.Analyzer}_{mc}.root"
    dst = f"/gv0/Users/isyoon/Vcb_Post_Analysis/Sample/{args.Era}/{args.Analyzer}/Vcb_{args.Analyzer}_{mc}.root"
    print(src, "to", dst)
   
    dst_dir = os.path.dirname(dst)
    os.makedirs(dst_dir, exist_ok=True)

    if os.path.exists(dst):
       os.remove(dst)
            
    #shutil.copy2(src, dst)
    subprocess.run(["rsync", "-a", "-t", "--info=progress2", src, dst], check=True)
    #subprocess.run(["cp", "-a", src, dst], check=True)
