#!/bin/bash

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`

echo "from $0"
echo "--------------------------"
echo $inPath, $outFile
ls $inPath/*.root

hadd -f ${outFile} ${inPath}/*.root
