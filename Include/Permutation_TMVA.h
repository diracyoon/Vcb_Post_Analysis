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
  Permutation_TMVA(const TString &a_era = "2018", const int &a_n_jet = 4, const bool &a_chk_permutation_pre = false);
  ~Permutation_TMVA();

protected:
  TString era;
  TString channel;

  bool chk_bvsc_only = false;

  int reduction;

  int n_jet;
  bool chk_prekin_cut;      // no chi2
  bool chk_permutation_pre; // no tagging info. of W candidates

  TFile *fin_el;
  TTree *tree_correct_el;
  TTree *tree_wrong_el;

  TFile *fin_mu;
  TTree *tree_correct_mu;
  TTree *tree_wrong_mu;

  TFile *fout;

  TMVA::Factory *factory;
  TMVA::DataLoader *data_loader;

  TCut cut_s;
  TCut cut_b;

  int n_train_signal = 0;
  int n_train_back = 0;

  ClassDef(Permutation_TMVA, 1);
};

#endif /* __Permutation_TMVA_h__  */
