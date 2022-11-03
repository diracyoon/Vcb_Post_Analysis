#ifndef __DATA_MC_COMPARISON_H__
#define __DATA_MC_COMPARISON_H__

#include <iostream>
#include <vector>

#include <TString.h>
#include <TFile.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TList.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>

using namespace std;

class Data_MC_Comparison : public TObject
{
public:
  Data_MC_Comparison(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_extension = "png");
  ~Data_MC_Comparison();

  void Run();

  typedef struct _Histo_Conf
  {
    TString variable_title;
    int n_bin;
    float x_low;
    float x_up;
  } Histo_Conf;

protected:
  TString era;
  TString channel;
  TString extension;

  int n_region;
  vector<TString> region_name;

  int n_syst;
  vector<TString> syst_name;

  int n_sample;
  vector<TString> sample_name;

  int n_variable;
  vector<TString> variable_name;

  vector<Histo_Conf> variable_conf;

  int color[7] = {2, 3, 4, 5, 6, 7, 8}; // n_sample

  TH1D *****histo_mc;   // n_region, n_syst, n_sample, n_variable
  THStack ****stack_mc; // n_region, n_syst, n_variable

  int n_pdf_error_set;
  TH1D *****histo_pdf_error_set;   // n_region, n_syst, n_sample, n_variable
  THStack ****stack_pdf_error_set; // n_region, n_pdf_error_set, n_variable
  // TH1D ****histo_merged_pdf_error_set; // n_region, n_sample, n_variable

  TH1D ***histo_data; // n_region, n_variable

  TH1D ***histo_ratio; // n_region, n_variable
  TGraphAsymmErrors ***gr_ratio;//n_region, n_variable

  TGraphAsymmErrors ****gr_variation;       // n_region, n_syst-1, n_variable
  TGraphAsymmErrors ***gr_variation_merged; // n_region, n_variable
  TList ***list_variation;                  // n_region, n_variable

  TCanvas ***canvas; // n_region, n_variable
  TPad ****pad;      // n_region, n_variable, 2

  TFile *fin;
  TFile *fout;

  void Compare();
  void Draw();
  void Envelope();
  void Merge_PDF_Error_Set();
  void Save();
  int Setup_Name(const TList *list, vector<TString> &vec_name, const bool &chk_excluding_pdf_error_set = false);
  void Stack_MC();

  ClassDef(Data_MC_Comparison, 1);
};

#endif /* __DATA_MC_COMPARISON_H__ */
