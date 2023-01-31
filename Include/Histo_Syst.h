#ifndef __Histo_Syst_H__
#define __Histo_Syst_H__

#include <iostream>
#include <map>
#include <vector>

#include <TObject.h>
#include <TString.h>
#include <TTree.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TGraphErrors.h>
// #include <THStack.h>

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event.h>

using namespace std;

class Histo_Syst : public TObject
{
public:
  Histo_Syst(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_run_flag = "Central", const TString &a_swap_mode = "Permutation_MVA");
  ~Histo_Syst();

  void Run();

  typedef struct _Histo_Conf
  {
    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
  } Histo_Conf;

protected:
  int n_split;
  int index_split;
  int reduction;

  Samples samples;
  int n_sample_merge_mc;
  vector<TString> vec_short_name_mc;

  TString era;
  TString channel;
  TString path_base;
  TString run_flag;

  TString data_short_name;

  map<TString, TFile *> map_fin_mc;
  map<TString, TFile *> map_fin_mc_jec_down;
  map<TString, TFile *> map_fin_mc_jec_up;
  map<TString, TFile *> map_fin_mc_jes_down;
  map<TString, TFile *> map_fin_mc_jes_up;
  map<TString, TFile *> map_fin_mc_cp5_down;
  map<TString, TFile *> map_fin_mc_cp5_up;
  map<TString, TFile *> map_fin_mc_hdamp_down;
  map<TString, TFile *> map_fin_mc_hdamp_up;
  map<TString, TFile *> map_fin_mc_mtop_171p5;
  map<TString, TFile *> map_fin_mc_mtop_173p5;

  map<TString, TTree *> map_tree_mc;
  map<TString, TTree *> map_tree_mc_jec_down;
  map<TString, TTree *> map_tree_mc_jec_up;
  map<TString, TTree *> map_tree_mc_jes_down;
  map<TString, TTree *> map_tree_mc_jes_up;
  map<TString, TTree *> map_tree_mc_cp5_down;
  map<TString, TTree *> map_tree_mc_cp5_up;
  map<TString, TTree *> map_tree_mc_hdamp_down;
  map<TString, TTree *> map_tree_mc_hdamp_up;
  map<TString, TTree *> map_tree_mc_mtop_171p5;
  map<TString, TTree *> map_tree_mc_mtop_173p5;

  map<TString, map<TString, TFile *> *> map_map_fin_mc;
  map<TString, map<TString, TTree *> *> map_map_tree_mc;

  map<TString, TFile *> map_fin_data;
  map<TString, TTree *> map_tree_data;

  Result_Event event;

  int n_region;
  vector<TString> region_name;

  int n_syst;
  vector<TString> syst_name;

  int n_variable;
  vector<Histo_Conf> variable_conf;

  int n_syst_b;
  vector<TString> name_syst_b;

  int n_syst_c;
  vector<TString> name_syst_c;

  TH1D **histo_tagging_rf_b; // n_syst_b
  TH1D **histo_tagging_rf_c; // n_syst_c

  float b_tag_rf;
  float c_tag_rf;

  TH1D *****histo_mc; // n_region, n_syst, n_sample, n_variable
  // TH1D ****histo_weight; // n_region, n_c_tag_weight, n_sample
  // THStack ****stack_mc;  // n_region, n_syst, n_variable

  TH1D ***histo_data; // n_region, n_variable

  TString region;

  TFile *fin_tagging_rf;
  TFile *fout;

  void Fill_Histo_Data(const int &region_index);
  void Fill_Histo_MC(const int &region_index, const int &sample_index, const TString &syst_fix = "None");
  inline int Get_Region_Index(const TString &region);
  float Get_Tagging_RF(const TString &syst_name);
  void Read_Tree();
  bool Set_Region();
  void Set_Tagging_RF();

  ClassDef(Histo_Syst, 1);
};

#endif /* __Histo_Syst_H__ */
