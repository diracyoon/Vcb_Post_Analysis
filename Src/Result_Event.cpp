#include "Result_Event.h"

ClassImp(Result_Event);

//////////

Result_Event::Result_Event(const TString& a_era, const TString& a_channel, const TString& a_swap_mode) : W_Event(a_era, a_channel, a_swap_mode)
{
}//Result_Event::Result_Event(const TString& a_swap_mode)

//////////

Result_Event::~Result_Event()
{
}//Result_Event::~Result_Event()

//////////

void Result_Event::Setup_Tree(TTree* tree)
{
  tree->SetBranchAddress("sf_b_tag", &sf_b_tag);
  tree->SetBranchAddress("sf_c_tag", &sf_c_tag);
  tree->SetBranchAddress("sf_mu_id", &sf_mu_id);
  tree->SetBranchAddress("sf_mu_iso", &sf_mu_iso);
  tree->SetBranchAddress("sf_mu_trig", &sf_mu_trig);
  tree->SetBranchAddress("sf_pujet_veto", &sf_pujet_veto);
  tree->SetBranchAddress("weight", &weight);
  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);
  tree->SetBranchAddress("weight_pileup", &weight_pileup);
  tree->SetBranchAddress("weight_prefire", &weight_prefire);
  tree->SetBranchAddress("weight_scale_variation", &weight_scale_variation);
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
  tree->SetBranchAddress("met_pt", &met_pt);
  tree->SetBranchAddress("met_phi", &met_phi);
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
}//void Result_Event::Setup_Tree(TTree* tree)

//////////
