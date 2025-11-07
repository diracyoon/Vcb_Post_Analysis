#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='')
parser.add_argument('-e', dest='Era', default='2016a')
parser.add_argument('-ch', dest='Channel', default='Mu')
parser.add_argument('-tagger', dest='Tagger', default='B')
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import ROOT

fin = ROOT.TFile.Open('Vcb_Histos_%s_%s_%s_tagger.root'%(args.Era,args.Channel,args.Tagger))

region = "Control"
syst = "hdamp_Up"
sample = "TTLJ_4"

histo_nominal = fin.Get('%s/%s/Nominal/Template_MVA_Score'%(region,sample))
histo_raw = fin.Get('%s/%s/%s/Template_MVA_Score'%(region,syst,sample))
histo_smoothed = fin.Get('%s/%s_Smoothed/%s/Template_MVA_Score'%(region,syst,sample))

canvas = ROOT.TCanvas('c1','c1',800,600)

histo_nominal.SetLineColor(ROOT.kBlack)
histo_nominal.SetLineWidth(2)
histo_nominal.GetYaxis().SetTitle('Events / Bin')
histo_nominal.GetXaxis().SetTitle('MVA Score')
histo_nominal.Draw('')

histo_raw.SetLineColor(ROOT.kRed)
histo_raw.SetLineWidth(2)
histo_raw.Draw('SAME')

histo_smoothed.SetLineColor(ROOT.kBlue)
histo_smoothed.SetLineWidth(2)
histo_smoothed.Draw('SAME')

canvas.BuildLegend()
canvas.SaveAs('Test_Smoothing.png')