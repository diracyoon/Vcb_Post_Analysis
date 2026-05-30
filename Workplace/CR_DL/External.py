#!/usr/bin/env python3

#Era = ['2016preVFP', '2016postVFP', '2017', '2018']
Era = ['2018']

Channel = ['MM', 'ME', 'EE']
#Channel = ['MM']

#Tagger = 'B'
Tagger = 'C'

## Submit_2D
with open("external.dat", "w") as dat:
    for era in Era:
        for channel in Channel:
            dat.write(f"{era}, {channel}, {Tagger}\n")
