#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser(description='')
parser.add_argument('-a', dest="Analyser", default='')
parser.add_argument('-e', dest='Era', default='2018')
parser.add_argument('-ch', dest='Channel', default='Mu')
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

mc_list = ['TTLJ_powheg',
           'TTLL_powheg',
           'SingleTop_sch_Lep',
           'SingleTop_tch_top_Incl',
           'SingleTop_tch_antitop_Incl',
           'SingleTop_tW_top_NoFullyHad',
           'SingleTop_tW_antitop_NoFullyHad',
           'DYJets_MG',
           'WJets_MG',
           'QCD_bEnriched_HT100to200',
           'QCD_bEnriched_HT200to300',
           'QCD_bEnriched_HT300to500',
           'QCD_bEnriched_HT500to700',
           'QCD_bEnriched_HT700to1000',
           'QCD_bEnriched_HT1000to1500',
           'QCD_bEnriched_HT1500to2000',
           'QCD_bEnriched_HT2000toInf',
           'TTLJ_WtoCB_powheg',
           'ttWToLNu',
           'ttWToQQ',
           'ttZToLLNuNu',
           'ttZToQQ',
           'ttHToNonbb',
           'ttHTobb',
           'WW_pythia',
           'WZ_pythia',
           'ZZ_pythia',
           'TTLJ_powheg_CP5Down',
           'TTLJ_powheg_CP5Up',
           'TTLJ_powheg_hdampDown',
           'TTLJ_powheg_hdampUp',
           'TTLJ_powheg_mtop171p5',
           'TTLJ_powheg_mtop173p5',
           'TTLL_powheg_CP5Down',
           'TTLL_powheg_CP5Up',
           'TTLL_powheg_hdampDown',
           'TTLL_powheg_hdampUp',
           'TTLL_powheg_mtop171p5',
           'TTLL_powheg_mtop173p5',
]

if args.Analyser == "Tagging_RF": out_base = f"/data6/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_Tagging_RF/{args.Era}"
elif args.Analyser ==  "Tagging_RF_DL": out_base = f"/data6/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_Tagging_RF_DL/{args.Era}"

flag_base = f"/data6/Users/isyoon/Vcb_Post_Analysis/Sample/{args.Era}/{args.Channel}"

import shutil
import os.path

out_base = ""
if args.Analyser == "Tagging_RF": out_base = f"/data6/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_Tagging_RF/{args.Era}"
elif args.Analyser == "Tagging_RF_DL": out_base = f"/data6/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_Tagging_RF_DL/{args.Era}"

out_path = f"{out_base}/Run{args.Channel}__"

for mc in mc_list:
    des = ""
    if args.Analyser == "Tagging_RF": des = f"{out_path}/Vcb_Tagging_RF_{mc}.root"
    elif args.Analyser == "Tagging_RF_DL": des = f"{out_path}/Vcb_Tagging_RF_DL_{mc}.root"

    goal = ""
    if args.Analyser == "Tagging_RF": goal = f"{flag_base}/Tagging_RF/"
    elif args.Analyser == "Tagging_RF_DL": goal = f"{flag_base}/Tagging_RF_DL/"                

    print(des, goal, "\n")

    if os.path.isfile(des):
       os.makedirs(os.path.dirname(goal), exist_ok=True)
       shutil.copy(des, goal)
    else: print("Can't find the file") 

    
        
