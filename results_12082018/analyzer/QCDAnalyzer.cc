// system include files
#include <memory>
#include <iostream>
#include <vector>
#include <cmath>
//
#include "DataFormats/HepMCCandidate/interface/FlavorHistoryEvent.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"


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

      bool isDSEvent( const edm::Event&, const edm::EventSetup& );
      bool isInEtaRange( const reco::Candidate&, double, double ); 
      bool isInEtaRange( double, double, double ); 
      bool isInSpecies( const reco::GenParticle& ); 
      bool isInSpecies( int ); 

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

      Float_t up, un, dp, dn, g, dpt, upt;

      int mcount;

      bool tEnable;

      Double_t pPos, pNeg, hPos, hNeg;
      Int_t ppdg; 
      vector<int> hid;

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


   hist2D_["x1VSQ2"] = fs->make<TH2F>("x1VSQ2","x vs Q2",
                           100, -5, 5,
                           10000, 0, 10000);


   hist2D_["x2VSQ2"] = fs->make<TH2F>("x2VSQ2","x vs Q2",
                           100, -5, 5,
                           10000, 0, 10000);
                           
   // hadron spectrum
   ///hist_["hspectrum"] = fs->make<TH1F>("hspectrum",";p_{T};counts",
   //                      hPtBins_.size()-1, &hPtBins_[0]);
   // charged hadron spectrum
   hist_["chspectrum"] = fs->make<TH1F>("chspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   // positively charged hadron spectrum
   hist_["pchspectrum"] = fs->make<TH1F>("pchspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist_["nchspectrum"] = fs->make<TH1F>("nchspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);


   hist_["jchspectrum"] = fs->make<TH1F>("jchspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   // positively charged hadron spectrum
   hist_["jpchspectrum"] = fs->make<TH1F>("jpchspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);

   hist_["jnchspectrum"] = fs->make<TH1F>("jnchspectrum",";p_{T};counts",
                           hPtBins_.size()-1, &hPtBins_[0]);


   hist_["qscale"] = fs->make<TH1F>("qscale",";p_{T}-hat;counts",
                           qScalePtBins_.size()-1, &qScalePtBins_[0]);

   hist_["events"] = fs->make<TH1F>("events",";;events",1,0.,2.);

        this->counter = 0;

	tEnable=0;

   	tree = new TTree("tree", "before and after hadronization");	

	tree->Branch("x1", &x1, "x1/F");
	tree->Branch("x2", &x2, "x2/F");
	tree->Branch("Q2", &Q2, "Q2/F");

	tree->Branch("up", &up, "up/F");
	tree->Branch("un", &un, "un/F");
	tree->Branch("upt", &upt, "upt/F");

	tree->Branch("dp", &dp, "dp/F");
	tree->Branch("dn", &dn, "dn/F");
	tree->Branch("dpt", &dn, "dpt/F");

	tree->Branch("g", &dp, "g/F");

	tree->Branch("hadronPos", &hadronPos, "hadronPos/F");
	tree->Branch("hadronNeg", &hadronNeg, "hadronNeg/F");


	if (tEnable){

   		tree2 = new TTree("tree2", "for nPos and nNeg per event");	
		tree2->Branch("mx1", &mx1, "mx1/F");
		tree2->Branch("mx2", &mx2, "mx2/F");
		tree2->Branch("hadronPos2", &hadronPos2, "hadronPos2/F");
		tree2->Branch("hadronNeg2", &hadronNeg2, "hadronNeg2/F");

		tree2->Branch("cPos", &cPos, "cPos/F");
		tree2->Branch("cNeg", &cPos, "cNeg/F");
		tree->Branch("mcount", &mcount, "mcount/I");
		tree->Branch("temp", temp, "temp[mcount]/F");


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

// ------------ method called for each event  ------------
void
QCDAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;
   using namespace std;

   edm::Handle<GenEventInfoProduct> genEvtInfo;
   iEvent.getByLabel("generator", genEvtInfo);

   edm::Handle<edm::HepMCProduct> genEvtHandle;
   iEvent.getByLabel("generator", genEvtHandle);

   
   if( pthatMin_ < 1. )
   {
     if( genEvtInfo->qScale() > pthatMax_ ) return;
   }


   if( ! isDSEvent(iEvent,iSetup) && onlyDS_ ) return;


   Handle<reco::GenParticleCollection> pcol;
   iEvent.getByLabel(genParticleSrc_,pcol);


   Handle<reco::JetMatchedPartonsCollection> fcol;
   if( doFlavor_) iEvent.getByLabel("flavourByRef",fcol);


   hist_["events"]->Fill(1);
   hist_["qscale"]->Fill(genEvtInfo->qScale());


   const HepMC::GenEvent *gE = genEvtHandle->GetEvent();
   const HepMC::PdfInfo *pdfinfo = gE->pdf_info();

    if(pdfinfo){

      this->x1 = pdfinfo->x1();
      this->x2 = pdfinfo->x2(); 
      this->Q2 = pdfinfo->scalePDF();

      hist2D_["x1VSQ2"]->Fill(this->x1, this->Q2);
      hist2D_["x2VSQ2"]->Fill(this->x2, this->Q2);

    }//close:if

    else{

      this->x1 = -9999.0; 
      this->x2 = -9999.0; 
      this->Q2 = -9999.0; 
    }


   // Loop over hadrons
   for( const auto & h : *pcol ){

     // skip decayed  particles
     if( h.status() != 1  ) continue;

     if( doSpecies_ && ! isInSpecies(h) ) continue;

     if( isInEtaRange(h, hEtaMin_, hEtaMax_) ){
	       hist_["chspectrum"]->Fill(h.pt()); 

       if( h.charge() > 0 ) {  
	       hist_["pchspectrum"]->Fill(h.pt()); 
	       hadronPos = h.pt();
       }
       if( h.charge() < 0 ) { 
	       hist_["nchspectrum"]->Fill(h.pt()); 
	       hadronNeg = h.pt();
       }
     }
   }



   for( const auto & mjp : *fcol ){

   		// Loop over parton associated with hadrons
		if(mjp.second.algoDefinitionParton().isNonnull()){

			auto ppid = mjp.second.algoDefinitionParton().get()->pdgId();
			auto ppt = mjp.second.algoDefinitionParton().get()->pt();

			if ( ppid == 1 || ppid == -1 ) dpt = ppt;
			if ( ppid == 2 || ppid == -2 ) upt = ppt;

			switch (ppid){

				case 1: dp = ppt; break; 
				case -1: dn = ppt; break; 
				case 2: up = ppt; break; 
				case -2: un = ppt; break; 
				case 21: g = ppt; break; 
			}


	   auto jt = mjp.first.get();
	   auto a1 = jt->getJetConstituentsQuick();

	   if ( a1.size() > 0 ){
		// Loop over hadrons 
		for ( auto l3 : a1 ){

			bool chk = isInSpecies(fabs(l3->pdgId()));
			if(chk){

				int hpdgId = l3->pdgId();

	       			hist_["jchspectrum"]->Fill(l3->pt()); 
	       			if (hpdgId > 0) {
					hist_["jpchspectrum"]->Fill(l3->pt()); 
					hadronPos = l3->pt();
				}

	       			if (hpdgId < 0){
					hist_["jnchspectrum"]->Fill(l3->pt()); 
					hadronNeg = l3->pt();

				}
			else{
				hadronPos=0;hadronNeg=0;
			}

	        	tree->Fill();
			}//close:if
		}//close:for
	   }


		}//close:if


   }//close:for


}//close:analyzer


bool 
QCDAnalyzer::isInEtaRange( double heta, double etaMin, double etaMax )
{
     if(  heta <= etaMax && heta >= etaMin ) return true;
     return false;
}


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
QCDAnalyzer::isInSpecies( int hpdgId )
{
  using namespace std;
  for( const auto inSpecies : { 2212, 2112, 3122, 3222, 3112, 3312, 3334, 211, 130, 310, 321 } )
  { 
	  if( hpdgId == inSpecies ) 
	  //cout << "From GenParticleCollection " << h.pdgId() << endl;
	  return true; 
  }

  return false;  
}


bool 
QCDAnalyzer::isInSpecies( const reco::GenParticle & h )
{
  using namespace std;
  for( const auto inSpecies : speciesId_ )
  { 
	  if( h.pdgId() == inSpecies ) 
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

