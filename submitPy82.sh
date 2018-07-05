#!/bin/bash

# Colliding objects of same type i.e. object1 + object2 


pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"

pdfSetA="PDF:pSetB=LHAPDF6:`pwd`/CT14nlo/"
pdfSetB="PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"

testRun(){
	userOptionsForPythia8="Beams:idA=2212","Beams:idB=-2212","Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:all=on",${pdfSetB},${pdfSetA}
	#userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:qq2qq=on",${pdfSetA}
	cmsRun Pythia8GenQCDAna_cfg.py \
		maxEvents=10 \
		Pythia8ConfigFile=`pwd`/pythia8_4C_custom.json \
		userOptionsForPythia8=${userOptionsForPythia8}\
		output=GenOutput/output.root
		#output=GenOutput/FreeCT14nlo.root
}

if [[ $1 == 1 ]]; then testRun; fi

submitJobs1(){

	scriptName=./submitPythia8ToSlurmArray_v1.sh
	configFile=`pwd`/pythia8_4C_custom.json pdfSet=$1 label=$2

	userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:all=on",${pdfSet}
	echo ${userOptionsForPythia8}
	$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
}


# Colliding objects of different type i.e object2 + object1
submitJobs2(){

	scriptName=./submitPythia8ToSlurmArray_v1.sh
	configFile=`pwd`/pythia8_4C_custom.json
	pdfSetA=$1
	pdfSetB=$2
	label=$3

	userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:all=on",${pdfSetA},${pdfSetB}
	echo ${userOptionsForPythia8}
	$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
}


run=$1
if [[ ${run} -eq  2 ]]; then

	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPS09_CT10nlo


	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPPS16_CT14nlo


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"  FreeProton_CT10nlo
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT14nlo/" FreeProton_CT14nlo
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPPS16_CT14nlo
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/" BoundProton_EPS09_CT10nlo

fi

