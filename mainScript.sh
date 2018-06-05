#!/bin/bash

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`

#echo "from mainScript.sh"
#echo ${SLURM_ARRAY_TASK_ID} 
#echo ${CMSRUN_FOLDERNAME}

export SEQ=${SLURM_ARRAY_TASK_ID}
export CMSRUN_LOGFILE=run-${CMSRUN_INFO}-${CMSRUN_PTHATLOW}-${CMSRUN_PTHATHIGH}-${SEQ}.log 
export CMSRUN_OUTPUT=Output_${CMSRUN_INFO}_${CMSRUN_PTHATLOW}to${CMSRUN_PTHATHIGH}-${SEQ}.root

#func(){
cmsRun Pythia8GenQCDAna_cfg.py \
	maxEvents="${CMSRUN_MAXEVENTS}" \
	Pythia8ConfigFile="${CMSRUN_PYTHIA8CONFIGFILE}" \
        userOptionsForPythia8="${CMSRUN_USEROPTIONSFORPYTHIA8}" \
        output="${CMSRUN_FOLDERNAME}/${CMSRUN_OUTPUT}" \
	ptHatLow="${CMSRUN_PTHATLOW}" \
        ptHatHigh="${CMSRUN_PTHATHIGH}" \
		&> "${CMSRUN_FOLDERNAME}/${CMSRUN_LOGFILE}" 

exit 0
