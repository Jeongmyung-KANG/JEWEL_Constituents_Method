void drawJetMass2030_norm_jetsize() {

    std::cout.precision(6);

    gStyle->SetOptStat(0);

    TFile* f1 = TFile::Open("/home/hyejilee/5jet/JetMass2030_1_norm_jetsize.root");
    TFile* f2 = TFile::Open("/home/hyejilee/HepMC3toRoot/jetResult/jetMass_AuAu_KR_T_350_502GeV_CENT_0_10_1.0_norm_jetsize.root");

    TH1F* hist1 = (TH1F*)f1->Get("jetMass");
    TH1F* hist2 = (TH1F*)f2->Get("jetMass");

    TCanvas* c1 = new TCanvas("jetMass_AA_pp", "jetMass_AA_pp", 800, 550);
    gStyle->SetPadBottomMargin(0.1);
    gStyle->SetPadLeftMargin(0.1);
    gStyle->SetPadRightMargin(0.1);
    gStyle->SetPadTopMargin(0.1);

    TPad *pad1 = new TPad("pad1", "First Pad", 0.1, 0.5, 0.9, 0.9);
    //pad1->SetFillColor(kBlue);
    pad1->Draw();

    TPad *pad2 = new TPad("pad2", "Second Pad", 0.1, 0.1, 0.9, 0.5);
    //pad2->SetFillColor(kRed);
    pad2->Draw();

    pad1->cd();

    TH1F* hDummy = new TH1F("hDummy", "", 250, 0, 200);
    hDummy->SetTitle("");
    hDummy->GetXaxis()->SetTitle("#it{M}_{jet} (GeV/#it{c}^{2})");
    hDummy->GetYaxis()->SetTitle("1/#it{N}_{jet} d#it{N}_{jet}/d#it{M}_{jet}");
    hDummy->GetXaxis()->SetTitleSize(0.04);
    hDummy->GetXaxis()->SetTitleOffset(1);
    hDummy->GetYaxis()->SetTitleSize(0.04);
    hDummy->GetYaxis()->SetTitleOffset(1);
    hDummy->GetYaxis()->SetRangeUser(0.00000000001, 0.15);
    hDummy->GetXaxis()->SetRangeUser(0.00000000001, 30);

    hist1->SetMarkerColor(kRed);
    hist1->SetMarkerStyle(20);
    //hist1->SetLineColor(kRed);
    //hist1->SetLineWidth(2);
    hist1->Scale(1.0/hist1->GetXaxis()->GetBinWidth(1));

    hist2->SetMarkerColor(kBlue);
    hist2->SetMarkerStyle(20);
    //hist2->SetLineColor(kBlue);
    //hist2->SetLineWidth(2);
    hist2->Scale(1.0/hist2->GetXaxis()->GetBinWidth(1));

    TLegend* legend = new TLegend(0.6, 0.6, 0.75, 0.75);
    legend->SetFillColor(4000);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.05);
    legend->AddEntry(hist1, " PYTHIA8", "lpf");
    legend->AddEntry(hist2, " JEWEL", "lpf");

    TLatex* txta = new TLatex(0.18, 0.84, "PYTHIA 8310, #it{pp}");
    txta->SetNDC(kTRUE);
    txta->SetTextSize(0.05);
    txta->SetTextFont(62);
    txta->SetTextColor(1);
    
    TLatex* txtb = new TLatex(0.18, 0.78, "JEWEL-2.3.0, AA 0-10%");
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
    
    TLatex* txte = new TLatex(0.18, 0.60, "40 GeV/#it{c} < #it{p}_{T}^{ch jet} < 60 GeV/#it{c}");
    txte->SetNDC(kTRUE); 
    txte->SetTextSize(0.04);
    txte->SetTextFont(42);
    txte->SetTextColor(1);
    
    TLatex* txtf = new TLatex(0.18, 0.54, "|#eta_{jet}| < 2.4, R = 0.4");
    txtf->SetNDC(kTRUE); 
    txtf->SetTextSize(0.04);
    txtf->SetTextFont(42);
    txtf->SetTextColor(1);
    
    //c1->cd();

    hDummy->Draw();

    hist1->Draw("SAME");
    hist2->Draw("SAME");

    txta->Draw("same");
    txtb->Draw("same");
    txtc->Draw("same");
    txtd->Draw("same");
    txte->Draw("same"); 
    txtf->Draw("same");

    legend->Draw("SAME");

    c1->Update();

    //ratio
    //TCanvas* c2 = new TCanvas("jetMass_ratio_AA_pp","jetMass_AA_pp", 800, 550);
    
    pad2->cd();

    TH1F *hist3 = (TH1F*)hist2->Clone("ratio"); //clone AA
    hist3->SetTitle("jetMass_ratio_AA_pp");

    Double_t threshold = 0.0001;
    for (int i = 1; i <= hist3->GetNbinsX(); ++i) {
        if (hist3->GetBinContent(i) < threshold) {
            hist3->SetBinContent(i, 0);
        }
    }

    for (int i = 1; i <= hist1->GetNbinsX(); ++i) {
        if (hist1->GetBinContent(i) < threshold) {
            hist1->SetBinContent(i, 0);
        }
    }

    hist3->Scale(1.0/hist3->GetXaxis()->GetBinWidth(1));

    hist3->Divide(hist3 , hist1 , 1.0 , 1.0);
    hist3->GetYaxis()->SetRangeUser(0.00000000001, 10);
    hist3->GetXaxis()->SetRangeUser(0.00000000001, 30);

    //c2->cd();
    
    hist3->Draw();

    //c2->Update();

    c1->SaveAs("jetMass_JEWEL_AA_PYTHIA_PP.pdf");
}
