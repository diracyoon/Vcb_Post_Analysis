#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='')
parser.add_argument('-e', dest='Era', default='2017')
args = parser.parse_args()

import os
path=os.environ['Vcb_Post_Analysis_WD']

channel_list = ["Mu", "El"]
region_list = ["Control0", "Signal"]

dict_ch_region_count = dict()

import ROOT
for channel in channel_list:
    #print(channel)

    fin_path=f"{path}/Workplace/Histo_Syst/{args.Era}/Vcb_Histos_{args.Era}_{channel}_All.root"

    fin = ROOT.TFile(fin_path)
    
    dict_region_count = dict()
    for region in region_list:
        #print(region)

        dict_count = dict()
        dir_nominal = fin.Get(f"{region}/Nominal")
        
        count_ttlj_sum = 0
        count_ttlj_45_sum = 0
        count_ttll_sum = 0
        count_st_sum = 0
        count_vjets_sum = 0
        count_ttv_sum = 0
        count_sum = 0

        key_list = dir_nominal.GetListOfKeys() 
        for key in key_list:
            sample = key.GetName()
            
            count = dir_nominal.Get(f"{sample}/N_BJets").Integral()

            dict_count[sample] = count

            if "TTLJ_2" in sample or "TTLJ_CC_2" in sample or "TTLJ_BB_2" in sample or "TTLJ_4" in sample or "TTLJ_CC_4" in sample or "TTLJ_BB_4" in sample:  
                count_ttlj_sum += count

            if "TTLJ_45" in sample or "TTLJ_CC_45" in sample or "TTLJ_BB_45" in sample:
                count_ttlj_45_sum += count

            if "TTLL" in sample:
                count_ttll_sum += count

            if "ST_" in sample:
                count_st_sum += count

            if "WJets" in sample or "DYJets" in sample:
                count_vjets_sum += count

            if "ttZ" in sample or "ttW" in sample or "ttH" in sample:
                count_ttv_sum += count

            count_sum += count
            
        dict_count["TTLJ_Sum"] = count_ttlj_sum
        dict_count["TTLJ_45_Sum"] = count_ttlj_45_sum
        dict_count["TTLL_Sum"] = count_ttll_sum              
        dict_count["ST"] = count_st_sum
        dict_count["VJets"] = count_vjets_sum 
        dict_count["ttV"] = count_ttv_sum
        dict_count["Sum"] = count_sum

        if region != "Signal":
            count = fin.Get(f"{region}/Data/N_BJets").Integral()
            dict_count["Data"] = count

        dict_region_count[region] = dict_count
    dict_ch_region_count[channel] = dict_region_count

#print(dict_ch_region_count["Mu"]["Control0"])        
#exit()

sample_sort =   {"TTLJ_45":"\\ttljWtoCB",
                 "TTLJ_CC_45":"\\ttljWtoCB\_C/CC",
                 "TTLJ_BB_45":"\\ttljWtoCB\_B/BB",
                 "hline0":"",
                 "TTLJ_45_Sum":"\\ttljWtoCB\_Sum",
                 "hline1":"",
                "TTLJ_2":"\\ttljWtoUD",
                "TTLJ_CC_2":"\\ttljWtoUD\_C/CC",
                "TTLJ_BB_2":"\\ttljWtoUD\_B/BB",
                "TTLJ_4":"\\ttljWtoCS",
                "TTLJ_CC_4":"\\ttljWtoCS\_C/CC",
                "TTLJ_BB_4":"\\ttljWtoCS\_B/BB",
                "hline3":"",
                "TTLJ_Sum":"\\ttlj\_Sum",
                "hline4":"",
                "TTLL":"\\ttll\_JJ",
                "TTLL_CC":"\\ttll\_C/CC",
                "TTLL_BB":"\\ttll\_B/BB",
                "hline5":"",
                "TTLL_Sum":"\\ttll\_Sum",
                "hline6":"",
                "ST":"\\st",
                "VJets":"\\vjets",
                "QCD_bEn":"\qcdbEn",
                "ttV":"\\ttv",
                "VV":"\\vv",
}

## print table ##
print('\\begin{table}[htbp]')
print('\\centering')
print('\\begin{tabular}{crrrr}')
print('\hline\hline')

print('& \multicolumn{2}{c}{Mu} & \multicolumn{2}{c}{El} \\\\')
print('& 2B CR & 3B SR & 2B CR & 3B SR\\\\')

print('\hline')
for sample in sample_sort:
    if "hline" in sample:
        print("\hline")
    else:
        print(f"{sample_sort[sample]} &", end="")
        for channel in channel_list:
            for region in region_list:
                print(f"{dict_ch_region_count[channel][region][sample]:0.2f}", end="")
                if channel == "El" and region == "Signal":
                    print("\\\\")
                else:
                    print("&", end="")
print('\hline')

print("MC Sum &", end="")
for channel in channel_list:
    for region in region_list:
        print(f"{dict_ch_region_count[channel][region]['Sum']:0.2f}", end="")
        if channel == "El" and region == "Signal":
            print("\\\\")
        else:
            print("&", end="")
print('\hline')

print("Data &", end="")
for channel in channel_list:
    for region in region_list:
        if region == "Signal":
            print("-", end="")
        else:
            print(f"{dict_ch_region_count[channel][region]['Data']:0.2f}", end="")
           
        if channel == "El" and region == "Signal":
            print("\\\\")
        else:
            print("&", end="")
print('\hline\hline')

print('\\end{tabular}')
print(f"\caption[Yields of SL region, {args.Era}]{{")
print(f'Yields of SL region, {args.Era}')
print(f"}}\label{{tab:EventSelection:sl_{args.Era}}}")
print('\\end{table}')
