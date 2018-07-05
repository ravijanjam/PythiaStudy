import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing


generator = cms.EDFilter("Pythia8GeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1),
#    PPbarInitialState = cms.PSet(),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    #fBeam1PZ = cms.vstring('Beams.idB=2112'),
    #fBeam2PZ = cms.vstring('Beams.idA=2112'),
    comEnergy = cms.double(5020.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
	processParameters = cms.vstring(),
        parameterSets = cms.vstring('processParameters'),
	#py8ProcessSettings = cms.vstring('Beams:idB=2112',
	#				 'Beams:idA=2112')
	#
    )
)



# =============== Analysis =============================

QCDAna = cms.EDAnalyzer('QCDAnalyzer2',
    genJetSrc = cms.InputTag("ak3GenJets"),
    genParticleSrc = cms.InputTag("genParticles"),
    doFlavor = cms.bool(False),
    doSpecies = cms.bool(False),
    onlyDS = cms.bool(False),
    flavorSrc = cms.InputTag("flavourByRef"),
    flavorId = cms.vint32(21), # gluons
    speciesId = cms.vint32(11), # gamma
    useRapidity = cms.bool(False),
    jetEtaMin = cms.double(-0.5),
    jetEtaMax = cms.double(0.5),
    hEtaMin = cms.double(-1.0),
    hEtaMax = cms.double(1.0),
    jetRadius = cms.double(0.3),
    pthatMin = cms.double(10), # dummy values - change during run
    pthatMax = cms.double(20), # dummy values - change during run
    jetPtBins = cms.vdouble(), 
    hPtBins = cms.vdouble(), 
    qScalePtBins = cms.vdouble(), 
    etaBins = cms.vdouble() 
)


'''
Pythia8Output = cms.OutputModule ("PoolOutputModule",

		    splitLevel = cms.untracked.int32(0),
		    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
		    #outputCommands = process.Pythia8Output.outputCommands,
		    fileName = cms.untracked.string('step1.root'),

		    dataset = cms.untracked.PSet(
			            filterName = cms.untracked.string(''),
			            dataTier = cms.untracked.string('GEN')
					        ),  
			        SelectEvents = cms.untracked.PSet(
					        SelectEvents = cms.vstring('gen_step')
							    )   
					)

'''
