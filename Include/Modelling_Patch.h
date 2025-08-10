#ifndef __Modelling_Patch_H__
#define __Modelling_Patch_H__

#include <iostream>
#include <vector>
#include <map>

#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <TDirectory.h>
#include <TList.h>

#include <Samples.h>

using namespace std;

class Modelling_Patch : public TObject
{
public:
  Modelling_Patch(const TString &a_mode);
  ~Modelling_Patch();

  float Get_Modelling_Patch(const TString &sample, const TString &variation);

  inline static bool
  Comparing_TString(const TString &str1, const TString &str2)
  {
    if (str1.CompareTo(str2) > 0)
      return true;
    else
      return false;
  } // bool Comparing_TString(const TString &str1, const TString &str2)

protected:
  TString mode;

  vector<TString> vec_eras;

  Samples samples;
  int n_sample_mc;

  vector<TString> vec_short_name_mc;
  int n_sample_merge_mc;

  TH1D **histo_baseline;          // n_sample
  TH1D **histo_pdf_alternative;   // n_sample
  TH1D ***histo_pdf_error_set;    // n_sample, n_pdf_error_set
  TH1D **histo_pdf_as_down;       // n_sample
  TH1D **histo_pdf_as_up;         // n_sample
  TH1D **histo_top_pt_reweight;   // n_sample
  TH1D **histo_scale_variation_1; // n_sample
  TH1D **histo_scale_variation_2; // n_sample
  TH1D **histo_scale_variation_3; // n_sample
  TH1D **histo_scale_variation_4; // n_sample
  TH1D **histo_scale_variation_6; // n_sample
  TH1D **histo_scale_variation_8; // n_sample
  TH1D **histo_ps_0;              // n_sample
  TH1D **histo_ps_1;              // n_sample
  TH1D **histo_ps_2;              // n_sample
  TH1D **histo_ps_3;              // n_sample
  TH1D **histo_cp5_down;          // n_sample
  TH1D **histo_cp5_up;            // n_sample
  TH1D **histo_hdamp_down;        // n_sample
  TH1D **histo_hdamp_up;          // n_sample
  TH1D **histo_mtop_down;         // n_sample
  TH1D **histo_mtop_up;           // n_sample

  // too bulky to store single float
  // TH1D ***histo_ratio;             // n_sample, n_variations
  map<TString, map<TString, float>> patch_all; // sample, variation

  int Histo_Index(const TString &sample_name);
  void Init_Histo();
  void Ratio();
  void Read_Histo();
  void Read_Ratio();
  void Setup_Name(const TList *list, vector<TString> &vec_name);

  ClassDef(Modelling_Patch, 1);
};

#endif /* __Modelling_Patch_H__ */