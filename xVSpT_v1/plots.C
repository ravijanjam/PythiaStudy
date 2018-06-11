
#include <iostream>
#include <TProfile.h>
#include <TFile.h>
#include <string>
#include <map>


/* Normalizing Variable Binned Histograms */
TH1D* normalizeVarBinHist (TH1D *hVarPt, double nVarBins)
{

	double lbinWidth, lbinContent, lbinError;	

	for (int tBin=1; tBin < nVarBins; tBin++){

		lbinWidth = hVarPt->GetBinWidth(tBin);
		lbinContent = hVarPt->GetBinContent(tBin);
		lbinError = hVarPt->GetBinError(tBin);
		hVarPt->SetBinContent(tBin, lbinContent/lbinWidth);
		hVarPt->SetBinError(tBin, lbinError/lbinWidth);

		/*
		cout << "hVar (binContent, binError) : "  << "\t"
		     << hVarPt->GetBinContent(tBin) << "\t"
		     << hVarPt->GetBinError(tBin) << "\t" << "\n";
		*/

	}

	/*
	cout << "\n\n";
	cout << "============= normalizeVarBinHist : Executed successfully =============" << endl;
	cout << "Number of bins in the rebinned histogram: " << hVarPt->GetSize() << endl;
	cout << "File used for normalizing: " << hVarPt->GetName() << endl;
	cout << "=======================================================================" << endl;
	cout << "\n\n";
	*/

	return hVarPt;

}//close-normalizeVarBinHist


TProfile* applyCosmetics(TProfile* hp, Color_t mCol, int mStyle, float mSize )
{

	        hp->SetLineColor(mCol);
		hp->SetMarkerStyle(mStyle);
	        hp->SetMarkerColor(mCol);
		hp->SetMarkerSize(mSize);
		hp->SetStats(0);
        
		return hp;

}//close-applyCosmetics


TH1D* applyCosmetics(TH1D* hp, Color_t mCol, int mStyle, float mSize )
{

	        hp->SetLineColor(mCol);
		hp->SetMarkerStyle(mStyle);
	        hp->SetMarkerColor(mCol);
		hp->SetMarkerSize(mSize);
		hp->SetStats(0);
        
		return hp;

}//close-applyCosmetics

