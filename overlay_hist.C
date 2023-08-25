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
#include "TLegend.h"

void overlay_hist(int filenum1,int filenum2,int filenum3,int filenum4){
    char filepath1[100];
    char filepath2[100];
    sprintf(filepath1,"data/ksc_%03d/ksc_%03d_%04d.root",filenum1,filenum1,filenum2);
    sprintf(filepath2,"data/ksc_%03d/ksc_%03d_%04d.root",filenum3,filenum3,filenum4);
    TFile *tf1 = TFile::Open(filepath1);
    TFile *tf2 = TFile::Open(filepath2);
    auto hist1 = dynamic_cast<TH1*>(tf1->Get("ADC_HIGH_0"));
    auto hist2 = dynamic_cast<TH1*>(tf2->Get("ADC_HIGH_0"));
    hist1->SetMarkerColor(2);
    hist1->SetLineColor(2);
    hist2->SetMarkerColor(4);
    hist2->SetLineColor(4);
    TLegend *legend = new TLegend( 0.65, 0.68, 0.84, 0.78) ; //（）の中は位置の指定（左下の x , y 、右上の x , y ）
    legend->AddEntry( hist1, filepath1 , "l") ; // AddEntry( pointer , "interpretation" , "option" )
    legend->AddEntry( hist2, filepath2 , "l") ; // option は　"f"=box, "l"="L"=line, "p"=marker
    legend->SetFillColor(0);
    
    hist1->Draw();
    hist2->Draw("sames");
    legend->Draw() ;
}