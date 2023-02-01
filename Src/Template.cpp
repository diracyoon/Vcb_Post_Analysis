#include "Template.h"

ClassImp(Template);

//////////

Template::Template(const TString &a_era, const TString &a_channel)
{
  cout << "[Template::Template]: Init analysis" << endl;

  era = a_era;
  channel = a_channel;

  reduction = 1;

  TMVA::gConfig().GetVariablePlotting().fNbins1D = 200;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  TString path_fin = path_base + "/Macro/Reco_Eval/Vcb_" + channel + "_Reco_Tree.root";

  fin = new TFile(path_fin);
  for (int i = 0; i < 5; i++)
  {
    TString key = "Reco_" + to_string(decay_modes[i]);
    tree[i] = (TTree *)fin->Get(key);
  }

  cout << tree[0] << endl;
  cout << tree[4] << endl;

  TString fout_name = "Vcb_" + channel + "_Template_MVA.root";
  fout = new TFile(fout_name, "RECREATE");

  factory = new TMVA::Factory("TMVAClassification", fout,
                              "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");

  data_loader = new TMVA::DataLoader("dataset");
  data_loader->AddVariable("pt_had_t_b", "pt_had_t_b", "units", 'F');
  data_loader->AddVariable("pt_w_u", "pt_w_u", "units", 'F');
  data_loader->AddVariable("pt_w_d", "pt_w_d", "units", 'F');
  data_loader->AddVariable("pt_lep_t_b", "pt_lep_t_b", "units", 'F');
  data_loader->AddVariable("eta_had_t_b", "eta_had_t_b", "units", 'F');
  data_loader->AddVariable("eta_w_u", "eta_w_u", "units", 'F');
  data_loader->AddVariable("eta_w_d", "eta_w_d", "units", 'F');
  data_loader->AddVariable("eta_lep_t_b", "eta_lep_t_b", "units", 'F');
  data_loader->AddVariable("best_mva_score", "best_mva_score", "units", 'F');
  data_loader->AddVariable("n_jets", "n_jets", "units", 'I');
  data_loader->AddVariable("n_bjets", "n_bjets", "units", 'I');
  data_loader->AddVariable("n_cjets", "n_cjets", "units", 'I');
  data_loader->AddVariable("bvsc_w_u", "bvsc_w_u", "units", 'F');
  data_loader->AddVariable("cvsb_w_u", "cvsb_w_u", "units", 'F');
  data_loader->AddVariable("cvsl_w_u", "cvsl_w_u", "units", 'F');
  data_loader->AddVariable("bvsc_w_d", "bvsc_w_d", "units", 'F');
  data_loader->AddVariable("cvsb_w_d", "cvsb_w_d", "units", 'F');
  data_loader->AddVariable("cvsl_w_d", "cvsl_w_d", "units", 'F');

  data_loader->AddSignalTree(tree[0], 1.0);
  data_loader->AddBackgroundTree(tree[4], 1.0);

  TCut cut_s = "";
  TCut cut_b = "";

  Long64_t n_train_signal = tree[0]->GetEntries() / 2;
  Long64_t n_train_back = n_train_signal; // tree[0]->GetEntries()/2;

  data_loader->PrepareTrainingAndTestTree(cut_s, cut_b,
                                          "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:V");

  // Adaptive Boost
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDT",
                      "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=200");

  // Gradient Boost
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTG",
                      "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=200:MaxDepth=2");

  // Bagging
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTB",
                      "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=200");

  // Decorrelation + Adaptive Boost
  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTD",
                      "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=200:VarTransform=Decorrelate");

  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
  //  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTF",
  //                   "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20");

  // DNN
  TString layoutString("Layout=TANH|128,TANH|128,TANH|128,LINEAR");

  TString trainingStrategyString = ("TrainingStrategy=LearningRate=1e-2,Momentum=0.9,"
                                    "ConvergenceSteps=20,BatchSize=100,TestRepetitions=1,"
                                    "WeightDecay=1e-4,Regularization=None,"
                                    "DropConfig=0.0+0.5+0.5+0.5");

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
} // Template::Template()

//////////

Template::~Template()
{
  fout->Close();

  delete factory;
  delete data_loader;
} // Template::~Template()

//////////
