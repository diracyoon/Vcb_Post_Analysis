#ifndef __Result_Event_CR_DL_H__
#define __Result_Event_CR_DL_H__

#include <vector>

#include "TObject.h"
#include "TTree.h"

#include "Const_Def.h"

using namespace std;

class Result_Event_CR_DL : public TObject
{
  friend class CR_DL;

public:
  Result_Event_CR_DL();
  ~Result_Event_CR_DL();

  void Setup_Tree(TTree *tree, const Syst syst, const bool chk_all = false, const bool chk_data = false);

protected:
  int n_pv;
  int n_pileup;

  float leading_lepton_eta;
  float leading_lepton_pt;
  float subleading_lepton_eta;
  float subleading_lepton_pt;
  float dilepton_mass;

  int n_jet;
  int n_b_jet;
  int n_c_jet;
  float ht;

  float leading_jet_bvsc;
  float leading_jet_cvsb;
  float leading_jet_cvsl;

  float subleading_jet_bvsc;
  float subleading_jet_cvsb;
  float subleading_jet_cvsl;

  float leading_jet_charge;
  float subleading_jet_charge;

  float leading_jet_pt;
  float subleading_jet_pt;

  float met_pt;
  float met_phi;

  int decay_mode;

  int gen_ttbar_id;

  vector<int> *vec_gen_hf_flavour = NULL;
  vector<int> *vec_gen_hf_origin = NULL;
  vector<int> *vec_sel_gen_hf_flavour = NULL;
  vector<int> *vec_sel_gen_hf_origin = NULL;

  float bvsc_third;
  float bvsc_fourth;

  float bvsc_third_pt;
  float bvsc_third_eta;

  float bvsc_fourth_pt;
  float bvsc_fourth_eta;

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

  float weight_el_id;
  float weight_el_id_down;
  float weight_el_id_up;

  float weight_el_reco;
  float weight_el_reco_down;
  float weight_el_reco_up;

  float weight_hem_veto;
  float weight_lumi;
  float weight_mc;

  float weight_mu_id;
  float weight_mu_id_down;
  float weight_mu_id_up;

  float weight_mu_iso;
  float weight_mu_iso_down;
  float weight_mu_iso_up;

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

  float weight_sl_trig;
  float weight_sl_trig_el_down;
  float weight_sl_trig_el_up;
  float weight_sl_trig_mu_down;
  float weight_sl_trig_mu_up;

  float weight_top_pt;

  ClassDef(Result_Event_CR_DL, 1);
};

#endif /* __Result_Event_CR_DL_H__ */
