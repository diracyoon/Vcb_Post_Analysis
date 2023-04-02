#include "Template.h"

ClassImp(Template);

//////////

Template::Template(const TString &a_era, const TString &a_channel)
{
  ROOT::EnableImplicitMT(5);

  cout << "[Template::Template]: Init analysis" << endl;

  era = a_era;
  channel = a_channel;

  reduction = 1;

  TMVA::gConfig().GetVariablePlotting().fNbins1D = 200;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  TString path_fin = path_base + "/Macro/Reco_Eval/Vcb_" + era + "_" + channel + "_Reco_Tree.root";

  fin = new TFile(path_fin);
  for (int i = 0; i < 5; i++)
  {
    TString key = "Reco_" + to_string(decay_modes[i]);
    tree[i] = (TTree *)fin->Get(key);
  }

  grid_learning_rate = {1e-4, 2e-4, 4e-4, 8e-4, 1e-3};
  n_grid_learning_rate = grid_learning_rate.size();

  grid_momentum = {0.0, 0.3, 0.6, 0.9};
  n_grid_momentum = grid_momentum.size();

  grid_batch_size = {100, 200, 400, 800};
  n_grid_batch_size = grid_batch_size.size();

  // default hyperparameters for DNN
  learning_rate = 1e-2;
  momentum = 0.9;
  convergence_steps = 20;
  batch_size = 100;
  weight_decay = 1e-4;
  regularization = "None";

  TString fout_name = "Vcb_" + channel + "_Template_MVA.root";
  // fout = new TFile(fout_name, "RECREATE");
}

//////////

Template::~Template()
{
  // fout->Close();

  // delete factory;
  // delete data_loader;
} // Template::~Template()

//////////

void Template::Analyze_Grid()
{
  cout << "[Template::Analyze_Grid]: Init" << endl;

  for (int i = 0; i < n_grid_learning_rate; i++)
  {
    for (int j = 0; j < n_grid_momentum; j++)
    {
      for (int k = 0; k < n_grid_batch_size; k++)
      {
        TFile *fin = new TFile(Form("Vcb_Mu_Template_MVA_Grid_HyperParameter_%d%d%d.root", i, j, k));

        TH1D *histo_roc = (TH1D *)fin->Get("dataset/Method_DL/DNN_CPU/MVA_DNN_CPU_rejBvsS");
        float auc = histo_roc->Integral();
        cout << i << j << k << ": " << auc << endl;

        fin->Close();
      } // loop over grid_batch_size
    }   // loop over grid_momentum
  }     // loop over grid_learning_rate

  return;
} // void Template::Analyze_Grid()

//////////

