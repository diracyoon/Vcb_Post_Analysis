#ifndef __DATA_MC_COMPARISON_H__
#define __DATA_MC_COMPARISON_H__

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
#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TLatex.h>
#include <TStyle.h>

#include <Const_Def.h>
#include <Samples.h>

using namespace std;

class Data_MC_Comparison : public TObject
{
public:
  Data_MC_Comparison(const TString &a_era, const TString &a_channel, const TString &a_analyser = "Vcb", const TString &a_extension = "png");
  ~Data_MC_Comparison();

  void Run();

  typedef struct _Histo_Conf
  {
    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
  } Histo_Conf;

protected:
  TString era;
  TString channel;
  TString analyser;
  TString extension;

  Samples samples;

  float lumi;

  int n_region;
  vector<TString> region_name;

  int n_c_tag_weight;
  vector<TString> c_tag_weight_name;
  vector<map<TString, float>> c_tag_rf;

  int n_syst;
  vector<TString> syst_name;

  int n_syst_name_short;
  vector<TString> syst_name_short;
  int index_mc_nominal;

  int n_sample;
  vector<TString> sample_name;
  map<int, TString> sample_name_order;

  int n_variable;
  vector<TString> variable_name;

  vector<Histo_Conf> variable_conf;

  map<TString, int> color;

  TH1D *****histo_mc;   // n_region, n_syst, n_sample, n_variable
  THStack ****stack_mc; // n_region, n_syst, n_variable

  TH1D ***histo_data; // n_region, n_variable

  TH1D ***histo_ratio;           // n_region, n_variable
  TGraphAsymmErrors ***gr_ratio; // n_region, n_variable

  TGraphAsymmErrors ****gr_variation;       // n_region, n_syst, n_variable
  TGraphAsymmErrors ***gr_variation_merged; // n_region, n_variable
  TList ***list_variation;                  // n_region, n_variable

  TCanvas ***canvas; // n_region, n_variable
  TPad ****pad;      // n_region, n_variable, 2

  // TCanvas ***canvas_each; // n_region, n_variable
  // TPad ****pad_each;      // n_region, n_variable, 2

  TDirectory ***dir_variable; // n_region, n_varaiable

  TFile *fin;
  TFile *fout;

  TLegend *tl;

  void Compare();
  void Draw();
  void Draw_Each(const TString &a_syst_name, const TString &a_variable_name = "All");
  void Envelope();
  vector<int> Get_Histo_Group(const TString &sample_name_merged);
  void Ordering_Sample_Name();
  void Save();
  void Setup_Histo_Data();
  void Setup_Histo_MC();
  void Setup_Name(const TList *list, vector<TString> &vec_name);
  void Setup_Reader_Template();
  void Stack_MC();

  ClassDef(Data_MC_Comparison, 1);
};

#endif /* __DATA_MC_COMPARISON_H__ */
