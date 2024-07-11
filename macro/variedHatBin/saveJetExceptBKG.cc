#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"

#include "TF1.h"
#include "TH1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>

using namespace fastjet;
using namespace std;

int main()
{

  TH1F* jetPT = new TH1F("jetPT","jetPT",600,0.0,300.0);

  jetPT->Sumw2();

  // Input file and tree
  TFile* fin = TFile::Open("Particle50100.root");
  TTree* tin = (TTree*)fin->Get("Particle");

  TClonesArray *particles = new TClonesArray("TParticle");
  tin->GetBranch("carray")->SetAutoDelete(kFALSE);
  tin->SetBranchAddress("carray",&particles);

  Int_t nentries = tin->GetEntries();

  // Event loop
  for(Int_t iev = 0; iev < nentries; iev++)
  {
    //pseudojet extract
    vector <PseudoJet> jetvec;

    particles->Clear();
    tin->GetEntry(iev); //마법의 코드 , particles에도 정보가 알아서 들어가짐.
    
    for(Int_t i=0; i<particles->GetEntriesFast(); i++)
    {
        TParticle* part = (TParticle*)particles->At(i);

        //statuscode 뭐냐? TMath::Abs(part->Eta()) < 2.0 && part->Pt() > 0.2
        //statuscode => The status code includes information on how a particle was produced
        Double_t pt = TMath::Sqrt(TMath::Power(part->Px() ,2) + TMath::Power(part->Py() ,2));

        /*
        if(part->GetFirstDaughter() == 0 ){
          cout << "Event : " << iev << "index : " <<i<< "pdg : " << part->GetPdgCode() << " pt : " << pt << " px : " << part->Px() << " py : " << part->Py() << " pz : " <<part->Pz() << "Da : " << part->GetFirstDaughter() << " MOM : " <<part->GetFirstMother()  << endl;  
        }
        */
        
        if (part->GetFirstDaughter() == 0 && TMath::Abs(part->Eta()) < 2.4 && pt > 0.15){
          jetvec.push_back(PseudoJet(part->Px(),part->Py(),part->Pz(),part->Energy())); 
        }
    }

    //making jet
    Double_t R = 0.4;
    JetDefinition jet_def(antikt_algorithm, R); //jet란 이런 것이다! 

    ClusterSequence cs(jetvec, jet_def); //jet를 clustering
    vector <PseudoJet> jets = sorted_by_pt(cs.inclusive_jets()); //허용된 최소한의 pt보다 큰 jet들만 //괄호에 minimum 값을 적으면 돼.

    for(Int_t i=0; i<jets.size(); i++){
      if(TMath::Abs(jets[i].eta()) <  2.4 - R){ //
        jetPT->Fill(jets[i].pt());
      }
    }
  }

  TFile* fout = TFile::Open("JetPt50100.root", "RECREATE");

  fout->cd();
  jetPT->Write();
  fout->Close();  
}

//g++ saveJetExceptBKG.cc -o saveJetExceptBKG.exe `root-config --cflags` `root-config --evelibs` `../fastjet-3.4.2/fastjet-config --cxxflags --libs`