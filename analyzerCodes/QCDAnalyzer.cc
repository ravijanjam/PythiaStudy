// system include files
#include <memory>
#include <iostream>
#include <vector>
//#include <pair>
//
#include "DataFormats/HepMCCandidate/interface/FlavorHistoryEvent.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

//#include "CMS.PhysicsTools/HepMCCandAlgos/interface/FlavorHistorySelectorUtil.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/PdfInfo.h"
#include "HepPDT/ParticleID.hh"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"


#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TRandom.h>

//
// class declaration
//

using namespace std;
class QCDAnalyzer : public edm::EDAnalyzer {
   public:
      explicit QCDAnalyzer(const edm::ParameterSet&);
      ~QCDAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      //void test(const reco::GenParticle& );
      bool isDSEvent( const edm::Event&, const edm::EventSetup& );
      bool isInEtaRange( const reco::Candidate&, double, double ); 
      bool isInFlavor( const reco::MatchedPartons& ); 
      bool isInSpecies( const reco::GenParticle& ); 
      void getParentHadron( const HepMC::GenEvent& );

      // ----------member data ---------------------------

      edm::InputTag genJetSrc_;
      edm::InputTag genParticleSrc_;
      bool doFlavor_;
      bool doSpecies_;
      bool onlyDS_;
      edm::InputTag flavorSrc_;
      std::vector<int> flavorId_;
      std::vector<int> speciesId_;
      bool useRapidity_;
      double jetEtaMin_;
      double jetEtaMax_;
      double hEtaMin_;
      double hEtaMax_;
      double jetRadius_;
      double pthatMin_;
      double pthatMax_;
      double meanpTPos, meanpTNeg, meanpTAll;

      std::vector<double> jetPtBins_;
      std::vector<double> hPtBins_;
      std::vector<double> qScalePtBins_;
      std::vector<double> etaBins_;

      std::map<std::string,TH1F*> hist_;
      std::map<std::string,TH2F*> hist2D_;
      vector<int> partonType;
      int hId;
      int counter;


      TTree *tree, *tree2;

      Float_t mx1, mx2, hadronPos2, hadronNeg2, posPt, negPt, x1, x2, Q2, ax1, ax2, hadronPos, hadronNeg, cPos, cNeg, temp[5]; 
      int mcount;


      bool tEnable;

      Double_t pPos, pNeg, hPos, hNeg;
      Int_t ppdg; 

	//double xvspTArr[11];
};


//
// constructors and destructor
//

