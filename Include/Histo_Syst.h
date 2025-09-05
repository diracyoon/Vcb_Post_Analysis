#ifndef __Histo_Syst_H__
#define __Histo_Syst_H__

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include <TObject.h>
#include <TString.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TDirectory.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TGraphErrors.h>
#include <TMVA/Reader.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TROOT.h"

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event.h>
#include <Tagging_RF.h>
#include <Tagging_RF_Flavor.h>
#include <Modelling_Patch.h>

using namespace std;

class Histo_Syst : public TObject
{
public:
  Histo_Syst(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_mode = "Fill", const int &a_index_tree_type = -1, const int &a_last_index_tree_type = -1, const TString &a_tagger = "B", const TString &a_swap_mode = "Permutation_MVA");
  ~Histo_Syst();

  class Histo_Conf
  {
  public:
    Histo_Conf() {}

    Histo_Conf(TString a_variable_title, int a_n_bin, float a_x_low, float a_x_up) : variable_title(a_variable_title), n_bin(a_n_bin), x_low(a_x_low), x_up(a_x_up)
    {
      chk_equal_interval = true;

      for (int i = 0; i < n_bin; i++)
        vec_bin.push_back(i * (x_up - x_low) / n_bin + x_low);
      vec_bin.push_back(x_up);
    }

    Histo_Conf(TString a_variable_title) : variable_title(a_variable_title)
    {
      chk_equal_interval = false;
    }

    ~Histo_Conf() {}

    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
    bool chk_equal_interval;
    vector<float> vec_bin;
  };

  inline static bool Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

private:
  bool debug;

  int n_split;
  int index_split;
  int reduction;

  TString era;
  TString channel;
  TString mode;
  TString run_flag;
  TString tagger;

  TString year;

  bool chk_bin_optimizer = false;

  bool chk_qcd_ben = true;

  int index_tree_type;
  int last_index_tree_type;
  vector<TString> vec_tree_type;
  int n_tree_type;

  bool chk_qcd_data_driven = true;

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

  // Tagging_RF tagging_rf;
  Tagging_RF_Flavor tagging_rf;
  Modelling_Patch modelling_patch;

  TString data_short_name;

  vector<map<TString, TFile *>> vec_map_fin_mc;
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

  vector<vector<float>> bin_template_mva_score;
  vector<float> bin_eta_tf;
  vector<float> bin_pt_tf;

  float b_tag_rf;
  float c_tag_rf;

  float n_bjets_f;
  float n_cjets_f;

  TH1 ******histo_mc; // n_abcd_region, n_region, n_syst, n_sample, n_variable
  TH1 ****histo_data; // n_abcd_region, n_region, n_variable

  TH3D ******histo_mc_dd; // n_abcd_region-2, n_region, n_syst, n_sample, n_variable
  TH3D ****histo_data_dd; // n_abcd_region-2, n_region, n_variable

  TH1D *****histo_mc_merge; // n_region, n_syst, n_sample, n_variable

  TH1 ***histo_subtracted_tf;            // n_abcd_region-2, n_region
  TH3D ****histo_subtracted_data_driven; // n_region, n_syst, n_variable
  TH3D ****histo_tf_corrected;           // n_region, n_syst, n_variable
  TH3D *****histo_tf_corrected_up;       // n_region, n_variable, tf n_bin_eta, tf n_bin_pt
  TH3D *****histo_tf_corrected_down;     // n_region, n_variable, tf n_bin_eta, tf n_bin_pt

  TH1D ***histo_envelop_up;   // n_region, n_variable
  TH1D ***histo_envelop_down; // n_region, n_variable

  TH2D **histo_tf; // n_region
  TH2D *histo_tf_combine;
  TH2D **histo_tf_qcd_mc; // n_region

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

  // void Apply_Modelling_Patch();
  void Calculate_TF();
  void Config_Sample();
  void Config_Syst();
  void Config_Variable();
  void Data_Driven();
  void Draw_TF();
  void Fill_Histo_Data();
  void Fill_Histo_MC(const TString &sample_name, const TString &sampple_name_short, const TString &tree_type);
  int Histo_Index(const TString &sample_name, bool &chk_discarded);
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
  float Reweight_TTHF(const TString &sample_name);
  void Setup_Template_Reader();
  int Set_ABCD_Region();
  int Set_Region();

  void printMemoryUsage(const std::string &stage)
  {
    std::ifstream status_file("/proc/self/status");
    std::string line;
    long rss_kb = 0;
    while (std::getline(status_file, line))
    {
      if (line.rfind("VmRSS:", 0) == 0)
      {
        sscanf(line.c_str(), "VmRSS:\t%ld kB", &rss_kb);
        break;
      }
    }
    std::cout << "[MemoryUsage] " << stage << ": " << rss_kb / 1024.0 << " MB" << std::endl;
  }

  ClassDef(Histo_Syst, 1);
};

#endif /* __Histo_Syst_H__ */
