#!/bin/bash

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
		#break;
	done

}

haddFiles


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

