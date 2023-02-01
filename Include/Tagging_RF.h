#ifndef __Tagging_RF_H__
#define __Tagging_RF_H__

#include <iostream>
#include <map>
#include <vector>

#include <TTree.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TFile.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TList.h>

#include <Samples.h>

using namespace std;

class Tagging_RF : public TObject
{
public:
  Tagging_RF(const TString &a_era = "2018", const TString &a_mode = "Application", const TString &a_extension = "png");
  ~Tagging_RF();

  float Get_Tagging_RF(const TString &syst_name, const int &n_jet);
  void Run_Analysis();

protected:
  int reduction;
  int n_bin;

  TString mode;
  TString era;
  TString channel;
  TString extension;

  Samples samples;
  int n_sample;
  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  int color[7] = {2, 3, 4, 5, 6, 7, 8}; // n_sample

  map<TString, TFile *> map_fin_mc;
  map<TString, TTree *> map_tree_mc;

  vector<TString> syst_name_b;
  vector<TString> syst_name_c;
  int n_syst_b;
  int n_syst_c;

  TH1D **histo_mc_before;   // n_sample
  TH1D ***histo_mc_after_b; // n_sample, n_syst_b
  TH1D ***histo_mc_after_c; // n_sample, n_syst_c
  TH1D **histo_data;        // n_sample

  THStack *stack_mc_before;
  THStack **stack_mc_after_b; // n_syst_b
  THStack **stack_mc_after_c; // n_syst_c

  TH1D **ratio_b; // n_syst_b
  TH1D **ratio_c; // n_syst_c

  TCanvas *canvas_before;
  TCanvas **canvas_after_b; // n_syst_b
  TCanvas **canvas_after_c; // n_syst_c
  TCanvas **canvas_ratio_b; // n_syst_b
  TCanvas **canvas_ratio_c; // n_syst_c

  TLegend *legend;

  int n_jets;

  float sf_mu_id;
  float sf_mu_iso;
  float sf_mu_trig;

  float weight_lumi;
  float weight_mc;
  float weight_pileup;
  float weight_prefire;
  float weight_top_pt;
  float weight_pujet_veto;

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
  TFile *fout;

  void Draw();
  void Fill_Histo_MC(const int &sample_index);
  void Ratio();
  void Read_Tree();
  void Setup_Analysis();
  void Setup_Application();
  void Setup_Histo();
  void Setup_Tree(TTree *tree);
  void Stack_MC();

  ClassDef(Tagging_RF, 1);
};

#endif /* __Tagging_RF_H__ */
