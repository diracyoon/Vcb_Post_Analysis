#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2017")
parser.add_argument('-ch', dest='channel', default="El")
parser.add_argument('-cal_tf', dest='cal_tf', type=bool, default=False)
parser.add_argument('-data_driven', dest='data_driven', type=bool, default=False)
parser.add_argument('-merge', dest='merge', type=bool, default=False)
parser.add_argument('-index_tree_type', dest='index_tree_type', type=int, default='-1')
parser.add_argument('-last_index_tree_type', dest='last_index_tree_type', type=int, default='-1')
parser.add_argument('-tagger', dest='tagger', default='C')
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

import ROOT
import os
path=os.environ['Vcb_Post_Analysis_WD']
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

if args.cal_tf:
    mode = "Cal_TF"
elif args.data_driven:
    mode = "Data_Driven"
elif args.merge:
    mode = "Merge"
else:
    mode = "2D"

histo_syst = ROOT.Histo_Syst(args.era, args.channel, mode, args.index_tree_type, args.last_index_tree_type, args.tagger)
