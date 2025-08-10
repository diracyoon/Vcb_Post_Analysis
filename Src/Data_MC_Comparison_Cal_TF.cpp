#include "Data_MC_Comparison_Cal_TF.h"

ClassImp(Data_MC_Comparison_Cal_TF);

//////////

Data_MC_Comparison_Cal_TF::Data_MC_Comparison_Cal_TF(const TString &a_era, const TString &a_channel, const TString& a_tagger, const TString &a_extension)
    : samples(a_era, a_channel, "Vcb")
{
  cout << "[Data_MC_Comparison_Cal_TF::Data_MC_Comparison_Cal_TF]: Init analysis" << endl;

  TH1::AddDirectory(kFALSE);

  gROOT->SetBatch(kTRUE);

  era = a_era;
  channel = a_channel;
  tagger = a_tagger;
  extension = a_extension;

  TString path_base = getenv("Vcb_Post_Analysis_WD");

  TString fin_name = path_base;
  fin_name += "/Workplace/Cal_TF/Vcb_TF_" + era + "_" + channel + ".root";

  color = {{"TTLJ_2", kSpring},
           {"TTLJ_CC_2", kSpring},
           {"TTLJ_BB_2", kSpring},
           {"TTLJ_4", kYellow},
           {"TTLJ_CC_4", kYellow},
           {"TTLJ_BB_4", kYellow},
           {"TTLJ_45", kRed},
           {"TTLJ_CC_45", kRed},
           {"TTLJ_BB_45", kRed},
           {"TTLL", kOrange},
           {"TTLL_CC", kOrange},
           {"TTLL_BB", kOrange},
           {"TTJJ", kPink},
           {"ST", kGreen},
           {"ttV", kTeal},
           {"VV", kCyan},
           {"VJets", kAzure},
           {"QCD", kBlue}};

  fin = new TFile(fin_name);
  if (fin == NULL)
  {
    cerr << "Can not find " << fin_name << ". Abort the process." << endl;
    exit(1);
  }

  // get region_name
  TList *list_region = fin->GetListOfKeys();
  Setup_Name(list_region, region_name);
  n_region = region_name.size();
  cout << "n_region = " << n_region << endl;

  // get abcd_region_name
  TList *list_abcd_region = ((TDirectory *)fin->Get(region_name[0]))->GetListOfKeys();
  Setup_Name(list_abcd_region, abcd_region_name);
  abcd_region_name.erase(find(abcd_region_name.begin(), abcd_region_name.end(), "TF"));
  abcd_region_name.erase(find(abcd_region_name.begin(), abcd_region_name.end(), "TF_QCD_Sample"));

  n_abcd_region = abcd_region_name.size();
  cout << "n_abcd_region = " << n_abcd_region << endl;

  // get sample_name
  TList *list_sample = ((TDirectory *)fin->Get(region_name[0] + "/" + abcd_region_name[0]))->GetListOfKeys();
  Setup_Name(list_sample, sample_name);
  sample_name.erase(find(sample_name.begin(), sample_name.end(), "Data"));
  sample_name.erase(find(sample_name.begin(), sample_name.end(), "Subtracted"));

  n_sample = sample_name.size();
  cout << "n_sample = " << n_sample << endl;
  for (int i = 0; i < sample_name.size(); i++)
    cout << sample_name[i] << endl;

  // setup histo_mc
  Setup_Histo_MC();

  // setup histo_data
  Setup_Histo_Data();

  Ordering_Sample_Name();
  Stack_MC();
  Draw();
} // Data_MC_Comparison_Cal_TF::Data_MC_Comparison_Cal_TF(const TString& a_era, const TString& a_channel))

//////////

Data_MC_Comparison_Cal_TF::~Data_MC_Comparison_Cal_TF()
{
} // Data_MC_Comparison_Cal_TF::~Data_MC_Comparison_Cal_TF()

//////////

