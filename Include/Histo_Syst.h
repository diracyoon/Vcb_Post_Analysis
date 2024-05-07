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
#include <TMVA/Reader.h>
#include "TROOT.h"

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event.h>
#include <Tagging_RF.h>

using namespace std;

class Histo_Syst : public TObject
{
public:
  Histo_Syst(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_mode = "Fill", const TString &a_run_flag = "Central", const TString &a_tagger = "B", const TString &a_swap_mode = "Permutation_MVA");
  ~Histo_Syst();

  void Run_Merge();

  typedef struct _Histo_Conf
  {
    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
  } Histo_Conf;

  inline static bool Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

protected:
  bool debug;

  int n_split;
  int index_split;
  int reduction;

  TString era;
  TString channel;
  TString mode;
  TString run_flag;
  TString tagger;

  bool chk_cut_best_mva_score_pre = false;
  float cut_best_mva_score_pre = -1;

  bool chk_cut_template_score = false;
  float cut_template_score = 0;

  TString path_base;

  Samples samples;

  vector<TString> vec_name_mc;
  int n_sample_mc;

  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  TString key_base;
  TString tree_name;

  Tagging_RF tagging_rf;

  TString data_short_name;

  map<TString, TFile *> map_fin_mc;
  // map<TString, TFile *> map_fin_mc_jec_down;
  // map<TString, TFile *> map_fin_mc_jec_up;
  // map<TString, TFile *> map_fin_mc_jer_down;
  // map<TString, TFile *> map_fin_mc_jer_up;
  // map<TString, TFile *> map_fin_mc_eec_down;
  // map<TString, TFile *> map_fin_mc_eec_up;
  // map<TString, TFile *> map_fin_mc_eer_down;
  // map<TString, TFile *> map_fin_mc_eer_up;
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
  // map<TString, TTree *> map_tree_mc_eec_down;
  // map<TString, TTree *> map_tree_mc_eec_up;
  // map<TString, TTree *> map_tree_mc_eer_down;
  // map<TString, TTree *> map_tree_mc_eer_up;
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

  int n_abcd_region = 4;
  vector<TString> abcd_region_name;

  int n_region;
  vector<TString> region_name;

  int n_syst;
  vector<TString> syst_name;

  int n_variable;
  vector<Histo_Conf> variable_conf;

  float b_tag_rf;
  float c_tag_rf;

  float n_bjets_f;
  float n_cjets_f;

  TH1 ******histo_mc; // n_abcd_region, n_region, n_syst, n_sample, n_variable
  TH1 ****histo_data; // n_abcd_region, n_region, n_variable

  TH2D ******histo_mc_dd; // n_abcd_region-2, n_region, n_syst, n_sample, n_variable
  TH2D ****histo_data_dd;   // n_abcd_region-2, n_region, n_variable

  TH1D ***histo_subtracted_tf;           // n_abcd_region-2, n_region
  TH2D ****histo_subtracted_data_driven; // n_region, n_syst, n_variable
  TH2D ****histo_tf_corrected;           // n_region, n_syst, n_variable
  TH2D ***histo_tf_corrected_up;         // n_region, n_variable
  TH2D ***histo_tf_corrected_down;       // n_region, n_variable

  TH1D **histo_tf; // n_region

  const int histo_tf_n_bin = 6;
  const float histo_tf_x_low = -3;
  const float histo_tf_x_up = 3;

  float bvsc_wp_m;
  float cvsb_wp_m;
  float cvsl_wp_m;

  TFile *fin_2d;
  TFile *fin_tf;

  TFile *fout;

  bool chk_merge_pdf_error_set = false;
  int n_pdf_error_set = 100;
  TH1D ****histo_mc_pdf_error_set_down; // n_region, n_sample, n_variable
  TH1D ****histo_mc_pdf_error_set_up;   // n_region, n_sample, n_variable

  TString template_mva_name;
  TMVA::Reader *reader_template;

  // template <typename T>
  // T ****Allocate_Array(const int &dim1, const int &dim2, const int &dim3);

  void Calculate_TF();
  void Config_Sample();
  void Config_Syst();
  void Config_Variable();
  void Data_Driven();
  void Fill_Histo_Data();
  void Fill_Histo_MC(const TString &sample_name, const TString &syst_fix = "None");
  int Histo_Index(const TString &sample_name);
  TString Histo_Name_RF(const TString &sample_name);
  int Get_Region_Index(const TString &region);
  void Init_Histo();
  void Init_Histo_Data_Driven();
  void Init_Histo_TF();
  void Init_Merge_PDF_Error_Set();
  void Merge_PDF_Error_Set();
  void Read_Tree();
  void Register_Sample();
  void Register_Sample_TF();
  void Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type);
  float Reweight_CKM(const TString &sample_name);
  void Setup_Template_Reader();
  int Set_ABCD_Region();
  int Set_Region();

  ClassDef(Histo_Syst, 1);
};

#endif /* __Histo_Syst_H__ */
