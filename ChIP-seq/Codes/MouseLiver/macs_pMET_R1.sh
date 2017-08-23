#!/bin/bash
#SBATCH --mem=8000
#SBATCH --output=macs2_pMET_R1.output.txt

macs2 callpeak -t /dscrhome/zd24/DataCommons/MouseLiver/ChIP-seq/FASTQ/pMET_R1.sorted.bam -c /dscrhome/zd24/DataCommons/MouseLiver/ChIP-seq/FASTQ/Input_pMET.sorted.bam -n pMET_R1 -g mm --broad --bdg
sort -k1,1 -k2,2n pMET_R1_treat_pileup.bdg > pMET_R1_sorted_pileup.bdg
rm pMET_R1_treat_pileup.bdg
bedGraphToBigWig pMET_R1_sorted_pileup.bdg mm8.chrom.sizes.txt pMET_R1.bw
