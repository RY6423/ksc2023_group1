/*
  analize.C
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

void fitting(int file_n_pre) {
  //[0]..高さ、[1]..波数、[2]..幅、[3]..中心間隔、[4]..距離
  TF1* func = new TF1("func","4*[0]*TMath::Power(TMath::Sin([1]*[2]*TMath::Sin(TMath::Atan2(x,[4]))/2)/([1]*[2]*TMath::Sin(TMath::Atan2(x,[4]))/2)*TMath::Cos([1]*[3]*TMath::Sin(TMath::Atan2(x,[4]))/2)),2)",0,50);
  func->Draw();
}