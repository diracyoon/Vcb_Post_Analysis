#ifndef __Template_H__
#define __Template_H__

#include <iostream>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <TMVA/Config.h>
#include <TMVA/Factory.h>
#include <TMVA/DataLoader.h>

using namespace std;

class Template : public TObject
{
public:
  Template(const TString &a_era = "2018", const TString &a_channel = "Mu");
  ~Template();

protected:
  TString era;
  TString channel;

  int reduction;
  int decay_modes[5] = {45, 21, 23, 41, 43};

  TTree *tree[5];

  TFile *fin;
  TFile *fout;

  TMVA::Factory *factory;
  TMVA::DataLoader *data_loader;

  ClassDef(Template, 1);
};

#endif /* __Template_H__ */
