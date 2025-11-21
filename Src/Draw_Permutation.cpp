#include <Draw_Permutation.h>

ClassImp(Draw_Permutation);

//////////

Draw_Permutation::Draw_Permutation(const TString &a_era, const TString &a_channel)
{
  era = a_era;
  channel = a_channel;

  Run();
} // Draw_Permutation::Draw_Permutation

//////////

Draw_Permutation::~Draw_Permutation()
{
} // Draw_Permutation::~Draw_Permutation()

//////////

void Draw_Permutation::Config_Variable()
{
  cout << "[Draw_Permutation::Config_Variable]: Init" << endl;

  // variable_conf = {{"Had_W_Mass", 100, 0, 300},
  //                  {"BvsC_W_u"}};

  cout << "[Draw_Permutation::Config_Variable]: Done" << endl;

  return;
} // void Draw_Permutation::Config_Variable()

//////////

void Draw_Permutation::Read_Tree()
{
  cout << "[Draw_Permutation::Read_Tree]: Init" << endl;

  TString path_base = getenv("out");
  path_base += "Run2UltraLegacy_v3/Vcb/" + era + "/RunPermutationTree__/";

  fin = new TFile(path_base + "Vcb_TTLJ_WtoCB_powheg.root", "READ");
  tree_correct = (TTree *)fin->Get(channel + "/Permutation_Correct");
  tree_wrong = (TTree *)fin->Get(channel + "/Permutation_Wrong");

  Set_Tree(tree_correct);
  Set_Tree(tree_wrong);

  cout << "[Draw_Permutation::Read_Tree]: Done" << endl;

  return;
} // void Draw_Permutation::Read_Tree()

//////////

void Draw_Permutation::Run()
{
  cout << "[Draw_Permutation::Run]: Init" << endl;

  Config_Variable();
  Setup_Histo();
  Read_Tree();

  cout << "[Draw_Permutation::Run]: Done" << endl;

  return;
} // void Draw_Permutation::Run()

//////////

