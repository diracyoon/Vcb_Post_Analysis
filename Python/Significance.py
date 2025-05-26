#!/usr/bin/env python3

import ROOT
import os
import argparse

parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-qcd_mc', action='store_true', default=False)
args = parser.parse_args()

year_list = ["2016preVFP", "2016postVFP", "2017", "2018"]
channel_list = ["Mu", "El"]

path=os.environ['Vcb_Post_Analysis_WD']

fin = {}
for year in year_list:
    fin[year] = {}
    for channel in channel_list:
        fin[year][channel] = ROOT.TFile.Open(f"{path}/Workplace/Binning_Optimizer/Vcb_Histos_{year}_{channel}.root")

nbin = fin["2017"]["Mu"].Get("ThreeB_CR/Nominal/TTLJ_45/Template_MVA_Score").GetNbinsX()
#print(nbin)

mc_keys = fin["2017"]["Mu"].Get("ThreeB_CR/Nominal").GetListOfKeys()
mc_keys = [mc for mc in mc_keys if mc.GetName() != "TTLJ_45"]
if args.qcd_mc:
    mc_keys = [mc for mc in mc_keys if mc.GetName() != "QCD_Data_Driven"]
elif not args.qcd_mc:
    mc_keys = [mc for mc in mc_keys if mc.GetName() != "QCD_bEn"]

entries_signal_accumulated = 0
entries_background_accumulated = 0
for i in range(nbin, 0, -1):
    entries_signal = 0
    entries_background = 0
    
    for year in year_list:
        for channel in channel_list:
            for mc in mc_keys:
                histo = fin[year][channel].Get(f"ThreeB_CR/Nominal/{mc.GetName()}/Template_MVA_Score")
                entries_background += histo.GetBinContent(i)
                entries_background_accumulated += histo.GetBinContent(i)
            
            entries_signal += fin[year][channel].Get(f"ThreeB_CR/Nominal/TTLJ_45/Template_MVA_Score").GetBinContent(i)
            entries_signal_accumulated += fin[year][channel].Get(f"ThreeB_CR/Nominal/TTLJ_45/Template_MVA_Score").GetBinContent(i)
            #print(i, entries_signal, entries_background) 
    
    significance = entries_signal / (entries_background ** 0.5)
    significance_accumulated = entries_signal_accumulated / (entries_background_accumulated ** 0.5)
    
    bin = histo.GetXaxis().GetBinLowEdge(i)
    bin = round(bin / 0.01)*0.01
    bin = round(bin, 2)            
    
    print(i, significance, significance_accumulated, bin)