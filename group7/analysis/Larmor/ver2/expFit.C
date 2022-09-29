void expFit()
{
    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(0000);
    gStyle->SetFuncWidth(2);

    const char *fp = "../../../data/4013.root";
    TFile *file = new TFile(fp);
    TH1D *hu;
    TH1D *hd;
    TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
    file->GetObject("single_ch0000_0", hu);
    file->GetObject("single_ch0001_0", hd);
    hu->Rebin(100);
    hd->Rebin(100);

    // fitting curve
    TF1 *fu = new TF1("fu", "[0] * exp(-x / [1]) + [2]");
    TF1 *fd = new TF1("fd", "[0] * exp(-x / [1]) + [2]");
    fu->SetParameters(200.0, 2000.0, 0.0);
    fu->SetParNames("N_U", "#tau_U", "BG_U");
    fd->SetParameters(100.0, 2000.0, 0.0);
    fd->SetParNames("N_D", "#tau_D", "BG_D");
    hu->Fit("fu", "", "", 300, 20000);
    hu->SetLineWidth(2.0);
    hd->Fit("fd", "", "", 300, 20000);
    hd->SetLineWidth(2.0);

    // graph and axis label
    hu->SetTitle("Cu, Al with B 33h;time [ns];count;");
    hd->SetTitle("Cu, Al with B 33h;time [ns];count;");
    hd->Draw();
    hd->SetLineColor(kRed);
    hd->GetXaxis()->SetRangeUser(0, 20000);
    hd->GetYaxis()->SetRangeUser(0.9, 500);
    hu->Draw("same");
    hu->GetXaxis()->SetRangeUser(0, 20000);
    hu->GetYaxis()->SetRangeUser(0.9, 500);
    // y axis to log scale
    c1->SetLogy();
}
