#!/bin/bash


declare -A hqn hqo

hqn["Quark"]="{\"bf\",\"bb\",\"ff\"}"
hqn["Gluon"]="{\"bf\",\"bb\",\"ff\"}"
hqn["All"]="{\"bf\",\"bb\",\"ff\"}"

echo ${hqn["Quark"]}
root -b -q testPlot2.C+"(${hqn["Quark"]}, ${hqn["Gluon"]}, ${hqn["All"]})"
#root -b -q testPlot2.C+"(${hqn["Quark"]})"

# Combine the files
haddFiles(){

	# get into HardQCDQuark, HardQCDGluon HardQCDAll
	fdl=( HardQCDQuark HardQCDGluon HardQCDAll )

	# Remove the previously hadded files
	for folder in ${fdl[@]}
	do
		mv $folder/*.root dump &> /dev/null
	done

	for folder in ${fdl[@]}
	do
		echo $folder


		lsf=`ls -d $folder/*`

		# Loop over subdirectories
		for ff in ${lsf[@]}
		do
			#echo "name" $ff
			#ls "in curr dir" $folder/*.root

			fName=`echo $ff | sed -n 's/\(.*\)\/\(.*\)/ptHatCombined_\2/p'`
			echo $fName

			# hadd the files and rename, keep date stamp
			hadd $ff/${fName}.root $ff/*.root

			# write to one directory above in 
			# same proctype directory
			mv $ff/${fName}.root $ff/../
			echo ;
			#break;
		done

		echo "after processing..."
		ls $folder/*.root

		echo ; echo ;
		break;
	done

}

#haddFiles


# Process the pT Hat Combined files with the macro 
genResults(){

	procType=$1

	# get the list of root files [r1,r2...,rn.root]
	flist=`ls $procType/*.root`

	# loop over the list of root files
	for file in ${flist[@]}
	do

		# generate labels for the macro xVSpT_dev.C
		k=`sed -n 's/\(.*\)\/\(.*\)_\(20.*\)/\2/p' <<< ${file}`
		oFileName=${procType}_${k}.root

		echo "infilepath:" $file $inFilePath $oFileName

		# call the macro: takes input and plots x{1,2} vs pT{+,-}
		# and writes to the current directory
		root -b -q xVSpT_dev.C+"(\"${file}\", \"${procType}\", \"${oFileName}\")"
		break

	done

}

opt=HardQCDQuark
#genResults $opt

opt=HardQCDGluon
#genResults $opt

opt=HardQCDAll
#genResults $opt


# use this for x{1,2} vs pT for the combining pT Hats per process type
genResults1(){
declare -A hq hg ha

	procType=$1

	# Get list of files per process type for example HardQCDAll
	flist=`ls -d ${procType}`

	echo "list of folders scanned"
	echo ${flist[@]}

	for file in ${flist[@]}
	do
		k=`sed -n 's/\(.*\)\/\(.*\)_\(20.*\)/\2/p' <<< ${file}`
		echo -e $file "\t" $k 


		#mtype="HardQCDQuark" # HardQCDGluon, HardQCDAll
		inFilePath=`ls ${procType}/Result_${file}*.root`
		oFileName=${procType}_${k}.root

		echo "infilepath:" $inFilePath $oFile

		# this is the combined file
		# search for HardQCDQuark/Result_$a*.root

		#root -b -q xVSpT_dev.C+"(\"${inFilePath}\", \"${mtype}\", \"${oFileName}\")"
	done
}

#procType="HardQCDQuark"
#genResults1 $procType



funcA(){

declare -A hq hg ha

# steps to populate the paths into associative arrays for calling
# easily i.e. hq[bb] = path to bb
ahq=( \
	"BoundProton_EPPS16_CT14nlo_2018-06-05-114432" \
	"BoundProton_EPPS16_CT14nlo_2018-06-05-114432"
	"FreeProton_CT10nlo_2018-06-05-114343" \
	"FreeProton_CT14nlo_2018-06-06-100723" \
	"BoundAndFreeProton_EPPS16_CT14nlo_2018-06-05-114319" \
	"BoundAndFreeProton_EPS09_CT10nlo_2018-06-05-114257" \
)

echo ${ahq[@]}

p=`sed -n 's/\(.*\)_\(2018-06-.*\)/\1/p' <<< ${ahq[0]}`
echo "p" $p

# for HardQCDQuark only
for e in ${ahq[@]}
do
	k=`sed -n 's/\(.*\)_\(2018-06-.*\)/\1/p' <<< ${e}`
	echo $e, $k
	hq["$k"]=$e
done

echo "after populating: FILEPATH " ${hq[@]}, ${!hq[@]}


# folder path to processed root files [HardQCDQuark, HardQCDGluon, HardQCDAll]
# per folder HardQCDQuark(bb, bf, ff), on top, the Result files are present
#a=BoundAndFreeProton_EPPS16_CT14nlo
#a=FreeProton_CT14nlo 
#a=BoundProton_EPPS16_CT14nlo 
#a=FreeProton_CT10nlo

k=BoundAndFreeProton_EPPS16_CT14nlo
mtype="HardQCDQuark" # HardQCDGluon, HardQCDAll
inFilePath=`ls ${mtype}/Result_${!hq[$k]}*.root`
oFileName=${mtype}_${k}.root

echo "infilepath:" $inFilePath $oFile

# this is the combined file
# search for HardQCDQuark/Result_$a*.root

root -b -q xVSpT_dev.C+"(\"${inFilePath}\", \"${mtype}\", \"${oFileName}\")"
#root -l xVSpT_dev.C+"(\"${inFilePath}\", \"${mtype}\", \"${oFileName}\")"
}


# Remove ROOT compiled files
echo ; echo ;
rm *.pcm *.so *.d &> /dev/null
