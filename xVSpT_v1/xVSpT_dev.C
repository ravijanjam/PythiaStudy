#include <iostream>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TProfile.h>

typedef const char* cchar;


TProfile* applyCosmetics(TProfile* hp, Color_t mCol, int mStyle, float mSize )
{
	        hp->SetLineColor(mCol);
		hp->SetMarkerStyle(mStyle);
	        hp->SetMarkerColor(mCol);
		hp->SetMarkerSize(mSize);
		hp->SetStats(0);
        
		return hp;

}//close-applyCosmetics

// Plot {x1, x2} vs pT (+, -) and write to file
void xVSpT_dev(cchar inFileName, string label, cchar oFileName){


	cout << "fileName: " << inFileName << "\t"
	     << "label: " << label << "\t"
	     << "output file: " << oFileName << "\n";

	// Open the file
	TFile *file = new TFile(inFileName);


	if (file->IsOpen()){

		cout << "file is open" << endl;

		map<string, string> ml;
		map<string, TProfile*> mprof; 
		map<string, string>::iterator mit;

		//ml = {{"x1_Pos","x1:hadronPos"}, {"x1_Neg","x1:hadronNeg"}, {"x2_Pos", "x2:hadronPos"}, {"x2_Neg", "x2:hadronNeg"}};
		ml = {{"x1_Pos","mx1:hadronPos2"}, {"x1_Neg","mx1:hadronNeg2"}, {"x2_Pos", "mx2:hadronPos2"}, {"x2_Neg", "mx2:hadronNeg2"}};

		TTree *tree = new TTree();
		tree = (TTree*)file->Get("tree2");
		cchar label;

		for ( mit = ml.begin() ; mit != ml.end(); ++mit){

			cout << mit->first << "\t"
			     << mit->second << "\n";

			label = mit->second.c_str();
			mprof[mit->first] = new TProfile(label, "", 10, 0, 100, -5, 5);
			tree ->Project(label, mit->second.c_str());

		}//close:for


		applyCosmetics(mprof["x1_Pos"], kRed, 20, 2);
		mprof["x1_Pos"]->Draw();

		applyCosmetics(mprof["x1_Neg"], kBlue, 20, 2);
		mprof["x1_Neg"]->Draw();


		applyCosmetics(mprof["x2_Pos"], kRed, 25, 2);
		mprof["x2_Pos"]->Draw();

		applyCosmetics(mprof["x2_Neg"], kBlue, 25, 2);
		mprof["x2_Neg"]->Draw();


		// Write to file
		TFile *ofile = new TFile(oFileName, "recreate");
		for (cchar ll: {"x1_Pos", "x2_Pos", "x1_Neg", "x2_Neg"}){
			mprof[ll]->Write(ll);
		}
		ofile->Close();
		


	}//close:processing the file


	

	// Plot x1 vs pT (+)
	// Plot x2 vs pT (+)
	
	// Plot x1 vs pT (-)
	// Plot x2 vs pT (-)
	// Plot x1 vs x2 (+)
	// Plot x1 vs x2 (-)
	

	// Write the results to a file

}//close:main


/* 
 * label: to identify the type of collision system, to be put into TLegend
 * red: positive, blue: negative
 * oFileName: output filename
 */