QCDAnalyzer::QCDAnalyzer(const edm::ParameterSet& iConfig):
genJetSrc_(iConfig.getParameter<edm::InputTag>("genJetSrc")),
genParticleSrc_(iConfig.getParameter<edm::InputTag>("genParticleSrc")),
doFlavor_(iConfig.getParameter<bool>("doFlavor")),
doSpecies_(iConfig.getParameter<bool>("doSpecies")),
onlyDS_(iConfig.getParameter<bool>("onlyDS")),
flavorSrc_(iConfig.getParameter<edm::InputTag>("flavorSrc")),
flavorId_(iConfig.getParameter<std::vector<int> >("flavorId")),
speciesId_(iConfig.getParameter<std::vector<int> >("speciesId")),
useRapidity_(iConfig.getParameter<bool>("useRapidity")),
jetEtaMin_(iConfig.getParameter<double>("jetEtaMin")),
jetEtaMax_(iConfig.getParameter<double>("jetEtaMax")),
hEtaMin_(iConfig.getParameter<double>("hEtaMin")),
hEtaMax_(iConfig.getParameter<double>("hEtaMax")),
jetRadius_(iConfig.getParameter<double>("jetRadius")),
pthatMin_(iConfig.getParameter<double>("pthatMin")),
pthatMax_(iConfig.getParameter<double>("pthatMax")),
jetPtBins_(iConfig.getParameter<std::vector<double> >("jetPtBins")),
hPtBins_(iConfig.getParameter<std::vector<double> >("hPtBins")),
qScalePtBins_(iConfig.getParameter<std::vector<double> >("qScalePtBins")),
etaBins_(iConfig.getParameter<std::vector<double> >("etaBins"))
{
   edm::Service<TFileService> fs;

   // Spectra histograms for eta (or y) within specified range

   //xvspTArr[] = {0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};



   // jet spectrum
   hist_["jetspectrum"] = fs->make<TH1F>("jetspectrum",";p_{T};counts",
                           jetPtBins_.size()-1, &jetPtBins_[0]);
   // hadron spectrum
   hist_["hspectrum"] = fs->make<TH1F>("hspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);
   // charged hadron spectrum
   hist_["chspectrum"] = fs->make<TH1F>("chspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   // positively charged hadron spectrum
   hist_["pchspectrum"] = fs->make<TH1F>("pchspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist_["nchspectrum"] = fs->make<TH1F>("nchspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist_["partIntoVtx"] = fs->make<TH1F>("partIntoVtx",";particles into vertex",
                          100, 0, 100);

   hist_["partOutVtx"] = fs->make<TH1F>("partOutVtx",";particles out of vertex",
                          100, 0, 100);


   hist_["vtxPerEvent"] = fs->make<TH1F>("vtxPerEvent","Vertices per event",
                          10000, 0, 10000);

   // Counting particles by type
   hist_["pdgOfPart"] = fs->make<TH1F>("pdgOfPart","pdg of partons per event",
                          10000, -10000, 10000);

   hist_["pdgOfHadrons"] = fs->make<TH1F>("pdgOfHadrons","pdg of hadrons just after hadronization per event",
                          10000, -10000, 10000);

   hist_["pdgOfFinalHadrons"] = fs->make<TH1F>("pdgOfFinalHadrons","pdg of final state hadrons per event",
                          10000, -10000, 10000);

   hist_["particlesPerEvent"] = fs->make<TH1F>("particlesPerEvent","Particles per event",
                          10000, 0, 10000);


   hist2D_["x1VSQ2"] = fs->make<TH2F>("x1VSQ2","x vs Q2",
                           100, -5, 5,
                           10000, 0, 10000);


   hist2D_["x2VSQ2"] = fs->make<TH2F>("x2VSQ2","x vs Q2",
                           100, -5, 5,
                           10000, 0, 10000);
                           

   // 2D histograms of (eta[y],pT) as above
   hist2D_["jetspectrum2D"] = fs->make<TH2F>("jetspectrum2D",";p_{T};counts",
                           etaBins_.size()-1, &etaBins_[0],
                           jetPtBins_.size()-1, &jetPtBins_[0]);

   hist2D_["hspectrum2D"] = fs->make<TH2F>("hspectrum2D",";p_{T};counts",
                           etaBins_.size()-1, &etaBins_[0],
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["chspectrum2D"] = fs->make<TH2F>("chspectrum2D",";p_{T};counts",
                           etaBins_.size()-1, &etaBins_[0],
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["pchspectrum2D"] = fs->make<TH2F>("pchspectrum2D",";p_{T};counts",
                           etaBins_.size()-1, &etaBins_[0],
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["nchspectrum2D"] = fs->make<TH2F>("nchspectrum2D",";p_{T};counts",
                           etaBins_.size()-1, &etaBins_[0],
                           hPtBins_.size()-1, &hPtBins_[0]);


   hist2D_["xVSpT_Beam_All"] = fs->make<TH2F>("xVSpT_Beam_All","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Target_All"] = fs->make<TH2F>("xVSpT_Target_All","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Beam_Neg"] = fs->make<TH2F>("xVSpT_Beam_Neg","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Target_Neg"] = fs->make<TH2F>("xVSpT_Target_Neg","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Beam_Pos"] = fs->make<TH2F>("xVSpT_Beam_Pos","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Target_Pos"] = fs->make<TH2F>("xVSpT_Target_Pos","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);


   hist2D_["xVSpT_Beam_uQuark"] = fs->make<TH2F>("xVSpT_Beam_uQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Target_uQuark"] = fs->make<TH2F>("xVSpT_Target_uQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);


   hist2D_["xVSpT_Beam_antiuQuark"] = fs->make<TH2F>("xVSpT_Beam_antiuQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Target_antiuQuark"] = fs->make<TH2F>("xVSpT_Target_antiuQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);


   hist2D_["xVSpT_Beam_dQuark"] = fs->make<TH2F>("xVSpT_Beam_dQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Target_dQuark"] = fs->make<TH2F>("xVSpT_Target_dQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);


   hist2D_["xVSpT_Beam_antidQuark"] = fs->make<TH2F>("xVSpT_Beam_antidQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist2D_["xVSpT_Target_antidQuark"] = fs->make<TH2F>("xVSpT_Target_antidQuark","",
                           10, 0, 1,
                           hPtBins_.size()-1, &hPtBins_[0]);


   // pt-hat or momentum transfer scale of PYTHIA process
   // this will be 0 for diffractive events
   hist_["qscale"] = fs->make<TH1F>("qscale",";p_{T}-hat;counts",
                           qScalePtBins_.size()-1, &qScalePtBins_[0]);
   hist2D_["ch_qscale2D"] = fs->make<TH2F>("ch_qscale2D",";p_{T}-hat;p_{T} h^{#pm}",
                           qScalePtBins_.size()-1, &qScalePtBins_[0],
                           hPtBins_.size()-1, &hPtBins_[0]);
   hist2D_["jet_qscale2D"] = fs->make<TH2F>("jet_qscale2D",";p_{T}-hat;p_{T} h^{#pm}",
                           qScalePtBins_.size()-1, &qScalePtBins_[0],
                           jetPtBins_.size()-1, &jetPtBins_[0]);

   hist_["pdgId"] = fs->make<TH1F>("pdgId", "pdgId", 5000, 0, 5000);

   hist_["pPos"] = fs->make<TH1F>("pPos", "# +vely charged partons just before hadronization", 1000, 0, 1000);
   hist_["pNeg"] = fs->make<TH1F>("pNeg", "# -vely charged partons just before hadronization", 1000, 0, 1000);

   hist_["hPos"] = fs->make<TH1F>("hPos", "# +vely charged hadrons just before hadronization", 1000, 0, 1000);
   hist_["hNeg"] = fs->make<TH1F>("hNeg", "# -vely charged hadrons just before hadronization", 1000, 0, 1000);

   hist_["pPos_uQuark"] = fs->make<TH1F>("pPos_uQuark", "# +vely charged u Quark just before hadronization", 1000, 0, 1000);
   hist_["pNeg_uQuark"] = fs->make<TH1F>("pNeg_uQuark", "# -vely charged u Quark just before hadronization", 1000, 0, 1000);

   hist_["pPos_dQuark"] = fs->make<TH1F>("pPos_dQuark", "# +vely charged d Quark just before hadronization", 1000, 0, 1000);
   hist_["pNeg_dQuark"] = fs->make<TH1F>("pNeg_dQuark", "# -vely charged d Quark just before hadronization", 1000, 0, 1000);

   // leading charged hadron in approximate tracker acceptance (|eta|<2.5)
   hist_["lead_track"] = fs->make<TH1F>("lead_track",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   // number of recorded events
   hist_["events"] = fs->make<TH1F>("events",";;events",1,0.,2.);

   // fragmentation function matrix - i.e. 2D histogram of hadrons vs jets
   // also for just charged and positively charged hadrons
   hist2D_["ffmatrix"] = fs->make<TH2F>("ffmatrix",";p_{T} Jet;p_{T} h",
                           hPtBins_.size()-1, &hPtBins_[0],
                           jetPtBins_.size()-1, &jetPtBins_[0]);
   hist2D_["cffmatrix"] = fs->make<TH2F>("cffmatrix",";p_{T} Jet;p_{T} h^{#pm}",
                           hPtBins_.size()-1, &hPtBins_[0],
                           jetPtBins_.size()-1, &jetPtBins_[0]);
   hist2D_["pcffmatrix"] = fs->make<TH2F>("pcffmatrix",";p_{T} Jet;p_{T} h^{+}",
                           hPtBins_.size()-1, &hPtBins_[0],
                           jetPtBins_.size()-1, &jetPtBins_[0]);


   // Create histograms for spectra per parton type 
   partonType = {1, 2, 3, 4, 5, 6, 7, 8, -1, -2, -3, -4, -5, -6, -7, -8};	
   for ( int parton : partonType ){
	char *label = Form("parton_PDGID_%d", parton);
	//cout << label << endl;
	hist_[label] = fs->make<TH1F>(label, label, hPtBins_.size()-1, &hPtBins_[0]);
   }

        this->counter = 0;

	tEnable=0;

	/*  
   	xVSpTTree = new TTree("xvspTTree", "x vs pT Tree");	
	xVSpTTree->Branch("ax1", &ax1, "ax1/F");
	xVSpTTree->Branch("ax2", &ax2, "ax2/F");
	xVSpTTree->Branch("hadronPos", &x2, "hadronPos/F");
	xVSpTTree->Branch("hadronNeg", &x2, "hadronNeg/F");
	*/

   	tree = new TTree("tree", "before and after hadronization");	
   	tree2 = new TTree("tree2", "for nPos and nNeg per event");	

	tree->Branch("x1", &x1, "x1/F");
	tree->Branch("x2", &x2, "x2/F");
	tree->Branch("Q2", &Q2, "Q2/F");

	tree->Branch("hadronPos", &hadronPos, "hadronPos/F");
	tree->Branch("hadronNeg", &hadronNeg, "hadronNeg/F");


	tree2->Branch("mx1", &mx1, "mx1/F");
	tree2->Branch("mx2", &mx2, "mx2/F");
	tree2->Branch("hadronPos2", &hadronPos2, "hadronPos2/F");
	tree2->Branch("hadronNeg2", &hadronNeg2, "hadronNeg2/F");

	tree2->Branch("cPos", &cPos, "cPos/F");
	tree2->Branch("cNeg", &cPos, "cNeg/F");
	tree->Branch("mcount", &mcount, "mcount/I");
	tree->Branch("temp", temp, "temp[mcount]/F");


	if (tEnable){
		int hpdg;

		//tree->Branch("partonPositive", &pPos, "pPos/D");
		//tree->Branch("partonNegative", &pNeg, "pNeg/D");
		//tree->Branch("hadronPositive", &hPos, "hPos/D");
		//tree->Branch("hadronNegative", &hNeg, "hNeg/D");
		tree->Branch("hpdg", &hpdg, "hpdg/I");
		tree->Branch("ppdg", &ppdg, "ppdg/I");
	}

	meanpTPos=0; meanpTNeg=0; meanpTAll=0;

   //IsStateFinal isSF;

}//close:constructor


QCDAnalyzer::~QCDAnalyzer()
{
}


//
// member functions
//

void QCDAnalyzer::getParentHadron( const HepMC::GenEvent& gE ){

}//close:getParentHadron()

// ------------ method called for each event  ------------
void
QCDAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   //this->counter = 0;
   // Pythia does not respect max pt-hat for MB process  
   // where the minimum is 0. In this case we need to 
   // remove events over the pt-hat maximum by hand.
   // We skip and do not count such events.
   // Here it is only coded for the MB_0_to_20 process
   edm::Handle<GenEventInfoProduct> genEvtInfo;
   iEvent.getByLabel("generator", genEvtInfo);

   //cout << "ME Partons: " << genEvtInfo->nMEPartons() << endl;

   edm::Handle<edm::HepMCProduct> genEvtHandle;
   iEvent.getByLabel("generator", genEvtHandle);

   const HepMC::GenEvent *gE = genEvtHandle->GetEvent();

   
   if( pthatMin_ < 1. )
   {
     if( genEvtInfo->qScale() > pthatMax_ ) return;
   }

   // Check if the event is DS and skip if configured
   if( ! isDSEvent(iEvent,iSetup) && onlyDS_ ) return;
  
   Handle<reco::GenParticleCollection> pcol;
   iEvent.getByLabel(genParticleSrc_,pcol);

   Handle<reco::GenJetCollection> gcol;
   if( !doFlavor_ ) iEvent.getByLabel(genJetSrc_,gcol);

   Handle<reco::JetMatchedPartonsCollection> fcol;
   if( doFlavor_) iEvent.getByLabel(flavorSrc_,fcol);

   hist_["events"]->Fill(1);
   hist_["qscale"]->Fill(genEvtInfo->qScale());

   // genjet spectra
   if ( doFlavor_ )
   {
     for( const auto & mjp : *fcol )
     {
        if( ! isInFlavor( mjp.second ) ) continue;
        const reco::Jet *aJet = mjp.first.get();
        if( isInEtaRange( *aJet, jetEtaMin_, jetEtaMax_ ) ) 
        {
           hist_["jetspectrum"]->Fill(aJet->pt());
           hist2D_["jet_qscale2D"]->Fill(genEvtInfo->qScale(),aJet->pt());
        }
        hist2D_["jetspectrum2D"]->Fill(aJet->eta(),aJet->pt());
     }
   }
   else
   {
     for( const auto & jet : *gcol )
     {
       if( isInEtaRange( jet, jetEtaMin_, jetEtaMax_ ) ) 
       {
         hist_["jetspectrum"]->Fill(jet.pt());
         hist2D_["jet_qscale2D"]->Fill(genEvtInfo->qScale(),jet.pt());
       }
       hist2D_["jetspectrum2D"]->Fill(jet.eta(),jet.pt());
     }
   }

   //cout << x1 << "\t" << x2 << "\t" << Q2 << endl;
   //double x1, x2, Q2; // double x2;

   /*  
   const HepMC::PdfInfo *pdfinfo = gE->pdf_info();

    if(pdfinfo){

      this->x1 = pdfinfo->x1();
      this->x2 = pdfinfo->x2(); 
      this->Q2 = pdfinfo->scalePDF();
    
      //cout << "x1: " << this->x1 << "\t" << "x2: " << this->x2 << "\t";

      //cout << this->x1 << "\t" << this->x2 << "\t" << this->Q2 << endl;
      hist2D_["x1VSQ2"]->Fill(this->x1, this->Q2);
      hist2D_["x2VSQ2"]->Fill(this->x2, this->Q2);

      //tree->Fill();
    }//close:if

    else{

      this->x1 = -9999.0; 
      this->x2 = -9999.0; 
      this->Q2 = -9999.0; 
      //tree->Fill();
    }


   mcount = 5;
   for (int i=0; i<mcount; i++){

	   temp[i] = i;
   }
   */

   //cout << (this->counter++) << endl;


   //cout << x1 << x2 << Q2 << endl;

   // charged particle spectra and ff matrix
   double lead_track_pt = 0.0;

   cPos=0; cNeg=0; hadronPos2=0; hadronNeg2=0;
   // vector<double> vhPos, vhNeg;
   for( const auto & h : *pcol )
   {
     // skip decayed  particles
     if( h.status() != 1  ) continue;
     // update leading track
	hist_["pdgOfFinalHadrons"]->Fill(h.pdgId());
     if( h.charge() != 0 && fabs(h.eta()) < 2.5 && lead_track_pt < h.pt() )
       lead_track_pt = h.pt(); 

     if( doSpecies_ && ! isInSpecies(h) ) continue;
       

     if( isInEtaRange(h, hEtaMin_, hEtaMax_) )
     {

	     // Loop over parton's pdgID and populate the histograms
	     for (auto parton : partonType){

	     	     char *label = Form("parton_PDGID_%d", parton);
		     if (parton == h.pdgId()) hist_[label]->Fill(h.pt());
		     //if (parton == h.pdgId()) cout << "p" << "," ;
	     }//close-for


       hist_["hspectrum"]->Fill(h.pt());
       if( h.charge() != 0 ){ 

	       //meanpTAll += h.pt(); 
	       hist_["chspectrum"]->Fill(h.pt()); 

   	       hist2D_["xVSpT_Beam_Pos"]->Fill(gE->pdf_info()->x1(), h.pt());
	       hist2D_["xVSpT_Target_Pos"]->Fill(gE->pdf_info()->x2(), h.pt());
       }

       if( h.charge() != 0 ) hist2D_["ch_qscale2D"]->Fill(genEvtInfo->qScale(),h.pt());

       hadronPos = 0;
       if( h.charge() > 0 ) { 

	       //meanpTPos += h.pt(); 
	       hist_["pchspectrum"]->Fill(h.pt()); 

   	       hist2D_["xVSpT_Beam_Pos"]->Fill(gE->pdf_info()->x1(), h.pt());
	       hist2D_["xVSpT_Target_Pos"]->Fill(gE->pdf_info()->x2(), h.pt());

	       cPos++;
	       hadronPos = h.pt();
		//cout <<"pos: " << h.pt() << "\t";

       }

       hadronNeg = 0;
       if( h.charge() < 0 ) { 

	       hist_["nchspectrum"]->Fill(h.pt()); 

   	       hist2D_["xVSpT_Beam_Neg"]->Fill(gE->pdf_info()->x1(), h.pt());
	       hist2D_["xVSpT_Target_Neg"]->Fill(gE->pdf_info()->x2(), h.pt());
	
	       cNeg++;
	       hadronNeg = h.pt();

	}//close:if

	//tree->Fill();

       // associate with the highest-pt jet for which 
       // the track is found in the jet cone
     
       double maxPtJet = 0.;
       if( doFlavor_)
       {
         for( const auto & mjp : *fcol )
         {
           const reco::Jet *aJet = mjp.first.get();
           if ( !isInEtaRange(*aJet,jetEtaMin_,jetEtaMax_) ) continue;
           if( !isInFlavor( mjp.second ) ) continue;
           double dr = deltaR(*aJet,h);
           if( dr < jetRadius_ && aJet->pt() > maxPtJet)
             maxPtJet = aJet->pt();
         }
       }
       else
       {
         for( const auto & jet : *gcol )
         {
           if( ! isInEtaRange( jet, jetEtaMin_, jetEtaMax_ ) ) continue; 
           double dr = deltaR(jet,h);
           if( dr < jetRadius_ && jet.pt() > maxPtJet)
             maxPtJet = jet.pt();
         }
       }

       hist2D_["ffmatrix"]->Fill( maxPtJet, h.pt());
       if( h.charge() != 0 ) hist2D_["cffmatrix"]->Fill( maxPtJet, h.pt());
       if( h.charge() > 0 ) hist2D_["pcffmatrix"]->Fill( maxPtJet, h.pt());
     }
       //cout << "\n";

   }//close:loop over hadrons


   hist_["lead_track"]->Fill(lead_track_pt);

}//close:analyzer


bool 
QCDAnalyzer::isInEtaRange( const reco::Candidate& c, double etaMin, double etaMax )
{
     if( useRapidity_ == false &&  c.eta() <= etaMax && c.eta() >= etaMin )
       return true;
     if( useRapidity_ == true &&  c.y() <= etaMax && c.y() >= etaMin )
       return true;
     return false;
}

bool
QCDAnalyzer::isInFlavor( const reco::MatchedPartons & aMatch )
{
  int flavor = 0;
  if( aMatch.heaviest().isNonnull() )
  {
    flavor = aMatch.heaviest().get()->pdgId();
  }
  for( const auto inFlavor : flavorId_ )
  { if( flavor == inFlavor ) return true; }
 
  return false;
}

bool 
QCDAnalyzer::isInSpecies( const reco::GenParticle & h )
{
  using namespace std;
  for( const auto inSpecies : speciesId_ )
  { if( h.pdgId() == inSpecies ) 
	  //cout << "From GenParticleCollection " << h.pdgId() << endl;
	  return true; 
  }

  return false;  
}

bool 
QCDAnalyzer::isDSEvent( const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

     using namespace edm;

     bool posDS = false; bool negDS = false;

     edm::ESHandle<ParticleDataTable> particleDataTable_;
     iSetup.getData(particleDataTable_);

     Handle<reco::GenParticleCollection> gcol;
     iEvent.getByLabel(genParticleSrc_, gcol);
     for( const auto & gen : * gcol )
     {
       // see if genpartice counts for DS
       HepPDT::ParticleID particleID(gen.pdgId());
       if (particleID.isValid())
       {
         ParticleData const * particleData = particleDataTable_->particle(particleID);
         if (particleData)
         { 
           double tau =  particleDataTable_->particle(particleID)->lifetime();
           if ( tau  > 1e-18 || tau == 0.0 )
           {
             if( gen.energy() > 3.0 && gen.eta() > 3.0 && gen.eta() < 5.0 ) posDS = true;
             if( gen.energy() > 3.0 && gen.eta() < -3.0 && gen.eta() > -5.0 ) negDS = true;
           }
         }
       }
     }

     if( posDS && negDS ) return true;
     else return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
QCDAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
QCDAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
QCDAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
QCDAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
QCDAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
QCDAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
QCDAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(QCDAnalyzer);

