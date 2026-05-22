#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='Era', default="2017")
parser.add_argument('-ch', dest='Channel', default="Mu")
parser.add_argument('-tagger', dest='Tagger', default="B")
parser.add_argument('-reco', dest='Reco', default="XGBoost")
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

if args.Era == "" or args.Era == "All" or args.Channel == "" or args.Channel == "All":
    args.Era = "Inclusive"
    
import os
path=os.environ['Vcb_Post_Analysis_WD']
if args.Tagger == "B":
    if args.Era == "Inclusive":
        path=f"{path}/Workplace/Histo_Syst/{args.Tagger}Tag_5f_Unblind_EqualBin/"
    else:
        path=f"{path}/Workplace/Histo_Syst/{args.Tagger}Tag_5f_NewBin_7_MP_Fix/{args.Era}/"
elif args.Tagger == "C":
    path=f"{path}/Workplace/Histo_Syst/{args.Tagger}Tag_5f_New/{args.Era}/"
    
import shutil

## TF ##
region_list = ['Control',]  

tf_list = ['Transfer_Function',
]

if args.Era != "Inclusive":
    for region in region_list:
        for tf in tf_list:
            origin = f"{tf}_{region}_{args.Era}_{args.Channel}.png"

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
            'BvsAll_Leading_Jet',
            'BvsAll_Subleading_Jet',
            'BvsAll_Had_t_b',
            'BvsAll_W_u',
            'BvsAll_W_d',
            'BvsAll_Lep_t_b',
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
            'HT',
            'N_Vertex',
            'Pt_Leading_Jet',
            'Pt_Subleading_Jet',
            'Pt_Had_t_b',
            'Pt_W_u',
            'Pt_W_d',
            'Pt_Lep_t_b',
            'Pt_TT',
            'Had_T',
            'Had_W',
            'Lep_T',
            'Lep_T_Partial',
            'Lep_W',
            'Theta_W_u_W_d',
            'Theta_Had_W_Had_t_b',
            'Theta_Lepton_Neu',
            'Theta_Lep_W_Lep_t_b',
            'Del_Phi_Had_T_Lep_T',
            'Least_M_bb',
            'Least_DR_bb',
            ]

for region in region_list:
    for var in var_list:

        ## Extract ## 
        if "Template_MVA_Score" in var:
            if args.Era == "Inclusive":
                origin = f"{region}_{args.Era}_{var}.png"
            else:
                origin = f"{region}_{args.Era}_{args.Channel}_{var}.png"
            print(origin)

            origin = f"{path}/{origin}"

            if args.Tagger == "B":
                if args.Era == "Inclusive":
                    des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtractionUnblind/{args.Era}/"
                else:
                    des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtractionUnblind/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/BRExtraction/{args.Era}/{args.Channel}"

            os.makedirs(des, exist_ok=True)
            shutil.copy(origin, des)

        ## Data vs MC ## 
        if args.Era == "2017" or args.Era == "Inclusive":
            if args.Era == "Inclusive":
                origin = f"{region}_{args.Era}_{var}.png"
            else:
                origin = f"{region}_{args.Era}_{args.Channel}_{var}.png"
            print(origin)
            
            origin = f"{path}/{origin}"

            if args.Tagger == "B":
                if args.Era == "Inclusive":
                    des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/DataMCComparison/{region}/{args.Era}"
                else:
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
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/SystematicsBreakdowns/Control/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/DataMCComparison/Control/{args.Era}/{args.Channel}"

            os.makedirs(des, exist_ok=True)
            shutil.copy(f"{path}/{each}", des)
        
        if "Signal" in each:
            if args.Tagger == "B":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/SystematicsBreakdowns/Signal/{args.Era}/{args.Channel}"
            elif args.Tagger == "C":
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/CTaggingAnalysis/BRExtraction/{args.Era}/{args.Channel}"

            os.makedirs(des, exist_ok=True)
            shutil.copy(f"{path}/{each}", des)

## Smoothing ##

syst_smoothing = ["CP5_Up_N_Jets", "CP5_Down_N_Jets",
                  "hdamp_Up_Pt_Gen_TT", "hdamp_Down_Pt_Gen_TT",
                  "mtop_171p5_", "mtop_173p5_",]

sample_smoothing = ["TTLJ_45",
                    "TTLJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
                    "TTLJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
                    "TTLL", "TTLL_CC", "TTLL_BB"] 

if args.Era == "2017" and args.Channel == "Mu":
    for syst in syst_smoothing:
        for sample in sample_smoothing:
            origin = f"Smoothing_{syst}_{sample}_{args.Era}_{args.Channel}.png"
            print(origin)

            origin = f"{path}/{origin}"

            if args.Tagger == "B": 
                des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/Systematics/{args.Era}/{args.Channel}"
            
                os.makedirs(des, exist_ok=True)
                shutil.copy(origin, des)
