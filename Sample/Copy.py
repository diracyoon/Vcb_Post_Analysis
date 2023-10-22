#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser(description='')
parser.add_argument('-a', dest="Analyser", default='')
parser.add_argument('-e', dest='Era', default='2017')
parser.add_argument('-ch', dest='Channel', default='Mu')
parser.add_argument('-data', action='store_true', default="")
parser.add_argument('-mc', action='store_true', default="")
parser.add_argument('-flag', dest='flag', default='All')
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
]

period = []
if args.Era == "2016preVFP": period = ['B_ver2', 'C', 'D', 'E', 'F']
elif args.Era == "2016postVFP": period = ['F', 'G', 'H']
elif args.Era == "2017": period = ['B', 'C', 'D', 'E', 'F']
elif args.Era == "2018": period = ['A', 'B', 'C', 'D']
 
single_muon_list = ['SingleMuon_' + p for p in period]
single_electron_list = list()
if args.Era == "2018":
    single_electron_list = ['EGamma_' + p for p in period]
else:
    single_electron_list = ['SingleElectron_' + p for p in period]

data_list = []
if args.Channel == "Mu" or args.Channel == "MM": data_list = single_muon_list
if args.Channel == "El" or args.Channel == "EE": data_list = single_electron_list
if args.Channel == "ME": data_list = single_muon_list + single_electron_list
 
out_base = ""
if args.Analyser == "Vcb": out_base = f"/data6/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb/{args.Era}"
elif args.Analyser ==  "Vcb_DL": out_base = f"/data6/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/Vcb_DL/{args.Era}" 

flag_base = f"/data6/Users/isyoon/Vcb_Post_Analysis/Sample/{args.Era}/{args.Channel}"

import shutil
import os.path

if args.mc == True:
    if args.flag == "Syst" or args.flag == 'All':
        out_path = ""
        if args.Analyser == "Vcb": out_path = f"{out_base}/Run{args.Channel}__RunResult__RunSyst__"
        elif args.Analyser == "Vcb_DL": out_path = f"{out_base}/Run{args.Channel}__RunSyst__" 

        for mc in mc_list:
            des = ""
            if args.Analyser == "Vcb": des = f"{out_path}/Vcb_{mc}.root"
            elif args.Analyser == "Vcb_DL": des =  f"{out_path}/Vcb_DL_{mc}.root"
        
            goal = f"{flag_base}/RunResult/Central_Syst/"
            
            #print(des, goal, "\n")
    
            if os.path.isfile(des):
                os.makedirs(os.path.dirname(goal), exist_ok=True)
                shutil.copy(des, goal)
            else: 
                print("Can't find the file") 
                print(des)
                    
    #Top syst      
    if args.flag == 'Syst_Top' or args.flag == 'All':
        out_path = ""
        if args.Analyser == "Vcb": out_path = f"{out_base}/Run{args.Channel}__RunResult__"
        elif args.Analyser == "Vcb_DL": out_path = f"{out_base}/Run{args.Channel}__"

        f_list = ['CP5Down', 'CP5Up', 'hdampDown', 'hdampUp', 'mtop171p5', 'mtop173p5']
        mc_list = ['TTLJ', 'TTLL']                

        for r_file in f_list:
            for mc in mc_list:
                des = ""
                if args.Analyser == "Vcb": des = f"{out_path}/Vcb_{mc}_powheg_{r_file}.root"
                elif args.Analyser == "Vcb_DL": des = f"{out_path}/Vcb_DL_{mc}_powheg_{r_file}.root"

                goal = f"{flag_base}/RunResult/Top_Syst/"
                #print(des, goal, "\n")
            
                if os.path.isfile(des):
                    os.makedirs(os.path.dirname(goal), exist_ok=True)
                    shutil.copy(des, goal)
                else: 
                    print("Can't find the file")
                    print(des)

if args.data == True:
    out_path = ""
    if args.Analyser == "Vcb": out_path = f"{out_base}/Run{args.Channel}__RunResult__/DATA"
    elif args.Analyser == "Vcb_DL": out_path = f"{out_base}/Run{args.Channel}__/DATA"

    for data in data_list:
        des = ""
        if args.Analyser == "Vcb": des = f"{out_path}/Vcb_{data}.root"
        elif args.Analyser == "Vcb_DL": des = f"{out_path}/Vcb_DL_{data}.root"
        
        goal = f"{flag_base}/RunResult/DATA/"
        print(des, goal, "\n")

        if os.path.isfile(des):
            os.makedirs(os.path.dirname(goal), exist_ok=True)
            shutil.copy(des, goal)
        else: 
            print("Can't find the file")
            print(des)
        
