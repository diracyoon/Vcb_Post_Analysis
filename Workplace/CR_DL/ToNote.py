#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser(description='')
parser.add_argument('-e', dest='Era', default='2017')
parser.add_argument('-ch', dest='Channel', default='MM')
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import os
path=os.environ['Vcb_Post_Analysis_WD']
path=f"{path}/Workplace/CR_DL/{args.Era}"

import shutil

## To DataMCComparison/CR_DL
var_list = ['BvsC_3rd_4th_Jets_Unrolled',
            'DiLepton_Mass',
            'HT',
            'Leading_Jet_BvsC',
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
            'Subleading_Jet_BvsC',
            'Subleading_Jet_CvsB',
            'Subleading_Jet_CvsL',
            'Subleading_Jet_Pt',
            'Subleading_Lepton_Eta',
            'Subleading_Lepton_Pt',]

var_to_comp = var_list
var_to_br_extract = ['BvsC_3rd_4th_Jets_Unrolled']

for var in var_list:
    origin = f"Control_DL_{args.Era}_{args.Channel}_{var}.png"
    print(origin)

    if var in var_to_comp and args.Era == "2017":
        des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/CR_DL/{args.Era}/{args.Channel}"
        
        os.makedirs(des, exist_ok=True)
        shutil.copy(f"{path}/{origin}", des)

    if var in var_to_br_extract:
        des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtraction/{args.Era}/{args.Channel}"
        
        os.makedirs(des, exist_ok=True)
        shutil.copy(f"{path}/{origin}", des)
        

## Each ##
if args.Era == "2017" and args.Channel == "MM":
    each_list = os.listdir(path)
    each_list = [each for each in each_list if 'Canvas_Each' in each and 'Control_DL' in each and 'png' in each and args.Channel in each]
    each_list = [each for each in each_list if 'BvsC_3rd_4th_Jets_Unrolled' in each]

    des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/CR_DL/{args.Era}/{args.Channel}"

    for each in each_list:
        print(each)
        
        des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/CR_DL/{args.Era}/{args.Channel}"
        
        os.makedirs(des, exist_ok=True)
        shutil.copy(f"{path}/{each}", des)
  
