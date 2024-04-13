#include "Permutation_TMVA.h"

ClassImp(Permutation_TMVA);

//////////

Permutation_TMVA::Permutation_TMVA(const TString &a_era, const TString &a_channel, const int &a_n_jet, const bool &a_chk_prekin_cut, const bool &a_chk_permutation_pre)
{
  ROOT::EnableImplicitMT(20);

  cout << "[Permutation_TMVA::Permutation_TMVA]: Init analysis" << endl;

  reduction = 1;

  chk_bvsc_only = true;

  era = a_era;
  channel = a_channel;
  n_jet = a_n_jet;
  chk_prekin_cut = a_chk_prekin_cut;
  chk_permutation_pre = a_chk_permutation_pre;

  cout << "Era = " << era << ", Channel = " << channel << ", N_Jets = " << n_jet << ", Chk_PreKin_Cut = " << chk_prekin_cut << ", Chk_Permutation_Pre = " << chk_permutation_pre << endl;

  TMVA::gConfig().GetVariablePlotting().fNbins1D = 200;
  TMVA::gConfig().GetVariablePlotting().fMaxNumOfAllowedVariablesForScatterPlots = 1;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/El/RunPermutationTree/";

  fin_el = new TFile(path_base + "Vcb_TTLJ_WtoCB_powheg.root");
  tree_correct_el = (TTree *)fin_el->Get("Permutation_Correct");
  tree_wrong_el = (TTree *)fin_el->Get("Permutation_Wrong");

  path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/Mu/RunPermutationTree/";

  fin_mu = new TFile(path_base + "Vcb_TTLJ_WtoCB_powheg.root");
  tree_correct_mu = (TTree *)fin_mu->Get("Permutation_Correct");
  tree_wrong_mu = (TTree *)fin_mu->Get("Permutation_Wrong");

  TString fout_name;
  if (chk_prekin_cut)
  {
    if (chk_permutation_pre)
      fout_name = Form("Vcb_PreKin_Cut_Pre_TTLJ_WtoCB_%dJets.root", n_jet);
    else
      fout_name = Form("Vcb_PreKin_Cut_TTLJ_WtoCB_%dJets.root", n_jet);
  }
  else
  {
    if (chk_permutation_pre)
      fout_name = Form("Vcb_Permutation_Pre_TTLJ_WtoCB_%dJets.root", n_jet);
    else
      fout_name = Form("Vcb_Permutation_TTLJ_WtoCB_%dJets.root", n_jet);
  }

  fout = TFile::Open(fout_name, "RECREATE");

  factory = new TMVA::Factory("TMVAClassification", fout,
                              "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");
  //factory->SetOptions("SaveState=kFALSE");

  data_loader = new TMVA::DataLoader("dataset");
  // data_loader->AddVariable( "weight",       "weight",       "units", 'F');
  // data_loader->AddVariable("n_jets", "n_jets", "units", 'I');
  data_loader->AddSpectator("n_jets", "n_jets", "", 'I');

  // data_loader->AddVariable("n_bjets",       "n_bjets",       "units", 'I');
  // data_loader->AddVariable("n_cjets",       "n_cjets",       "units", 'I');

  // data_loader->AddVariable("lepton_pt", "lepton_pt", 'F');
  data_loader->AddVariable("met_pt", "met_pt", "GeV", 'F');
  data_loader->AddVariable("neutrino_p", "neutrino_p", "GeV", 'F');
  //data_loader->AddVariable("lepton_pt", "lepton_pt", "GeV", 'F');
  //data_loader->AddVariable("pt_ratio", "pt_ratio", "", 'F');

  data_loader->AddVariable("pt_had_t_b", "pt_had_t_b", "GeV", 'F');
  data_loader->AddVariable("pt_w_u", "pt_w_u", "GeV", 'F');
  data_loader->AddVariable("pt_w_d", "pt_w_d", "GeV", 'F');
  data_loader->AddVariable("pt_lep_t_b", "pt_lep_t_b", "GeV", 'F');

  // data_loader->AddVariable("eta_had_t_b", "eta_had_t_b", "units", 'F');
  // data_loader->AddVariable("eta_w_u", "eta_w_u", "units", 'F');
  // data_loader->AddVariable("eta_w_d", "eta_w_d", "units", 'F');
  // data_loader->AddVariable("eta_lep_t_b", "eta_lep_t_b", "units", 'F');

  data_loader->AddVariable("bvsc_had_t_b", "bvsc_had_t_b", "", 'F');
  if(!chk_bvsc_only)
    {
      data_loader->AddVariable("cvsb_had_t_b", "cvsb_had_t_b", "", 'F');
      data_loader->AddVariable("cvsl_had_t_b", "cvsl_had_t_b", "", 'F');
    }

  if (!chk_permutation_pre)
  {
    data_loader->AddVariable("bvsc_w_u", "bvsc_w_u", "", 'F');
    if(!chk_bvsc_only)
      {
	data_loader->AddVariable("cvsb_w_u", "cvsb_w_u", "", 'F');
	data_loader->AddVariable("cvsl_w_u", "cvsl_w_u", "", 'F');
      }

    data_loader->AddVariable("bvsc_w_d", "bvsc_w_d", "", 'F');
    if(!chk_bvsc_only)
      {
	data_loader->AddVariable("cvsb_w_d", "cvsb_w_d", "", 'F');
	data_loader->AddVariable("cvsl_w_d", "cvsl_w_d", "", 'F');
      }
  }

  data_loader->AddVariable("bvsc_lep_t_b", "bvsc_lep_t_b", "", 'F');
  if(!chk_bvsc_only)
    {
      data_loader->AddVariable("cvsb_lep_t_b", "cvsb_lep_t_b", "", 'F');
      data_loader->AddVariable("cvsl_lep_t_b", "cvsl_lep_t_b", "", 'F');
    }

  // data_loader->AddVariable("del_phi_w_u_w_d", "del_phi_w_u_w_d", "units", 'F');
  // data_loader->AddVariable("del_eta_w_u_w_d", "del_eta_w_u_w_d", "units", 'F');
  // data_loader->AddVariable("del_r_w_u_w_d", "del_r_w_u_w_d", "units", 'F');
  data_loader->AddVariable("theta_w_u_w_d", "theta_w_u_w_d", "radian", 'F');

  // data_loader->AddVariable("del_phi_had_w_had_t_b", "del_phi_had_w_had_t_b", "units", 'F');
  // data_loader->AddVariable("del_eta_had_w_had_t_b", "del_eta_had_w_had_t_b", "units", 'F');
  // data_loader->AddVariable("del_r_had_w_had_t_b", "del_r_had_w_had_t_b", "units", 'F');
  data_loader->AddVariable("theta_had_w_had_t_b", "theta_had_w_had_t_b", "radian", 'F');

  // data_loader->AddVariable("del_phi_lep_neu", "del_phi_lep_neu", "units", 'F');
  // data_loader->AddVariable("del_eta_lep_neu", "del_eta_lep_neu", "units", 'F');
  // data_loader->AddVariable("del_r_lep_neu", "del_r_lep_neu", "units", 'F');
  data_loader->AddVariable("theta_lep_neu", "theta_lep_neu", "radian", 'F');

  // data_loader->AddVariable("del_phi_lep_w_lep_t_b", "del_phi_lep_w_lep_t_b", "units", 'F');
  // data_loader->AddVariable("del_eta_lep_w_lep_t_b", "del_eta_lep_w_lep_t_b", "units", 'F');
  // data_loader->AddVariable("del_r_lep_w_lep_t_b", "del_r_lep_w_lep_t_b", "units", 'F');
  data_loader->AddVariable("theta_lep_w_lep_t_b", "theta_lep_w_lep_t_b", "radian", 'F');

  data_loader->AddVariable("del_phi_had_t_lep_t", "del_phi_had_t_lep_t", "radian", 'F');
  // data_loader->AddVariable("del_eta_had_t_lep_t", "del_eta_had_t_lep_t", "units", 'F');
  // data_loader->AddVariable("del_r_had_t_lep_t", "del_r_had_t_lep_t", "units", 'F');
  // data_loader->AddVariable("theta_had_t_lep_t", "theta_had_t_lep_t", "units", 'F');

  data_loader->AddVariable("had_t_mass", "had_t_mass", "GeV", 'F');
  data_loader->AddVariable("had_w_mass", "had_w_mass", "GeV", 'F');
  data_loader->AddVariable("lep_t_mass", "lep_t_mass", "GeV", 'F');
  data_loader->AddVariable("lep_t_partial_mass", "lep_t_partial_mass", "GeV", 'F');

  if (!chk_prekin_cut)
  {
    data_loader->AddVariable("chi2_jet_had_t_b", "chi2_jet_had_t_b", "", 'F');
    data_loader->AddVariable("chi2_jet_w_u", "chi2_jet_w_u", "", 'F');
    data_loader->AddVariable("chi2_jet_w_d", "chi2_jet_w_d", "", 'F');
    data_loader->AddVariable("chi2_jet_lep_t_b", "chi2_jet_lep_t_b", "", 'F');

    if (n_jet == 4)
      data_loader->AddSpectator("chi2_jet_extra", "chi2_jet_extra", "", 'F');
    else
      data_loader->AddVariable("chi2_jet_extra", "chi2_jet_extra", "", 'F');

    data_loader->AddVariable("chi2_constraint_had_t", "chi2_constraint_had_t", "", 'F');
    data_loader->AddVariable("chi2_constraint_had_w", "chi2_constraint_had_w", "", 'F');
    data_loader->AddVariable("chi2_constraint_lep_t", "chi2_constraint_lep_t", "", 'F');
    data_loader->AddVariable("chi2_constraint_lep_w", "chi2_constraint_lep_w", "", 'F');

    // data_loader->AddVariable("chi2",         "chi2",   "units", 'F');
  }

  data_loader->AddSignalTree(tree_correct_el, 1.0);
  data_loader->AddBackgroundTree(tree_wrong_el, 1.0);
  
  data_loader->AddSignalTree(tree_correct_mu, 1.0);
  data_loader->AddBackgroundTree(tree_wrong_mu, 1.0);

  TCut cut_base;
  if (6 == n_jet)
    cut_base = Form("%d>=n_jets&&met_pt<200&&neutrino_p<600&&lepton_pt<250&&pt_had_t_b<300&&pt_w_u<250&&pt_w_d<250&&pt_lep_t_b<300&&had_t_mass<600&&had_w_mass<300&&lep_t_mass<600&&lep_t_partial_mass<400", n_jet);
  else
    cut_base = Form("%d==n_jets&&met_pt<200&&neutrino_p<600&&lepton_pt<250&&pt_had_t_b<300&&pt_w_u<250&&pt_w_d<250&&pt_lep_t_b<300&&had_t_mass<600&&had_w_mass<300&&lep_t_mass<600&&lep_t_partial_mass<400", n_jet);

  /*
    if (!chk_pre_cut)
    {
      cut_base += "chi2_jet_had_t_b<30&&chi2_jet_w_u<30&&chi2_jet_w_d<30&&chi2_jet_lep_t_b<30&&chi2_constraint_had_t<30";
      cut_base += "chi2_constraint_had_w<30&&chi2_constraint_lep_t<30&&chi2_constraint_lep_w<30";
      if (n_jet != 4)
        cut_base += "chi2_jet_extra<30";
    }
  */

  cut_s = cut_base;
  cut_b = cut_base;

  // to save time
  if (6 == n_jet)
  {
    cut_s += "0<n_matched_jets";
    cut_b += "0<n_matched_jets";
  }

  n_train_signal = 0;
  n_train_back = 0;

  n_train_signal += tree_correct_el->GetEntries(cut_s) / 2 / reduction;
  n_train_back += tree_wrong_el->GetEntries(cut_b) / 2 / reduction;

  n_train_signal += tree_correct_mu->GetEntries(cut_s) / 2 / reduction;
  n_train_back += tree_wrong_mu->GetEntries(cut_b) / 2 / reduction;

  // for debugging
  // n_train_signal = 100;
  // n_train_back = 100;

  data_loader->PrepareTrainingAndTestTree(cut_s, cut_b,
                                          Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:NormMode=NumEvents:V", n_train_signal, n_train_back, n_train_signal, n_train_back));

  // //Adaptive Boost
  // factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDT",
  //                  "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100");

  // Gradient Boost
  int n_tree;
  if (strcmp(getenv("NTREE"), "") != 0)
    n_tree = atoi(getenv("NTREE"));
  else
    n_tree = 200;
  cout << "N_Tree = " << n_tree << endl;

  factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTG",
                      Form("!H:!V:NTrees=%d:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.1:UseBaggedBoost=True:BaggedSampleFraction=0.5:nCuts=200:MaxDepth=2", n_tree));

  // Fisher
  // factory->BookMethod(data_loader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=100:NsmoothMVAPdf=10" );

  // DNN_CPU
  // General layout
  //  TString layoutString("Layout=RELU|128,RELU|128,RELU|128,LINEAR");

  // //Define Training strategy
  // TString trainingStrategyString = ("TrainingStrategy=LearningRate=1e-4,Momentum=0.9,"
  //                                "ConvergenceSteps=20,BatchSize=100,TestRepetitions=1,"
  //                                "WeightDecay=1e-4,Regularization=None,"
  //                                "DropConfig=0.0+0.5+0.5+0.5");

  // //General options
  // TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
  //                  "WeightInitialization=XAVIERUNIFORM");
  // dnnOptions.Append (":"); dnnOptions.Append (layoutString);
  // dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

  // TString cpuOptions = dnnOptions + ":Architecture=CPU";

  // factory->BookMethod(data_loader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
} // Permutation_TMVA::Permutation_TMVA()

//////////

Permutation_TMVA::~Permutation_TMVA()
{
  delete factory;
  delete data_loader;

  TDirectory *dir_dataset = (TDirectory *)fout->Get("dataset");
  dir_dataset->Delete("TestTree;*");
  dir_dataset->Delete("TrainTree;*");
  
  //fout->cd();
  //dir_dataset->Write();

  fout->Write();
  fout->Close();
} // Permutation_TMVA::~Permutation_TMVA()

//////////
