#ifndef __Template_H__
#define __Template_H__

#include <iostream>
#include <map>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <THStack.h>
#include <TMVA/Reader.h>
#include <TCanvas.h>

#include <Samples.h>
#include <Result_Event.h>
#include <Const_Def.h>

class Template : public TObject
{
 public:
  Template(const TString& a_era="2018", const TString& a_channel="Mu", const TString& a_swap_mode="Permutation_MVA", const TString& a_draw_extension="png");
  ~Template();
  
  void Run();

 protected:
  Samples samples;
  
  TString era;
  TString channel;
  TString draw_extension;
  
  float bvsc_wp_m;
  float cvsb_wp_m;
  float cvsl_wp_m;

  map<TString, TFile*> map_fin_mc;
  map<TString, TTree*> map_tree_mc;

  vector<TString> vec_short_name_mc;
  vector<TString> vec_histo_sample;
  int n_sample_merge_mc;
  int n_histo_sample;
  
  TH1D*** histo_template;
  THStack** stack_template;
  
  Result_Event event;

  TString fail_reason[4] = {"10",//included && !HF contaminated
                            "00",//!included && !HF contaminated
                            "11",//included && HF contaminated
                            "01"};//!included && HF contaminated
  int n_fail_reason;
  int n_histo_type;

  int color[5] = {2, 4, 5, 1, 6};//this index should be index of fail reason + 1

  const int n_bin_histo_score;

  TMVA::Reader* reader;
  float mva_score;

  TCanvas* canvas[2];
  
  void Draw();
  void Fill_Histo(const TString& short_name, const int& index_decay_mode, const int& index_fail_reason);
  int Get_Index(const TString& name);
  int Get_Index(const TString& short_name, const int& index_decay_mode);
  void Read_Tree();

  ClassDef(Template, 1);
};

#endif /* __Template_H__ */
