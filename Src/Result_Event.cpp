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

void Result_Event::Setup_Tree(TTree *tree, const Syst syst, const bool chk_all, const bool chk_data)
{
  // cout << "[Result_Event::Setup_Tree]: Init" << endl;

  tree->SetBranchAddress("weight_mu_id", &weight_mu_id);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_mu_id_down", &weight_mu_id_down);
    tree->SetBranchAddress("weight_mu_id_up", &weight_mu_id_up);
  }

  tree->SetBranchAddress("weight_mu_iso", &weight_mu_iso);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_mu_iso_down", &weight_mu_iso_down);
    tree->SetBranchAddress("weight_mu_iso_up", &weight_mu_iso_up);
  }

  tree->SetBranchAddress("weight_el_id", &weight_el_id);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_el_id_down", &weight_el_id_down);
    tree->SetBranchAddress("weight_el_id_up", &weight_el_id_up);
  }

  tree->SetBranchAddress("weight_el_reco", &weight_el_reco);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_el_reco_down", &weight_el_reco_down);
    tree->SetBranchAddress("weight_el_reco_up", &weight_el_reco_up);
  }

  tree->SetBranchAddress("weight_sl_trig", &weight_sl_trig);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_sl_trig_down", &weight_sl_trig_down);
    tree->SetBranchAddress("weight_sl_trig_up", &weight_sl_trig_up);
  }

  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_b_tag", &weight_b_tag);
    tree->SetBranchAddress("weight_b_tag_down_hf", &weight_b_tag_hf_down);
    tree->SetBranchAddress("weight_b_tag_up_hf", &weight_b_tag_hf_up);
    tree->SetBranchAddress("weight_b_tag_down_lf", &weight_b_tag_lf_down);
    tree->SetBranchAddress("weight_b_tag_up_lf", &weight_b_tag_lf_up);
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
  else if (syst == Syst::JetEnDown)
    tree->SetBranchAddress("weight_b_tag_down_jes", &weight_b_tag_jes_down);
  else if (syst == Syst::JetEnUp)
    tree->SetBranchAddress("weight_b_tag_up_jes", &weight_b_tag_jes_up);
  else
    tree->SetBranchAddress("weight_b_tag", &weight_b_tag);

  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_c_tag", &weight_c_tag);
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
  }
  else if (syst == Syst::JetResDown)
    tree->SetBranchAddress("weight_c_tag_down_jer", &weight_c_tag_jer_down);
  else if (syst == Syst::JetResUp)
    tree->SetBranchAddress("weight_c_tag_up_jer", &weight_c_tag_jer_up);
  else if (syst == Syst::JetEnDown ||
           syst == Syst::JetEnAbsoluteDown ||
           syst == Syst::JetEnBBEC1Down ||
           syst == Syst::JetEnEC2Down ||
           syst == Syst::JetEnFlavorQCDDown ||
           syst == Syst::JetEnHFDown ||
           syst == Syst::JetEnRelativeBalDown ||
           syst == Syst::JetEnAbsolute2016Down ||
           syst == Syst::JetEnBBEC12016Down ||
           syst == Syst::JetEnEC22016Down ||
           syst == Syst::JetEnHF2016Down ||
           syst == Syst::JetEnRelativeSample2016Down ||
           syst == Syst::JetEnAbsolute2017Down ||
           syst == Syst::JetEnBBEC12017Down ||
           syst == Syst::JetEnEC22017Down ||
           syst == Syst::JetEnHF2017Down ||
           syst == Syst::JetEnRelativeSample2017Down ||
           syst == Syst::JetEnAbsolute2018Down ||
           syst == Syst::JetEnBBEC12018Down ||
           syst == Syst::JetEnEC22018Down ||
           syst == Syst::JetEnHF2018Down ||
           syst == Syst::JetEnRelativeSample2018Down)
    tree->SetBranchAddress("weight_c_tag_down_jes_total", &weight_c_tag_jes_total_down);
  else if (syst == Syst::JetEnUp ||
           syst == Syst::JetEnAbsoluteUp ||
           syst == Syst::JetEnBBEC1Up ||
           syst == Syst::JetEnEC2Up ||
           syst == Syst::JetEnFlavorQCDUp ||
           syst == Syst::JetEnHFUp ||
           syst == Syst::JetEnRelativeBalUp ||
           syst == Syst::JetEnAbsolute2016Up ||
           syst == Syst::JetEnBBEC12016Up ||
           syst == Syst::JetEnEC22016Up ||
           syst == Syst::JetEnHF2016Up ||
           syst == Syst::JetEnRelativeSample2016Up ||
           syst == Syst::JetEnAbsolute2017Up ||
           syst == Syst::JetEnBBEC12017Up ||
           syst == Syst::JetEnEC22017Up ||
           syst == Syst::JetEnHF2017Up ||
           syst == Syst::JetEnRelativeSample2017Up ||
           syst == Syst::JetEnAbsolute2018Up ||
           syst == Syst::JetEnBBEC12018Up ||
           syst == Syst::JetEnEC22018Up ||
           syst == Syst::JetEnHF2018Up ||
           syst == Syst::JetEnRelativeSample2018Up)
    tree->SetBranchAddress("weight_c_tag_up_jes_total", &weight_c_tag_jes_total_up);
  else
    tree->SetBranchAddress("weight_c_tag", &weight_c_tag);

  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);
  tree->SetBranchAddress("weight_hem_veto", &weight_hem_veto);

  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_pdf_alternative", &weight_pdf_alternative);
    tree->SetBranchAddress("weight_pdf_error_set", weight_pdf_error_set);
    tree->SetBranchAddress("weight_pdf_as_down", &weight_pdf_as_down);
    tree->SetBranchAddress("weight_pdf_as_up", &weight_pdf_as_up);
  }

  tree->SetBranchAddress("weight_pileup", &weight_pileup);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_pileup_down", &weight_pileup_down);
    tree->SetBranchAddress("weight_pileup_up", &weight_pileup_up);
  }

  tree->SetBranchAddress("weight_prefire", &weight_prefire);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_prefire_down", &weight_prefire_down);
    tree->SetBranchAddress("weight_prefire_up", &weight_prefire_up);
  }

  if (syst == Syst::Central && chk_all)
    tree->SetBranchAddress("weight_ps", weight_ps);

  tree->SetBranchAddress("weight_pujet_veto", &weight_pujet_veto);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_pujet_veto_down", &weight_pujet_veto_down);
    tree->SetBranchAddress("weight_pujet_veto_up", &weight_pujet_veto_up);
  }

  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_scale_variation_1", &weight_scale_variation_1);
    tree->SetBranchAddress("weight_scale_variation_2", &weight_scale_variation_2);
    tree->SetBranchAddress("weight_scale_variation_3", &weight_scale_variation_3);
    tree->SetBranchAddress("weight_scale_variation_4", &weight_scale_variation_4);
    tree->SetBranchAddress("weight_scale_variation_6", &weight_scale_variation_6);
    tree->SetBranchAddress("weight_scale_variation_8", &weight_scale_variation_8);
  }

  tree->SetBranchAddress("weight_top_pt", &weight_top_pt);

  tree->SetBranchAddress("n_vertex", &n_pv);

  tree->SetBranchAddress("lepton_pt", &lepton_pt);
  tree->SetBranchAddress("lepton_pt_uncorr", &lepton_pt_uncorr);
  tree->SetBranchAddress("lepton_eta", &lepton_eta);
  tree->SetBranchAddress("lepton_rel_iso", &lepton_rel_iso);
  // tree->SetBranchAddress("lepton_mva", &lepton_mva);

  tree->SetBranchAddress("n_jets", &n_jets);
  tree->SetBranchAddress("n_bjets", &n_bjets);
  tree->SetBranchAddress("n_cjets", &n_cjets);

  tree->SetBranchAddress("ht", &ht);

  tree->SetBranchAddress("m_had_t", &m_had_t);
  tree->SetBranchAddress("m_had_w", &m_had_w);
  tree->SetBranchAddress("m_lep_t", &m_lep_t);
  tree->SetBranchAddress("m_lep_w", &m_lep_w);

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

  tree->SetBranchAddress("eta_had_t_b", &eta_had_t_b);
  tree->SetBranchAddress("eta_w_u", &eta_w_u);
  tree->SetBranchAddress("eta_w_d", &eta_w_d);
  tree->SetBranchAddress("eta_lep_t_b", &eta_lep_t_b);

  tree->SetBranchAddress("swapped_mva", &swapped_mva);
  //tree->SetBranchAddress("template_score", &template_score);
  tree->SetBranchAddress("template_score_eraInclusive", &template_score);
  // tree->SetBranchAddress("template_score_fewshot", &template_score);
  // tree->SetBranchAddress("template_score_sigRegionOnly", &template_score);
  // tree->SetBranchAddress("template_score_focal", &template_score);
  // tree->SetBranchAddress("template_score_focal_sigOnly", &template_score);
  // tree->SetBranchAddress("template_score_mixed", &template_score);

  // for MC
  if (!chk_data)
  {
    tree->SetBranchAddress("n_pileup", &n_pileup);

    tree->SetBranchAddress("decay_mode", &decay_mode);
    tree->SetBranchAddress("genTtbarId", &gen_ttbar_id);

    // tree->SetBranchAddress("Gen_HF_Flavour", &vec_gen_hf_flavour);
    // tree->SetBranchAddress("Gen_HF_Origin", &vec_gen_hf_origin);
    // tree->SetBranchAddress("Sel_Gen_HF_Flavour", &vec_sel_gen_hf_flavour);
    // tree->SetBranchAddress("Sel_Gen_HF_Origin", &vec_sel_gen_hf_origin);

    tree->SetBranchAddress("swapped_truth", &swapped_truth);

    tree->SetBranchAddress("chk_reco_correct", &chk_reco_correct);
    tree->SetBranchAddress("chk_included", &chk_included);
    tree->SetBranchAddress("chk_hf_contamination", &chk_hf_contamination);
    tree->SetBranchAddress("chk_gentau_conta", &chk_gentau_conta);

    tree->SetBranchAddress("pu_conta_had_t_b", &pu_conta_had_t_b);
    tree->SetBranchAddress("pu_conta_w_u", &pu_conta_w_u);
    tree->SetBranchAddress("pu_conta_w_d", &pu_conta_w_d);
    tree->SetBranchAddress("pu_conta_lep_t_b", &pu_conta_lep_t_b);

    tree->SetBranchAddress("Jet_Pt", &vec_jet_pt);
    tree->SetBranchAddress("Jet_Eta", &vec_jet_eta);
    tree->SetBranchAddress("Jet_Flavor", &vec_jet_flavor);
  }
  // cout << "[Result_Event::Setup_Tree]: Done" << endl;

  return;
} // void Result_Event::Setup_Tree(TTree* tree)

//////////
