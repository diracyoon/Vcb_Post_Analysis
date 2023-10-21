#ifndef __Tagging_RF_H__
#define __Tagging_RF_H__

#include <iostream>
#include <map>
#include <vector>

#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>
#include <TFile.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TList.h>
#include <TROOT.h>
#include <TDirectory.h>
#include <TList.h>
#include <TMath.h>
#include <TString.h>
#include <TStyle.h>
#include <TLatex.h>

#include <Samples.h>

using namespace std;

class Tagging_RF : public TObject
{
public:
  Tagging_RF(const TString &a_era = "2018", const TString &a_mode = "Application", const TString &a_channel = "Mu", const TString &a_extension = "png");
  ~Tagging_RF();

  float Get_Tagging_RF_B_Tag(const TString &sample, const TString &syst, const int &n_jet, const float &ht);
  float Get_Tagging_RF_C_Tag(const TString &sample, const TString &syst, const int &n_pv_, const float &ht_);

  inline static bool Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

protected:
  int reduction;

  TString mode;
  TString era;
  TString channel;
  TString extension;

  bool chk_combine;

  Samples samples;
  int n_sample;
  
  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  vector<TString> vec_sample_tagging_rf;
  int n_sample_tagging_rf;

  int color[7] = {2, 3, 4, 5, 6, 7, 8}; // n_sample

  vector<float> bin_njet;
  vector<float> bin_ht;
  vector<float> bin_npv;

  map<TString, TFile *> map_fin_mc;
  map<TString, TTree *> map_tree_mc;
  map<TString, TTree *> map_tree_mc_jec_down;
  map<TString, TTree *> map_tree_mc_jec_up;
  map<TString, TTree *> map_tree_mc_jer_down;
  map<TString, TTree *> map_tree_mc_jer_up;

  map<TString, map<TString, TTree *> *> map_map_tree_mc;

  vector<TString> sample_name;
  vector<TString> syst_name_b;
  vector<TString> syst_name_c;
  int n_sample_name;
  int n_syst_b;
  int n_syst_c;

  TH2D ***histo_mc_before_b; // n_sample, n_syst_b
  TH2D ***histo_mc_after_b;  // n_sample, n_syst_b

  TH2D ***histo_mc_before_c; // n_sample, n_syst_c
  TH2D ***histo_mc_after_c;  // n_sample, n_syst_c

  TH1D ****histo_closure_n_jet; // n_sample, 3 (no tagging SF, tagging SF, tagging SF+RF)
  TH1D ****histo_closure_ht;    // n_sample, 3
  TH1D ****histo_closure_n_pv;  // n_sample, 3

  TH1D ****histo_closure_bvsc; // n_sample, 3 (no tagging SF, tagging SF, tagging SF+RF)
  TH1D ****histo_closure_cvsb; // n_sample,
  TH1D ****histo_closure_cvsl; // n_sample,

  TH1D ****histo_closure_eta; // n_sample, 3 (no tagging SF, tagging SF, tagging SF+RF)
  TH1D ****histo_closure_pt;

  THStack *stack_mc_before;
  THStack **stack_mc_after_b; // n_syst_b
  THStack **stack_mc_after_c; // n_syst_c

  TH2D ***ratio_b; // n_sample, n_syst_b
  TH2D ***ratio_c; // n_sample, n_syst_c

  TH2D ***ratio_b_averaged;
  TH2D ***ratio_c_averaged;

  TCanvas **canvas_before_b; // n_sample
  TCanvas **canvas_before_c; // n_sample
  TCanvas ***canvas_after_b; // n_sample, n_syst_b
  TCanvas ***canvas_after_c; // n_sample, n_syst_c
  TCanvas ***canvas_ratio_b; // n_sample, n_syst_b
  TCanvas ***canvas_ratio_c; // n_sample, n_syst_c

  TLegend *legend;

  int n_jets;
  int n_pv;

  float ht;

  float leading_jet_bvsc;
  float leading_jet_cvsb;
  float leading_jet_cvsl;

  float leading_jet_eta;
  float leading_jet_pt;

  float subleading_jet_bvsc;
  float subleading_jet_cvsb;
  float subleading_jet_cvsl;

