#include "Pythia8/Pythia.h"
#include <TH1.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TParticle.h>
#include <TString.h>
#include <TTimeStamp.h>
#include <string.h>
#include <iostream> // Include this to work with command-line arguments
using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " pthatmin pthatmax" << endl;
    return 1;
  }

  // Convert pthatmin and pthatmax from command-line arguments to float
  const float pthatmin = atof(argv[1]);
  const float pthatmax = atof(argv[2]);
  const float numiter = atof(argv[3]);

  TFile* outFile = new TFile(TString::Format("path_naem_of_outfile_%.f%.f_%.f.root", pthatmin, pthatmax, numiter), "RECREATE"); 
  
  TTree* tree; 
  tree = new TTree("treeName1"," tree Title 1");
  
  const int   nEvents = 10000; //Number of maximum events
  const float ymax = 1.0; //Maximum rapidity  
  
  Pythia8::Pythia pythia;
  
  pythia.readString("HardQCD:all = on");
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212 ");
  pythia.readString("Beams:eCM = 5020."); // 5020 GeV 
  //pythia.readString("Tune:pp = 14"); //TUNED BY Monash 2014
  pythia.readString("PhaseSpace:pTHatMin = " + to_string(pthatmin));
  pythia.readString("PhaseSpace:pTHatMax = " + to_string(pthatmax));
  //IMPORTANT! This will only create a seed on compilation, not on execution. Re-running the same executable will generate the same seed.
  pythia.readString("Random:setSeed = on"); 
  pythia.readString("Random:seed = " + to_string(numiter));

  cout<<"Random:seed = " + to_string(numiter)<<endl;

  int entries = pythia.event.size();

  pythia.init();

  TClonesArray* trackArray = new TClonesArray("TParticle", 100);
  tree->Branch("tracks", "TClonesArray", &trackArray);

///////////////////////////////////////////////Pseudo Code for save initial hard-scatter parton///////////////////////////////////////////
	
  int bin_num; 
  int bin_x_min; 
  int bin_x_max; 

  TH1F *hIHP_pT = new TH1F("hist_name", "hist_name", bin_num, bin_x_min, bin_x_max);
  int event_size;
  int track_size; 

  for (int i = 0; i < event_size; i++){
	for (int j = 0; j < track_size; j++){
		bool isInitHardParton; 
		double pT;
		if (!isInitHardParton) continue; 
		hIHP->Fill(pT); 
	}
  }		

  outFile->cd(); 
  hIHP->Write(); 
  outFile->Close();
  return 0;
}
