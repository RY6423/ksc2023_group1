/*
  delete.C
*/
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

void delete(int file_n_pre) {
  std::string file_n = std::to_string(file_n_pre);
  std::string file_ini = "ksc_"+std::string(std::max(0, 3 - (int)file_n.size()), '0') + file_n;
  int number=0;
  while(true) {
    std::cout << number << " ";
    std::string file;
    std::string s_i = std::to_string(number);
    s_i = std::string(std::max(0, 4 - (int)s_i.size()), '0') + s_i;
    file = "data/" + file_ini + "/" + file_ini + "_" + s_i + ".root";
    TFile* tf = TFile::Open(file.c_str());
    if(!tf) break;
    TH1F* th1 = (TH1F*)tf->Get("ADC_HIGH_0");
    if(!th1){
      error_n++;
      number++;
    }
    if(!th1) continue;
    const int sensor_n=64;
    for(int i=0;i<sensor_n;i++){
      tf->FindKey(("ADC_HIGH_"+std::to_string(i)).c_str())->Delete()
    }
    number++;
  }
}