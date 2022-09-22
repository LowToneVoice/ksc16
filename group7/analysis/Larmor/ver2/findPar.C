void findPar()
{
    TH1D *h1;
    TH1D *h2;
    TH1D *h3;
    TH1D *h4;
    TH1D *h1_init;
    TH1D *h2_init;
    const char *fp = "../../../data/4013.root";
    TFile *file = new TFile(fp);
    // parameters
    int i, j, k;
    int offset = -30;
    double alpha;
    // results
    Double_t erfun;
    Double_t ndf;
    Double_t prob;
    // BGs
    const double bg_u = 6.39559e+00;
    const double bg_d = 8.27831e+00;
    // output files
    std::ofstream outputfile_chi2("param_chi2.dat");
    std::ofstream outputfile_prob("param_prob.dat");
    // hists
    file->GetObject("single_ch0000_0", h1);
    file->GetObject("single_ch0001_0", h2);
    h1_init = (TH1D *)h1->Clone();
    h2_init = (TH1D *)h2->Clone();

    for (i = 0; i < 500; i++)
    {
        alpha = 0.1 + i * 0.01;

        h1 = (TH1D *)h1_init->Clone();
        h2 = (TH1D *)h2_init->Clone();

        // move h2 left for offset
        for (k = 0; k < h2->GetXaxis()->GetNbins(); k++)
        {
            if (offset + k < 0)
            {
                h2->SetBinContent(k, 0);
            }
            else
            {
                h2->SetBinContent(k, h2_init->GetBinContent(k + offset));
            }
        }

        // rebin
        h1->Rebin(100);
        h2->Rebin(100);

        // for error evaluation
        h1->Sumw2();
        h2->Sumw2();

        // delete BGs
        for (k = 0; k < h1->GetXaxis()->GetNbins(); k++)
        {
            h1->SetBinContent(k, h1->GetBinContent(k) - bg_u);
        }
        for (k = 0; k < h2->GetXaxis()->GetNbins(); k++)
        {
            h2->SetBinContent(k, h2->GetBinContent(k) - bg_d);
        }
        h3 = (TH1D *)h1->Clone();
        h4 = (TH1D *)h2->Clone();

        // h2 times alpha
        for (k = 0; k < h2->GetXaxis()->GetNbins(); k++)
        {
            h2->SetBinContent(k,  (h4->GetBinContent(k) * alpha));
            h2->SetBinError(k, (h4->GetBinError(k) * alpha));
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

        // output chi^2/ndf and prob
        erfun = h4->GetFunction("f")->GetChisquare();
        ndf = h4->GetFunction("f")->GetNDF();
        prob = h4->GetFunction("f")->GetProb();
        outputfile_chi2 << alpha << " " << erfun / ndf << " ";
        outputfile_prob << alpha << " " << prob << " ";

        // change the lines of
        outputfile_chi2 << endl;
        outputfile_prob << endl;
    }
    outputfile_chi2.close();
    outputfile_prob.close();
}
