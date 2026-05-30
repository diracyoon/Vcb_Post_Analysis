#!/usr/bin/env python3

def Print(list_histo):
    for histo in list_histo:
        print(histo.GetBinContent(1))
    
import ROOT

fin = ROOT.TFile.Open("Vcb_DL_Histos_2017_MM_B_tagger.root")

list_histo_nominal = list()
list_histo_nominal.append(fin.Get("Control_DL/Nominal/TTLL_JJ/Total"))
list_histo_nominal.append(fin.Get("Control_DL/Nominal/TTLL_CC/Total"))
list_histo_nominal.append(fin.Get("Control_DL/Nominal/TTLL_BB/Total"))

Print(list_histo_nominal)

list_histo_bbec1_down = list()
list_histo_bbec1_down.append(fin.Get("Control_DL/Jet_En_BBEC1_Down/TTLL_JJ/Total"))
list_histo_bbec1_down.append(fin.Get("Control_DL/Jet_En_BBEC1_Down/TTLL_CC/Total"))
list_histo_bbec1_down.append(fin.Get("Control_DL/Jet_En_BBEC1_Down/TTLL_BB/Total"))

Print(list_histo_bbec1_down)