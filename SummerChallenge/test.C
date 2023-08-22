/*
  test.C
*/
#include <iostream>
#include "TROOT.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1I.h"

void test(TH1I* t){
  TCanvas* canvas = new TCanvas("c","",0,0,600,400);
  double mean = 830.;

  //define fit function
  TF1 *func = new TF1("func","[0]*TMath::Gaus(x,[1],[2])");
  //set initial parameter 
  func->SetParameters(50,mean, 10);
  func->SetParNames("const","mean","sigma");

  //fit//
  t->Fit(func,"","",820,870);
  func->SetParameters(50, 9.11E+02, 10);
  t->Fit(func,"+","",860,940);
  func->SetParameters(50, 9.85E+02, 10);
  t->Fit(func,"+","",940,1020);
  func->SetParameters(50, 1.06E+03, 10);
  t->Fit(func,"+","",1020,1090);
  func->SetParameters(50, 1.13E+03, 10);
  t->Fit(func,"+","",1090,1170);
  func->SetParameters(50, 1.21E+03, 10);
  t->Fit(func,"+","",1170,1250);
  func->SetParameters(50, 1.28E+03, 10);
  t->Fit(func,"+","",1250,1320);
  func->SetParameters(50, 1.36E+03, 10);
  t->Fit(func,"+","",1320,1390);
  func->SetParameters(50, 1.43E+03, 10);
  t->Fit(func,"+","",1390,1470);
  func->SetParameters(50, 1.50E+03, 10);
  t->Fit(func,"+","",1470,1540);
  
  double meanFit = func->GetParameter(1);
  std::cout<<"mean = " <<meanFit<<std::endl;
}
