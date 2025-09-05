#!/usr/bin/env python3

Era = ['2016preVFP', '2016postVFP', '2017', '2018']
#Era = ['2016preVFP']

Channel = ['MM', 'ME', 'EE']
#Channel = ['EE']

Tagger = 'B'

## Submit_2D
with open("external.dat", "w") as dat:
    for era in Era:
        for channel in Channel:
            dat.write(f"{era}, {channel}, {Tagger}\n")
