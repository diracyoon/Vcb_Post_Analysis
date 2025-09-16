#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='Era', default="2017")
parser.add_argument('-ch', dest='Channel', default="Mu")
parser.add_argument('-tagger', dest='Tagger', default="B")
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import os
path=os.environ['Vcb_Post_Analysis_WD']
path=f"{path}/Workplace/Histo_Syst/{args.Tagger}Tag/{args.Era}/"

import shutil

## TF ##
region_list = ['Control',]  

tf_list = ['Transfer_Function',
]

for region in region_list:
    for tf in tf_list:
        origin = f"{tf}_{region}_{args.Era}_{args.Channel}.png"
        print(origin)

        origin = f"{path}/{origin}"

        if args.Tagger == "B":
            des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataDriven/{args.Era}/{args.Channel}"
        elif args.Tagger == "C":
            continue
            
        os.makedirs(des, exist_ok=True)
        shutil.copy(origin, des)

region_list = ['Control', 'Signal']
var_list = ['Best_MVA_Score',
            'Template_MVA_Score',
            'BvsC_Leading_Jet',
            'BvsC_Subleading_Jet',
            'BvsC_W_u',
            'BvsC_W_d',
            'CvsB_Leading_Jet',
            'CvsB_Subleading_Jet',
            'CvsB_W_u',
            'CvsB_W_d',
            'CvsL_Leading_Jet',
            'CvsL_Subleading_Jet',
            'CvsL_W_u',
            'CvsL_W_d',
            'Eta_Leading_Jet',
            'Eta_Subleading_jet',
            'Lepton_Eta',
            'Lepton_Pt',
            'Met_Phi',
            'Met_Pt',
            'N_BJets',
            'N_CJets',
            'N_Jets',
            'N_Vertex',
            'Pt_Leading_Jet',
            'Pt_Subleading_Jet',
            'Had_T',
            'Had_W',
            'Lep_T',
            'Lep_W',
            ]

for region in region_list:
    for var in var_list:

        ## Extract ## 
        if "Template_MVA_Score" in var:
            origin = f"{region}_{args.Era}_{args.Channel}_{var}.png"
            print(origin)

            origin = f"{path}/{origin}"

            if args.Tagger == "B":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtraction/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/BRExtraction/{args.Era}/{args.Channel}"

            os.makedirs(des, exist_ok=True)
            shutil.copy(origin, des)

        ## Data vs MC ## 
        if args.Era == "2017":
            origin = f"{region}_{args.Era}_{args.Channel}_{var}.png"
            print(origin)
            
            origin = f"{path}/{origin}"

            if args.Tagger == "B":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/{region}/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/DataMCComparison/{region}/{args.Era}/{args.Channel}"
                
            #dst_dir = os.path.dirname(des)
            os.makedirs(des, exist_ok=True)
            shutil.copy(origin, des)


        ## QCD Comparison ##
        if args.Era == "2017":
            origin = f"QCD_Comp_{region}_{args.Era}_{args.Channel}_{var}.png"
            print(origin)

            origin = f"{path}/{origin}"

            if args.Tagger == "B":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataDriven/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                continue
                
            os.makedirs(des, exist_ok=True)
            shutil.copy(origin, des)

## Each ##
if args.Era == "2017":
    each_list = os.listdir(path)
    each_list = [each for each in each_list if 'Canvas_Each' in each and 'png' in each and args.Channel in each]

    for each in each_list:
        print(each)
     
        if "Control" in each:
            if args.Tagger == "B":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/Control/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/DataMCComparison/Control/{args.Era}/{args.Channel}"

            os.makedirs(des, exist_ok=True)
            shutil.copy(f"{path}/{each}", des)
        
        if "Signal" in each:
            if args.Tagger == "B":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtraction/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/BRExtraction/{args.Era}/{args.Channel}"

            os.makedirs(des, exist_ok=True)
            shutil.copy(f"{path}/{each}", des)
