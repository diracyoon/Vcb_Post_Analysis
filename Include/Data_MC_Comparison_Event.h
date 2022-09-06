#ifndef __Data_MC_Comparison_Event_H__
#define __Data_MC_Comparison_Event_H__

#include <TString.h>
#include <TTree.h>
#include <TH1D.h>

#include "W_Event.h"

class Data_MC_Comparison;

class Data_MC_Comparison_Event : public W_Event
{
  friend class Data_MC_Comparison;
 public:
  Data_MC_Comparison_Event(const TString& a_era, const TString& a_channel, const TString& a_swap_mode);
  virtual ~Data_MC_Comparison_Event();

  void Setup_Tree(TTree* tree);
  
 protected:
  float weight;
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
  
  ClassDef(Data_MC_Comparison_Event, 1);
};

#endif /* __Data_MC_Comparison_Event_H__  */
