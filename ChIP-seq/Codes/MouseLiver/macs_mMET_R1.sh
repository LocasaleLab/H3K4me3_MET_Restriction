#!/bin/bash
#SBATCH --mem=8000
#SBATCH --output=macs2_mMET_R1.output.txt

macs2 callpeak -t /dscrhome/zd24/DataCommons/MouseLiver/ChIP-seq/FASTQ/mMET_R1.sorted.bam -c /dscrhome/zd24/DataCommons/MouseLiver/ChIP-seq/FASTQ/Input_mMET.sorted.bam -n mMET_R1 -g mm --broad --bdg
sort -k1,1 -k2,2n mMET_R1_treat_pileup.bdg > mMET_R1_sorted_pileup.bdg
rm mMET_R1_treat_pileup.bdg
bedGraphToBigWig mMET_R1_sorted_pileup.bdg mm8.chrom.sizes.txt mMET_R1.bw
