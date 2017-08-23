load pMET_R1_PeakFeatures.txt
load pMET_R2_PeakFeatures.txt
load mMET_R1_PeakFeatures.txt
load mMET_R2_PeakFeatures.txt

PFNames={'Height','Area','Width','Entropy','M3','M4','M5'};

pMET_PF=pMET_R1_PeakFeatures/2+pMET_R2_PeakFeatures/2;
mMET_PF=mMET_R1_PeakFeatures/2+mMET_R2_PeakFeatures/2;
Genes=textread('Genes_MergedPeaks.txt','%s','delimiter','\n');
BadPos=find(0*sum(pMET_PF')~=0 | 0*sum(mMET_PF')~=0);
Genes(BadPos)=[];
pMET_PF(BadPos,:)=[];
mMET_PF(BadPos,:)=[];

ChangePF=mMET_PF(:,1:3)./pMET_PF(:,1:3);

figure;
for i=1:3
    subplot(1,3,i);
    title(PFNames(i));
    dscatter(pMET_PF(:,i),mMET_PF(:,i));
	xlabel('+MET');
	ylabel('-MET');
end

HC=ChangePF(:,1);
AC=ChangePF(:,2);
WC=ChangePF(:,3);