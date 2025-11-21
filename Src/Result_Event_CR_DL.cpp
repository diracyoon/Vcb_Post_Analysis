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

void Result_Event_CR_DL::Clear()
{
  weight = 1;

  weight_baseline = 1;

  weight_b_tag = 1;
  weight_b_tag_hf_down = 1;
  weight_b_tag_hf_up = 1;
  weight_b_tag_lf_down = 1;
  weight_b_tag_lf_up = 1;
  weight_b_tag_jes_down = 1;
  weight_b_tag_jes_up = 1;
  weight_b_tag_lfstats1_down = 1;
  weight_b_tag_lfstats1_up = 1;
  weight_b_tag_lfstats2_down = 1;
  weight_b_tag_lfstats2_up = 1;
  weight_b_tag_cferr1_down = 1;
  weight_b_tag_cferr1_up = 1;
  weight_b_tag_cferr2_down = 1;
  weight_b_tag_cferr2_up = 1;
  weight_b_tag_hfstats1_down = 1;
  weight_b_tag_hfstats1_up = 1;
  weight_b_tag_hfstats2_down = 1;
  weight_b_tag_hfstats2_up = 1;

  weight_c_tag = 1;
  weight_c_tag_extrap_down = 1;
  weight_c_tag_extrap_up = 1;
  weight_c_tag_interp_down = 1;
  weight_c_tag_interp_up = 1;
  weight_c_tag_lhe_scale_muf_down = 1;
  weight_c_tag_lhe_scale_muf_up = 1;
  weight_c_tag_lhe_scale_mur_down = 1;
  weight_c_tag_lhe_scale_mur_up = 1;
  weight_c_tag_ps_fsr_fixed_down = 1;
  weight_c_tag_ps_fsr_fixed_up = 1;
  weight_c_tag_ps_isr_fixed_down = 1;
  weight_c_tag_ps_isr_fixed_up = 1;
  weight_c_tag_pu_down = 1;
  weight_c_tag_pu_up = 1;
  weight_c_tag_stat_down = 1;
  weight_c_tag_stat_up = 1;
  weight_c_tag_xsec_br_unc_dyjets_b_down = 1;
  weight_c_tag_xsec_br_unc_dyjets_b_up = 1;
  weight_c_tag_xsec_br_unc_dyjets_c_down = 1;
  weight_c_tag_xsec_br_unc_dyjets_c_up = 1;
  weight_c_tag_xsec_br_unc_wjets_c_down = 1;
  weight_c_tag_xsec_br_unc_wjets_c_up = 1;
  weight_c_tag_jer_down = 1;
  weight_c_tag_jer_up = 1;
  weight_c_tag_jes_total_down = 1;
  weight_c_tag_jes_total_up = 1;

  weight_el_id = 1;
  weight_el_id_down = 1;
  weight_el_id_up = 1;

  weight_el_reco = 1;
  weight_el_reco_down = 1;
  weight_el_reco_up = 1;

  weight_hem_veto = 1;
  weight_lumi = 1;
  weight_mc = 1;

  weight_mu_id = 1;
  weight_mu_id_down = 1;
  weight_mu_id_up = 1;

  weight_mu_iso = 1;
  weight_mu_iso_down = 1;
  weight_mu_iso_up = 1;

  weight_pdf_alternative = 1;
  memset(weight_pdf_error_set, 1, sizeof(weight_pdf_error_set));
  weight_pdf_as_down = 1;
  weight_pdf_as_up = 1;

  weight_pileup = 1;
  weight_pileup_down = 1;
  weight_pileup_up = 1;

  weight_prefire = 1;
  weight_prefire_down = 1;
  weight_prefire_up = 1;

  memset(weight_ps, 1, sizeof(weight_ps));

  weight_pujet_veto = 1;
  weight_pujet_veto_down = 1;
  weight_pujet_veto_up = 1;

  weight_scale_variation_1 = 1;
  weight_scale_variation_2 = 1;
  weight_scale_variation_3 = 1;
  weight_scale_variation_4 = 1;
  weight_scale_variation_6 = 1;
  weight_scale_variation_8 = 1;

  weight_sl_trig = 1;
  weight_sl_trig_el_down = 1;
  weight_sl_trig_el_up = 1;
  weight_sl_trig_mu_down = 1;
  weight_sl_trig_mu_up = 1;

  weight_top_pt = 1;

  return;
} // void Result_Event_CR_DL::Clear()

//////////

