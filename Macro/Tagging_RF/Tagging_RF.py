#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2018")
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

import ROOT
import os
path=os.environ['Vcb_Post_Analysis_WD']
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

tagging_rf = ROOT.Tagging_RF(args.era, "Analysis")
#tagging_rf = ROOT.Tagging_RF(args.era)
#print(tagging_rf.Get_Tagging_RF("Ratio_B_Tag_Nominal", 4))
