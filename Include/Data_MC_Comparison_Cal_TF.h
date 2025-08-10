#ifndef __Data_MC_Comparison_Cal_TF_H__
#define __Data_MC_Comparison_Cal_TF_H__

#include <iostream>
#include <vector>

#include <TString.h>
#include <TFile.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TList.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TMath.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TH2D.h>

#include <Const_Def.h>
#include <Samples.h>

using namespace std;

class Data_MC_Comparison_Cal_TF : public TObject
{
public:
  Data_MC_Comparison_Cal_TF(const TString &a_era, const TString &a_channel, const TString& a_tagger = "C", const TString &a_extension = "png");
  ~Data_MC_Comparison_Cal_TF();

protected:
  TString era;
  TString channel;
  TString tagger;
  TString extension;

  Samples samples;

  map<TString, int> color;

  int n_region;
  vector<TString> region_name;

  int n_abcd_region;
  vector<TString> abcd_region_name;

  int n_sample;
  vector<TString> sample_name;
  map<int, TString> sample_name_order;

  TH1 ****histo_mc;   // n_region, n_abcd_region, n_sample
  THStack ***stack_mc_eta; // n_region, n_abcd_region
  THStack ***stack_mc_pt;  // n_region, n_abcd_region

  TH1 ***histo_data; // n_region, n_abcd_region

  TCanvas **canvas; // n_region

  TFile *fin;

  void Draw();
  vector<int> Get_Histo_Group(const TString &sample_name_merged);
  void Ordering_Sample_Name();
  void Setup_Histo_Data();
  void Setup_Histo_MC();
  void Setup_Name(const TList *list, vector<TString> &vec_name);
  void Stack_MC();

  ClassDef(Data_MC_Comparison_Cal_TF, 1);
};

#endif /* __Data_MC_Comparison_Cal_TF_H__ */