void plots(){

	map<string, map<string, TFile*>> mfile;
	map<string, map<string, map<string, TProfile*>>> mprof;
	map<string, map<string, map<string, TH1D*>>> mh;
	map<string, vector<string>> mv;

	/* file names here */
	mv["Quark"] = {

		"HardQCDQuark/ptHatCombined_BoundAndFreeProton_EPPS16_CT14nlo_2018-06-10-171447.root",
		"HardQCDQuark/ptHatCombined_BoundProton_EPPS16_CT14nlo_2018-06-08-180204.root",
		"HardQCDQuark/ptHatCombined_FreeProton_CT14nlo_2018-06-08-180115.root"

	};

	mv["Gluon"] = {

		"HardQCDGluon/ptHatCombined_BoundAndFreeProton_EPPS16_CT14nlo_2018-06-09-102951.root",
		"HardQCDGluon/ptHatCombined_BoundProton_EPPS16_CT14nlo_2018-06-08-131516.root",
		"HardQCDGluon/ptHatCombined_FreeProton_CT14nlo_2018-06-08-131232.root"

	};

	mv["All"] = {

		"HardQCDAll/ptHatCombined_BoundAndFreeProton_EPPS16_CT14nlo_2018-06-10-232002.root",
		"HardQCDAll/ptHatCombined_BoundProton_EPPS16_CT14nlo_2018-06-10-232617.root",
		"HardQCDAll/ptHatCombined_FreeProton_CT14nlo_2018-06-10-232302.root"
	};
	

	// Read the files
	for (auto ss : {"Quark", "Gluon", "All"}){

		cout << ss << "\t\n";
		for (auto ptyp : {"hPos", "hNeg"}){

			cout << ptyp << endl;
			mfile[ss]["bf"] = new TFile(mv[ss].at(0).c_str());
			mh[ss]["bf"][ptyp] = (TH1D*)mfile[ss]["bf"]->Get(ptyp);

			mfile[ss]["bb"] = new TFile(mv[ss].at(1).c_str());
			mh[ss]["bb"][ptyp] = (TH1D*)mfile[ss]["bb"]->Get(ptyp);

			mfile[ss]["ff"] = new TFile(mv[ss].at(2).c_str());
			mh[ss]["ff"][ptyp] = (TH1D*)mfile[ss]["ff"]->Get(ptyp);


		}//close:for

		for (auto ptyp : {"prof_x1Pos", "prof_x2Pos", "prof_x1Neg", "prof_x2Neg", "hPos", "hNeg"}){

			cout << ptyp << endl;
			mfile[ss]["bf"] = new TFile(mv[ss].at(0).c_str());
			mprof[ss]["bf"][ptyp] = (TProfile*)mfile[ss]["bf"]->Get(ptyp);
			cout << "file Name: " << mv[ss].at(0).c_str() << endl;

			mfile[ss]["bb"] = new TFile(mv[ss].at(1).c_str());
			mprof[ss]["bb"][ptyp] = (TProfile*)mfile[ss]["bb"]->Get(ptyp);
			cout << "file Name: " << mv[ss].at(1).c_str() << endl;

			mfile[ss]["ff"] = new TFile(mv[ss].at(2).c_str());
			mprof[ss]["ff"][ptyp] = (TProfile*)mfile[ss]["ff"]->Get(ptyp);
			cout << "file Name: " << mv[ss].at(2).c_str() << endl;

		}//close:for
	}//close:for
	

	// (x1 or x2) vs pT : Pos vs Neg based on process type
	if (0){

		string mt, qt, ct;
		qt = "Quark"; // Gluon, Quark, All
		ct = "x1"; // x1, x2
		mt = "bf"; // bf, ff, bb

		map<int, TProfile*> ptemp ;
		TLegend *leg = new TLegend(0.9, 0.8, 0.7, 0.6);
		leg->SetHeader("x1 vs pT, Positive");

		ptemp[1] = mprof[qt][mt][Form("prof_%sPos", ct.c_str())];
		applyCosmetics(ptemp[1], kRed, 20, 1.5);
		leg->AddEntry(ptemp[1], Form("%s:pPb, Pos", qt.c_str()), "lep");
		ptemp[1]->Draw("same");

		ptemp[2] = mprof[qt][mt][Form("prof_%sNeg", ct.c_str())];
		applyCosmetics(ptemp[2], kBlue, 20, 1.5);
		leg->AddEntry(ptemp[2], Form("%s:pPb, Neg", qt.c_str()), "lep");
		ptemp[2]->Draw("same");


		leg->Draw("same");
		gPad->SetLogy();
		gPad->SetGrid();
	}



	// (x1 or x2) vs pT : Pos vs Neg based on process type
	if (0){

		string mt, qt, ct;
		qt = "Gluon"; // Gluon, Quark, All
		ct = "x1"; // x1, x2
		mt = "bf"; // bf, ff, bb

		map<int, TProfile*> ptemp ;
		TLegend *leg = new TLegend(0.9, 0.8, 0.7, 0.6);
		leg->SetHeader("x1 vs pT, Positive");

		ptemp[1] = mprof[qt][mt][Form("prof_%sPos", ct.c_str())];
		applyCosmetics(ptemp[1], kRed, 20, 1.5);
		leg->AddEntry(ptemp[1], Form("%s:pPb, Pos", qt.c_str()), "lep");
		ptemp[1]->Draw("same");

		ptemp[2] = mprof[qt][mt][Form("prof_%sNeg", ct.c_str())];
		applyCosmetics(ptemp[2], kBlue, 20, 1.5);
		leg->AddEntry(ptemp[2], Form("%s:pPb, Neg", qt.c_str()), "lep");
		ptemp[2]->Draw("same");


		leg->Draw("same");
		gPad->SetLogy();
		gPad->SetGrid();
	}


	// (x1 or x2) vs pT : Pos vs Neg based on process type
	if (0){

		string mt, qt, ct;
		qt = "All"; // Gluon, Quark, All
		ct = "x1"; // x1, x2
		mt = "bf"; // bf, ff, bb

		map<int, TProfile*> ptemp ;
		TLegend *leg = new TLegend(0.9, 0.8, 0.7, 0.6);
		leg->SetHeader("x1 vs pT, Positive");

		ptemp[1] = mprof[qt][mt][Form("prof_%sPos", ct.c_str())];
		applyCosmetics(ptemp[1], kRed, 20, 1.5);
		leg->AddEntry(ptemp[1], Form("%s:pPb, Pos", qt.c_str()), "lep");
		ptemp[1]->Draw("same");

		ptemp[2] = mprof[qt][mt][Form("prof_%sNeg", ct.c_str())];
		applyCosmetics(ptemp[2], kBlue, 20, 1.5);
		leg->AddEntry(ptemp[2], Form("%s:pPb, Neg", qt.c_str()), "lep");
		ptemp[2]->Draw("same");


		leg->Draw("same");
		gPad->SetLogy();
		gPad->SetGrid();

	}



	// (x1 or x2) vs pT for Gluons, Quarks and All, positive or negative
	if (1){

		string qt, ct;
	        qt = "All"; // Gluon, Quark, All

		// prof_x1Pos, prof_x1Neg
		ct = "prof_x1Pos";
	

		map<int, TProfile*> ptemp ;
		TLegend *leg = new TLegend(0.9, 0.8, 0.7, 0.6);
		leg->SetHeader("x1 vs pT, Positive");

		ptemp[1] = mprof[qt]["bf"][ct];
		applyCosmetics(ptemp[1], kRed, 20, 1.5);
		leg->AddEntry(ptemp[1], Form("%s:pPb", qt.c_str()), "lep");
		ptemp[1]->Draw("same");

		ptemp[2] = mprof[qt]["bb"][ct];
		applyCosmetics(ptemp[2], kBlue, 20, 1.5);
		leg->AddEntry(ptemp[2], Form("%s:PbPb", qt.c_str()), "lep");
		ptemp[2]->Draw("same");


		ptemp[3] = mprof[qt]["ff"][ct];
		applyCosmetics(ptemp[3], kGreen+3, 20, 1.5);
		leg->AddEntry(ptemp[3], Form("%s:pp", qt.c_str()), "lep");
		ptemp[3]->Draw("same");

		leg->Draw("same");
		gPad->SetLogy();
		gPad->SetGrid();

	}//close:if


	/*
	// Cross check if neg/pos trend is as expected 
	if (0){

		Double_t varBinArr[] = { 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 3.2, 
			                4, 4.8, 5.6, 6.4, 7.2, 9.6, 12, 14.4, 19.2, 24, 28.8, 35.2, 41.6, 48, 60.8, 
					73.6, 86.4, 103.6, 120.8, 138, 155.2, 172.4, 189.6, 206.8 }; 

		const int nVarBins = sizeof(varBinArr)/sizeof(double)-1;
		cout << "Number of Variable Bins declared: " << nVarBins << endl;

		// Gluon, Quark, All
		string qt = "All";

		map<int, TH1D*> hn, hp, hratio ;
		TLegend *leg = new TLegend(0.9, 0.8, 0.7, 0.6);
		//leg->SetHeader(Form("%s, Ratio of Neg/Pos", qt.c_str()));

		hp[1] = mh[qt]["bf"]["hPos"];
		hp[11] = (TH1D*)hp[1]->Rebin(nVarBins, "hPos-varBins", varBinArr);
		hp[11] = normalizeVarBinHist(hp[11], nVarBins);
		applyCosmetics(hp[11], kRed, 20, 1.5);
		//hp[11]->Draw("same");

		hn[1] = mh[qt]["bf"]["hNeg"];
		hn[11] = (TH1D*)hn[1]->Rebin(nVarBins, "hNeg-varBin", varBinArr);
		hn[11] = normalizeVarBinHist(hn[11], nVarBins);
		applyCosmetics(hn[11], kBlue, 20, 1.5);
		//hn[11]->Draw("same");

		hratio[11] = (TH1D*)hn[11]->Clone("hRatio");
		hratio[11]->Divide(hp[11]);
		//hratio[1]->Rebin(3);
		hratio[11]->Draw("same");


		leg->Draw("same");
		gPad->SetLogy();
		gPad->SetGrid();


	}//close:if

	*/




}//close:main
