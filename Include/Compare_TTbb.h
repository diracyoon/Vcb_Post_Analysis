#ifndef __Compare_TTbb_H__
#define __Compare_TTbb_H__

#include <iostream>
#include <vector>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event.h>
#include <Tagging_RF_Flavor.h>

using namespace std;

class Compare_TTbb : public TObject
{
public:
  Compare_TTbb(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_tagger = "C");
  ~Compare_TTbb();

private:
  int reduction;

  TString era;
  TString channel;
  TString tagger;

  Samples samples;

  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  map<TString, TTree *> map_tree;

  TFile *fin_5f;
  TFile *fin_4f;
  TFile *fin_dps;

  TTree *tree_5f;
  TTree *tree_4f;
  TTree *tree_dps;

  TH1D **histo;
  TH1D **histo_ratio;

  Result_Event event;
  Tagging_RF_Flavor tagging_rf;

  void Draw();
  void Fill_Histo_MC(const TString &sample_name);
  int Histo_Index(const TString &sample_name);
  TString Histo_Name_RF(const TString &sample_name);
  void Init_Histo();
  void Read_Tree();
  float Reweight_CKM(const TString &sample_name);
  float Reweight_TTHF(const TString &sample_name);
  int Set_ABCD_Region();

  ClassDef(Compare_TTbb, 1);
};

#endif /* __Compare_TTbb_H__ */