  float subleading_jet_eta;
  float subleading_jet_pt;

  int decay_mode;

  vector<int> *vec_gen_hf_flavour = NULL;
  vector<int> *vec_gen_hf_origin = NULL;
  vector<int> *vec_sel_gen_hf_flavour = NULL;
  vector<int> *vec_sel_gen_hf_origin = NULL;

  float sf_mu_id;
  float sf_mu_iso;

  float sf_el_id;
  float sf_el_reco;

  float sf_sl_trig;

  float weight_hem_veto;
  float weight_lumi;
  float weight_mc;

  float weight_pileup;
  float weight_pileup_down;
  float weight_pileup_up;

  float weight_ps[4];

  float weight_prefire;
  float weight_top_pt;
  float weight_pujet_veto;

  float weight_scale_variation_1;
  float weight_scale_variation_2;
  float weight_scale_variation_3;
  float weight_scale_variation_4;
  float weight_scale_variation_6;
  float weight_scale_variation_8;

  float weight_b_tag;
  float weight_b_tag_hf_down;
  float weight_b_tag_hf_up;
  float weight_b_tag_jes_down;
  float weight_b_tag_jes_up;
  float weight_b_tag_lfstats1_down;
  float weight_b_tag_lfstats1_up;
  float weight_b_tag_lfstats2_down;
  float weight_b_tag_lfstats2_up;
  float weight_b_tag_cferr1_down;
  float weight_b_tag_cferr1_up;
  float weight_b_tag_cferr2_down;
  float weight_b_tag_cferr2_up;
  float weight_b_tag_hfstats1_down;
  float weight_b_tag_hfstats1_up;
  float weight_b_tag_hfstats2_down;
  float weight_b_tag_hfstats2_up;

  float weight_c_tag;
  float weight_c_tag_extrap_down;
  float weight_c_tag_extrap_up;
  float weight_c_tag_interp_down;
  float weight_c_tag_interp_up;
  float weight_c_tag_lhe_scale_muf_down;
  float weight_c_tag_lhe_scale_muf_up;
  float weight_c_tag_lhe_scale_mur_down;
  float weight_c_tag_lhe_scale_mur_up;
  float weight_c_tag_ps_fsr_fixed_down;
  float weight_c_tag_ps_fsr_fixed_up;
  float weight_c_tag_ps_isr_fixed_down;
  float weight_c_tag_ps_isr_fixed_up;
  float weight_c_tag_pu_down;
  float weight_c_tag_pu_up;
  float weight_c_tag_stat_down;
  float weight_c_tag_stat_up;
  float weight_c_tag_xsec_br_unc_dyjets_b_down;
  float weight_c_tag_xsec_br_unc_dyjets_b_up;
  float weight_c_tag_xsec_br_unc_dyjets_c_down;
  float weight_c_tag_xsec_br_unc_dyjets_c_up;
  float weight_c_tag_xsec_br_unc_wjets_c_down;
  float weight_c_tag_xsec_br_unc_wjets_c_up;
  float weight_c_tag_jer_down;
  float weight_c_tag_jer_up;
  float weight_c_tag_jes_total_down;
  float weight_c_tag_jes_total_up;

  TFile *fin;
  TFile *fin_mu;
  TFile *fin_el;
  TFile *fout;

  void Combine_Decay_Mode();
  void Combine_Lepton_Channel();
  void Draw_Result();
  void Draw_Validation();
  void Fill_Histo_MC(const TString &sample_name, const TString &syst_type);
  void Fill_Histo_Validation_MC(const TString &sample_name, const TString &syst_type);
  int Histo_Index(const TString &sample_name);
  TString Histo_Name_RF(const TString &sample_name);
  void Ratio();
  void Read_Tree();
  void Run_Analysis();
  void Run_Combine();
  void Run_Draw_Validation();
  void Run_Validation();
  void Setup_Analysis();
  void Setup_Application();
  void Setup_Binning();
  void Setup_Histo();
  void Setup_Histo_Validation();
  void Setup_Tree(TTree *tree, const TString &syst);

private:
  bool chk_draw_called = false;

  ClassDef(Tagging_RF, 1);
};

#endif /* __Tagging_RF_H__ */
