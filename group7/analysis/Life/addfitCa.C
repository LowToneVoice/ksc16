void addfitCa()
{
    const char *fp4011 = "../../data/4011.root";
    const char *fp4012 = "../../data/4012.root";
    const char *fp4013 = "../../data/4013.root";
    TFile *file4011 = new TFile(fp4011);
    TFile *file4012 = new TFile(fp4012);
    TFile *file4013 = new TFile(fp4013);
    TH1D *hu;
    TH1D *hd;
    TH1D *hu4012;
    TH1D *hd4012;
    TH1D *hu4013;
    TH1D *hd4013;

    // styles of canvas
    gStyle->SetOptStat(1111); // statistics of hist: RMS, average, # of data, hist name
    gStyle->SetOptFit(1111);  // statistics of fitting curve: Prob, χ2/Ndof, error, par names and values
    gStyle->SetFuncWidth(4);  // width of fitting curve
    gStyle->SetFuncColor(2);  // color of fitting curve

    // get hists for each channel
    file4011->GetObject("single_ch0002_0", hu);
    file4011->GetObject("single_ch0003_0", hd);
    file4012->GetObject("single_ch0002_0", hu4012);
    file4012->GetObject("single_ch0003_0", hd4012);
    file4013->GetObject("single_ch0002_0", hu4013);
    file4013->GetObject("single_ch0003_0", hd4013);
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    hu->Add(hu4012, 1);
    hd->Add(hd4012, 1);
    hu->Add(hu4013, 1);
    hd->Add(hd4013, 1);
    hu->Add(hd, 1);
    // configure the bin of hist
    hu->Rebin(100);
    // hist range
    hu->GetXaxis()->SetRangeUser(0, 20000);
    hu->GetYaxis()->SetRangeUser(0.1, 300);
    hu->Draw();

    // FITTING CURVE
    TF1 *f1 = new TF1("f1", "[ 0 ] * exp( -(x / [ 1 ])) + [ 2 ] * exp( -(x / [ 3 ])) +  [ 4 ] ");
    // initializing parameters
    f1->SetParameters(150, 2000.0, 120, 900.0, 2.0);
    // names of parameters
    f1->SetParNames("N_{+}", "#tau_{+}", "N_{-}", "#tau_{-}", "BG");
    // fit the curve on the hist
    hu->Fit("f1", "", "", 100, 20000);
    // width of the fitting curve
    hu->SetLineWidth(2.0);
    // y axis to log scale
    c1->SetLogy();
    // labels of the graph and the axis
    hu->SetTitle("CaCO3 51h;time [ns];count;");
}
