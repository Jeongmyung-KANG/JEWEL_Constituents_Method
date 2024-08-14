void drawJetMassAll_norm_jetsize() {
    std::cout.precision(6);

    gStyle->SetOptStat(0);

    gStyle->SetPadBottomMargin(0.1);
    gStyle->SetPadLeftMargin(0.1);
    gStyle->SetPadRightMargin(0.1);
    gStyle->SetPadTopMargin(0.1);
    
    TFile* f1 = TFile::Open("/home/hyejilee/5jet/JetMassAll2030_1_norm_jetsize.root");
    TFile* f2 = TFile::Open("/home/hyejilee/HepMC3toRoot/jetResult/jetMassAll_AuAu_KR_T_350_502GeV_CENT_0_10_1.0_norm_jetsize.root");
    
    TH1F* hDummy = new TH1F("hDummy", "", 250, 0, 200);
    hDummy->SetTitle("");
    hDummy->GetXaxis()->SetTitle("#it{M}_{jet} (GeV/#it{c}^{2})");
    hDummy->GetYaxis()->SetTitle("1/#it{N}_{jet} d#it{N}_{jet}/d#it{M}_{jet}");
    hDummy->GetXaxis()->SetTitleSize(0.04);
    hDummy->GetXaxis()->SetTitleOffset(1);
    hDummy->GetYaxis()->SetTitleSize(0.04);
    hDummy->GetYaxis()->SetTitleOffset(1);
    hDummy->GetYaxis()->SetRangeUser(0.00000000001, 0.3);
    hDummy->GetXaxis()->SetRangeUser(0.00000000001, 50);

    TCanvas * c[6];
    TH1F* hist1[6];
    TH1F* hist2[6];

    TString massRange[6] = {"510","1020","2040","4060","60100","100150"};
    TString mini[6] = {"5","10","20","40","60","100"};
    TString maxi[6] = {"10","20","40","60","100","150"};

    for(Int_t i =0;i<6;i++){
        hist1[i] = (TH1F*)f1->Get(Form("jetMass%s",massRange[i].Data()));
        hist2[i] = (TH1F*)f2->Get(Form("jetMass%s",massRange[i].Data()));

	c[i] = new TCanvas(Form("jetMass_pt%s",massRange[i].Data()),Form("jetMass_pt%s",massRange[i].Data()), 800, 550);
        
        hist1[i]->SetMarkerColor(kRed);
        hist1[i]->SetMarkerStyle(20);
        //hist1[i]->Scale(1.0/hist1[i]->GetXaxis()->GetBinWidth(1));

	//for(Int_t ix = 0;ix < hist1[i]->GetXaxis()->GetNbins(); ix++){
	 //   Double_t error = hist1[i]->GetBinError(ix+1);
          //  cout << error << endl;
	//}
	
        hist2[i]->SetMarkerColor(kBlue);
        hist2[i]->SetMarkerStyle(20);
        //hist2[i]->Scale(1.0/hist2[i]->GetXaxis()->GetBinWidth(1));

        TLegend* legend = new TLegend(0.65, 0.6, 0.8, 0.75);
        legend->SetFillColor(4000);
        legend->SetBorderSize(0);
        legend->SetTextSize(0.05);
        legend->AddEntry(hist1[i], " PYTHIA8", "lpf");
        legend->AddEntry(hist2[i], " JEWEL", "lpf");

        TLatex* txta = new TLatex(0.18, 0.84, "PYTHIA 8, #it{pp}");
        txta->SetNDC(kTRUE);
        txta->SetTextSize(0.05);
        txta->SetTextFont(62);
        txta->SetTextColor(1);
        
        TLatex* txtb = new TLatex(0.18, 0.78, "JEWEL-2.3.0, AA, 0-10%");
        txtb->SetNDC(kTRUE);
        txtb->SetTextSize(0.05);
        txtb->SetTextFont(62);
        txtb->SetTextColor(1);
    
        TLatex* txtc = new TLatex(0.18, 0.72, "#sqrt{#it{s}} = 5.02 TeV");
        txtc->SetNDC(kTRUE);
        txtc->SetTextSize(0.05);
        txtc->SetTextFont(62);
        txtc->SetTextColor(1);

        TLatex* txtd = new TLatex(0.18, 0.66, "Ch.-particle anti-#it{k}_{T} jets");
        txtd->SetNDC(kTRUE);
        txtd->SetTextSize(0.04);
        txtd->SetTextFont(42);
        txtd->SetTextColor(1);
        
        TLatex* txte = new TLatex(0.18, 0.60, Form("%s GeV/#it{c} < #it{p}_{T}^{ch jet} < %s GeV/#it{c}",mini[i].Data(),maxi[i].Data()));
        txte->SetNDC(kTRUE); 
        txte->SetTextSize(0.04);
        txte->SetTextFont(42);
        txte->SetTextColor(1);
        
        TLatex* txtf = new TLatex(0.18, 0.54, "|#eta_{jet}| < 2.4, R = 0.4");
        txtf->SetNDC(kTRUE); 
        txtf->SetTextSize(0.04);
        txtf->SetTextFont(42);
        txtf->SetTextColor(1);    

        c[i]->cd();

        hDummy->Draw();

        hist1[i]->Draw("e0 SAME");
        hist2[i]->Draw("e0 SAME");

        txta->Draw("same");
        txtb->Draw("same");
        txtc->Draw("same");
        txtd->Draw("same");
        txte->Draw("same"); 
        txtf->Draw("same");

        legend->Draw("SAME");

        c[i]->Update();

        c[i]->SaveAs(Form("jetMassAll_jetPt%s.pdf",massRange[i].Data()));
    }
}
