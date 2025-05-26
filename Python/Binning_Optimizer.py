#!/usr/bin/env python3

def Last_Bin_Checker(region, binning):
    dir_name = region
    if region == "Signal":
        dir_name = "ThreeB_CR"

    entries = 0
    for mc in mc_keys:
        
        histo = fin.Get(f"{dir_name}/Nominal/{mc.GetName()}/Template_MVA_Score")

        bin_low = histo.FindBin(binning[0]+0.001)
        bin_high = histo.FindBin(binning[1]-0.001)
        #print(bin_low, bin_high)
        entries += histo.Integral(bin_low, bin_high)

    cut_value = entry_cut[region]
    if entries < cut_value:
        #print("Last bin is not enough", entries)
        binning.remove(binning[1])
    
    return
    
def Print(region, binning):
    if region == "TwoB":
        print("bin_template_mva_score[0] = {", end="")
    elif region == "ThreeB_CR":
        print("bin_template_mva_score[1] = {", end="")
    elif region == "Signal":
        print("bin_template_mva_score[2] = {", end="")    

    for i in range(len(binning)):
        if i != len(binning)-1:
            print(f"{binning[i]}, ", end="")
        else:
            print(f"{binning[i]}}};")

if __name__ == "__main__":

    import argparse
    parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
    parser.add_argument('-e', dest='era', default="2016b")
    parser.add_argument('-ch', dest='channel', default="El")
    parser.add_argument('-qcd_mc', action='store_true', default=False)
    args = parser.parse_args()

    if args.era=="2016a": args.era="2016preVFP"
    if args.era=="2016b": args.era="2016postVFP"

    import os
    import ROOT

    sr_cut_value = 0.95
    regions = ["TwoB", "ThreeB_CR", "Signal"]
    entry_cut = {"TwoB":5000, "ThreeB_CR":5000, "Signal":50,}

    path=os.environ['Vcb_Post_Analysis_WD']
    fin = ROOT.TFile.Open(f"{path}/Workplace/Binning_Optimizer/Vcb_Histos_{args.era}_{args.channel}.root")

    for region in regions:
        dir_name = region
    
        if region == "TwoB":
            binning = {1}
        elif region == "ThreeB_CR":
            binning = {sr_cut_value}
        elif region == "Signal":
            binning = {1}    
            dir_name = "ThreeB_CR"
    
        histo = fin.Get(f"{dir_name}/Nominal/TTLJ_45/Template_MVA_Score")
        nbin = histo.GetNbinsX()

        mc_keys = fin.Get(f"{dir_name}/Nominal").GetListOfKeys()
        if args.qcd_mc:
            mc_keys = [mc for mc in mc_keys if mc.GetName() != "QCD_Data_Driven"]
        elif not args.qcd_mc:
            mc_keys = [mc for mc in mc_keys if mc.GetName() != "QCD_bEn"]

        entries = 0
        for i in range(nbin+1, 0, -1):
            #print(i)
        
            bin = round((histo.GetXaxis().GetBinLowEdge(i)) / 0.01)*0.01
            bin = round(bin, 2)
        
            if region == "Signal" and bin < sr_cut_value:
                continue
            elif region == "ThreeB_CR" and bin > sr_cut_value:
                continue
        
            #print(i, bin)
        
            for mc in mc_keys:
                histo = fin.Get(f"{dir_name}/Nominal/{mc.GetName()}/Template_MVA_Score")
                entries += histo.GetBinContent(i)
    
            if entries > entry_cut[region]:
                #print(entries, bin)
                binning.add(bin)
                entries = 0        
            
        if region == "TwoB" or region == "ThreeB_CR":
            binning.add(0)
        elif region == "Signal":
            binning.add(sr_cut_value)
    
        binning = sorted(binning)        
        #print(region, binning)
        Last_Bin_Checker(region, binning)
        #print(region, binning)
        
        Print(region, binning)
