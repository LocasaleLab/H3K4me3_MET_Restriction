#!/bin/bash
#SBATCH --mem=16000

#Do peak annotation for H3K4me3 peaks in mouse liver

annotatePeaks.pl Homer_MergedPeaks.txt mm8 > Annotation_MergedPeaks.txt

