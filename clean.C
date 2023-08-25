#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TROOT.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

std::string file_raw = "../data";

void clean(int filen_pre) {
  std::string file_n = std::to_string(file_n_pre);
  std::string file_ini = "ksc_"+std::string(std::max(0, 3 - (int)file_n.size()), '0') + file_n;

  int number=0;
  while(true) {
    std::cout << number << std::endl;

    //データファイルオープン
    std::string file;
    std::string s_i = std::to_string(number);
    s_i = std::string(std::max(0, 4 - (int)s_i.size()), '0') + s_i;
    file = file_raw + "/data/" + file_ini + "/" + file_ini + "_" + s_i + ".root";
    TFile* tf = TFile::Open(file.c_str());
    if(!tf) break;

    //コピー
    TH1F* th1 = (TH1F*)tf->Get("ADC_HIGH_0");
    TH1F* th1_copy = th1->Clone()

    //書き込みファイルオープン
    mkdir(("out/"+file_ini).c_str(),0777);
    std::string file_w =  "data/" + file_ini + "/" + file_ini + "_" + s_i + ".root";
    TFile* result = new TFile(file_w.c_str(), "recreate");

    if(!th1) continue;

    //書き込み
    th1->Write();
    result->Write();
    result->Close();
    number++;
  }
}