void Data_MC_Comparison_Cal_TF::Draw()
{
  gStyle->SetOptStat(0);

  canvas = new TCanvas *[n_region];
  TH1D ****histo_data_proj = new TH1D ***[n_region];

  for (unsigned int i = 0; i < n_region; i++)
  {
    TString canvas_name = region_name[i];
    canvas[i] = new TCanvas(canvas_name, canvas_name, 1400, 1200);
    canvas[i]->Divide(4, 2);

    histo_data_proj[i] = new TH1D **[n_abcd_region];

    for (unsigned int j = 0; j < n_abcd_region; j++)
    {
      histo_data_proj[i][j] = new TH1D *[2];

      for (unsigned int k = 0; k < 2; k++)
      {
        TString title;
        if (k == 0)
        {
          histo_data_proj[i][j][k] = (TH1D *)(dynamic_cast<TH2D *>(histo_data[i][j])->ProjectionX(region_name[i] + "_" + abcd_region_name[j] + "_Eta")->Clone());
          title = "Lepton #eta";
        }
        else if (k == 1)
        {
          histo_data_proj[i][j][k] = (TH1D *)(dynamic_cast<TH2D *>(histo_data[i][j])->ProjectionY(region_name[i] + "_" + abcd_region_name[j] + "_Pt")->Clone());
          title = "Lepton p_{T}";
        }

        if(k==1)
          canvas[i]->cd(1 + j + k * n_abcd_region)->SetLogx();
        canvas[i]->cd(1 + j + k * n_abcd_region);

        histo_data_proj[i][j][k]->Sumw2();
        histo_data_proj[i][j][k]->SetMarkerStyle(8);
        histo_data_proj[i][j][k]->GetXaxis()->SetTitle(title);
        histo_data_proj[i][j][k]->GetYaxis()->SetRangeUser(0, histo_data_proj[i][j][k]->GetMaximum() * 1.5);
        histo_data_proj[i][j][k]->SetTitle(abcd_region_name[j]);
        histo_data_proj[i][j][k]->Draw();

        if (k == 0)
          stack_mc_eta[i][j]->Draw("SAME BAR");
        else if (k == 1)
          stack_mc_pt[i][j]->Draw("SAME BAR");

        histo_data_proj[i][j][k]->Draw("SAME");

      } // eta or pt
    } // loop over n_abcd_region

    canvas[i]->Print("ABCD_" + era + "_" + channel + "_" + region_name[i] + "." + extension, extension);
  } // loop over n_region

  return;
} // void Data_MC_Comparison_Cal_TF::Draw()

//////////

vector<int> Data_MC_Comparison_Cal_TF::Get_Histo_Group(const TString &sample_name_merged)
{
  vector<int> vec_histo_group;
  for (unsigned i = 0; i < sample_name.size(); i++)
  {
    if (samples.map_short_short_name[sample_name[i]] == sample_name_merged)
      vec_histo_group.push_back(i);
  }

  if (vec_histo_group.size() == 0)
  {
    int index = find(sample_name.begin(), sample_name.end(), sample_name_merged) - sample_name.begin();
    vec_histo_group.push_back(index);
  }

  return vec_histo_group;
} // vector<int> Data_MC_Comparison_Cal_TF::Get_Histo_Group(const TString &sample_name_merged)

//////////

void Data_MC_Comparison_Cal_TF::Ordering_Sample_Name()
{
  sample_name_order[0] = "VV";
  sample_name_order[1] = "ttV";
  sample_name_order[2] = "VJets";
  sample_name_order[3] = "TTJJ";
  sample_name_order[4] = "ST";
  sample_name_order[5] = "TTLL";
  sample_name_order[6] = "TTLL_CC";
  sample_name_order[7] = "TTLL_BB";
  sample_name_order[8] = "TTLJ_2";
  sample_name_order[9] = "TTLJ_CC_2";
  sample_name_order[10] = "TTLJ_BB_2";
  sample_name_order[11] = "TTLJ_4";
  sample_name_order[12] = "TTLJ_CC_4";
  sample_name_order[13] = "TTLJ_BB_4";
  sample_name_order[14] = "TTLJ_45";
  sample_name_order[15] = "TTLJ_CC_45";
  sample_name_order[16] = "TTLJ_BB_45";
  sample_name_order[17] = "QCD";

  return;
} // void Data_MC_Comparison_Cal_TF::Ordering_Sample_Name()

//////////

