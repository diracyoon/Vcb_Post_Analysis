#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2018")
parser.add_argument('-mu', action='store_true', default=False)
parser.add_argument('-el', action='store_true', default=False)
parser.add_argument('-analysis', action='store_true', default=False)
parser.add_argument('-combine', action='store_true', default=False)
parser.add_argument('-validation', action='store_true', default=False)
parser.add_argument('-application', action='store_true', default=False)
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

import ROOT
#ROOT.gROOT.SetBatch(True)

import os

path=os.environ['Vcb_Post_Analysis_WD']
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

if args.analysis == True:
    import time
    start = time.time()

    channel = ""
    if args.mu == True:
        channel = "Mu"
    elif args.el == True:
        channel = "El"
            
    tagging_rf = ROOT.Tagging_RF(args.era, "Analysis", channel)
        
    print("time :", time.time() - start)

if args.combine == True:
    ROOT.Tagging_RF(args.era, "Analysis", "dummy")

if args.validation == True:
    channel = ""
    if args.mu == True:
        channel = "Mu"
    elif args.el == True:
        channel = "El"

    ROOT.Tagging_RF(args.era, "Validation", channel)

if args.application == True:
    tagging_rf = ROOT.Tagging_RF(args.era, "Application")
    print(tagging_rf.Get_Tagging_RF("TTLJ", "B_Tag_Nominal", 5, 180.)) 
