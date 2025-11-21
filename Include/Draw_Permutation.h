#ifndef __DRAW_PERMUTATION_H__
#define __DRAW_PERMUTATION_H__

#include <iostream>

#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>

using namespace std;

class Draw_Permutation : public TObject
{
public:
  Draw_Permutation(const TString &a_era = "2017", const TString &a_channel = "Mu");
  virtual ~Draw_Permutation();

  class Histo_Conf
  {
  public:
    Histo_Conf() {}

    Histo_Conf(TString a_variable_title, int a_n_bin, float a_x_low, float a_x_up) : variable_title(a_variable_title), n_bin(a_n_bin), x_low(a_x_low), x_up(a_x_up)
    {
      chk_equal_interval = true;

      for (int i = 0; i < n_bin; i++)
        vec_bin.push_back(i * (x_up - x_low) / n_bin + x_low);
      vec_bin.push_back(x_up);
    }

    Histo_Conf(TString a_variable_title) : variable_title(a_variable_title)
    {
      chk_equal_interval = false;
    }

    ~Histo_Conf() {}

    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
    bool chk_equal_interval;
    vector<float> vec_bin;
  };

private:
  TString era;
  TString channel;

  TFile *fin;

  TTree *tree_correct;
  TTree *tree_wrong;

  TH1D ****histo; // n_jet (4, 5, 6>=), 2 (correct, wrong), n_variable

  int era_index;
  int n_jets;

  float bvsc_had_t_b;
  float bvsc_w_u;
  float bvsc_w_d;
  float bvsc_lep_t_b;

  float had_t_mass;
  float had_w_mass;
  float lep_t_mass;
  float lep_t_partial_mass;

  float pt_had_t_b;
  float pt_w_u;
  float pt_w_d;
  float pt_lep_t_b;

  float pt_had_w;
  float pt_had_t;
  float pt_lep_w;
  float pt_lep_t;
  float pt_tt;

  float del_phi_had_t_lep_t;

  float neutrino_p;

  float theta_w_u_w_d;
  float theta_had_w_had_t_b;
  float theta_lep_neu;
  float theta_lep_w_lep_t_b;

  void Config_Variable();
  void Read_Tree();
  void Run();
  void Set_Tree(TTree *tree);
  void Setup_Histo();

  ClassDef(Draw_Permutation, 1);
};

#endif /* __DRAW_PERMUTATION_H__ */