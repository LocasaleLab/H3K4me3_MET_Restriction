library("DESeq2")
directory <- "C:/Research/Histone Methylation/Data/Hct116_pmMET/RNA-seq/";
setwd(directory);
sampleFiles <- grep("treated",list.files(directory),value=TRUE);
sampleCondition <- sub("(.*treated).*","\\1",sampleFiles);
sampleTable <- data.frame(sampleName = sampleFiles,fileName = sampleFiles, condition=sampleCondition);
ddsHTSeq <- DESeqDataSetFromHTSeqCount(sampleTable = sampleTable, directory = directory,design= ~ condition);
dds <- ddsHTSeq[ rowSums(counts(ddsHTSeq)) > 1, ];
dds <- DESeq(dds);
res <- results(dds);

rld <- rlog(dds,blind=FALSE)
vsd <- varianceStabilizingTransformation(dds,blind=FALSE)
vsd.fast <- vst(dds,blind=FALSE)
expnorm=assay(rld);

P <- res[,5]
P_BH <- p.adjust(P, method = "BH", n = length(P))
write.csv(res,file = "DiffExpRes.csv")
write.csv(P_BH,file="PAdj_BH.txt")
write.csv(expnorm,file="Exp_12Mice.csv")
