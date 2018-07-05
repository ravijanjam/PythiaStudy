#!/bin/bash

# Colliding objects of same type i.e. object1 + object2 


testRun(){


	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"

	pdfSetA="PDF:pSetB=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"

	#userOptionsForPythia8="Beams:idA=2212","Beams:idB=-2212","Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:all=on",${pdfSetB},${pdfSetA}
	#userOptionsForPythia8="Beams:idA=2212","Beams:idB=-2212","Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on",'HardQCD:gg2qqbar=on','HardQCD:qq2qq=on','HardQCD:qq2qq=on','HardQCD:qqbar2qqbarNew=on',${pdfSetB},${pdfSetA}
	#userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:qq2qq=on",${pdfSetA}

	userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on",'HardQCD:gg2qqbar=on','HardQCD:qq2qq=on','HardQCD:qqbar2qqbarNew=on',${pdfSet}
	cmsRun Pythia8GenQCDAna_cfg.py \
		maxEvents=10 \
		Pythia8ConfigFile=`pwd`/pythia8_4C_custom.json \
		userOptionsForPythia8=${userOptionsForPythia8}\
		output=GenOutput/output.root
		#output=GenOutput/FreeCT14nlo.root
}

if [[ $1 == 1 ]]; then testRun; fi

# Colliding objects of same type: bb, ff
submitJobs1(){

	scriptName=./submitPythia8ToSlurmArray_v1.sh
	configFile=`pwd`/pythia8_4C_custom.json pdfSet=$1 label=$2

	case $opt in

		"HardQCDAll")
			echo "HardQCDAll option"
			userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:all=on",${pdfSet}
			echo ${userOptionsForPythia8}
			$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
		;;

		"HardQCDQuark")
			echo "HardQCDQuark option"
			userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on",'HardQCD:gg2qqbar=on','HardQCD:qq2qq=on','HardQCD:qqbar2qqbarNew=on',${pdfSet}
			echo ${userOptionsForPythia8}
			$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
		;;  

		"HardQCDGluon")
			echo "HardQCDGluon option"
			userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on",'HardQCD:gg2gg=on','HardQCD:qqbar2gg=on',${pdfSet}
			echo ${userOptionsForPythia8}
			$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
		;;  


		*)
			echo "-------------------------------------------------"
			echo "NO OPTION SELECTED, please look up the script $0"
			echo "-------------------------------------------------"
			;;

	esac
}


# Colliding objects of different type i.e object2 + object1, bf
submitJobs2(){

	scriptName=./submitPythia8ToSlurmArray_v1.sh
	configFile=`pwd`/pythia8_4C_custom.json
	pdfSetA=$1
	pdfSetB=$2
	label=$3
	opt=$4

	case $opt in

		"HardQCDAll")
			echo "HardQCDAll option"
			userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on","HardQCD:all=on",${pdfSetA},${pdfSetB}

			echo ${userOptionsForPythia8}
			$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
			;;


		"HardQCDQuark")
			echo "HardQCDQuark option"
			userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on",'HardQCD:gg2qqbar=on','HardQCD:qq2qq=on','HardQCD:qqbar2qqbarNew=on',${pdfSetB},${pdfSetA}
			echo ${userOptionsForPythia8}
			$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
		;;  


		"HardQCDGluon")
			echo "HardQCDGluon option"
			userOptionsForPythia8="Init:showAllSettings=on","Next:numberCount=10000","HadronLevel:Hadronize=on",'HardQCD:gg2gg=on','HardQCD:qqbar2gg=on',${pdfSetB},${pdfSetA}
			echo ${userOptionsForPythia8}
			$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
		;;  

		*)
			echo "-------------------------------------------------"
			echo "NO OPTION SELECTED, please look up the script $0"
			echo "-------------------------------------------------"
			;;

	esac
	
#	echo ${userOptionsForPythia8}
#	$scriptName pdfNPrSw ${label} ${configFile} ${userOptionsForPythia8}
	

}


run=$1
if [[ ${run} -eq  2 ]]; then

	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPS09_CT10nlo $opt


	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPPS16_CT14nlo


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"  FreeProton_CT10nlo $
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT14nlo/" FreeProton_CT14nlo
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPPS16_CT14nlo
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/" BoundProton_EPS09_CT10nlo

fi


## Since ppbar collision takes place with different PDF distributions, it is worth cehcking
## how they turn out to be when they are flipped
run=$1
if [[ ${run} -eq  3 ]]; then


	pdfSetA="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo/"
	pdfSetB="PDF:pSet=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"
	submitJobs2 ${pdfSetA} ${pdfSetB} FreeAndBoundProton_EPS09_CT10nlo


	pdfSetA="PDF:pSetB=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"
	submitJobs2 ${pdfSetA} ${pdfSetB} FreeAndBoundProton_EPPS16_CT14nlo
	

	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo/"
	submitJobs2 ${pdfSetA} ${pdfSetB} FreeCT14AndFreeCT10

	pdfSetA="PDF:pSetB=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
	submitJobs2 ${pdfSetA} ${pdfSetB} FreeCT10AndFreeCT14

