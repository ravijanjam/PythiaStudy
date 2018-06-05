#!/bin/bash

source ~/.BashFormatting.sh
fRed="\e[38;5;196m"

message()
{

		fbDGray="\e[48;5;234m"
		fcRed="\e[38;5;124m"
		fcPWhite="\e[38;5;128m"
		fb="\e[1;4m"
		clear 
		echo "Run the script with the following :"
		echo "==================================="

		echo "For all manual options"
		echo "-------------------------------------------------------------------------------------------------"
		echo -e "${fbDGray} $0 allM  ${fc}${fu}<PDFSetName>${fc} ${fu}<Process-Switch>${fc} ${fu}<nEventsPerJob>${fc} "
		echo -e "\t \t \t \t ${fu}<nJobs>${fc} ${fu}<pTHatLow>${fc} ${fu}<pTHatHigh>${fc} ${fu}<timeStamp> ${fc}${fc}"

		echo ;
		echo -e "Eg: ${fcRed}$0 allM NNPDF2.3-QCD+QED-NNLO HardQCDAll 20 30 2016-08-26-155615 20 500000${fc}" 
		echo "-------------------------------------------------------------------------------------------------"
		echo ; echo ; 

		echo "To run over all {PDF Sets, Process Switches, pTHats}"
		echo "-------------------------------------------------------------------------------------------------"
		echo -e "${fbDGray}$0 all  ${fc}"
		echo "-------------------------------------------------------------------------------------------------"
		echo ; echo ; 


		echo "To run over 5:PDFSets, 11:pTHats, 1:Process Switch  "
		echo "-------------------------------------------------------------------------------------------------"
		echo -e "${fbDGray}$0 allPDFNpTHat <Process-Switch>${fc}"
		echo "-------------------------------------------------------------------------------------------------"
		echo ; echo ; 

		echo "To run over selected (PDFSets, Process Switch) and all the 11:pTHats  "
		echo "-------------------------------------------------------------------------------------------------"
		echo -e "${fbDGray}$0 pdfNPrSw <Process-Switch> <PDFSetName> ${fc}"
		echo "-------------------------------------------------------------------------------------------------"
		echo ; echo ;

		echo "================================================================================================="
		echo -e "Options available :"
		echo "----------------------"
		echo -e "${fb}pTHats${fc}: \e[36m${pTHat[@]}${fc}"
		echo ; 
		echo -e "${fb}Process Switches${fc}: \e[37m${procSwitchLabels[@]}${fc}"
		echo ; 
		echo -e "${fb}PDF Set Labels: ${fc} ${fcPWhite}${pdfSetLabelsComplete[@]}${fc}"
		echo ; 
		echo ; echo ; 


		echo ; echo ; 
}