void Template::Classification_BDT()
{
  factory = new TMVA::Factory("TMVAClassification", fout,
                              "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification");

  data_loader = new TMVA::DataLoader("dataset");
  // data_loader->AddVariable("pt_had_t_b", "pt_had_t_b", "units", 'F');
  // data_loader->AddVariable("pt_w_u", "pt_w_u", "units", 'F');
  // data_loader->AddVariable("pt_w_d", "pt_w_d", "units", 'F');
  // data_loader->AddVariable("pt_lep_t_b", "pt_lep_t_b", "units", 'F');
  // data_loader->AddVariable("eta_had_t_b", "eta_had_t_b", "units", 'F');
  // data_loader->AddVariable("eta_w_u", "eta_w_u", "units", 'F');
  // data_loader->AddVariable("eta_w_d", "eta_w_d", "units", 'F');
  // data_loader->AddVariable("eta_lep_t_b", "eta_lep_t_b", "units", 'F');
  data_loader->AddVariable("best_mva_score", "best_mva_score", "units", 'F');
  // data_loader->AddVariable("n_jets", "n_jets", "units", 'I');
  data_loader->AddVariable("n_bjets", "n_bjets", "units", 'I');
  data_loader->AddVariable("n_cjets", "n_cjets", "units", 'I');
  data_loader->AddVariable("bvsc_w_u", "bvsc_w_u", "units", 'F');
  data_loader->AddVariable("cvsb_w_u", "cvsb_w_u", "units", 'F');
  data_loader->AddVariable("cvsl_w_u", "cvsl_w_u", "units", 'F');
  data_loader->AddVariable("bvsc_w_d", "bvsc_w_d", "units", 'F');
  data_loader->AddVariable("cvsb_w_d", "cvsb_w_d", "units", 'F');
  data_loader->AddVariable("cvsl_w_d", "cvsl_w_d", "units", 'F');

  data_loader->AddSignalTree(tree[0], 1.0);
  data_loader->AddBackgroundTree(tree[1], 1.0);
  data_loader->AddBackgroundTree(tree[2], 1.0);
  data_loader->AddBackgroundTree(tree[3], 1.0);
  data_loader->AddBackgroundTree(tree[4], 1.0);

  TCut cut_s = "";
  TCut cut_b = "";

  Long64_t n_train_signal = tree[0]->GetEntries() / 2 / reduction;
  Long64_t n_train_back = tree[1]->GetEntries() / 2 / reduction;
  n_train_back += tree[2]->GetEntries() / 2 / reduction;
  n_train_back += tree[3]->GetEntries() / 2 / reduction;
  n_train_back += tree[4]->GetEntries() / 2 / reduction;

  data_loader->PrepareTrainingAndTestTree(cut_s, cut_b,
                                          Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:NormMode=NumEvents:V", n_train_signal, n_train_back, n_train_signal, n_train_back));

  // Adaptive Boost
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDT",
                      "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=200");

  // Gradient Boost
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTG",
                      "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=200:MaxDepth=2");

  // Bagging
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTB",
                      "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=200");

  // // Decorrelation + Adaptive Boost
  // factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTD",
  //                     "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=200:VarTransform=Decorrelate");

  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTF",
                      "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=200");

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();

  // fout->Close();

  return;
} // Template::Classification_BDT()

//////////

