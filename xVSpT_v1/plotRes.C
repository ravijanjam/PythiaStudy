#include <iostream>
#include <map>

#include <TFile.h>
#include <TH1D.h>

void plotRes(){
	
	// x1_Pos, x1_Neg, x2_Pos, x2_Neg
	// bf{n,o}: pPb; bb{n,o}: PbPb; ff{n,o}: pp
	// n: new, o: old
	
	map<string, map<string, TFile*>> mfile;
	map<string, map<string, map<string, TH1D*>> mh;

	
	// Reading the files from the directories
	for (string cType : {"HardQCDQuark", "HardQCDGluon", "HardQCDAll"}){


		// EPPS16,CT14nlo based
		mfile["bfn"][cType] = new TFile(Form("%s_ptHatCombined_BoundAndFreeProton_EPPS16_CT14nlo.root",cType.c_str()));
		mfile["bfn"][cType] = new TFile(Form("%s_ptHatCombined_BoundProton_EPPS16_CT14nlo.root",cType.c_str()));
		mfile["ffn"][cType] = new TFile(Form("%s_ptHatCombined_FreeProton_CT14nlo.root",cType.c_str()));


		// EPS09,CT10nlo based
		mfile["bfo"][cType] = new TFile(Form("%s_ptHatCombined_BoundAndFreeProton_EPS09_CT10nlo.root",cType.c_str()));
		mfile["bbo"][cType] = new TFile(Form("%s_ptHatCombined_BoundProton_EPS09_CT10nlo.root",cType.c_str());
		mfile["ffo"][cType] = new TFile(Form("%s_ptHatCombined_FreeProton_CT10nlo.root",cType.c_str());

		for (string tt : {"x1_Pos", "x1_Neg", "x2_Pos", "x2_Neg"}){

			mh["bfn"][cType][tt] = (TH1D*)mfile["bfn"][cType]->Get(tt.c_str());
			mh["bbn"][cType][tt] = (TH1D*)mfile["bbn"][cType]->Get(tt.c_str());
			mh["ffn"][cType][tt] = (TH1D*)mfile["ffn"][cType]->Get(tt.c_str());

			mh["bfo"][cType][tt] = (TH1D*)mfile["bfo"][cType]->Get(tt.c_str());
			mh["bbo"][cType][tt] = (TH1D*)mfile["bbo"][cType]->Get(tt.c_str());
			mh["ffo"][cType][tt] = (TH1D*)mfile["ffo"][cType]->Get(tt.c_str());

		}//close:for


	}//close:for


		// x1 vs pT : HardQCD{Quark, Gluon, All} for pos pT
		if(1){
		}//close:if

		// x2 vs pT : HardQCD{Quark, Gluon, All} for pos pT
		
		// x1 vs pT : HardQCD{Quark, Gluon, All} for neg pT
		// x2 vs pT : HardQCD{Quark, Gluon, All} for neg pT


		// x1 vs pT : HardQCD{Quark, Gluon, All} for (pos,neg) pT
		// x2 vs pT : HardQCD{Quark, Gluon, All} for (pos,neg) pT
		// {} in separate plots




}//close:plotRes

