#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='Era', default="2018")
parser.add_argument('-ch', dest='Channel', default="Mu")
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import os
path=os.environ['Vcb_Post_Analysis_WD']

import ROOT
#ROOT.gROOT.SetBatch(True)
#ROOT.gStyle.SetCanvasPreferGL(False);
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

reco_eval = ROOT.Reco_Eval(args.Era, args.Channel)
reco_eval.Run();
