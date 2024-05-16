#ifndef __Result_Event_H__
#define __Result_Event_H__

#include <TString.h>
#include <TTree.h>
#include <TH1D.h>

#include "Const_Def.h"
#include "W_Event.h"

class Data_MC_Comparison;

class Result_Event : public W_Event
{
  friend class Template;
  friend class Histo_Syst;
  friend class Reco_Eval;
  friend class QCD_Data_Driven;

public:
  Result_Event(const TString &a_era = "2017", const TString &a_channel = "Mu", const TString &a_swap_mode = "Permutation_MVA");
  virtual ~Result_Event();

  void Setup_Tree(TTree *tree, const Syst syst, const bool chk_all = false);

protected:
  float weight_mu_id;
  float weight_mu_id_down;
  float weight_mu_id_up;

  float weight_mu_iso;
  float weight_mu_iso_down;
  float weight_mu_iso_up;

  float weight_el_id;
  float weight_el_id_down;
  float weight_el_id_up;

  float weight_el_reco;
  float weight_el_reco_down;
  float weight_el_reco_up;

  float weight_sl_trig;
  float weight_sl_trig_down;
  float weight_sl_trig_up;

  float weight;

  float weight_b_tag;
  float weight_b_tag_hf_down;
  float weight_b_tag_hf_up;
  float weight_b_tag_lf_down;
  float weight_b_tag_lf_up;
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

  float weight_lumi;
  float weight_mc;
  float weight_hem_veto;

  float weight_pdf_alternative;
  float weight_pdf_error_set[100];
  float weight_pdf_as_down;
  float weight_pdf_as_up;

  float weight_pileup;
  float weight_pileup_down;
  float weight_pileup_up;

  float weight_prefire;
  float weight_prefire_down;
  float weight_prefire_up;

  float weight_ps[4];

  float weight_pujet_veto;
  float weight_pujet_veto_down;
  float weight_pujet_veto_up;

  float weight_scale_variation_1;
  float weight_scale_variation_2;
  float weight_scale_variation_3;
  float weight_scale_variation_4;
  float weight_scale_variation_6;
  float weight_scale_variation_8;

  float weight_top_pt;

  int n_pv;

  float lepton_pt;
  float lepton_pt_uncorr;
  float lepton_eta;
  float lepton_rel_iso;

  int n_jets;
  int n_bjets;
  int n_cjets;

  float ht;

  float pt_leading_jet;
  float pt_subleading_jet;

  float eta_leading_jet;
  float eta_subleading_jet;

  float bvsc_leading_jet;
  float cvsb_leading_jet;
  float cvsl_leading_jet;

  float bvsc_subleading_jet;
  float cvsb_subleading_jet;
  float cvsl_subleading_jet;

  float met_pt;
  float met_phi;

  float best_mva_score_pre;
  float best_mva_score;
  float best_chi2;

  float mt;

  float m_had_t;
  float m_had_w;
  float m_lep_t;
  float m_lep_w;

  float mva_hf_score;

  float bvsc_had_t_b;
  float cvsb_had_t_b;
  float cvsl_had_t_b;

  float bvsc_lep_t_b;
  float cvsb_lep_t_b;
  float cvsl_lep_t_b;

  float pt_had_t_b;
  float pt_lep_t_b;

  float eta_had_t_b;
  float eta_lep_t_b;

  float template_score;

  // For MC
  int decay_mode;

  vector<int> *vec_gen_hf_flavour = NULL;
  vector<int> *vec_gen_hf_origin = NULL;
  vector<int> *vec_sel_gen_hf_flavour = NULL;
  vector<int> *vec_sel_gen_hf_origin = NULL;

  bool chk_reco_correct;
  bool chk_included;
  bool chk_hf_contamination;
  bool chk_gentau_conta;

  bool pu_conta_had_t_b;
  bool pu_conta_lep_t_b;

  ClassDef(Result_Event, 1);
};

#endif /* __Result_Event_H__  */
