#!/bin/bash
#SBATCH --mem=6000

sort -k2,2 -k3,3n GenomicRegions.txt > GR_Sorted.txt
