#ifndef __Permutation_TMVA_h__
#define __Permutation_TMVA_h__

#include <iostream>

#include <TObject.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <TMVA/Config.h>
#include <TMVA/Factory.h>
#include <TMVA/DataLoader.h>

using namespace std;
// using namespace TMVA;

class Permutation_TMVA : public TObject
{
public:
  Permutation_TMVA(const TString &a_era = "2018", const TString &a_channel = "Mu", const int &a_n_jet = 4, const bool &a_chk_prekin_cut = false, const bool &a_chk_permutation_pre = false);
  ~Permutation_TMVA();

protected:
  TString era;
  TString channel;

  int reduction;

  int n_jet;
  bool chk_prekin_cut;      // no chi2
  bool chk_permutation_pre; // no tagging info. of W candidates

  TFile *fin;
  TTree *tree_correct;
  TTree *tree_wrong;

  TFile *fout;

  TMVA::Factory *factory;
  TMVA::DataLoader *data_loader;

  TCut cut_s;
  TCut cut_b;

  int n_train_signal;
  int n_train_back;

  ClassDef(Permutation_TMVA, 1);
};

#endif /* __Permutation_TMVA_h__  */
