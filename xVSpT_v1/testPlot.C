#include <iostream>

using namespace std;


TProfile* applyCosmetics(TProfile* hp, Color_t mCol, int mStyle, float mSize )
{

	        hp->SetLineColor(mCol);
		hp->SetMarkerStyle(mStyle);
	        hp->SetMarkerColor(mCol);
		hp->SetMarkerSize(mSize);
		hp->SetStats(0);
        
		return hp;

}//close-applyCosmetics

void testPlot(){

	map<string, TFile*> mfile;
	map<string, TProfile*> mp;
	TLegend *leg = new TLegend(0.9, 0.8, 0.7, 0.6);

	mfile["HardQCDAll"] = new TFile("HardQCDAll/ptHatCombined_BoundAndFreeProton_EPPS16_CT14nlo_2018-06-07-221932.root ");

	for (string ss: {"prof_x1Pos", "prof_x1Neg", "prof_x2Pos", "prof_x2Neg"}){
		mp[ss] = (TProfile*)mfile["HardQCDAll"]->Get(ss.c_str());
	}


	applyCosmetics(mp["prof_x1Pos"], kRed, 20, 1.5);
	applyCosmetics(mp["prof_x2Pos"], kRed, 22, 1.5);

	applyCosmetics(mp["prof_x1Neg"], kBlue, 20, 1.5);
	applyCosmetics(mp["prof_x2Neg"], kBlue, 22, 1.5);


	leg->AddEntry(mp["prof_x1Pos"], "x1 vs pT(+)", "lep");
	leg->AddEntry(mp["prof_x2Pos"], "x2 vs pT(+)", "lep");

	leg->AddEntry(mp["prof_x1Neg"], "x1 vs pT(-)", "lep");
	leg->AddEntry(mp["prof_x2Neg"], "x2 vs pT(-)", "lep");

	mp["prof_x1Pos"]->Draw("same");
	mp["prof_x2Pos"]->Draw("same");

	mp["prof_x1Neg"]->Draw("same");
	mp["prof_x2Neg"]->Draw("same");

	leg->Draw("same");

	gPad->SetLogy();
	gPad->SetGrid();




}//close:main
