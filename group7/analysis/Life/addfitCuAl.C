void addfitCuAl()
{
    const char *fp4011 = "../../data/4011.root";
    TFile *file4011 = new TFile(fp4011);
    TH1D *hu;
    TH1D *hd;

    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);
    gStyle->SetFuncWidth(4);
    gStyle->SetFuncColor(2);

    // get hists
    file4011->GetObject("single_ch0000_0", hu);
    file4011->GetObject("single_ch0001_0", hd);
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

    hu->Add(hd, 1);
    hu->Rebin(50);

    TF1 *f1 = new TF1("f1", "[ 0 ] * exp( -x / [ 1 ]) + [ 2 ] * exp( -(x + [ 3 ]) / [ 4 ]) +  [ 5 ] ");
    f1->SetParameters(100, 2200.0, 100, 0, 2100.0, 0.0);
    f1->SetParNames("N_{+}", "#tau_{+}", "N_{-}", "td_0", "#tau_{-}", "BG");
    hu->Fit("f1", "", "", 100, 20000);

    hu->GetXaxis()->SetRangeUser(0, 20000);
    hu->GetYaxis()->SetRangeUser(1, 1000);
    hu->SetLineWidth(2.0);
    // y axis to log scale
    c1->SetLogy();
    hu->SetTitle("Cu+Al 18h;time [ns];count;");
    hu->Draw();
}