void Template::Classification_DNN()
{
  // ROOT::EnableThreadSafety();

  factory = new TMVA::Factory("TMVAClassification", fout,
                              "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification");

  data_loader = new TMVA::DataLoader("dataset");
  // data_loader->AddVariable("pt_had_t_b", "pt_had_t_b", "units", 'F');
  // data_loader->AddVariable("pt_w_u", "pt_w_u", "units", 'F');
  // data_loader->AddVariable("pt_w_d", "pt_w_d", "units", 'F');
  // data_loader->AddVariable("pt_lep_t_b", "pt_lep_t_b", "units", 'F');
  // data_loader->AddVariable("eta_had_t_b", "eta_had_t_b", "units", 'F');
  // data_loader->AddVariable("eta_w_u", "eta_w_u", "units", 'F');
  // data_loader->AddVariable("eta_w_d", "eta_w_d", "units", 'F');
  // data_loader->AddVariable("eta_lep_t_b", "eta_lep_t_b", "units", 'F');
  data_loader->AddVariable("best_mva_score", "best_mva_score", "units", 'F');
  // data_loader->AddVariable("n_jets", "n_jets", "units", 'I');
  data_loader->AddVariable("n_bjets", "n_bjets", "units", 'I');
  data_loader->AddVariable("n_cjets", "n_cjets", "units", 'I');
  data_loader->AddVariable("bvsc_w_u", "bvsc_w_u", "units", 'F');
  data_loader->AddVariable("cvsb_w_u", "cvsb_w_u", "units", 'F');
  data_loader->AddVariable("cvsl_w_u", "cvsl_w_u", "units", 'F');
  data_loader->AddVariable("bvsc_w_d", "bvsc_w_d", "units", 'F');
  data_loader->AddVariable("cvsb_w_d", "cvsb_w_d", "units", 'F');
  data_loader->AddVariable("cvsl_w_d", "cvsl_w_d", "units", 'F');

  data_loader->AddSignalTree(tree[0]);
  data_loader->AddBackgroundTree(tree[1]);
  data_loader->AddBackgroundTree(tree[2]);
  data_loader->AddBackgroundTree(tree[3]);
  data_loader->AddBackgroundTree(tree[4]);

  data_loader->SetSignalWeightExpression("weight");
  data_loader->SetBackgroundWeightExpression("weight");

  TCut cut_s = "";
  TCut cut_b = "";

  Long64_t n_train_signal = tree[0]->GetEntries() / 2 / reduction;
  Long64_t n_train_back = tree[1]->GetEntries() / 2 / reduction;
  n_train_back += tree[2]->GetEntries() / 2 / reduction;
  n_train_back += tree[3]->GetEntries() / 2 / reduction;
  n_train_back += tree[4]->GetEntries() / 2 / reduction;

  data_loader->PrepareTrainingAndTestTree(cut_s, cut_b,
                                          Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:NormMode=NumEvents:V", n_train_signal, n_train_back, n_train_signal, n_train_back));

  // DNN
  TString layoutString("Layout=RELU|128,RELU|64,RELU|32,LINEAR");

  TString trainingStrategyString = "TrainingStrategy=";
  trainingStrategyString.Append(Form("LearningRate=%f,", learning_rate));
  trainingStrategyString.Append(Form("Momentum=%f,", momentum));
  trainingStrategyString.Append(Form("ConvergenceSteps=%d,", convergence_steps));
  trainingStrategyString.Append(Form("BatchSize=%d,", batch_size));
  trainingStrategyString.Append("TestRepetitions=2,");
  trainingStrategyString.Append(Form("WeightDecay=%f,", weight_decay));
  trainingStrategyString.Append("Regularization=" + regularization + ",");
  trainingStrategyString.Append("DropConfig=0.0+0.5+0.5+0.5");

  TString dnnOptions("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                     "WeightInitialization=XAVIERUNIFORM");
  dnnOptions.Append(":");
  dnnOptions.Append(layoutString);
  dnnOptions.Append(":");
  dnnOptions.Append(trainingStrategyString);

  TString cpuOptions = dnnOptions + ":Architecture=CPU";
  factory->BookMethod(data_loader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();

  fout->Close();

  return;
} // void Template::Classificatation_DNN()

//////////

void Template::MultiClassification()
{
  return;
} // void Template::MultiClassification()

//////////

void Template::SetHyperParameter_DNN(const int &a_grid)
{
  int grid = a_grid;

  float grid_learning_rate[] = {1e-4, 1e-3, 2e-3, 4e-3};
  int n_grid_learning_rate = sizeof(grid_learning_rate) / sizeof(float);

  float grid_momentum[] = {0.0, 0.3, 0.6, 0.9};
  int n_grid_momentum = sizeof(grid_momentum) / sizeof(float);

  int grid_batch_size[] = {100, 200, 400, 800};
  int n_grid_batch_size = sizeof(grid_batch_size) / sizeof(int);

  if (grid < 0)
    return;

  if (n_grid_learning_rate * n_grid_momentum * n_grid_batch_size < grid)
  {
    cout << "[Template::SetHyperParameter_DNN]: out of index" << endl;
    return;
  }

  cout << grid << endl;
  // cout << n_grid_learning_rate * n_grid_momentum * n_grid_batch_size << endl;

  int index_learning_rate = grid / (n_grid_momentum * n_grid_batch_size);

  grid = grid - index_learning_rate * n_grid_momentum * n_grid_batch_size;

  int index_momentum = grid / n_grid_batch_size;
  int index_batch_size = grid % n_grid_batch_size;

  learning_rate = grid_learning_rate[index_learning_rate];
  momentum = grid_momentum[index_momentum];
  batch_size = grid_batch_size[index_batch_size];

  cout << "Hyper Parameter set" << endl;
  cout << "Learning Rate = " << learning_rate << endl;
  cout << "Momenmum = " << momentum << endl;
  cout << "Batch Size = " << batch_size << endl;
  cout << "Weight Decay = " << weight_decay << endl;
  cout << "Regularization = " << regularization << endl;
  cout << endl;
  cout << endl;

  TString fout_name = "Vcb_" + channel + "_Template_MVA_Grid_HyperParameter_" + to_string(index_learning_rate) + to_string(index_momentum) + to_string(index_batch_size) + ".root";

  // fout->Close();
  fout = new TFile(fout_name, "RECREATE");

  return;
} // void Template::SetHyperParameter_DNN()

//////////
