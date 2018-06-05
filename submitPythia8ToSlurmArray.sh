#!/bin/bash


declare -a pTHat=(10 20 30 50 80 120 170 230 300 380 10000)
pTHat_size=${#pTHat[@]}

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

