#!/usr/bin/env python3 

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')                                                                                                                                       
parser.add_argument('-e', dest='era', default="2017")                                                                                                                                                            
parser.add_argument('-ch', dest='channel', default="Mu")                                                                                             
parser.add_argument('-s', dest='sample', default="TTLJ_45")
args = parser.parse_args() 

import ROOT
ROOT.gROOT.SetBatch(True)

fin_dd = ROOT.TFile(f"DD/Vcb_Histos_{args.era}_{args.channel}.root")
fin_no_dd = ROOT.TFile(f"No_DD/Vcb_Histos_{args.era}_{args.channel}.root")

histo_dd = fin_dd.Get(f"Signal/Nominal/{args.sample}/Template_MVA_Score")      
histo_no_dd = fin_no_dd.Get(f"Signal/Nominal/{args.sample}/Template_MVA_Score")

canvas = ROOT.TCanvas("canvas", "canvas", 1400, 1000)
canvas.Divide(3, 1)

canvas.cd(1)
histo_dd.Draw()
print(histo_dd.Integral())

canvas.cd(2)
histo_no_dd.Draw()

canvas.cd(3)
histo_ratio = histo_dd.Clone("histo_ratio")
histo_ratio.Divide(histo_no_dd)
histo_ratio.Draw()

canvas.Update()
canvas.Print(f"Compare_{args.era}_{args.channel}.pdf", "pdf")