void Result_Event_CR_DL::Setup_Tree(TTree *tree, const Syst syst, const bool chk_all, const bool chk_data)
{

  tree->SetBranchAddress("n_pv", &n_pv);

  tree->SetBranchAddress("leading_lepton_eta", &leading_lepton_eta);
  tree->SetBranchAddress("leading_lepton_pt", &leading_lepton_pt);

  tree->SetBranchAddress("subleading_lepton_eta", &subleading_lepton_eta);
  tree->SetBranchAddress("subleading_lepton_pt", &subleading_lepton_pt);

  tree->SetBranchAddress("dilepton_mass", &dilepton_mass);

  tree->SetBranchAddress("pt_gen_tt", &pt_gen_tt);

  tree->SetBranchAddress("n_jet", &n_jet);
  tree->SetBranchAddress("n_b_jet", &n_b_jet);
  tree->SetBranchAddress("n_c_jet", &n_c_jet);
  tree->SetBranchAddress("ht", &ht);

  tree->SetBranchAddress("leading_jet_bvsc", &leading_jet_bvsc);
  tree->SetBranchAddress("leading_jet_cvsb", &leading_jet_cvsb);
  tree->SetBranchAddress("leading_jet_cvsl", &leading_jet_cvsl);

  tree->SetBranchAddress("subleading_jet_bvsc", &subleading_jet_bvsc);
  tree->SetBranchAddress("subleading_jet_cvsb", &subleading_jet_cvsb);
  tree->SetBranchAddress("subleading_jet_cvsl", &subleading_jet_cvsl);

  tree->SetBranchAddress("leading_jet_charge", &leading_jet_charge);
  tree->SetBranchAddress("subleading_jet_charge", &subleading_jet_charge);

  tree->SetBranchAddress("leading_jet_pt", &leading_jet_pt);
  tree->SetBranchAddress("subleading_jet_pt", &subleading_jet_pt);

  tree->SetBranchAddress("met_pt", &met_pt);
  tree->SetBranchAddress("met_phi", &met_phi);

  tree->SetBranchAddress("bvsc_third", &bvsc_third);
  tree->SetBranchAddress("bvsc_fourth", &bvsc_fourth);

  tree->SetBranchAddress("bvsc_third_pt", &bvsc_third_pt);
  tree->SetBranchAddress("bvsc_third_eta", &bvsc_third_eta);

  tree->SetBranchAddress("bvsc_fourth_pt", &bvsc_fourth_pt);
  tree->SetBranchAddress("bvsc_fourth_eta", &bvsc_fourth_eta);

  if (!chk_data)
  {
    tree->SetBranchAddress("n_pileup", &n_pileup);
    tree->SetBranchAddress("decay_mode", &decay_mode);

    tree->SetBranchAddress("genTtbarId", &gen_ttbar_id);

    // tree->SetBranchAddress("Gen_HF_Flavour", &vec_gen_hf_flavour);
    // tree->SetBranchAddress("Gen_HF_Origin", &vec_gen_hf_origin);
    // tree->SetBranchAddress("Sel_Gen_HF_Flavour", &vec_sel_gen_hf_flavour);
    // tree->SetBranchAddress("Sel_Gen_HF_Origin", &vec_sel_gen_hf_origin);

    tree->SetBranchAddress("Jet_Pt", &vec_jet_pt);
    tree->SetBranchAddress("Jet_Eta", &vec_jet_eta);
    tree->SetBranchAddress("Jet_Flavor", &vec_jet_flavor);

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
      tree->SetBranchAddress("weight_b_tag_down_jes", &weight_b_tag_jes_down);
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
  } // if(!chk_data)

  return;
} // void Result_Event_CR_DL::Setup_Tree(TTree *tree, const bool &chk_syst)

//////////

void Result_Event_CR_DL::Swap_Scale_Variation(const TString &sample_name_short)
{
  // quick and dirty method
  // it seems index for scale_variation is different between POWHEG and madgraph
  // To save time, let's swap it in here although code become dirty
  if (sample_name_short == "TTLJ_WtoCB" || sample_name_short == "TTLJ" || sample_name_short == "TTLL" ||
      sample_name_short == "TTLJ_TTbb_4f" || sample_name_short == "TTLJ_bbDPS" || sample_name_short == "TTLL_TTbb_4f" || sample_name_short == "TTLL_bbDPS" ||
      sample_name_short == "ST_tch" || sample_name_short == "ST_tw" ||
      sample_name_short == "ttHTobb" || sample_name_short == "ttHToNonbb")
  {
    // index is OK
    // do nothing
  }
  else if (sample_name_short == "ST_sch" ||
           sample_name_short == "WJets" || sample_name_short == "DYJets" ||
           sample_name_short == "QCD_bEn" ||
           sample_name_short == "ttWToLNu" || sample_name_short == "ttWToQQ" ||
           sample_name_short == "ttZToLLNuNu" || sample_name_short == "ttZToQQ" || sample_name_short == "ttZToQQ")
  {
    float temp = weight_scale_variation_1;
    weight_scale_variation_1 = weight_scale_variation_3;
    weight_scale_variation_3 = temp;

    temp = weight_scale_variation_2;
    weight_scale_variation_2 = weight_scale_variation_6;
    weight_scale_variation_6 = temp;

    temp = weight_scale_variation_5;
    weight_scale_variation_5 = weight_scale_variation_7;
    weight_scale_variation_7 = temp;
  }
  else if (sample_name_short == "WW" || sample_name_short == "WZ" || sample_name_short == "ZZ" ||
           sample_name_short.Contains("CP5") || sample_name_short.Contains("mtop") || sample_name_short.Contains("hdamp"))
  {
    // irrelevant
    // do nothing
  }
  else
    cerr << "[Result_Event::Swap_Scale_Variation]: Unknown sample name: " << sample_name_short << endl;
    
  return;
} // void Result_Event_CR_DL::Swap_Scale_Variation(const TString &sample_name_short)

    //////////
