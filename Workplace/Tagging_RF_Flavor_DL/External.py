#!/usr/bin/env python3

chk_jes_breakdown = False

Era = ['2016preVFP', '2016postVFP', '2017', '2018']
#Era = ['2018']

Channel = ['MM', 'ME', 'EE']

Tagger = "B"

if chk_jes_breakdown:
    last_index_type_type = 27
else:
    last_index_type_type = 5

## Analysis
with open("external_analysis.dat", "w") as analysis_d:
    for era in Era:
        for channel in Channel:
            for index_tree_type in range(0, last_index_type_type):
                    analysis_d.write(f"{era}, {channel}, {index_tree_type}, {Tagger}\n")

## Hadd
with open("external_hadd.dat", "w") as hadd_d:
    for era in Era:
        for channel in Channel:
            hadd_d.write(f"{era}, {channel}, {Tagger}\n")

## Combine
with open("external_combine.dat", "w") as combine_d:
    for era in Era:
        combine_d.write(f"{era}, {Tagger}\n")

        
