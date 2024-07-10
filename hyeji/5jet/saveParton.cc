#include "TParticle.h"
//#include "Pythia8/Pythia.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"

//#include "fastjet/ClusterSequence.hh"
//#include "fastjet/ClusterSequenceArea.hh"
//#include "fastjet/PseudoJet.hh"
//#include "fastjet/Selector.hh"
//#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
//#include "fastjet/tools/Subtractor.hh"

#include <iostream>

//using namespace Pythia8;
//using namespace fastjet;
using namespace std;

int main()
{
  TH1F* pPt = new TH1F("pPt" , "pPt" , 200, 0.0, 50.0);
  pPt -> Sumw2();

  // Input file and tree
  TFile* fin = TFile::Open("Particle510.root");
  TTree* tin = (TTree*)fin->Get("Particle");

  TClonesArray *particles = new TClonesArray("TParticle");
  tin->GetBranch("carray")->SetAutoDelete(kFALSE);
  tin->SetBranchAddress("carray",&particles);
  
  Int_t nentries = tin->GetEntries();
  
  // Event loop
  for(Int_t iev = 0; iev < nentries; iev++)
  {
    particles->Clear();
    tin->GetEntry(iev); //마법의 코드 , particles에도 정보가 알아서 들어가짐.
    
    // Particle loop
    for(Int_t i=0; i<particles->GetEntriesFast(); i++)
    {
        TParticle* part = (TParticle*)particles->At(i);
        //statuscode 뭐냐? TMath::Abs(part->Eta()) < 2.0 && part->Pt() > 0.2
        //statuscode => The status code includes information on how a particle was produced
        Double_t pt = TMath::Sqrt(TMath::Power(part->Px() ,2) + TMath::Power(part->Py() ,2));
        if(TMath::Abs(part->GetFirstMother())==3 && TMath::Abs(part->GetStatusCode())== 23 ){
            pPt->Fill(pt ,1.387 * TMath::Power(10 ,-1));
        }
    }
  }

  TFile* fout = TFile::Open("PartonPt510.root", "RECREATE");
  
  fout->cd();
  pPt->Write();
  fout->Close();
 }  

//g++ saveParton.cc -o saveParton.exe `root-config --cflags` -I/home/hyejilee/pythia8310/include -O2 -std=c++17 -pedantic -W -Wall -Wshadow -fPIC -pthread  -L/home/hyejilee/pythia8310/lib -Wl,-rpath,/home/hyejilee/pythia8310/lib -lpythia8 -ldl `root-config --evelibs`