declare -a pTHat=(10 20 30 50 80 120 170 230 300 380 10000)
pTHat_size=${#pTHat[@]}

declare -a procSwitchLabels=( Inelastic\
			NSD \
			HardQCDSelectionGluon \
			HardQCDSelectionQuark \
			HardQCDAll \
			HardQCDMix \
			)

procSwitchLabels_size=${#procSwitchLabels[@]}



declare -a pdfSetLabels=( GRV94L-LO \
			 MRST-LO\(2008-CentralMember\) \
			 CTEQ6L1-LO \
			 CT09MC2-NLO \
			 NNPDF2.3-QCD+QED-NNLO \ 
			 )

pdfSetLabels_size=${#pdfSetLabels[@]}

declare -a pdfSetLabelsComplete=(GRV94L-LO \
			CTEQ5L-LO \
			MRST-LO\(2007\) \
			MRST-LO\(2008\) \
			MRST-LO\(2008-CentralMember\) \
			MRST-NLO\(2008-CentralMember\) \
			CTEQ6L-NLO \
			CTEQ6L1-LO \
			CTEQ66.00 \
			CT09MC1-LO \
			CT09MC2-NLO \
			CT09MCS-NLO \
			NNPDF2.3-QCD+QED-LO13 \
			NNPDF2.3-QCD+QED-LO14 \
			NNPDF2.3-QCD+QED-NLO \
			NNPDF2.3-QCD+QED-NNLO \
			)

pdfSetLabelsComplete_size=${#pdfSetLabelsComplete[@]}


#selectPDFNProcNpTHat(){
selectPDFNProcNpTHat(){

	echo $check;
	echo ; echo ; 
	infoLabel=$1
	pythia8ConfigFile=$2
	userOptionsForPythia8=$3
	lowpTHat=$4
	highpTHat=$5
	folderName=$6
	nJobs=$7
	nEventsPerJob=$8

	echo "====================================================================================================="
	echo "Running over selected PDF for ${nEventsPerJob} (events per job) x ${nJobs} (parallel jobs) with the options" \
              ${infoLabel} ${Procame} ${lowpTHat} ${highpTHat}
	echo "====================================================================================================="

	# per pT bin
	#for SEQ in {1..20}; do
	#for SEQ in $(eval echo {1..${nJobs}}); do

		export USER_ACCOUNT=cms_stage3
		export CMSRUN_MAXEVENTS=${nEventsPerJob}
		export CMSRUN_PROCESSTYPE=${ProcName}
		export CMSRUN_INFO=${infoLabel}

		export CMSRUN_PTHATLOW=${lowpTHat}
		export CMSRUN_PTHATHIGH=${highpTHat}
		export CMSRUN_OUTPUT=Output_${CMSRUN_INFO}_${CMSRUN_PROCESSTYPE}_${CMSRUN_PTHATLOW}to${CMSRUN_PTHATHIGH}_${SEQ}.root
		export CMSRUN_FOLDERNAME=${folderName}
		export CMSRUN_USEROPTIONSFORPYTHIA8=${userOptionsForPythia8}


			#-o "${CMSRUN_FOLDERNAME}/${CMSRUN_PDFSETNAME}_${CMSRUN_PROCESSTYPE}.o%j" \
			#-e "${CMSRUN_FOLDERNAME}/${CMSRUN_PDFSETNAME}_${CMSRUN_PROCESSTYPE}.e%j" \


		func(){
		sbatch -J "${CMSRUN_PDFSETNAME}_${CMSRUN_PROCESSTYPE}" \
			runPYTHIA8_SelectedProcesses_4C_v2.slurm \
				--account=${USER_ACCOUNT} \
				--export=CMSRUN_TUNE,\
				CMSRUN_MAXEVENTS,\
				CMSRUN_OUTPUT \
				CMSRUN_LOGFILE,\
				CMSRUN_FOLDERNAME,\
				CMSRUN_PTHATLOW,\
				CMSRUN_PTHATHIGH,\
				CMSRUN_INFO,\
				CMSRUN_USEROPTIONSFORPYTHIA8
				
		}
		echo "Name of output file " ${CMSRUN_PDFSETNAME} "in the folder: " ${folderName}
		echo ; echo ; 
	
	#done
}


checkPDFLabel(){

	count=1
	for pdf in ${pdfSetLabelsComplete[@]}
	do
		if [[ $1 == ${pdf} ]]
		then 
			echo -e "\e[48;5;34m Match found for $1 ${fc}" 
			echo ;
			break
		fi
	

		if [[ ${count} == ${pdfSetLabelsComplete_size} ]]
		then
			echo -e "\e[38;5;160mMatch not found for $1${fc}"
			echo -e "${fblink}${fRed} ERROR : Incorrect PDF Label ${fc}${fc}"
			exit 1;
		fi
		let count=count+1
	done

}


checkProcSwLabel(){

	count=1
	for ps in ${procSwitchLabels[@]}
	do
		if [[ $1 == ${ps} ]]
		then 
			echo -e "\e[48;5;34m Match found for $1 ${fc}" 
			echo ;
			break;
			#exit 0
		fi
		#echo $count, $procSwitchLabels_size

		if [[ ${count} == ${procSwitchLabels_size} ]]
		then
			echo -e "\e[38;5;160mMatch not found for $1${fc}"
			echo -e "${fblink}${fRed} ERROR : Incorrect Process Switch Label ${fc}${fc}"
			exit 1;
		fi
		let count=count+1
	done

}

proceedToSubmission(){

		echo "Type y, then press [ENTER], if you would like to proceed..."
		read check
		if [[ ${check} != "y" ]];then 
			echo "You chose to quit"
			exit 1
		fi
}

		
case $1 in 

	pdfNPrSw)

		if [ $# != 4 ];then message; exit 1; fi;

		infoLabel=$2
		pythia8ConfigFile=$3
		userOptionsForPythia8=$4

		# Write a check to chcck if the config file is peroperly json formatted

		echo ; echo ; 
		clear

		nJobs=20
		nEventsPerJob=50000
		timeStamp=`date +%F-%H%M%S`
		echo ${timeStamp}

		echo -e "Running jobs for the parameter file: ${fcBlue}${pythia8ConfigFile}${fc}"
		echo -e "and additional options provided by user: ${fcBlue}${userOptionsForPythia8}${fc}" 

		echo "========================================================================="
		echo -e "\n ${nEventsPerJob} Events Per Job x ${nJobs} Jobs"
		echo "========================================================================="

		# Ask for user if he would like to submit
		proceedToSubmission

		seq_pTHat={1..$(( ${pTHat_size}-1 ))}

				for i in $(eval echo ${seq_pTHat})
				do
					lowPt=${pTHat[$(( i-1 ))]}
					highPt=${pTHat[$(( i ))]}
					folderName=/scratch/rjanjam/${infoLabel}_${lowPt}To${highPt}_${timeStamp}

					mkdir ${folderName}

					echo ${folderName} 
					echo ${folderName} >> jobSubmissions_${infoLabel}_${timeStamp}.txt

					echo "Submitting Jobs in the pTHat Interval : " ${lowPt}, ${highPt} "in the folder: " ${folderName}
					echo "--------------------------------------------------------------------"




	echo "====================================================================================================="
	echo "Running over selected PDF for ${nEventsPerJob} (events per job) x ${nJobs} (parallel jobs) with the options" \
              ${infoLabel} ${lowpTHat} ${highpTHat}
	echo "====================================================================================================="

		export USER_ACCOUNT=cms_stage3
		export CMSRUN_MAXEVENTS=${nEventsPerJob}
		export CMSRUN_PROCESSTYPE=${ProcName}
		export CMSRUN_INFO=${infoLabel}

		export CMSRUN_PTHATLOW=${lowPt}
		export CMSRUN_PTHATHIGH=${highPt}
		export CMSRUN_OUTPUT=Output_${CMSRUN_INFO}_${CMSRUN_PTHATLOW}to${CMSRUN_PTHATHIGH}.root
		export CMSRUN_FOLDERNAME=${folderName}
		export CMSRUN_USEROPTIONSFORPYTHIA8=${userOptionsForPythia8}
		export CMSRUN_PYTHIA8CONFIGFILE=${pythia8ConfigFile}

			#-o "${CMSRUN_FOLDERNAME}/${CMSRUN_PDFSETNAME}_${CMSRUN_PROCESSTYPE}.o%j" \
			#-e "${CMSRUN_FOLDERNAME}/${CMSRUN_PDFSETNAME}_${CMSRUN_PROCESSTYPE}.e%j" \


funca(){
echo "==============================================="
echo "Environment Variables from submission script.sh" 
echo "==============================================="
echo -e "${CMSRUN_FOLDERNAME}/${CMSRUN_OUTPUT}\n" \
"${CMSRUN_MAXEVENTS}\n" \
"${CMSRUN_INFO}\n" \
"${CMSRUN_PTHATLOW}\n" \
"${CMSRUN_PTHATHIGH}\n" \
"${CMSRUN_FOLDERNAME}/${CMSRUN_LOGFILE}\n" \
"user options: ${CMSRUN_USEROPTIONSFORPYTHIA8}" \
${CMSRUN_OUTPUT}
echo "==============================================="
}


		sbatch -J "${CMSRUN_INFO}" \
			runPYTHIA8_SelectedProcesses_4C.slurm \
				--account=${USER_ACCOUNT} \
				--export=CMSRUN_TUNE,\
				CMSRUN_MAXEVENTS,\
				CMSRUN_OUTPUT, \
				CMSRUN_LOGFILE,\
				CMSRUN_FOLDERNAME,\
				CMSRUN_PTHATLOW,\
				CMSRUN_PTHATHIGH,\
				CMSRUN_INFO,\
				CMSRUN_USEROPTIONSFORPYTHIA8,\
				CMSRUN_PYTHIA8CONFIGFILE

				
		echo  "Jobs are in the folder: " ${folderName}
		echo ; echo ; 
					echo -c "${fblink} Waiting for 2 seconds, before going to the next ptHat range, to not overwhelm slurm ${fc}"
					sleep 2s

		done

				;;
	*)
		message	
esac

