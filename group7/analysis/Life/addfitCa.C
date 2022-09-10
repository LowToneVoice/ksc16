void addfitCa()
{
    const char *fp4011 = "4011.root";
    const char *fp4012 = "4012.root";
    const char *fp4013 = "4013.root";
    TFile *file4011 = new TFile(fp4011);
    TFile *file4012 = new TFile(fp4012);
    TFile *file4013 = new TFile(fp4013);
    TH1D *hu;
    TH1D *hd;
    TH1D *hu4012;
    TH1D *hd4012;
    TH1D *hu4013;
    TH1D *hd4013;

    gStyle->SetOptStat(1111); // ヒストグラムの統計情報表示。順にRMS・平均値・データ数・ヒストグラム名
    gStyle->SetOptFit(1111);  // 近似曲線の統計情報表示。順に確率・χ2/Ndof・エラー・パラメーター名と値
    gStyle->SetFuncWidth(4);  // 近似曲線の太さ
    gStyle->SetFuncColor(2);  // 近似曲線の色。1は黒。

    // チャンネルのhistを作成
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
    // histのbin数を設定
    hu->Rebin(100);
    // histの軸の最小・最大
    hu->GetXaxis()->SetRangeUser(0, 20000);
    hu->GetYaxis()->SetRangeUser(0.1, 300);
    // ヒストグラム描画
    hu->Draw();

    // 近似曲線
    // [ # ] をパラメーターとして、chi^2を最小化するように最適化してくれるよ！すごいっ！！
    TF1 *f1 = new TF1("f1", "[ 0 ] * exp( -(x / [ 1 ])) + [ 2 ] * exp( -(x / [ 3 ])) +  [ 4 ] ");
    // 最適化しやすいようにパラメーターを初期化させよう！
    f1->SetParameters(150, 2000.0, 120, 900.0, 2.0);
    //  パラメーターの名前を[ 0 ]から順番につけるよ！
    f1->SetParNames("N_{+}", "#tau_{+}", "N_{-}", "#tau_{-}", "BG");
    // histに曲線をフィットさせるぜ！
    hu->Fit("f1", "", "", 100, 20000);
    // 近似曲線の太さ
    hu->SetLineWidth(2.0);
    // y axis to log scale
    c1->SetLogy();
    // グラフと軸に表示するラベル
    hu->SetTitle("CaCO3 51h;time [ns];count;");
}
