#ifndef __Reco_Eval_H__
#define __Reco_Eval_H__

#include <iostream>
#include <map>

#include <TObject.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>
#include <TTree.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TLatex.h>

#include <Samples.h>
#include <Result_Event.h>
#include <Tagging_RF.h>
#include <Const_Def.h>

using namespace std;

class Reco_Eval : public TObject
{
  // class for evaluation of recostruction and signal study

public:
  Reco_Eval(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_swap_mode = "Permutation_MVA", const TString &a_draw_extension = "png");
  ~Reco_Eval();

  void Run();

  inline static bool Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

  typedef struct _Discriminating_Variables_Histo_Conf
  {
    TString discriminator;
    int n_bin;
    float x_low;
    float x_up;
  } DV_Histo_Conf;

protected:
  int reduction;

  Samples samples;
  Tagging_RF tagging_rf;

  TString era;
  TString channel;
  TString draw_extension;

  map<TString, TFile *> map_fin;
  map<TString, TTree *> map_tree;
  vector<TString> vec_short_name_mc;
  vector<TString> vec_histo_sample;

  int n_sample_merge_mc;
  int n_histo_sample;

  Result_Event event;

  int decay_modes[6] = {45, 21, 23, 41, 43, 999};

  TString fail_reason[4] = {"10",  // included && !HF contaminated
                            "00",  //! included && !HF contaminated
                            "11",  // included && HF contaminated
                            "01"}; //! included && HF contaminated
  int n_fail_reason;
  int n_histo_type;

  int color[5] = {2, 4, 5, 1, 6}; // this index should be index of fail reason + 1

  vector<DV_Histo_Conf> dv_histo_conf;
  int n_discriminators;

  float bvsc_wp_m;
  float cvsb_wp_m;
  float cvsl_wp_m;

  bool chk_swap;

  TFile *fout_tree;
  TTree *out_tree[5];
  // TTree *out_tree[5][5];

  TH1D ***histo_mva_pre;
  TH1D ***histo_mva;
  TH1D ***histo_cutflow;
  TH1D ***histo_had_w;
  TH1D ***histo_had_t;
  TH1D ***histo_lep_w;
  TH1D ***histo_lep_t;
  TH1D ***histo_template;

  TH2D ***histo_count;
  TH2D **histo_prob;

  TH1D **histo_swap;
  TH1D **histo_swap_fix;

  TH1D ****histo_discriminator;

  TGraphErrors *gr_significance;

  TLegend *legend;

  THStack ***stack_dv;

  void Calculate_Significance();
  void Calculate_Prob();
  void Draw_DV();
  void Draw_Raw();
  void Draw_Sample_By_Sample();
  void Draw_Significance();
  void Draw_Swap();
  void Fill_Cutflow(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason, const int &n_cut);
  void Fill_Histo(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason);
  void Fill_Histo_Count(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason);
  void Fill_Histo_Discriminators(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason);
  void Fill_Histo_Swap(const TString &short_name, const int &index_decay_mode);
  void Fill_Output_Tree(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason);
  int Get_Index(const TString &name);
  int Get_Index(const TString &short_name, const int &index_decay_mode);
  void Read_Tree();

  ClassDef(Reco_Eval, 1);
};

#endif /* __Reco_Eval_H__ */
