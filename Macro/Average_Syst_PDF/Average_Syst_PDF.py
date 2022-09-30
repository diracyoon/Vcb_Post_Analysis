#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='Era', default="")
parser.add_argument('-ch', dest='Channel', default="")
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import ROOT
ROOT.gSystem.Load("../../Build/libVcb_Post_Analysis.so")

average_syst_pdf = ROOT.Average_Syst_PDF(args.Era, args.Channel)
