#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-a', dest='Analyzer', default="Vcb")
parser.add_argument('-e', dest='Era', default="2017")
parser.add_argument('-data', dest='Data', type=bool, default=False)
parser.add_argument('-mc', dest='MC', type=bool, default=False)
parser.add_argument('-flag', dest='Flag', default=All)
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import shutil

## data
if args.Data == True:
   print('lazy') 
    
if args.MC == True:
    if args.Flag =="Syst_Top"  or args.Flag == "All":
        mc_list = ["TTLJ_powheg_CP5Down",
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
            src = f"/gv0/Users/isyoon/SKFlatOutput/Run2UltraLegacy_v3/{args.Analyzer}/{args.Era}/{args.Analyzer}_{mc}.root"
            dst = f"/gv0/Users/isyoon/Vcb_Post_Analysis/Sample/{args.Era}/{args.Analyzer}/Top_Syst/{args.Analyzer}_{mc}.root"
            print(src, dst)