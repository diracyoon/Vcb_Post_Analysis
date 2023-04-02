#ifndef __Template_H__
#define __Template_H__

#include <iostream>
#include <vector>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <TMVA/Config.h>
#include <TMVA/Factory.h>
#include <TMVA/DataLoader.h>
#include <TH1D.h>

using namespace std;

class Template : public TObject
{
public:
  Template(const TString &a_era = "2018", const TString &a_channel = "Mu");
  ~Template();

  void Analyze_Grid();
  void Classification_BDT();
  void Classification_DNN();
  void Classification_Keras();
  void MultiClassification();
  void SetHyperParameter_DNN(const int &a_grid);

protected:
  TString era;
  TString channel;

  int reduction;
  int decay_modes[5] = {45, 21, 23, 41, 43};

  TTree *tree[5];

  TFile *fin;
  TFile *fout;

  // dnn hyperparameters

  int batch_size;
  int convergence_steps;
  int max_epochs;
  float learning_rate;
  float momentum;
  TString regularization;
  float weight_decay;

  vector<int> grid_batch_size;
  int n_grid_batch_size;

  vector<int> grid_max_epochs;
  int n_grid_max_epochs;

  vector<float> grid_learning_rate;
  int n_grid_learning_rate;

  vector<float> grid_momentum;
  int n_grid_momentum;

  // vector

  TMVA::Factory *factory;
  TMVA::DataLoader *data_loader;

  ClassDef(Template, 1);
};

#endif /* __Template_H__ */
