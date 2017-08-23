#!/bin/bash

#Generate scripts for peak feature computation
#Submit PF_pMET_R1.sh PF_pMET_R2.sh PF_mMET_R1.sh PF_mMET_R2.sh after generating them

hrs=("pMET" "mMET")
declare -i i=0

line1="#!/bin/bash"
line2="#SBATCH --mem=8000"

for((i=1;i<=2;i=i+1))
do
	for j in "${hrs[@]}"
	do
		RunID="${j}_R${i}"
		Cmd1="./fc $RunID"
		Cmd2="./pf $RunID"
		shname="PF_${RunID}.sh"
		rm $shname
		touch $shname
	     	echo $line1 >> $shname
		echo $line2 >> $shname
	     	echo $Cmd1 >> $shname
		echo $Cmd2 >> $shname
		chmod +x $shname
		sbatch ./$shname
	done
done

