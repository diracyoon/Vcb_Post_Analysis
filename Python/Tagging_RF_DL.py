#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2018")
parser.add_argument('-ch', dest='Channel', default='MM')
parser.add_argument('-tagger', dest='Tagger', default='C')
parser.add_argument('-analysis', action='store_true', default=False)
parser.add_argument('-combine', action='store_true', default=False)
parser.add_argument('-validation', action='store_true', default=False)
parser.add_argument('-draw_validation', action='store_true', default=False)
parser.add_argument('-application', action='store_true', default=False)
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

import ROOT
import os

path=os.environ['Vcb_Post_Analysis_WD']
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

if args.analysis == True:
    tagging_rf_dl = ROOT.Tagging_RF_DL(args.era, "Analysis", args.Channel)

if args.combine == True:
    ROOT.Tagging_RF_DL(args.era, "Combine", "dummy")

if args.validation == True:
    ROOT.Tagging_RF_DL(args.era, "Validation", args.Channel)

if args.draw_validation == True:
    ROOT.Tagging_RF_DL(args.era, "Draw_Validation", args.Channel, args.Tagger)

#if args.application == True:
#    tagging_rf = ROOT.Tagging_RF(args.era, "Application")
#    print(tagging_rf.Get_Tagging_RF("TTLJ", "B_Tag_Nominal", 5, 180.)) 
