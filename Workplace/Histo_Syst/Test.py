#!/usr/bin/env python3

import ROOT
import argparse
import ctypes

parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2017")
parser.add_argument('-ch', dest='channel', default="El")
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

ROOT.gROOT.SetBatch(True)

fin = ROOT.TFile.Open(f"Vcb_Histos_{args.era}_{args.channel}_B_tagger.root", "READ")

region_list = ["Control"]

for region in region_list:
    #print(region)
 
    entry_dict = {}
    error_dict = {}
    
    dir_nominal = fin.GetDirectory(f"{region}/Nominal")
    keys = dir_nominal.GetListOfKeys()
    
    entry_total = 0;    
    for key in keys:
        dir_process = dir_nominal.GetDirectory(f"{key.GetName()}")
        histo_process = dir_process.Get(f"Total")
        
        #print(f"{key.GetName()}")
        error = ctypes.c_double(0.0)      
        entry_dict[f"{key.GetName()}"] = histo_process.IntegralAndError(1, 1, error)
        error_dict[f"{key.GetName()}"] = error
        
        if "QCD" in key.GetName(): 
            continue
    
        entry_total += entry_dict[f"{key.GetName()}"]
    
    print(entry_dict)
    print(error_dict)
    
    if args.channel == "Mu":
        # sample = "QCD_Data_Driven"
        sample = "QCD_MuEn"
    elif args.channel == "El":
        sample = "QCD_Data_Driven"
        #sample = "QCD_bEn"

    qcd_yield = entry_dict[sample]
    qcd_error = error_dict[sample].value
        
    print(entry_total, qcd_yield/(entry_total+qcd_yield), qcd_error/(entry_total+qcd_yield))
       
