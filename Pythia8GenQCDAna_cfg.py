import FWCore.ParameterSet.Config as cms
import random, sys, re
import json

process = cms.Process('GEN')

# ========= Import of Standard Configurations ==============
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.Configuration.RecoGenJets_cff")
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger = cms.Service("MessageLogger")
process.MessageLogger.cerr.FwkReport.reportEvery = 100000

# ================= Input Variable Parsing ======================
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('standard')


options.register('forCrab',
                 1,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "create folder for crab")


options.register('userOptionsForPythia8',
		[],
		VarParsing.multiplicity.list,
		VarParsing.varType.string,
		"check")


options.register('folderName',
                 'GenOutput/',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
		 "test")


options.register('Pythia8ConfigFile',
                 'pythia8_4C_Defaults.json',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
		 "Pythia8 Configuration File")

options.output = options.folderName + 'QCDAna.root'

options.maxEvents = 10


options.register('tune',
                 "4C",
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "Pythia tune")


options.register('processType',
                 "HardQCDSelectionGluon",
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "Pythia process type with pT_hat range")


options.register('pdfSetName',
                 'MRST-LO(2008)',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "PDF Set Name")

options.register('sqrtS',
                 5020.0,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.float,
                 "Center-of-mass energy")

options.register('ptHatLow',
                 380,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "Minimum pt-hat")

options.register('ptHatHigh',
                 10000,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "Maximum pt-hat")

options.parseArguments()

#print "======================\n"
#print options.maxEvents, "\n", options.Pythia8ConfigFile, "\n", options.userOptionsForPythia8, "\n", options.output, "\n", options.ptHatLow, "\n", options.ptHatHigh
#print "======================\n"

# ===================== General Settings ===================

process.Timing = cms.Service("Timing", 
		summaryOnly = cms.untracked.bool(True)
		)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.load('Pythia8GenQCDAna_cff')
process.generator = process.generator.clone()


# Read the configuration file
inFile = open(options.Pythia8ConfigFile, "r")
pythia8Parameters = json.dumps(json.load(inFile))
py8Param = re.sub(r'\[|\]', r'', pythia8Parameters).split(",")

process.generator.comEnergy = options.sqrtS

# Options from the json file
process.generator.PythiaParameters.processParameters += py8Param
obj = process.generator.PythiaParameters.processParameters

obj += ['PhaseSpace:pTHatMin='+str(options.ptHatLow)] 
obj += ['PhaseSpace:pTHatMax='+str(options.ptHatHigh)] 

# If user has provided some additional options, it gets appended into the pythia config list
# potentially overriding what's in the cards, check before proceeding
for userOption in options.userOptionsForPythia8:
	obj += [userOption]

#print "typeof obj: ", type(process.generator.PythiaParameters.processParameters )
#print "typeof obj: ", type(pythia8Parameters)


from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper
randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
randSvc.populate()

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService", 
		generator = cms.PSet(

		       initialSeed = cms.untracked.uint32(random.randint(1, 999999)),
		       engineName = cms.untracked.string('TRandom3')
		)
		)

'''
process.rnsParams = cms.PSet(

		       initialSeed = cms.untracked.uint32(random.randint(1, 999999)),
		       engineName = cms.untracked.string('TRandom3')
		)

process.RandomNumberGeneratorService.generator = process.rnsParams.clone(

       initialSeed = cms.untracked.uint32(random.randint(1, 999999)),
       engineName = cms.untracked.string('TRandom3')
		
		)
'''

process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    makeTriggerResults = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True)
)

#process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.output)
)

# ============= PYTHIA Generator =====================

'''
from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper
randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
randSvc.populate()
'''

# =============== Analysis =============================
process.QCDAna = process.QCDAna.clone()

# ============= Gen Jets ================================

process.ak3GenJets = process.ak5GenJets.clone( rParam = 0.3 )
process.ak4GenJets = process.ak5GenJets.clone( rParam = 0.4 )
process.ak7GenJets = process.ak5GenJets.clone( rParam = 0.7 )


# ========== Flavor Matching ==========================

process.myPartons = cms.EDProducer("PartonSelector",
    withLeptons = cms.bool(False),
    src = cms.InputTag("genParticles")
)

process.flavourByRef = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak3GenJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("myPartons")
)
# hadron pT bins from 0 to 200 in increments of 2
# jet pT bins from 0 to 1000 in increments of 10
# qscale pT bins from 0 to 1000 in increments of 10
for x in range(0,101):
  process.QCDAna.hPtBins += [ float(x)*2.0 ]
  process.QCDAna.jetPtBins += [ float(x)*10.0 ]
  process.QCDAna.qScalePtBins += [ float(x)*10.0 ]

#eta bins from -2.4 to 2.4 in increments of 0.4
for x in range(0,13):
  process.QCDAna.etaBins += [ float(x)*0.4-2.4 ]

longLivedBaryonIDs = cms.vint32( 2212, -2212, # p (pbar)
                                 2112, -2112, # n
                                 3122, -3122, # lambda
                                 3222, -3222, # sigma+
                                 3112, -3112, # sigma-
                                 3312, -3312, # xi
                                 3334, -3334  # omega
)

longLivedMesonIDs = cms.vint32( 211, -211, # pion
                                130, 310, 321, -321, # K
)

quarks = cms.vint32( -6,-5,-4,-3,-2,-1,1,2,3,4,5,6 )

process.QCDAna_baryons = process.QCDAna.clone(
    doSpecies = cms.bool(True),
    speciesId = longLivedBaryonIDs
)

process.QCDAna_mesons = process.QCDAna.clone(
    doSpecies = cms.bool(True),
    speciesId = longLivedMesonIDs
)                             

process.QCDAna_qJets = process.QCDAna.clone( 
    doFlavor = cms.bool(True),
    flavorId = quarks
)

process.QCDAna_qJets_baryons = process.QCDAna_baryons.clone(
    doFlavor = cms.bool(True),
    flavorId = quarks
)

process.QCDAna_qJets_mesons = process.QCDAna_mesons.clone(
    doFlavor = cms.bool(True),
    flavorId = quarks
)

process.QCDAna_gJets = process.QCDAna.clone(
    doFlavor = cms.bool(True),
    flavorId = cms.vint32(21) 
)

process.QCDAna_gJets_baryons = process.QCDAna_baryons.clone(
    doFlavor = cms.bool(True),
    flavorId = cms.vint32(21) 
)

process.QCDAna_gJets_mesons = process.QCDAna_mesons.clone(
    doFlavor = cms.bool(True),
    flavorId = cms.vint32(21) 
)

#pathName='gen_step'
#process.Pythia8Output.SelectEvents.SelectEvents = cms.vstring(pathName)



# ================== Paths and Schedule ===================
process.gen_step = cms.Path(process.generator * process.genParticles )
process.genjet_step = cms.Path(process.genJetParticles 
			* process.ak3GenJets
			#     * process.ak4GenJets
			#     * process.ak5GenJets
			#     * process.ak7GenJets
	)

process.flavor_step = cms.Path( process.myPartons * process.flavourByRef )
process.ana_step = cms.Path(
		process.QCDAna *
		process.QCDAna_baryons *
		process.QCDAna_mesons *
		process.QCDAna_qJets *
		process.QCDAna_qJets_baryons *
		process.QCDAna_qJets_mesons *
		process.QCDAna_gJets *
		process.QCDAna_gJets_baryons *
		process.QCDAna_gJets_mesons 
	)

process.schedule = cms.Schedule(process.gen_step,
				process.genjet_step,
				process.flavor_step,
				process.ana_step)
#singleInst()
