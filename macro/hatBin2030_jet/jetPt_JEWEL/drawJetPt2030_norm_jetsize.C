void drawJetPt2030_norm_evt() {

    gStyle->SetOptStat(0);

    TFile* f1 = TFile::Open("/home/hyejilee/5jet/JetPt2030_1_norm_evt.root");
    TFile* f2 = TFile::Open("/home/hyejilee/HepMC3toRoot/jetResult/jetPt_AuAu_KR_T_350_502GeV_CENT_0_10_1.0_norm_evt.root");

    TH1F* hist1 = (TH1F*)f1->Get("jetPT");
    TH1F* hist2 = (TH1F*)f2->Get("jetPT");

    TCanvas* c1 = new TCanvas("jetPT", "", 800, 550);
    gStyle->SetPadBottomMargin(0.1);
    gStyle->SetPadLeftMargin(0.1);
    gStyle->SetPadRightMargin(0.01);
    gStyle->SetPadTopMargin(0.01);

    TH1F* hDummy = new TH1F("hDummy", "", 250, 0, 200);
    hDummy->SetTitle("");
    hDummy->GetXaxis()->SetTitle("#it{p}_{T_jet} (GeV/#it{c})");
    hDummy->GetYaxis()->SetTitle("1/#it{N}_{event} d#it{N}_{jet}/d#it{p}_{T_jet}");
    hDummy->GetXaxis()->SetTitleSize(0.04);
    hDummy->GetXaxis()->SetTitleOffset(1);
    hDummy->GetYaxis()->SetTitleSize(0.04);
    hDummy->GetYaxis()->SetTitleOffset(1);
    hDummy->GetYaxis()->SetRangeUser(0.00000000001, 5);
    hDummy->GetXaxis()->SetRangeUser(0.00000000001, 100);

    hist1->SetLineColor(kRed);
    hist1->SetLineWidth(2);
    hist1->Scale(1.0/hist1->GetXaxis()->GetBinWidth(1));
    
    hist2->SetLineColor(kBlue);
    hist2->SetLineWidth(2);
    hist2->Scale(1.0/hist2->GetXaxis()->GetBinWidth(1));

    TLegend* legend = new TLegend(0.7, 0.75, 0.85, 0.85);
    legend->SetFillColor(4000);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.05);
    legend->AddEntry(hist1, "Pythia - pp", "lpf");
    legend->AddEntry(hist2, "Jewel - AA", "lpf");

    c1->cd();

    hDummy->Draw();
    hist1->Draw("same");
    hist2->Draw("same");
    legend->Draw("same");

    c1->Update();

    c1->SaveAs("jetpt_norm_evt.pdf");
}

