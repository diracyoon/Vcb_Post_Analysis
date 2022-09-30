#include "Average_Syst_PDF.h"

ClassImp(Average_Syst_PDF);

//////////

Average_Syst_PDF::Average_Syst_PDF(const TString& a_era, const TString& a_channel) : samples(a_era, a_channel)
{
  era = a_era;
  channel = a_channel;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/RunPDF/";

  if(samples.map_mc.size()!=samples.map_short_name_mc.size())
    {
      cout << "size of samples::maps is not matched. Check it first." << endl;
      exit(EXIT_FAILURE);
    }

  for(auto it=samples.map_mc.begin(); it!=samples.map_mc.end(); it++)
    {
      cout << it->first << endl;

      //for debug
      if(it->first=="TTLJ_WtoCB_powheg") map_fin_mc[it->first] = new TFile(path_base+it->second);
    }
    
  fout = new TFile("Test.root", "RECREATE");
  
  //Do Average
  for(auto it=map_fin_mc.begin(); it!=map_fin_mc.end(); it++)
    {
      cout << it->first << endl;
      
      for(int i=0; i<1; i++)
	{
	  cout << "Control" << i << endl;
	  
	  TString dir_name = Form("POGTightWithTightIso_Central/Control%d", i);
	  
	  TDirectory* dir_central = (TDirectory*)map_fin_mc[it->first]->Get(dir_name);
	  TList* list = dir_central->GetListOfKeys();
	  
	  TDirectory* dir_average = new TDirectory(dir_name, dir_name);
	  
	  //for(int j=0; j<list->GetEntries(); j++)
	  for(int j=0; j<1; j++)
	    {
	      TString histo_name = list->At(j)->GetName();
	      cout << histo_name << endl;
	      
	      TH1D* histo_central = (TH1D*)dir_central->Get(histo_name);
	      
	      int n_bin = histo_central->GetNbinsX();
	      float x_low = histo_central->GetBinLowEdge(1);
	      float x_up = histo_central->GetBinLowEdge(n_bin) + histo_central->GetBinWidth(n_bin);
	      
	      cout << n_bin << " " << x_low << " " << x_up << endl;
	      
	      histo_average = new TH1D(histo_name, histo_name, n_bin, x_low, x_up);
	      histo_average->SetBit(TH1::kIsAverage);
	      
	      for(int k=0; k<100; k++)
	       	{
	      	  cout << "PDF Error Sets = " << k << endl;
		  
	       	  TString error_set_name = Form("POGTightWithTightIso_Syst_PDF_Error_Set%d", k);
	       	  TDirectory* dir_syst = (TDirectory*)map_fin_mc[it->first]->Get(error_set_name+Form("/Control%d", i));
		  
	       	  TH1D* histo_syst = (TH1D*)dir_syst->Get(histo_name)->Clone();
	       	  histo_syst->SetBit(TH1::kIsAverage);
		  
	      // 	  //cout << histo_name << endl;
	      // 	  int n_bin_syst = histo_syst->GetNbinsX();
	      // 	  float x_low_syst = histo_syst->GetBinLowEdge(1);
	      // 	  float x_up_syst = histo_syst->GetBinLowEdge(n_bin) + histo_syst->GetBinWidth(n_bin);

	      // 	  //cout << n_bin_syst << " " << x_low_syst << " " << x_up_syst << endl;
		  
	       	  histo_average->Add(histo_average, histo_syst);
	      
		}//loop over PDF Error sets
	      
	      //dir_average->cd();
	      //histo_average->Write();
	      //dir_average->
	    }//loop over histos
	  
	  fout->cd();
	  dir_average->Write();
	}//loop over regions
    }//loop over sample
}//Average_Syst_PDF::Average_Syst_PDF(const TString& a_era, const TString& a_channel)

//////////

Average_Syst_PDF::~Average_Syst_PDF()
{
  fout->Close();
}//Average_Syst_PDF::~Average_Syst_PDF()

//////////

//////////
