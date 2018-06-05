#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=32G
#SBATCH --time=0-48:00:00
#SBATCH --array=1-20
#SBATCH --account=cms_stage3


echo "from runpythia"
echo "=============="

echo "foldername: " ${CMSRUN_FOLDERNAME}
#srun --task-epilog=${SLURM_SUBMIT_DIR}/resubmitIfJobFailed.sh ./mainScript.sh ${SLURM_ARRAY_TASK_ID}
srun ./mainScript.sh ${SLURM_ARRAY_TASK_ID}

echo "-----done------"
