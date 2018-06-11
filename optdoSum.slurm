#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=4G
#SBATCH --time=0-0:60:00
#SBATCH --account=cms_stage3

#echo $xs, $fn, $label, $fpath, $outFile


source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`

echo $xs $fn $fpath $outFile
root -b -q "doSum_new.C+(${xs}, ${fn}, \"${fpath}\", \"${outFile}\")"
