#ifndef __QCD_Data_Driven_H__
#define __QCD_Data_Driven_H__

#include <iostream>
#include <map>

#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event.h>
#include <Tagging_RF.h>

using namespace std;

class QCD_Data_Driven : public TObject
{
public:
  QCD_Data_Driven(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_run_flag = "Central", const TString &a_tagger = "B");
  ~QCD_Data_Driven();

  void Run();

  inline static bool Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

protected:
  int n_split;
  int index_split;
  int reduction;

  TString era;
  TString channel;
  TString tagger;
  TString run_flag;

  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  TString key_base;
  TString path_base;

  Samples samples;

  Tagging_RF tagging_rf;

  TString data_short_name;

  map<TString, TFile *> map_fin_mc;
  map<TString, TFile *> map_fin_mc_cp5_down;
  map<TString, TFile *> map_fin_mc_cp5_up;
  map<TString, TFile *> map_fin_mc_hdamp_down;
  map<TString, TFile *> map_fin_mc_hdamp_up;
  map<TString, TFile *> map_fin_mc_mtop_171p5;
  map<TString, TFile *> map_fin_mc_mtop_173p5;

  map<TString, TTree *> map_tree_mc;
  map<TString, TTree *> map_tree_mc_jec_down;
  map<TString, TTree *> map_tree_mc_jec_up;
  map<TString, TTree *> map_tree_mc_jer_down;
  map<TString, TTree *> map_tree_mc_jer_up;
  map<TString, TTree *> map_tree_mc_ue_down;
  map<TString, TTree *> map_tree_mc_ue_up;
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

  int n_b_region = 2;

  vector<TString> region_name = {"A", "B", "C", "D"};

  int n_syst;
  vector<TString> syst_name;

  TH2D ****histo_2d_mc; // n_syst, n_sample, n_b_region
  TH2D **histo_2d_data; // n_b_region

  TFile *fout;

  void Fill_Histo_Data();
  void Fill_Histo_MC(const TString &sample_name, const TString &syst_fix = "None");
  int Histo_Index(const TString &sample_name);
  TString Histo_Name_RF(const TString &sample_name);
  void Init();
  void Read_Tree();
  void Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type);
  int Set_B_Region();
  int Set_Region();

  ClassDef(QCD_Data_Driven, 1);
};

#endif /* __QCD_Data_Driven_H__ */