void Data_MC_Comparison_Cal_TF::Setup_Histo_Data()
{
  cout << "[Data_MC_Comparison_Cal_TF::Setup_Histo_Data]: Init" << endl;

  histo_data = new TH1 **[n_region];
  for (unsigned int i = 0; i < n_region; i++)
  {
    histo_data[i] = new TH1 *[n_abcd_region];
    for (unsigned int j = 0; j < n_abcd_region; j++)
    {
      TString histo_name = region_name[i] + "/" + abcd_region_name[j] + "/Data";

      histo_data[i][j] = (TH2D *)fin->Get(histo_name);
    } // loop over n_abcd_region
  } // loop over n_region

  cout << "[Data_MC_Comparison_Cal_TF::Setup_Histo_Data]: Done" << endl;

  return;
} // void Data_MC_Comparison_Cal_TF::Setup_Histo_Data()

//////////

void Data_MC_Comparison_Cal_TF::Setup_Histo_MC()
{
  cout << "[Data_MC_Comparison_Cal_TF::Setup_Histo_MC]: Init" << endl;

  histo_mc = new TH1 ***[n_region];
  for (unsigned int i = 0; i < n_region; i++)
  {
    histo_mc[i] = new TH1 **[n_abcd_region];
    for (unsigned int j = 0; j < n_abcd_region; j++)
    {
      histo_mc[i][j] = new TH1 *[n_sample];
      for (unsigned int k = 0; k < n_sample; k++)
      {
        TString histo_name = region_name[i] + "/" + abcd_region_name[j] + "/" + sample_name[k];

        histo_mc[i][j][k] = (TH2D *)fin->Get(histo_name);
      } // loop over n_sample
    } // loop over n_abcd_region
  } // loop over n_region

  cout << "[Data_MC_Comparison_Cal_TF::Setup_Histo_Data]: Done" << endl;

  return;
} // void Data_MC_Comparison_Cal_TF::Setup_Histo_Data()

//////////

void Data_MC_Comparison_Cal_TF::Setup_Name(const TList *list, vector<TString> &vec_name)
{
  int n_entries = list->GetEntries();

  for (int i = 0; i < n_entries; i++)
  {
    TDirectory *dir = (TDirectory *)list->At(i);
    TString name = dir->GetName();

    vec_name.push_back(name);
  }

  return;
} // int Data_MC_Comparison_Cal_TF::Setup_Name(const TList* list, TString* name)

//////////

void Data_MC_Comparison_Cal_TF::Stack_MC()
{
  cout << "[Data_MC_Comparison_Cal_TF::Stack_MC]: Init" << endl;

  stack_mc_eta = new THStack **[n_region];
  stack_mc_pt = new THStack **[n_region];
  for (unsigned int i = 0; i < n_region; i++)
  {
    stack_mc_eta[i] = new THStack *[n_abcd_region];
    stack_mc_pt[i] = new THStack *[n_abcd_region];

    for (unsigned int j = 0; j < n_abcd_region; j++)
    {
      TString stack_name = region_name[i] + "_" + abcd_region_name[j];
      cout << "stack_name = " << stack_name << endl;

      stack_mc_eta[i][j] = new THStack(stack_name + "_Eta", stack_name + "_Eta");
      stack_mc_pt[i][j] = new THStack(stack_name + "_Pt", stack_name + "_Pt");

      for (auto it = sample_name_order.begin(); it != sample_name_order.end(); it++)
      {
        vector<int> vec_sample_index = Get_Histo_Group(it->second);
        for (unsigned int k = 0; k < vec_sample_index.size(); k++)
        {
          histo_mc[i][j][vec_sample_index[k]]->SetLineColor(color[it->second]);
          histo_mc[i][j][vec_sample_index[k]]->SetLineWidth(2);

          histo_mc[i][j][vec_sample_index[k]]->SetFillColor(color[it->second]);

          stack_mc_eta[i][j]->Add((TH1D *)dynamic_cast<TH2D *>(histo_mc[i][j][vec_sample_index[k]])->ProjectionX()->Clone());
          stack_mc_pt[i][j]->Add((TH1D *)dynamic_cast<TH2D *>(histo_mc[i][j][vec_sample_index[k]])->ProjectionY()->Clone());
        } // loop over histo group
      } // loop over sample_name_order

    } // loop over n_abcd_region
  } // loop over n_region

  cout << "[Data_MC_Comparison_Cal_TF::Stack_MC]: Done" << endl;

  return;
} // void Data_MC_Comparison_Cal_TF::Stack_MC()

//////////
