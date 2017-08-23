#!/bin/bash

#Generate and submit scripts for raw reads alignment

line0="#!/bin/bash"
line1="#SBATCH --mem=16000"
line2="#SBATCH --cpus-per-task=8"

IDs=("pMET_R1" "pMET_R2" "mMET_R1" "mMET_R2" "Input_pMET" "Input_mMET")

declare -i i=0
for id in ${IDs[@]}
do
	name="${id}.fastq.gz"
	samname="${id}.sam"
	shname="align_${id}.sh"
	cmd="~/Histone_Methylation/Tools/bowtie2-2.2.9/bowtie2 -p 8 -x ~/DataCommons/Genome/mm8 -U $name -S $samname"
	echo $line0 > line0.txt
	echo $line1 > line1.txt
	echo $line2 > line2.txt
	echo $cmd > cmd.txt
	cat line0.txt line1.txt line2.txt cmd.txt > $shname
	chmod +x $shname
	sleep 2
	sbatch ./$shname
done
