#include "Result_Event.h"

ClassImp(Result_Event);

//////////

Result_Event::Result_Event(const TString &a_era, const TString &a_channel, const TString &a_swap_mode) : W_Event(a_era, a_channel, a_swap_mode)
{
} // Result_Event::Result_Event(const TString& a_swap_mode)

//////////

Result_Event::~Result_Event()
{
} // Result_Event::~Result_Event()

//////////

void Result_Event::Setup_Tree(TTree *tree, const bool &chk_syst)
{
  tree->SetBranchAddress("sf_mu_id", &sf_mu_id);
  if (chk_syst)
  {
    tree->SetBranchAddress("sf_mu_id_down", &sf_mu_id_down);
    tree->SetBranchAddress("sf_mu_id_up", &sf_mu_id_up);
  }

  tree->SetBranchAddress("sf_mu_iso", &sf_mu_iso);
  if (chk_syst)
  {
    tree->SetBranchAddress("sf_mu_iso_down", &sf_mu_iso_down);
    tree->SetBranchAddress("sf_mu_iso_up", &sf_mu_iso_up);
  }

  tree->SetBranchAddress("sf_el_id", &sf_el_id);
  if( chk_syst)
    {
      tree->SetBranchAddress("sf_el_id_down", &sf_el_id_down);
      tree->SetBranchAddress("sf_el_id_up", &sf_el_id_up);
    }

  tree->SetBranchAddress("sf_el_reco", &sf_el_reco);
  if( chk_syst)
    {
      tree->SetBranchAddress("sf_el_reco_down", &sf_el_reco_down);
      tree->SetBranchAddress("sf_el_reco_up", &sf_el_reco_up);
    }

  tree->SetBranchAddress("sf_sl_trig", &sf_sl_trig);
  if (chk_syst)
  {
    tree->SetBranchAddress("sf_sl_trig_down", &sf_sl_trig_down);
    tree->SetBranchAddress("sf_sl_trig_up", &sf_sl_trig_up);
  }

  tree->SetBranchAddress("weight_b_tag", &weight_b_tag);
  if (chk_syst)
  {
    tree->SetBranchAddress("weight_b_tag_down_hf", &weight_b_tag_hf_down);
    tree->SetBranchAddress("weight_b_tag_up_hf", &weight_b_tag_hf_up);
    tree->SetBranchAddress("weight_b_tag_down_jes", &weight_b_tag_jes_down);
    tree->SetBranchAddress("weight_b_tag_up_jes", &weight_b_tag_jes_up);
    tree->SetBranchAddress("weight_b_tag_down_lfstats1", &weight_b_tag_lfstats1_down);
    tree->SetBranchAddress("weight_b_tag_up_lfstats1", &weight_b_tag_lfstats1_up);
    tree->SetBranchAddress("weight_b_tag_down_lfstats2", &weight_b_tag_lfstats2_down);
    tree->SetBranchAddress("weight_b_tag_up_lfstats2", &weight_b_tag_lfstats2_up);
    tree->SetBranchAddress("weight_b_tag_down_cferr1", &weight_b_tag_cferr1_down);
    tree->SetBranchAddress("weight_b_tag_up_cferr1", &weight_b_tag_cferr1_up);
    tree->SetBranchAddress("weight_b_tag_down_cferr2", &weight_b_tag_cferr2_down);
    tree->SetBranchAddress("weight_b_tag_up_cferr2", &weight_b_tag_cferr2_up);
    tree->SetBranchAddress("weight_b_tag_down_hfstats1", &weight_b_tag_hfstats1_down);
    tree->SetBranchAddress("weight_b_tag_up_hfstats1", &weight_b_tag_hfstats1_up);
    tree->SetBranchAddress("weight_b_tag_down_hfstats2", &weight_b_tag_hfstats2_down);
    tree->SetBranchAddress("weight_b_tag_up_hfstats2", &weight_b_tag_hfstats2_up);
  }

  tree->SetBranchAddress("weight_c_tag", &weight_c_tag);
  if (chk_syst)
  {
    tree->SetBranchAddress("weight_c_tag_down_extrap", &weight_c_tag_extrap_down);
    tree->SetBranchAddress("weight_c_tag_up_extrap", &weight_c_tag_extrap_up);
    tree->SetBranchAddress("weight_c_tag_down_interp", &weight_c_tag_interp_down);
    tree->SetBranchAddress("weight_c_tag_up_interp", &weight_c_tag_interp_up);
    tree->SetBranchAddress("weight_c_tag_down_lhe_scale_muf", &weight_c_tag_lhe_scale_muf_down);
    tree->SetBranchAddress("weight_c_tag_up_lhe_scale_muf", &weight_c_tag_lhe_scale_muf_up);
    tree->SetBranchAddress("weight_c_tag_down_lhe_scale_mur", &weight_c_tag_lhe_scale_mur_down);
    tree->SetBranchAddress("weight_c_tag_up_lhe_scale_mur", &weight_c_tag_lhe_scale_mur_up);
    tree->SetBranchAddress("weight_c_tag_down_ps_fsr_fixed", &weight_c_tag_ps_fsr_fixed_down);
    tree->SetBranchAddress("weight_c_tag_up_ps_fsr_fixed", &weight_c_tag_ps_fsr_fixed_up);
    tree->SetBranchAddress("weight_c_tag_down_ps_isr_fixed", &weight_c_tag_ps_isr_fixed_down);
    tree->SetBranchAddress("weight_c_tag_up_ps_isr_fixed", &weight_c_tag_ps_isr_fixed_up);
    tree->SetBranchAddress("weight_c_tag_down_pu", &weight_c_tag_pu_down);
    tree->SetBranchAddress("weight_c_tag_up_pu", &weight_c_tag_pu_up);
    tree->SetBranchAddress("weight_c_tag_down_stat", &weight_c_tag_stat_down);
    tree->SetBranchAddress("weight_c_tag_up_stat", &weight_c_tag_stat_up);
    tree->SetBranchAddress("weight_c_tag_down_xsec_brunc_dyjets_b", &weight_c_tag_xsec_br_unc_dyjets_b_down);
    tree->SetBranchAddress("weight_c_tag_up_xsec_brunc_dyjets_b", &weight_c_tag_xsec_br_unc_dyjets_b_up);
    tree->SetBranchAddress("weight_c_tag_down_xsec_brunc_dyjets_c", &weight_c_tag_xsec_br_unc_dyjets_c_down);
    tree->SetBranchAddress("weight_c_tag_up_xsec_brunc_dyjets_c", &weight_c_tag_xsec_br_unc_dyjets_c_up);
    tree->SetBranchAddress("weight_c_tag_down_xsec_brunc_wjets_c", &weight_c_tag_xsec_br_unc_wjets_c_down);
    tree->SetBranchAddress("weight_c_tag_up_xsec_brunc_wjets_c", &weight_c_tag_xsec_br_unc_wjets_c_up);
    tree->SetBranchAddress("weight_c_tag_down_jer", &weight_c_tag_jer_down);
    tree->SetBranchAddress("weight_c_tag_up_jer", &weight_c_tag_jer_up);
    tree->SetBranchAddress("weight_c_tag_down_jes_total", &weight_c_tag_jes_total_down);
    tree->SetBranchAddress("weight_c_tag_up_jes_total", &weight_c_tag_jes_total_up);
  }

  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);

  if (chk_syst)
  {
    tree->SetBranchAddress("weight_pdf_alternative", &weight_pdf_alternative);
    tree->SetBranchAddress("weight_pdf_error_set", weight_pdf_error_set);
    tree->SetBranchAddress("weight_pdf_as_down", &weight_pdf_as_down);
    tree->SetBranchAddress("weight_pdf_as_up", &weight_pdf_as_up);
  }

  tree->SetBranchAddress("weight_pileup", &weight_pileup);
  if (chk_syst)
  {
    tree->SetBranchAddress("weight_pileup_down", &weight_pileup_down);
    tree->SetBranchAddress("weight_pileup_up", &weight_pileup_up);
  }

  tree->SetBranchAddress("weight_prefire", &weight_prefire);
  if (chk_syst)
  {
    tree->SetBranchAddress("weight_prefire_down", &weight_prefire_down);
    tree->SetBranchAddress("weight_prefire_up", &weight_prefire_up);
  }

  tree->SetBranchAddress("weight_pujet_veto", &weight_pujet_veto);
  if (chk_syst)
  {
    tree->SetBranchAddress("weight_pujet_veto_down", &weight_pujet_veto_down);
    tree->SetBranchAddress("weight_pujet_veto_up", &weight_pujet_veto_up);
  }

  if (chk_syst)
  {
    tree->SetBranchAddress("weight_scale_variation_1", &weight_scale_variation_1);
    tree->SetBranchAddress("weight_scale_variation_2", &weight_scale_variation_2);
    tree->SetBranchAddress("weight_scale_variation_3", &weight_scale_variation_3);
    tree->SetBranchAddress("weight_scale_variation_4", &weight_scale_variation_4);
    tree->SetBranchAddress("weight_scale_variation_6", &weight_scale_variation_6);
    tree->SetBranchAddress("weight_scale_variation_8", &weight_scale_variation_8);
  }

  tree->SetBranchAddress("weight_top_pt", &weight_top_pt);

  tree->SetBranchAddress("n_vertex", &n_vertex);
  tree->SetBranchAddress("lepton_pt", &lepton_pt);
  tree->SetBranchAddress("lepton_eta", &lepton_eta);
  tree->SetBranchAddress("n_jets", &n_jets);
  tree->SetBranchAddress("n_bjets", &n_bjets);
  tree->SetBranchAddress("n_cjets", &n_cjets);
  tree->SetBranchAddress("pt_leading_jet", &pt_leading_jet);
  tree->SetBranchAddress("pt_subleading_jet", &pt_subleading_jet);
  tree->SetBranchAddress("eta_leading_jet", &eta_leading_jet);
  tree->SetBranchAddress("eta_subleading_jet", &eta_subleading_jet);
  tree->SetBranchAddress("bvsc_leading_jet", &bvsc_leading_jet);
  tree->SetBranchAddress("cvsb_leading_jet", &cvsb_leading_jet);
  tree->SetBranchAddress("cvsl_leading_jet", &cvsl_leading_jet);
  tree->SetBranchAddress("bvsc_subleading_jet", &bvsc_subleading_jet);
  tree->SetBranchAddress("cvsb_subleading_jet", &cvsb_subleading_jet);
  tree->SetBranchAddress("cvsl_subleading_jet", &cvsl_subleading_jet);
  tree->SetBranchAddress("met_pt", &met_pt);
  tree->SetBranchAddress("met_phi", &met_phi);
  tree->SetBranchAddress("best_mva_score_pre", &best_mva_score_pre);
  tree->SetBranchAddress("best_mva_score", &best_mva_score);
  tree->SetBranchAddress("best_chi2", &best_chi2);
  tree->SetBranchAddress("mt", &mt);
  tree->SetBranchAddress("mva_hf_score", &mva_hf_score);
  tree->SetBranchAddress("bvsc_had_t_b", &bvsc_had_t_b);
  tree->SetBranchAddress("cvsb_had_t_b", &cvsb_had_t_b);
  tree->SetBranchAddress("cvsl_had_t_b", &cvsl_had_t_b);
  tree->SetBranchAddress("bvsc_w_u", &bvsc_w_u);
  tree->SetBranchAddress("cvsb_w_u", &cvsb_w_u);
  tree->SetBranchAddress("cvsl_w_u", &cvsl_w_u);
  tree->SetBranchAddress("bvsc_w_d", &bvsc_w_d);
  tree->SetBranchAddress("cvsb_w_d", &cvsb_w_d);
  tree->SetBranchAddress("cvsl_w_d", &cvsl_w_d);
  tree->SetBranchAddress("bvsc_lep_t_b", &bvsc_lep_t_b);
  tree->SetBranchAddress("cvsb_lep_t_b", &cvsb_lep_t_b);
  tree->SetBranchAddress("cvsl_lep_t_b", &cvsl_lep_t_b);
  tree->SetBranchAddress("pt_had_t_b", &pt_had_t_b);
  tree->SetBranchAddress("pt_w_u", &pt_w_u);
  tree->SetBranchAddress("pt_w_d", &pt_w_d);
  tree->SetBranchAddress("pt_lep_t_b", &pt_lep_t_b);
  tree->SetBranchAddress("swapped_mva", &swapped_mva);
  tree->SetBranchAddress("m_w_u", &m_w_u);
  tree->SetBranchAddress("m_w_d", &m_w_d);
  tree->SetBranchAddress("decay_mode", &decay_mode);
  tree->SetBranchAddress("chk_reco_correct", &chk_reco_correct);
  tree->SetBranchAddress("chk_included", &chk_included);
  tree->SetBranchAddress("chk_hf_contamination", &chk_hf_contamination);
  tree->SetBranchAddress("pu_conta_had_t_b", &pu_conta_had_t_b);
  tree->SetBranchAddress("pu_conta_w_u", &pu_conta_w_u);
  tree->SetBranchAddress("pu_conta_w_d", &pu_conta_w_d);
  tree->SetBranchAddress("pu_conta_lep_t_b", &pu_conta_lep_t_b);
  tree->SetBranchAddress("swapped_truth", &swapped_truth);

  return;
} // void Result_Event::Setup_Tree(TTree* tree)

//////////
