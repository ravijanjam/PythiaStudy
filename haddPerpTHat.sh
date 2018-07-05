
#take the datestamp

#search for all the directories

echo "from $0"

haddFromScratch(){

	path=/scratch/rjanjam
	date=$1
	lff=(`ls -d ${path}/*${date}`)

	temp=${path}/temp
	fdName=`echo ${lff[0]} | sed 's/\/scratch\/rjanjam\///g' | sed -n 's/\(.*\)_[0-9]\{1,\}To[0-9]\{1,\}_\(.*\)/\1_\2/p'`

	echo "before loop" ${temp}/${fdName}
	rm -rf ${temp}/${fdName} &> /dev/null
	es=`echo $?`
	if [[ ${es} -eq "0" ]]
	then
		#echo $es
		echo "creating a new directory in:" $temp
		mkdir ${temp}/${fdName}
	fi

	for fd in ${lff[@]}
	do

		ls $fd/*.root

		echo ;
		echo ${fdName}
		fk=`echo ${fdName} | sed -n 's/\(.*\)_[0-9]\{4\}-[0-9]\{2\}.*/\1/p'`
		ptHat=`echo ${fd} | sed 's/\/scratch\/rjanjam\///g' | sed -n 's/\(.*\)_\([0-9]\{1,\}To[0-9]\{1,\}\)_\(.*\)/\2/p'`
		echo "ptHat" ${ptHat}

		echo "fk" $fk
		export outFile=${temp}/${fdName}/Output_${fk}_${ptHat}_${date}.root
		export inPath=${fd}

		echo $inPath
		echo "outfile" $outFile
		#export fn=

		#break;
		# One job per pT Hat i.e 20 files are hadded per job
		sbatch -J $date --account=cms_stage3 --time=60:00 \
			--export=inPath,outFile \
			forhaddPerpTHat.slurm

		#break
		sleep 1

	done
}

date=2018-06-11-094500
haddFromScratch $date

date=2018-06-11-094523
haddFromScratch $date

date=2018-06-11-094743
haddFromScratch $date