void Draw_Permutation::Set_Tree(TTree *tree)
{
  // tree->SetBranchAddress("weight", &weight);
  // tree->SetBranchAddress("era_index", &era_index);
  tree->SetBranchAddress("n_jets", &n_jets);
  // tree->SetBranchAddress("n_bjets", &n_b_jet);
  // tree->SetBranchAddress("n_cjets", &n_c_jet);
  // tree->SetBranchAddress("lepton_pt", &lepton_pt);
  // tree->SetBranchAddress("pt_ratio", &pt_ratio);
  // tree->SetBranchAddress("chk_gentau_conta", &chk_gentau_conta);
  // tree->SetBranchAddress("n_matched_jets", &n_matched_jets);
  // tree->SetBranchAddress("gen_neutrino_px", &gen_neutrino_px);
  // tree->SetBranchAddress("gen_neutrino_py", &gen_neutrino_py);
  // tree->SetBranchAddress("gen_neutrino_pz", &gen_neutrino_pz);
  // tree->SetBranchAddress("met_pt", &met_pt);
  // tree->SetBranchAddress("met_px", &met_px);
  // tree->SetBranchAddress("met_py", &met_py);
  // tree->SetBranchAddress("met_rebalance_px", &met_rebalance_px);
  // tree->SetBranchAddress("met_rebalance_py", &met_rebalance_py);
  // tree->SetBranchAddress("neutrino_pz_sol", &neutrino_pz_sol);
  // tree->SetBranchAddress("neutrino_pz_sol_unrebal", &neutrino_pz_sol_unrebal);
  tree->SetBranchAddress("neutrino_p", &neutrino_p);
  // tree->SetBranchAddress("chk_real_neu_pz", &chk_real_neu_pz);
  // tree->SetBranchAddress("mt_gen", &mt_gen);
  // tree->SetBranchAddress("mt_met", &mt_met);
  // tree->SetBranchAddress("mt_met_rebalance", &mt_met_rebalance);
  tree->SetBranchAddress("pt_had_t_b", &pt_had_t_b);
  tree->SetBranchAddress("pt_w_u", &pt_w_u);
  tree->SetBranchAddress("pt_w_d", &pt_w_d);
  tree->SetBranchAddress("pt_lep_t_b", &pt_lep_t_b);
  // tree->SetBranchAddress("eta_had_t_b", &eta_had_t_b);
  // tree->SetBranchAddress("eta_w_u", &eta_w_u);
  // tree->SetBranchAddress("eta_w_d", &eta_w_d);
  // tree->SetBranchAddress("eta_lep_t_b", &eta_lep_t_b);
  tree->SetBranchAddress("bvsc_had_t_b", &bvsc_had_t_b);
  // tree->SetBranchAddress("cvsb_had_t_b", &cvsb_had_t_b);
  // tree->SetBranchAddress("cvsl_had_t_b", &cvsl_had_t_b);
  tree->SetBranchAddress("bvsc_w_u", &bvsc_w_u);
  // tree->SetBranchAddress("cvsb_w_u", &cvsb_w_u);
  // tree->SetBranchAddress("cvsl_w_u", &cvsl_w_u);
  tree->SetBranchAddress("bvsc_w_d", &bvsc_w_d);
  // tree->SetBranchAddress("cvsb_w_d", &cvsb_w_d);
  // tree->SetBranchAddress("cvsl_w_d", &cvsl_w_d);
  tree->SetBranchAddress("bvsc_lep_t_b", &bvsc_lep_t_b);
  // tree->SetBranchAddress("cvsb_lep_t_b", &cvsb_lep_t_b);
  // tree->SetBranchAddress("cvsl_lep_t_b", &cvsl_lep_t_b);
  // tree->SetBranchAddress("met_rebalance_px", &met_rebalance_px);
  // tree->SetBranchAddress("met_rebalance_py", &met_rebalance_py);
  tree->SetBranchAddress("pt_had_w", &pt_had_w);
  tree->SetBranchAddress("pt_had_t", &pt_had_t);
  tree->SetBranchAddress("pt_lep_w", &pt_lep_w);
  tree->SetBranchAddress("pt_lep_t", &pt_lep_t);
  tree->SetBranchAddress("pt_tt", &pt_tt);
  // tree->SetBranchAddress("del_phi_w_u_w_d", &del_phi_w_u_w_d);
  // tree->SetBranchAddress("del_phi_had_w_had_t_b", &del_phi_had_w_had_t_b);
  // tree->SetBranchAddress("del_phi_lep_neu", &del_phi_lep_neu);
  // tree->SetBranchAddress("del_phi_lep_w_lep_t_b", &del_phi_lep_w_lep_t_b);
  tree->SetBranchAddress("del_phi_had_t_lep_t", &del_phi_had_t_lep_t);
  // tree->SetBranchAddress("del_eta_w_u_w_d", &del_eta_w_u_w_d);
  // tree->SetBranchAddress("del_eta_had_w_had_t_b", &del_eta_had_w_had_t_b);
  // tree->SetBranchAddress("del_eta_lep_neu", &del_eta_lep_neu);
  // tree->SetBranchAddress("del_eta_lep_w_lep_t_b", &del_eta_lep_w_lep_t_b);
  // tree->SetBranchAddress("del_eta_had_t_lep_t", &del_eta_had_t_lep_t);
  // tree->SetBranchAddress("del_r_w_u_w_d", &del_r_w_u_w_d);
  // tree->SetBranchAddress("del_r_had_w_had_t_b", &del_r_had_w_had_t_b);
  // tree->SetBranchAddress("del_r_lep_neu", &del_r_lep_neu);
  // tree->SetBranchAddress("del_r_lep_w_lep_t_b", &del_r_lep_w_lep_t_b);
  // tree->SetBranchAddress("del_r_had_t_lep_t", &del_r_had_t_lep_t);
  tree->SetBranchAddress("theta_w_u_w_d", &theta_w_u_w_d);
  tree->SetBranchAddress("theta_had_w_had_t_b", &theta_had_w_had_t_b);
  tree->SetBranchAddress("theta_lep_neu", &theta_lep_neu);
  tree->SetBranchAddress("theta_lep_w_lep_t_b", &theta_lep_w_lep_t_b);
  // tree->SetBranchAddress("theta_had_t_lep_t", &theta_had_t_lep_t);
  tree->SetBranchAddress("had_t_mass", &had_t_mass);
  tree->SetBranchAddress("had_w_mass", &had_w_mass);
  tree->SetBranchAddress("lep_t_mass", &lep_t_mass);
  tree->SetBranchAddress("lep_t_partial_mass", &lep_t_partial_mass);
  // tree->SetBranchAddress("chi2_jet_had_t_b", &chi2_jet_had_t_b);
  // tree->SetBranchAddress("chi2_jet_w_u", &chi2_jet_w_u);
  // tree->SetBranchAddress("chi2_jet_w_d", &chi2_jet_w_d);
  // tree->SetBranchAddress("chi2_jet_lep_t_b", &chi2_jet_lep_t_b);
  // tree->SetBranchAddress("chi2_jet_extra", &chi2_jet_extra);
  // tree->SetBranchAddress("chi2_constraint_had_t", &chi2_constraint_had_t);
  // tree->SetBranchAddress("chi2_constraint_had_w", &chi2_constraint_had_w);
  // tree->SetBranchAddress("chi2_constraint_lep_t", &chi2_constraint_lep_t);
  // tree->SetBranchAddress("chi2_constraint_lep_w", &chi2_constraint_lep_w);
  // tree->SetBranchAddress("chi2", &chi2);
  // tree->SetBranchAddress("had_w_charge_abs", &had_w_charge_abs);
  // tree->SetBranchAddress("had_t_charge_abs", &had_t_charge_abs);
  // tree->SetBranchAddress("lep_t_charge_abs", &lep_t_charge_abs);
  // tree->SetBranchAddress("tt_charge", &tt_charge);

  return;
} // void Draw_Permutation::Set_Tree(const TTree& tree)

//////////

void Draw_Permutation::Setup_Histo()
{
  cout << "[Draw_Permutation::Setup_Histo]: Init" << endl;

  for (int i = 0; i < 2; i++)
  {
    // for (int j = 0; j < n_variables_to_draw; j++)
    // {
    // }
  }

  cout << "[Draw_Permutation::Setup_Histo]: Done" << endl;

  return;
} // void Draw_Permutation::Setup_Histo()

//////////