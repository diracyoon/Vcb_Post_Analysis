#ifndef __CR_DL_H__
#define __CR_DL_H__

#include <iostream>
#include <vector>
#include <map>

#include "TObject.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "THStack.h"
#include "TMath.h"
#include "TPad.h"

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event_CR_DL.h>
#include <Tagging_RF_DL.h>
#include <Tagging_RF_Flavor_DL.h>
#include <Modelling_Patch.h>

using namespace std;

class CR_DL : public TObject
{
public:
  CR_DL(const TString &a_era = "2018", const TString &a_channel = "MM", const TString &a_mode = "Main", const TString &a_tagger = "C", const int &a_index_tree_type = -1);
  ~CR_DL();

  inline static bool Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

  typedef struct _Histo_Conf
  {
    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
  } Histo_Conf;

  typedef struct _Histo_2D_Conf
  {
    TString variable_title;
    int n_bin_x;
    float *binning_x;
    int n_bin_y;
    float *binning_y;
  } Histo_2D_Conf;

protected:
  int reduction;

  TString era;
  TString channel;
  TString mode;
  TString tagger;
  int index_tree_type;

  TString year;

  TString path_base;

  bool chk_print; // only for debug

  bool chk_bin_optimizer = false;

  vector<TString> vec_tree_type;

  bool chk_merge_pdf_error_set = false;

  Samples samples;

  vector<TString> vec_name_mc;
  int n_sample_mc;

  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  // Tagging_RF_DL tagging_rf_dl;
  Tagging_RF_Flavor_DL tagging_rf_dl;
  Modelling_Patch modelling_patch;

  map<TString, TFile *> map_fin_mc;
  map<TString, TFile *> map_fin_mc_cp5_down;
  map<TString, TFile *> map_fin_mc_cp5_up;
  map<TString, TFile *> map_fin_mc_hdamp_down;
  map<TString, TFile *> map_fin_mc_hdamp_up;
  map<TString, TFile *> map_fin_mc_mtop_171p5;
  map<TString, TFile *> map_fin_mc_mtop_173p5;
  map<TString, TFile *> map_fin_mc_tt_4f;

  vector<map<TString, TTree *>> vec_map_tree_mc;
  map<TString, TTree *> map_tree_mc_central;
  map<TString, TTree *> map_tree_mc_cp5_down;
  map<TString, TTree *> map_tree_mc_cp5_up;
  map<TString, TTree *> map_tree_mc_hdamp_down;
  map<TString, TTree *> map_tree_mc_hdamp_up;
  map<TString, TTree *> map_tree_mc_mtop_171p5;
  map<TString, TTree *> map_tree_mc_mtop_173p5;
  map<TString, TTree *> map_tree_mc_tt_4f;

  // map<TString, map<TString, TFile *> *> map_map_fin_mc;
  map<TString, map<TString, TTree *> *> map_map_tree_mc;

  map<TString, TFile *> map_fin_data;
  map<TString, TTree *> map_tree_data;

  Result_Event_CR_DL event;

  float b_tag_rf;
  float c_tag_rf;

  int n_syst;
  vector<TString> syst_name;

  vector<float> bin_bvsc;
  int n_bin_bvsc;

  int n_variable;
  vector<Histo_Conf> variable_conf;

  int n_variable_2d;
  vector<Histo_2D_Conf> variable_conf_2d;

  TH1D ****histo_mc;    // n_syst, n_sample, n_variable
  TH2D ****histo_mc_2d; // n_syst, n_sample, n_variable_2d

  TH1D ***histo_mc_pdf_error_set_down; // n_sample, n_variable
  TH1D ***histo_mc_pdf_error_set_up;   // n_sample, n_variable

  TH1D ****histo_para_smoothing; // n_syst, n_samples in TT_powheg, 2 for pt_gen_tt or n_jets
  TH3D ***histo_3d_smoothing;    // n_samples in TT_powheg, 2 for  vs. pt_gen_tt or vs. n_jets
  TH2D ***histo_2d_smoothing;    // n_samples in TT_powheg, 2 for template mva score vs. pt_gen_tt or vs. n_jets

  TH1D ***histo_mc_smoothing_unrolled; // n_syst, n_sample

  TH1D **histo_data;    // n_variable
  TH2D **histo_data_2d; // n_variable_2d

  int n_pdf_error_set = 100;
  THStack ***stack_pdf_error_set; // n_pdf_error_set, n_variable

  TFile *fout;
  TFile *fin;
  TFile *fin_para_smoothing;

  Long64_t n_test_event;

  float modelling_patch_baseline;
  float modelling_patch_pdf_alternative;
  float modelling_patch_pdf_error_set[100];
  float modelling_patch_pdf_as_down;
  float modelling_patch_pdf_as_up;
  float modelling_patch_top_pt_reweight;
  float modelling_patch_scale_variation_1;
  float modelling_patch_scale_variation_2;
  float modelling_patch_scale_variation_3;
  float modelling_patch_scale_variation_4;
  float modelling_patch_scale_variation_6;
  float modelling_patch_scale_variation_8;
  float modelling_patch_ps_0;
  float modelling_patch_ps_1;
  float modelling_patch_ps_2;
  float modelling_patch_ps_3;

  TString sample_name_modelling_patch;
  TString sample_name_modelling_patch_prev;

  void Config_Sample();
  void Config_Syst();
  void Config_Variable();
  void Config_Vec_Tree_Type();
  void Draw_Smoothing();
  void Fill_Histo_Data();
  void Fill_Histo_MC(const TString &sample_name, const TString &sample_name_short, const TString &syst_fix);
  int Histo_Index(const TString &sample_name, bool &chk_discarded);
  TString Histo_Name_Modelling_Patch(const TString &sample_name);
  TString Histo_Name_RF(const TString &sample_name);
  void Init_Histo();
  void Init_Histo_Draw_Smoothing();
  void Init_Histo_Para_Smoothing();
  void Init_Histo_Smoothing();
  void Merge_PDF_Error_Set();
  void Ratio_Para_Smoothing();
  void Read_Tree();
  void Register_Sample();
  void Register_Sample_Para_Smoothing();
  void Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type);
  float Reweight_CKM(const TString &sample_name);
  float Reweight_TTHF(const TString &sample_name);
  void Smoothing();
  void Smoothing(const int syst_index, const int sample_index);
  void Smoothing(TH1D *histo_nominal, TH1D *histo_variated);

  void Unroller();
  int Unroller(const float &bvsc_third, const float &bvsc_fourth);
  void Unroller_Para_Smoothing();

  ClassDef(CR_DL, 1);
};

#endif /* __CR_DL_H__ */
