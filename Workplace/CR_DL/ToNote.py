#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser(description='')
parser.add_argument('-e', dest='Era', default='2017')
parser.add_argument('-ch', dest='Channel', default='MM')
parser.add_argument('-tagger', dest='Tagger', default='B')
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

if args.Era == "All" or args.Era == "" or args.Channel == "All" or args.Channel == "":
    args.Era = "Inclusive"

import os
path=os.environ['Vcb_Post_Analysis_WD']
path=f"{path}/Workplace/CR_DL/{args.Tagger}Tag_5f_Unitary/"

import shutil
    

## To DataMCComparison/CR_DL
var_list = ['BvsAll_3rd_4th_Jets_Unrolled',
            'DiLepton_Mass',
            'HT',
            'Leading_Jet_BvsAll',
            'Leading_Jet_CvsB',
            'Leading_Jet_CvsL',
            'Leading_Jet_Pt',
            'Leading_Lepton_Eta',
            'Leading_Lepton_Pt',
            'Met_Phi',
            'Met_Pt',
            'N_B_Jet',
            'N_Jet',
            'N_Vertex',
            'Subleading_Jet_BvsAll',
            'Subleading_Jet_CvsB',
            'Subleading_Jet_CvsL',
            'Subleading_Jet_Pt',
            'Subleading_Lepton_Eta',
            'Subleading_Lepton_Pt',]

var_to_comp = var_list
var_to_br_extract = ['BvsAll_3rd_4th_Jets_Unrolled']

for var in var_list:
    if args.Era == "Inclusive":
        origin = f"Control_DL_{args.Era}_{var}.png"
    else:
        origin = f"Control_DL_{args.Era}_{args.Channel}_{var}.png"
    print(origin)

    if var in var_to_comp:
        if args.Tagger == "B":
            if args.Era == "Inclusive":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/CR_DL/{args.Era}/"
            else:
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/CR_DL/{args.Era}/{args.Channel}"
        elif args.Tagger == "C":
            des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/DataMCComparison/CR_DL/{args.Era}/{args.Channel}"
            
        os.makedirs(des, exist_ok=True)
        shutil.copy(f"{path}/{origin}", des)

    if var in var_to_br_extract:
        if args.Tagger == "B":
            if args.Era == "Inclusive":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtractionUnblind/{args.Era}/"
            else:
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtractionUnblind/{args.Era}/{args.Channel}"
        elif args.Tagger == "C":
            des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/BRExtraction/{args.Era}/{args.Channel}"
            
        os.makedirs(des, exist_ok=True)
        shutil.copy(f"{path}/{origin}", des)
        

## Each ##
if args.Era == "2017" and args.Channel == "MM":
    each_list = os.listdir(path)
    each_list = [each for each in each_list if 'Canvas_Each' in each and 'Control_DL' in each and 'png' in each and args.Channel in each]
    each_list = [each for each in each_list if 'BvsAll_3rd_4th_Jets_Unrolled' in each]

    for each in each_list:
        print(each)

        if args.Tagger == "B":
            des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/CR_DL/{args.Era}/{args.Channel}"
        elif args.Tagger == "C":
            continue
            
        os.makedirs(des, exist_ok=True)
        shutil.copy(f"{path}/{each}", des)
  
