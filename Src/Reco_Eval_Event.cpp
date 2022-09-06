#include "Reco_Eval_Event.h"

ClassImp(Reco_Eval_Event);

//////////

Reco_Eval_Event::Reco_Eval_Event(const TString& a_era, const TString& a_channel, const TString& a_swap_mode) : W_Event(a_era, a_channel, a_swap_mode)
{
}//Reco_Eval_Event::Reco_Eval_Event(const TString& a_swap_mode)

//////////

Reco_Eval_Event::~Reco_Eval_Event()
{
}//Reco_Eval_Event::~Reco_Eval_Event()

//////////

void Reco_Eval_Event::Setup_Tree(TTree* tree)
{
  tree->SetBranchAddress("weight", &weight);
  tree->SetBranchAddress("n_jets", &n_jets);
  tree->SetBranchAddress("n_bjets", &n_bjets);
  tree->SetBranchAddress("n_cjets", &n_cjets);
  tree->SetBranchAddress("decay_mode", &decay_mode);
  tree->SetBranchAddress("best_mva_score_pre", &best_mva_score_pre);
  tree->SetBranchAddress("best_mva_score", &best_mva_score);
  tree->SetBranchAddress("chk_included", &chk_included);
  tree->SetBranchAddress("chk_hf_contamination", &chk_hf_contamination);  
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
  tree->SetBranchAddress("m_w_u", &m_w_u);
  tree->SetBranchAddress("m_w_d", &m_w_d);
  tree->SetBranchAddress("swapped_mva", &swapped_mva);
  tree->SetBranchAddress("pu_conta_had_t_b", &pu_conta_had_t_b);
  tree->SetBranchAddress("pu_conta_w_u", &pu_conta_w_u);
  tree->SetBranchAddress("pu_conta_w_d", &pu_conta_w_d);
  tree->SetBranchAddress("pu_conta_lep_t_b", & pu_conta_lep_t_b);
  tree->SetBranchAddress("swapped_truth", &swapped_truth);
  
  return;
}//void Reco_Eval_Event::Setup_Tree(TTree* tree)

//////////
