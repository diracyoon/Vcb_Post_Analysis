#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2018")
parser.add_argument('-ch', dest='Channel', default='Mu')
parser.add_argument('-tagger', dest='Tagger', default='C')
parser.add_argument('-analysis', action='store_true', default=False)
parser.add_argument('-ratio', action='store_true', default=False)
parser.add_argument('-combine', action='store_true', default=False)
parser.add_argument('-validation', action='store_true', default=False)
parser.add_argument('-draw_validation', action='store_true', default=False)
parser.add_argument('-application', action='store_true', default=False)
parser.add_argument('-index_tree_type', dest='index_tree_type', type=int, default=-1)
parser.add_argument('-last_index_tree_type', dest='last_index_tree_type', type=int, default=-1)
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

import ROOT
import os

path=os.environ['Vcb_Post_Analysis_WD']
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

if args.analysis == True:
    tagging_rf = ROOT.Tagging_RF_Flavor(args.era, "Analysis", args.Channel, args.Tagger, args.index_tree_type, args.last_index_tree_type)
elif args.ratio == True:
    tagging_rf = ROOT.Tagging_RF_Flavor(args.era, "Ratio", args.Channel, args.Tagger, -1 , -1)
elif args.combine == True:
    tagging_rf = ROOT.Tagging_RF_Flavor(args.era, "Combine", args.Channel, args.Tagger, -1 , -1)
elif args.validation == True:
    tagging_rf = ROOT.Tagging_RF_Flavor(args.era, "Validation", args.Channel, args.Tagger, args.index_tree_type, args.last_index_tree_type)
elif args.draw_validation == True: 
    tagging_rf = ROOT.Tagging_RF_Flavor(args.era, "Draw_Validation", args.Channel, args.Tagger, -1, -1)
