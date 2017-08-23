#!/bin/bash

#Generate and submit scripts for alignment data processing

line0="#!/bin/bash"
line1="#SBATCH --mem=16000"

IDs=("pMET_R1" "pMET_R2" "mMET_R1" "mMET_R2" "Input_pMET" "Input_mMET")

declare -i i=0
for id in ${IDs[@]}
do
	samname="${id}.sam"
	bamname="${id}.bam"
	bsname="${id}.sorted.bam"
	shname="sam_${id}.sh"
	cmd1="samtools view -bS -q 5 ${samname} > ${bamname}"
	cmd2="rm -f ${samname}"
	cmd3="samtools sort -m 4000000000 ${bamname} -o ${bsname}"
	cmd4="rm -f ${bamname}"
	cmd5="samtools index ${bsname}" 
	cmd6="samtools flagstat ${bsname}"
	echo $line0 > $shname
	echo $line1 >> $shname
	echo $cmd1 >> $shname
	echo $cmd2 >> $shname
	echo $cmd3 >> $shname
	echo $cmd4 >> $shname
	echo $cmd5 >> $shname
	echo $cmd6 >> $shname
	chmod +x $shname
	sleep 10
	sbatch ./$shname
done
