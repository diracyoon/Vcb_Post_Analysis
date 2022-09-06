#ifndef __Reco_Eval_Event_H__
#define __Reco_Eval_Event_H__

#include <TString.h>
#include <TTree.h>
#include <TH1D.h>

#include "W_Event.h"

class Data_MC_Comparison;

class Reco_Eval_Event : public W_Event
{
  friend class Reco_Eval;
 public:
  Reco_Eval_Event(const TString& a_era, const TString& a_channel, const TString& a_swap_mode);
  virtual ~Reco_Eval_Event();

  void Setup_Tree(TTree* tree);
  
 protected:
  float weight;
  int n_jets;
  int n_bjets;
  int n_cjets;
  int decay_mode;
  
  float best_mva_score_pre;
  float best_mva_score;
  bool chk_included;
  bool chk_hf_contamination;

  float bvsc_had_t_b;
  float cvsb_had_t_b;
  float cvsl_had_t_b;
  
  float bvsc_lep_t_b;
  float cvsb_lep_t_b;
  float cvsl_lep_t_b;

  float pt_had_t_b;
  float pt_lep_t_b;

  bool pu_conta_had_t_b;
  bool pu_conta_lep_t_b;
  
  ClassDef(Reco_Eval_Event, 1);
};

#endif /* __Reco_Eval_Event_H__  */
