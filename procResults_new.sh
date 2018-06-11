#!/bin/bash


perpTHatProc(){


date=$1
cType=$3
xSec=( ${@:4} )
files=( ${cType}_10To20_$date ${cType}_20To30_$date ${cType}_30To50_$date ${cType}_50To80_$date ${cType}_80To120_$date ${cType}_120To170_$date ${cType}_170To230_$date ${cType}_230To300_$date ${cType}_300To380_$date ${cType}_380To10000_$date )

for e in `seq 0 9`
do
	export xs=`echo {\"${xSec[$e]}\"}`
	export fn=`echo {\"Output_${files[$e]}\"}`

	export label=${lType}_$date
	export fpath=/scratch/rjanjam/temp/$label

	# create the file in the local directory
	rm -rf $2/${label}
	mkdir $2
	mkdir $2/$label

	export outFile=$2/$label

	sleep 0.5

	sbatch -J ${date}_${e} --account=cms_stage3 --time=60:00 \
		--export=xs,fn,label,fpath,outFile \
		optdoSum.slurm
	#break;
done

}



fnhan(){

	colType=HardQCDAll

	date=$1
	lType=BoundAndFreeProton_EPPS16_CT14nlo
	xSec="1.8824 0.13655 0.031433 0.00376675 0.00045149 6.2701e-05 1.0165e-05 1.8787e-06 3.8172e-07 1.02095e-07"
	perpTHatProc $date $colType $lType $xSec

	date=$2
	lType=BoundProton_EPPS16_CT14nlo
	xSec="1.63105 0.128875 0.0302075 0.0038676 0.000466785 6.1404e-05 9.88e-06 1.89495e-06 3.60195e-07 1.0423e-07"
	perpTHatProc $date $colType $lType $xSec

	date=$3
	lType=FreeProton_CT14nlo
	xSec="1.9538 0.137345 0.031081 0.0036761 0.000349985 6.08135e-05 9.91555e-06 1.854e-06 3.82355e-07 9.3465e-08"
	perpTHatProc $date $colType $lType $xSec
}

# bf bb ff
fnhan 2018-06-10-232002 2018-06-10-232617 2018-06-10-232302


fnhqn(){

	colType=HardQCDQuark

	date=$1
	lType=BoundAndFreeProton_EPPS16_CT14nlo
	xSec="0.10152 0.0087455 0.00233115 0.00034324 5.1393e-05 8.9437e-06 1.80945e-06 4.14115e-07 1.02925e-07 3.691e-08"
	perpTHatProc $date $colType $lType $xSec

	date=$2
	lType=BoundProton_EPPS16_CT14nlo
	xSec="0.09458 0.00842045 0.00228835 0.000342985 5.19435e-05 9.0783e-06 1.83125e-06 4.15315e-07 1.0205e-07 3.5827e-08"
	perpTHatProc $date $colType $lType $xSec

	date=$3
	lType=FreeProton_CT14nlo
	xSec="0.108315 0.00906715 0.00237195 0.000343575 5.0842e-05 8.81445e-06 1.78575e-06 4.10635e-07 1.0307e-07 3.7532e-08"
	perpTHatProc $date $colType $lType $xSec
}

# bf bb ff
#fnhqn 2018-06-10-171447 2018-06-10-171634 2018-06-10-171658



fnhgn(){

	colType=HardQCDGluon

	date=$1
	lType=BoundAndFreeProton_EPPS16_CT14nlo
	xSec="1.1125 0.074825 0.0160275 0.00172205 0.000179405 2.121e-05 2.8631e-06 4.3203e-07 7.012e-08 1.44205e-08"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-08-131516
	lType=$2
	xSec="1.08095 0.0752515 0.0164305 0.00179365 0.000188245 2.2139e-05 2.9574e-06 4.37945e-07 6.9726e-08 1.393e-08"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-08-131232
	lType=$3
	xSec="1.13985 0.0741365 0.015615 0.00165135 0.000171185 2.0284e-05 2.76895e-06 4.25185e-07 7.0429e-08 1.4857e-08"
	perpTHatProc $date $colType $lType $xSec
}


#bf bb ff
#fnhgn 2018-06-09-102951 2018-06-09-103051 2018-06-09-103028





fnhao(){

	colType=HardQCDAll

	date=2018-06-07-130628
	lType=BoundAndFreeProton_EPS09_CT10nlo
	xSec="1.8824 0.13655 0.031433 0.00376675 0.00045149 6.2701e-05 1.0165e-05 1.8787e-06 3.8172e-07 1.02095e-07"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-07-131849
	lType=BoundProton_EPS09_CT10nlo
	xSec="1.63105 0.128875 0.0302075 0.0038676 0.000466785 6.1404e-05 9.88e-06 1.89495e-06 3.60195e-07 1.0423e-07"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-07-131551
	lType=FreeProton_CT10nlo
	xSec="1.9538 0.137345 0.031081 0.0036761 0.000349985 6.08135e-05 9.91555e-06 1.854e-06 3.82355e-07 9.3465e-08"
	perpTHatProc $date $colType $lType $xSec
}


fnhqo(){

	colType=HardQCDQuark

	date=2018-06-07-130628
	lType=BoundAndFreeProton_EPS09_CT10nlo
	xSec="1.8824 0.13655 0.031433 0.00376675 0.00045149 6.2701e-05 1.0165e-05 1.8787e-06 3.8172e-07 1.02095e-07"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-07-131849
	lType=BoundProton_EPS09_CT10nlo
	xSec="1.63105 0.128875 0.0302075 0.0038676 0.000466785 6.1404e-05 9.88e-06 1.89495e-06 3.60195e-07 1.0423e-07"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-07-131551
	lType=FreeProton_CT10nlo
	xSec="1.9538 0.137345 0.031081 0.0036761 0.000349985 6.08135e-05 9.91555e-06 1.854e-06 3.82355e-07 9.3465e-08"
	perpTHatProc $date $colType $lType $xSec
}


fnhgo(){

	colType=HardQCDGluon

	date=2018-06-07-130628
	lType=BoundAndFreeProton_EPS09_CT10nlo
	xSec="1.8824 0.13655 0.031433 0.00376675 0.00045149 6.2701e-05 1.0165e-05 1.8787e-06 3.8172e-07 1.02095e-07"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-07-131849
	lType=BoundProton_EPS09_CT10nlo
	xSec="1.63105 0.128875 0.0302075 0.0038676 0.000466785 6.1404e-05 9.88e-06 1.89495e-06 3.60195e-07 1.0423e-07"
	perpTHatProc $date $colType $lType $xSec

	date=2018-06-07-131551
	lType=FreeProton_CT10nlo
	xSec="1.9538 0.137345 0.031081 0.0036761 0.000349985 6.08135e-05 9.91555e-06 1.854e-06 3.82355e-07 9.3465e-08"
	perpTHatProc $date $colType $lType $xSec
}
