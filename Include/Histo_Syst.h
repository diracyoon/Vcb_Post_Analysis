#ifndef __Histo_Syst_H__
#define __Histo_Syst_H__

#include <iostream>
#include <map>
#include <vector>

#include <TObject.h>
#include <TString.h>
#include <TTree.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <Const_Def.h>
#include <Samples.h>
#include <Result_Event.h>

using namespace std;

class Histo_Syst : public TObject
{
 public:
  Histo_Syst(const TString& a_era="2018", const TString& a_channel="Mu", const TString& a_swap_mode="Permutation_MVA");
  ~Histo_Syst();

  void Run();

  typedef struct _Histo_Conf
  {
    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
  } Histo_Conf;

 protected:
  Samples samples;
  int n_sample_merge_mc;
  
  TString era;
  TString channel;
  TString data_short_name;

  map<TString, TFile*> map_fin_mc;
  map<TString, TFile*> map_fin_data;
  vector<TString> vec_short_name_mc;

  map<TString, TTree*> map_tree_mc;
  map<TString, TTree*> map_tree_data;

  Result_Event event;

  int n_region;
  vector<TString> region_name;
  
  int n_syst;
  vector<TString> syst_name;
  
  int n_variable;
  vector<Histo_Conf> variable_conf;

  TH1D***** histo_mc;//n_region, n_syst, n_sample, n_variable
  TH1D*** histo_data;//n_region, n_variable

  TString region;

  TFile* fout;

  void Fill_Histo_Data(const int& region_index);
  void Fill_Histo_MC(const int& region_index, const int& sample_index);
  inline int Get_Region_Index(const TString& region);
  void Read_Tree();
  bool Set_Region();

  ClassDef(Histo_Syst, 1);
};

#endif /* __Histo_Syst_H__ */
