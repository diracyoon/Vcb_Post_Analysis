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
    
def Print(binning):
    print("bin_bvsc = {", end="")
    for i in range(len(binning)):
        bin = round(binning[i], 4)
        if i != len(binning)-1:
            print(f"{bin}, ", end="")
        else:
            print(f"{bin}}};")

if __name__ == "__main__":

    import argparse
    parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
    parser.add_argument('-e', dest='era', default="2016b")
    parser.add_argument('-ch', dest='channel', default="EE")
    args = parser.parse_args()

    if args.era=="2016a": args.era="2016preVFP"
    if args.era=="2016b": args.era="2016postVFP"

    import os
    import ROOT
    import numpy as np

    regions = ["Control_DL"]
    entry_cut = {"Control_DL":50,}

    path=os.environ['Vcb_Post_Analysis_WD']
    fin = ROOT.TFile.Open(f"{path}/Workplace/Binning_Optimizer/Vcb_DL_Histos_{args.era}_{args.channel}.root")
    
    histo = fin.Get("Control_DL/Nominal/TTLL_JJ/BvsC_3rd_4th_Jets")
    nbin = histo.GetNbinsX()
    bin_width = 1./nbin
    
    axis = histo.GetXaxis()    
    bin_edges = np.array([round(axis.GetBinLowEdge(i + 1)/bin_width)*bin_width for i in range(nbin)] + [axis.GetBinUpEdge(nbin)])
    #print(bin_edges)
    
    contents = np.zeros((nbin, nbin))
 
    mc_keys = fin.Get("Control_DL/Nominal").GetListOfKeys()

    for i in range(1, nbin + 1):
        for j in range(1, nbin + 1):   
            if i < j:
                continue
            
            #print(i, j)
          
            entries = 0
            for mc in mc_keys:
                histo = fin.Get(f"Control_DL/Nominal/{mc.GetName()}/BvsC_3rd_4th_Jets")
                entries += histo.GetBinContent(i, j)    
            
            contents[i - 1, j - 1] = entries
            
    bin_sums = np.array([np.sum(contents[i, :i+1]) for i in range(nbin)])         
    
    used = np.zeros_like(contents, dtype=bool)
    blocks = []
    
    for i in reversed(range(nbin)):
        for j in reversed(range(nbin)):
            if i < j or used[i, j] or contents[i, j] == 0:
                continue
            
            for size in range(1, min(i, j) + 2):
                start = i - size + 1
                end = i + 1
                if start < 0:
                    break
                block = contents[start:end, start:end]
                block_mask = used[start:end, start:end]
                
                if np.any(block_mask):
                    continue
                
                if np.sum(block) >= entry_cut["Control_DL"]:
                    blocks.append((start, end))
                    used[start:end, start:end] = True
                    break
    
    #print(blocks)            
    
    indices = sorted(set([b[0] for b in blocks] + [b[1] for b in blocks]))
    new_bin = np.array([bin_edges[i] for i in indices]).tolist()
    
    # Return bin edges in increasing order
    #print(np.array(sorted(new_bin_edges)))           
    Print(new_bin)            

