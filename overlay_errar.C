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

void overlay(int filename1,int filename2){
    char filepath1[100];
    char filepath2[100];
    sprintf(filepath1,"out/ksc_%03d/ksc_%03d_analize.root",filename1,filename1);
    sprintf(filepath2,"out/ksc_%03d/ksc_%03d_analize.root",filename2,filename2);
    TFile *tf1 = TFile::Open(filepath1);
    TFile *tf2 = TFile::Open(filepath2);
    auto graph1 = dynamic_cast<TGraph*>(tf1->Get("Graph"));
    auto graph2 = dynamic_cast<TGraph*>(tf2->Get("Graph"));
    graph1->SetMarkerColor(2);
    graph1->SetMarkerStyle(20);
    graph2->SetMarkerColor(4);
    graph2->SetMarkerStyle(20);
    TLegend *legend = new TLegend( 0.65, 0.68, 0.84, 0.78) ; //（）の中は位置の指定（左下の x , y 、右上の x , y ）
    legend->AddEntry( graph1, "double-slit open" , "p") ; // AddEntry( pointer , "interpretation" , "option" )
    legend->AddEntry( graph2, "double-slit close" , "p") ; // option は　"f"=box, "l"="L"=line, "p"=marker
    legend->SetFillColor(0);
    
    graph1->Draw("ap");
    graph2->Draw("samep");
    legend->Draw() ;
}