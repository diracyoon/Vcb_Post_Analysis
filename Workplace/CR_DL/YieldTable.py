#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='')
parser.add_argument('-e', dest='Era', default='2017')
args = parser.parse_args()

import os
path=os.environ['Vcb_Post_Analysis_WD']

channel_list = ["MM", "ME", "EE"]

count_table = dict()

import ROOT
for channel in channel_list:
    dict_count = dict()
    fin_path=f"{path}/Workplace/CR_DL/{args.Era}/Vcb_DL_Histos_{args.Era}_{channel}.root"

    fin = ROOT.TFile(fin_path)
    dir_nominal = fin.Get("Control_DL/Nominal")
    
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
        count = dir_nominal.Get(f"{sample}/N_B_Jet").Integral()
        dict_count[sample] = count
    
        if "TTLJ_JJ_2" in sample or "TTLJ_CC_2" in sample or "TTLJ_BB_2" in sample or "TTLJ_JJ_4" in sample or "TTLJ_CC_4" in sample or "TTLJ_BB_4" in sample:
            count_ttlj_sum += count

        if "TTLJ_jj_45" in sample or "TTLJ_CC_45" in sample or "TTLJ_BB_45" in sample:
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

    count = fin.Get("Control_DL/Data/N_B_Jet").Integral()
    dict_count["Data"] = count

    count_table[channel] = dict_count

#print(count_table["MM"])

sample_sort = {"TTLJ_JJ_45":"\\ttljWtoCB",
               "TTLJ_CC_45":"\\ttljWtoCB\_C/CC",
               "TTLJ_BB_45":"\\ttljWtoCB\_B/BB",
               "hline0":"",
                "TTLJ_45_Sum":"\\ttljWtoCB\_Sum",
                "hline1":"", 
                "TTLL_JJ":"\\ttll\_JJ",
                "TTLL_CC":"\\ttll\_C/CC",
                "TTLL_BB":"\\ttll\_B/BB",
                "hline2":"",
                "TTLL_Sum":"\\ttll\_Sum",
                "hline3":"",
                "TTLJ_JJ_2":"\\ttljWtoUD",
                "TTLJ_CC_2":"\\ttljWtoUD\_C/CC",
                "TTLJ_BB_2":"\\ttljWtoUD\_B/BB",
                "TTLJ_JJ_4":"\\ttljWtoCS",
                "TTLJ_CC_4":"\\ttljWtoCS\_C/CC",
                "TTLJ_BB_4":"\\ttljWtoCS\_B/BB",
                "hline4":"",
                "TTLJ_Sum":"\\ttlj\_Sum",
                "hline5":"",
               "ST":"\st",
               "VJets":"\\vjets", 
               "ttV":"\\ttv", 
               "VV":"\\vv",
               "QCD_bEn":"\qcdbEn"}

## print table ##
print('\\begin{table}[htbp]')
print('\\centering')
print('\\begin{tabular}{crrr}')
print('\hline\hline')

print('& \multicolumn{1}{c}{MM} & \multicolumn{1}{c}{ME} & \multicolumn{1}{c}{EE} \\\\')

print('\hline')
for sample in sample_sort:
    if "hline" in sample:
        print("\hline")
    else:
        print(f"{sample_sort[sample]} &", end="")

        for channel in channel_list:
            print(f"{count_table[channel][sample]:0.2f}", end="")
            if channel != "EE":
                print("&", end="")
            else:
                print("\\\\")
print('\hline')

print("MC Sum &", end="")
for channel in channel_list:
    print(f"{count_table[channel]['Sum']:0.2f}", end="")
    if channel != "EE":
        print("&", end="")
    else:
        print("\\\\")
print('\hline')

print("Data &", end="")
for channel in channel_list:
    print(f"{count_table[channel]['Data']:0.2f}", end="")
    if channel != "EE":
        print("&", end="")
    else:
        print("\\\\")
print('\hline\hline')

print('\\end{tabular}')
print(f'\caption[Yields of DL region, {args.Era}]{{')
print(f'Yields of DL region, {args.Era}')
print(f"}}\label{{tab:EventSelection:dl_{args.Era}}}")
print('\\end{table}')
