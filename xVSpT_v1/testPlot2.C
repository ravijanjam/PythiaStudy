
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <TProfile.h>
#include <TFile.h>

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

void testPlot2(vector<string> hqn, vector<string> hgn, vector<string> han){
//void testPlot2(vector<string> hqn){


	for (auto hh : {hqn, hgn, han}){
		int i=0;
		for (auto ss : hh){
			cout << i << "\t" << ss << endl;
			i++;
		}//close:for
		cout << "\n";
	}//close:for

	map<string, TFile*> mfile;
	for (auto ss : hqn){

		mfile["HardQCDAll_New"] = new TFile(ss.c_str());

	}//close:for


	/*
	cout << hqn.at(0) << endl;

	map<string, TFile*> mfile;
	cout << mfile["HardQCDAll_New"]->GetName();


	map<string, TFile*> mfile;
	map<string, map<string, TProfile*>> mp[cType];
	TLegend *leg = new TLegend(0.9, 0.8, 0.7, 0.6);

	string cType = "HardQCDAll";
	mfile[cType] = new TFile(Form("%s/ptHatCombined_BoundAndFreeProton_EPPS16_CT14nlo_2018-06-07-221932.root ", cType.c_str()));

	for (string ss: {"prof_x1Pos", "prof_x1Neg", "prof_x2Pos", "prof_x2Neg"}){
		mp[cType][ss] = (TProfile*)mfile["HardQCDAll"]->Get(ss.c_str());
	}


	applyCosmetics(mp[cType]["prof_x1Pos"], kRed, 20, 1.5);
	applyCosmetics(mp[cType]["prof_x2Pos"], kRed, 22, 1.5);

	applyCosmetics(mp[cType]["prof_x1Neg"], kBlue, 20, 1.5);
	applyCosmetics(mp[cType]["prof_x2Neg"], kBlue, 22, 1.5);


	leg->AddEntry(mp[cType]["prof_x1Pos"], "x1 vs pT(+)", "lep");
	leg->AddEntry(mp[cType]["prof_x2Pos"], "x2 vs pT(+)", "lep");

	leg->AddEntry(mp[cType]["prof_x1Neg"], "x1 vs pT(-)", "lep");
	leg->AddEntry(mp[cType]["prof_x2Neg"], "x2 vs pT(-)", "lep");

	mp[cType]["prof_x1Pos"]->Draw("same");
	mp[cType]["prof_x2Pos"]->Draw("same");

	mp[cType]["prof_x1Neg"]->Draw("same");
	mp[cType]["prof_x2Neg"]->Draw("same");

	leg->Draw("same");

	gPad->SetLogy();
	gPad->SetGrid();

	*/


}//close:main
