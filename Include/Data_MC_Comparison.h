#ifndef __DATA_MC_COMPARISON_H__
#define __DATA_MC_COMPARISON_H__

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

#include <TString.h>
#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TPaveStats.h>

#include <Const_Def.h>
#include <Samples.h>
#include <Data_MC_Comparison_Event.h>

using namespace std;

class Data_MC_Comparison : public TObject
{
  //friend class W_Data;
  
 public:
  Data_MC_Comparison(const TString& a_era="2018", const TString& a_channel="Mu", const TString& a_swap_mode="PDF");
  ~Data_MC_Comparison();

    typedef struct _Histo_Conf
    {
      TString variable_title;
      int n_bin;
      float x_low;
      float x_up;
    } Histo_Conf;

  void Run();
  void Set_Region(const TString& a_region_type);
  
 protected:
  Samples samples;
  int n_sample_merge_mc;

  TString era;
  TString channel;
  TString data_short_name;
  TString region_type;

  map<TString, TFile*> map_fin_mc;
  map<TString, TFile*> map_fin_data;
  vector<TString> vec_short_name_mc;
  
  map<TString, TTree*> map_tree_mc;
  map<TString, TTree*> map_tree_data;

  Data_MC_Comparison_Event event;
  
  vector<Histo_Conf> histo_conf;
  int n_histo_conf;
  TH1D*** histo_mc;
  TH1D** histo_data;
  TH1D** histo_ratio;

  //cuts for selection
  float mva_score_cut;
  float pt_had_t_b_cut;
  float pt_lep_t_b_cut;

  bool Cut();
  void Draw();
  void Fill_Histo_Data();
  void Fill_Histo_MC(const int& sample_index);
  void Read_Tree();
  
  ClassDef(Data_MC_Comparison, 1);
};

#endif /* __DATA_MC_COMPARISON_H__ */
