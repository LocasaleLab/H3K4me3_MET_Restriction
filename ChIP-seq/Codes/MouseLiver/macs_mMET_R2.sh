#!/bin/bash
#SBATCH --mem=8000
#SBATCH --output=macs2_mMET_R2.output.txt

macs2 callpeak -t /dscrhome/zd24/DataCommons/MouseLiver/ChIP-seq/FASTQ/mMET_R2.sorted.bam -c /dscrhome/zd24/DataCommons/MouseLiver/ChIP-seq/FASTQ/Input_mMET.sorted.bam -n mMET_R2 -g mm --broad --bdg
sort -k1,1 -k2,2n mMET_R2_treat_pileup.bdg > mMET_R2_sorted_pileup.bdg
rm mMET_R2_treat_pileup.bdg
bedGraphToBigWig mMET_R2_sorted_pileup.bdg mm8.chrom.sizes.txt mMET_R2.bw
