#ifndef __Result_Event_H__
#define __Result_Event_H__

#include <TString.h>
#include <TTree.h>
#include <TH1D.h>

#include "W_Event.h"

class Data_MC_Comparison;

class Result_Event : public W_Event
{
  friend class Data_MC_Comparison;
  friend class Template;

 public:
  Result_Event(const TString& a_era, const TString& a_channel, const TString& a_swap_mode);
  virtual ~Result_Event();

  void Setup_Tree(TTree* tree);
  
 protected:
  float sf_b_tag;
  float sf_c_tag;
  float sf_mu_id;
  float sf_mu_iso;
  float sf_mu_trig;
  float sf_pujet_veto;
  float weight;
  float weight_lumi;
  float weight_mc;
  float weight_pileup;
  float weight_prefire;
  float weight_scale_variation;
  float weight_top_pt;
  int n_vertex;
  float lepton_pt;
  float lepton_eta;
  int n_jets;
  int n_bjets;
  int n_cjets;
  float pt_leading_jet;
  float pt_subleading_jet;
  float eta_leading_jet;
  float eta_subleading_jet;
  float met_pt;
  float met_phi;
  float best_mva_score;
  float best_chi2;
  float mt;
  float mva_hf_score;
  float bvsc_had_t_b;
  float cvsb_had_t_b;
  float cvsl_had_t_b;
  float bvsc_lep_t_b;
  float cvsb_lep_t_b;
  float cvsl_lep_t_b;
  float pt_had_t_b;
  float pt_lep_t_b;
  
  //For MC
  int decay_mode;
  bool chk_reco_correct;
  bool chk_included;
  bool chk_hf_contamination;
  bool pu_conta_had_t_b;
  bool pu_conta_lep_t_b;
  
  ClassDef(Result_Event, 1);
};

#endif /* __Result_Event_H__  */
