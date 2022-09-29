void cosFitStage1()
{
    TH1D *h1;
    TH1D *h2;
    TH1D *h3;
    TH1D *h4;
    TH1D *h1_init;
    TH1D *h2_init;
    const char *fp = "../../../data/4013.root";
    TFile *file = new TFile(fp);
    int offset = -30;
    int i, j;
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
            h2->SetBinContent(i, h2->GetBinContent(i + offset));
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

    // U-D/U+D
    h3 = (TH1D *)h1->Clone();
    h4 = (TH1D *)h2->Clone();
    h3->Add(h4, -1);
    h1->Add(h2);
    h3->Divide(h1);

    // fitting asymmetry
    TF1 *f1 = new TF1("f1","([0] - [1] * [2] * cos([3] * x)) / ([1] + [0] * [2] * cos([3] * x))");
    f1->SetParNames("U-D", "U+D", "AP", "#omega");
    f1->SetParameters(10,200,1,2*M_PI/0.03);
    h3->Fit("f1","","");
}
