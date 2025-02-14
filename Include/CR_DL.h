#ifndef __CR_DL_H__
#define __CR_DL_H__

#include <iostream>
#include <vector>
#include <map>

#include "TObject.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "THStack.h"
#include "TMath.h"

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event_CR_DL.h>
#include <Tagging_RF_DL.h>

using namespace std;

class CR_DL : public TObject
{
public:
  CR_DL(const TString &a_era = "2018", const TString &a_channel = "MM", const TString &a_tagger = "C", const int &a_index_tree_type = -1);
  ~CR_DL();

  inline static bool Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

  void Run();

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
  TString tagger;
  int index_tree_type;

  TString year;

  bool chk_rf_tthf_breakdown = false;
  bool chk_jes_breakdown = false;
  bool chk_print; // only for debug

  vector<TString> vec_tree_type;

  bool chk_merge_pdf_error_set = false;

  Samples samples;

  vector<TString> vec_name_mc;
  int n_sample_mc;

  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  Tagging_RF_DL tagging_rf_dl;

  map<TString, TFile *> map_fin_mc;
  map<TString, TFile *> map_fin_mc_cp5_down;
  map<TString, TFile *> map_fin_mc_cp5_up;
  map<TString, TFile *> map_fin_mc_hdamp_down;
  map<TString, TFile *> map_fin_mc_hdamp_up;
  map<TString, TFile *> map_fin_mc_mtop_171p5;
  map<TString, TFile *> map_fin_mc_mtop_173p5;

  vector<map<TString, TTree *>> vec_map_tree_mc;
  map<TString, TTree *> map_tree_mc_central;
  // map<TString, TTree *> map_tree_mc;
  // map<TString, TTree *> map_tree_mc_jec_down;
  // map<TString, TTree *> map_tree_mc_jec_up;
  // map<TString, TTree *> map_tree_mc_jer_down;
  // map<TString, TTree *> map_tree_mc_jer_up;
  // map<TString, TTree *> map_tree_mc_ue_down;
  // map<TString, TTree *> map_tree_mc_ue_up;
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

  int n_variable;
  vector<Histo_Conf> variable_conf;

  int n_variable_2d;
  vector<Histo_2D_Conf> variable_conf_2d;

  TH1D ****histo_mc;    // n_syst, n_sample, n_variable
  TH2D ****histo_mc_2d; // n_syst, n_sample, n_variable_2d

  TH1D ***histo_mc_pdf_error_set_down; // n_sample, n_variable
  TH1D ***histo_mc_pdf_error_set_up;   // n_sample, n_variable

  TH1D **histo_data;    // n_variable
  TH2D **histo_data_2d; // n_variable_2d

  int n_pdf_error_set = 100;
  THStack ***stack_pdf_error_set; // n_pdf_error_set, n_variable

  TFile *fout;

  void Fill_Histo_Data();
  void Fill_Histo_MC(const TString &sample_name, const TString &sample_name_short, const TString &syst_fix);
  int Histo_Index(const TString &sample_name);
  TString Histo_Name_RF(const TString &sample_name);
  void Merge_PDF_Error_Set();
  void Read_Tree();
  void Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type);
  float Reweight_CKM(const TString &sample_name);
  float Reweight_TTHF(const TString &sample_name);
  void Setup_Analysis();
  void Setup_Histo();
  void Unroller();
  int Unroller(const float &bvsc_third, const float &bvsc_fourth);

  ClassDef(CR_DL, 1);
};

#endif /* __CR_DL_H__ */