fi



# Make sure, the HardQCD{Quark, Gluon} are modified before submitting and 
# taken note of 
# Just the recent ones EPPS16, CT14nlo, bb, bf, ff
run=$1
if [[ ${run} -eq  31 ]]; then


	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"
	opt="HardQCDAll"

	echo "Running over the option:" $opt
	echo "for different collision system bf,ff,bb:EPPS16, CT14nlo"
	echo "------------------------"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPPS16_CT14nlo $opt


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT14nlo/" FreeProton_CT14nlo $opt
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPPS16_CT14nlo $opt

fi


# Make sure, the HardQCD{Quark, Gluon} are modified before submitting and 
# taken note of 
# Just the old ones EPS09, CT10nlo, bb, bf, ff
run=$1
if [[ ${run} -eq  32 ]]; then

	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"
	opt="HardQCDAll"

	echo "Running over the option:" $opt
	echo "for different collision system bf,ff,bb:EPS09, CT10nlo"
	echo "-----------------------------"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPS09_CT10nlo $opt


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"  FreeProton_CT10nlo $opt
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPPS16_CT14nlo $opt

fi


# Make sure, the HardQCD{Quark, Gluon} are modified before submitting and 
# taken note of 
# Just the recent ones EPPS16, CT14nlo, bb, bf, ff
run=$1
if [[ ${run} -eq  41 ]]; then


	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"
	opt="HardQCDGluon"

	echo "Running over the option:" $opt
	echo "for different collision system bf,ff,bb:EPPS16, CT14nlo"
	echo "------------------------"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPPS16_CT14nlo $opt


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT14nlo/" FreeProton_CT14nlo $opt
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPPS16_CT14nlo $opt

fi


# Make sure, the HardQCD{Quark, Gluon} are modified before submitting and 
# taken note of 
# Just the old ones EPS09, CT10nlo, bb, bf, ff
run=$1
if [[ ${run} -eq  42 ]]; then

	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"
	opt="HardQCDGluon"

	echo "Running over the option:" $opt
	echo "for different collision system bf,ff,bb:EPS09, CT10nlo"
	echo "------------------------"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPS09_CT10nlo $opt


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"  FreeProton_CT10nlo $opt
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPPS16_CT14nlo $opt
fi


# Make sure, the HardQCD{Quark, Gluon} are modified before submitting and # taken note of 
# Just the recent ones EPPS16, CT14nlo, bb, bf, ff
run=$1
if [[ ${run} -eq  51 ]]; then


	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT14nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/"
	opt="HardQCDQuark"

	echo "Running over the option:" $opt
	echo "for different collision system bf,ff,bb:EPPS16, CT14nlo"
	echo "------------------------"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPPS16_CT14nlo $opt


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT14nlo/" FreeProton_CT14nlo $opt
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPPS16_CT14nlo $opt

fi


# Make sure, the HardQCD{Quark, Gluon} are modified before submitting and 
# taken note of 
# Just the old ones EPS09, CT10nlo, bb, bf, ff
run=$1
if [[ ${run} -eq  52 ]]; then

	pdfSetA="PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"
	pdfSetB="PDF:pSetB=LHAPDF6:`pwd`/CT10nlo_EPS09NLOR_82_208/"
	opt="HardQCDQuark"

	echo "Running over the option:" $opt
	echo "for different collision system bf,ff,bb:EPS09, CT10nlo"
	echo "------------------------"
	submitJobs2 ${pdfSetA} ${pdfSetB} BoundAndFreeProton_EPS09_CT10nlo $opt


	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/CT10nlo/"  FreeProton_CT10nlo $opt
	submitJobs1 "PDF:pSet=LHAPDF6:`pwd`/EPPS16nlo_CT14nlo_Pb208/" BoundProton_EPS09_CT10nlo $opt
fi

if [[ $# == 0 ]]
then
	echo "-------------------------------"
	echo "Following options are available"
	echo "-------------------------------"

	echo new:EPPS16, CT14nlo, old:EPS09, CT10nlo
	echo ;
	echo 31: HardQCDAll_new, 32: HardQCDAll_old
	echo 41: HardQCDGluon_new, 42: HardQCDGluon_old
	echo 51: HardQCDQuark_new, 52: HardQCDQuark_old

	echo "-------------------------------"
fi


#CT10nlo/
#CT10nlo_EPS09NLOR_82_208/
#CT14nlo/
#EPPS16nlo_CT14nlo_Pb208/
