void fitAsym()
{
    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);
    gStyle->SetFuncWidth(2);

    TH1D *h1;
    TH1D *h2;
    TH1D *h3;
    TH1D *h4;
    TH1D *h1_init;
    TH1D *h2_init;
    const char *fp = "../../../data/4013.root";
    TFile *file = new TFile(fp);
    // parameters
    int i;
    int offset = -30;
    double alpha = 2.23;
    // BGs
    const double bg_u = 6.39559e+00;
    const double bg_d = 8.27831e+00;
    // hists
    file->GetObject("single_ch0000_0", h1);
    file->GetObject("single_ch0001_0", h2);
    h1_init = (TH1D *)h1->Clone();
    h2_init = (TH1D *)h2->Clone();

    // move h2 left for offset
    for (i = 0; i < h2->GetXaxis()->GetNbins(); i++)
    {
        if (offset + i < 0)
        {
            h2->SetBinContent(i, 0);
        }
        else
        {
            h2->SetBinContent(i, h2_init->GetBinContent(i + offset));
        }
    }

    // rebin
    h1->Rebin(100);
    h2->Rebin(100);

    // for error evaluation
    h1->Sumw2();
    h2->Sumw2();

    // delete BGs
    for (i = 0; i < h1->GetXaxis()->GetNbins(); i++)
    {
        h1->SetBinContent(i, h1->GetBinContent(i) - bg_u);
    }
    for (i = 0; i < h2->GetXaxis()->GetNbins(); i++)
    {
        h2->SetBinContent(i, h2->GetBinContent(i) - bg_d);
    }
    h3 = (TH1D *)h1->Clone();
    h4 = (TH1D *)h2->Clone();

    // h2 times alpha
    for (i = 0; i < h2->GetXaxis()->GetNbins(); i++)
    {
        h2->SetBinContent(i, h4->GetBinContent(i) * alpha);
        h2->SetBinError(i, h4->GetBinError(i) * alpha);
    }

    // D-U/D+U
    h3 = (TH1D *)h1->Clone();
    h4 = (TH1D *)h2->Clone();
    h4->Add(h3, -1);
    h2->Add(h1);
    h4->Divide(h2);

    // fitting by cos
    TF1 *f = new TF1("f", "[0] * cos(2 * TMath::Pi() * (x - [2]) / [1])");
    f->SetParNames("A", "T", "t_offset");
    f->SetParameters(.1, 3500, .0);
    f->SetParLimits(0, .01, .5);
    f->SetParLimits(1, 2500, 4500);
    f->SetParLimits(2, -500., 500.);
    h4->Fit("f", "", "", 300, 5000);

    h4->SetTitle("Asymmetry;time [ns];A");
    h4->Draw();
    h4->GetXaxis()->SetRangeUser(0, 10000);
    h4->GetYaxis()->SetRangeUser(-.4, .7);
}
