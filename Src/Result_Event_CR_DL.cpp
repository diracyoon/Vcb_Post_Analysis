#include "Result_Event_CR_DL.h"

ClassImp(Result_Event_CR_DL);

//////////

Result_Event_CR_DL::Result_Event_CR_DL()
{
} // Result_Event_CR_DL::Result_Event_CR_DL()

//////////

Result_Event_CR_DL::~Result_Event_CR_DL()
{
} // Result_Event_CR_DL::~Result_Event_CR_DL()

//////////

void Result_Event_CR_DL::Setup_Tree(TTree *tree, const Syst syst, const bool chk_all)
{

  tree->SetBranchAddress("n_pv", &n_pv);

  tree->SetBranchAddress("leading_lepton_eta", &leading_lepton_eta);
  tree->SetBranchAddress("leading_lepton_pt", &leading_lepton_pt);

  tree->SetBranchAddress("subleading_lepton_eta", &subleading_lepton_eta);
  tree->SetBranchAddress("subleading_lepton_pt", &subleading_lepton_pt);

  tree->SetBranchAddress("dilepton_mass", &dilepton_mass);

  tree->SetBranchAddress("n_jet", &n_jet);
  tree->SetBranchAddress("n_b_jet", &n_b_jet);
  tree->SetBranchAddress("ht", &ht);

  tree->SetBranchAddress("leading_jet_bvsc", &leading_jet_bvsc);
  tree->SetBranchAddress("leading_jet_cvsb", &leading_jet_cvsb);
  tree->SetBranchAddress("leading_jet_cvsl", &leading_jet_cvsl);

  tree->SetBranchAddress("subleading_jet_bvsc", &subleading_jet_bvsc);
  tree->SetBranchAddress("subleading_jet_cvsb", &subleading_jet_cvsb);
  tree->SetBranchAddress("subleading_jet_cvsl", &subleading_jet_cvsl);

  tree->SetBranchAddress("met_pt", &met_pt);
  tree->SetBranchAddress("met_phi", &met_phi);

  tree->SetBranchAddress("leading_jet_charge", &leading_jet_charge);
  tree->SetBranchAddress("subleading_jet_charge", &subleading_jet_charge);

  tree->SetBranchAddress("bvsc_third", &bvsc_third);
  tree->SetBranchAddress("bvsc_fourth", &bvsc_fourth);

  tree->SetBranchAddress("decay_mode", &decay_mode);

  tree->SetBranchAddress("Gen_HF_Flavour", &vec_gen_hf_flavour);
  tree->SetBranchAddress("Gen_HF_Origin", &vec_gen_hf_origin);
  tree->SetBranchAddress("Sel_Gen_HF_Flavour", &vec_sel_gen_hf_flavour);
  tree->SetBranchAddress("Sel_Gen_HF_Origin", &vec_sel_gen_hf_origin);

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
  else if (syst == Syst::JetEnDown)
    tree->SetBranchAddress("weight_c_tag_down_jes_total", &weight_c_tag_jes_total_down);
  else if (syst == Syst::JetEnUp)
    tree->SetBranchAddress("weight_c_tag_up_jes_total", &weight_c_tag_jes_total_up);
  else if (syst == Syst::JetResDown)
    tree->SetBranchAddress("weight_c_tag_down_jer", &weight_c_tag_jer_down);
  else if (syst == Syst::JetResUp)
    tree->SetBranchAddress("weight_c_tag_up_jer", &weight_c_tag_jer_up);
  else
    tree->SetBranchAddress("weight_c_tag", &weight_c_tag);

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

  tree->SetBranchAddress("weight_hem_veto", &weight_hem_veto);
  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);

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

  tree->SetBranchAddress("weight_sl_trig", &weight_sl_trig);
  if (syst == Syst::Central && chk_all)
  {
    tree->SetBranchAddress("weight_sl_trig_el_down", &weight_sl_trig_el_down);
    tree->SetBranchAddress("weight_sl_trig_el_up", &weight_sl_trig_el_up);
    tree->SetBranchAddress("weight_sl_trig_mu_down", &weight_sl_trig_mu_down);
    tree->SetBranchAddress("weight_sl_trig_mu_up", &weight_sl_trig_mu_up);
  }

  tree->SetBranchAddress("weight_top_pt", &weight_top_pt);

  return;
} // void Result_Event_CR_DL::Setup_Tree(TTree *tree, const bool &chk_syst)

//////////
