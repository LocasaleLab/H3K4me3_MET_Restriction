#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>

//Calculate subpeak area, width and count relevant to genomic element types


using namespace std;


struct Region{
	string chr;
	int p1;
	int p2;
};
typedef Region *PRegion;

struct RegionList{
	vector<Region> regions;
	vector<int> types;
};

struct PosList{
	vector<Region> regions;
	vector<double> pileups;
};

vector<string> splitString(string str, char delimiter)
//Split string to several substrings by a predefined delimiter
{
	vector<string> internal;
	stringstream ss(str);
	string tok;
	while(getline(ss,tok,delimiter))
		internal.push_back(tok);
	return internal;
}

int str2int(string str)
{
	stringstream ss(str);
	int val;
	ss>>val;
	return val;
}

double str2double(string str)
{
	stringstream ss(str);
	double val;
	ss>>val;
	return val;
}

PosList readPosInfo(string posFile)
//Read peak coordinates from file
{
	ifstream fin(posFile.c_str());
	string line;
	vector<Region> poses;
	vector<double> hs;
	if(fin.is_open())
	{
		while(getline(fin,line))
		{
			vector<string> words=splitString(line,'\t');
			string chr=words[0];
			string begin=words[1];
			string end=words[2];
			string h=words[3];
			Region pos;
			pos.chr=chr;
			pos.p1=str2int(begin);
			pos.p2=str2int(end);
			double pileup=str2double(h);
			poses.push_back(pos);
			hs.push_back(pileup);
		}
		fin.close();
	}
	PosList PL;
	PL.regions=poses;
	PL.pileups=hs;
	return PL;
}


RegionList readRegionInfo(void)
//Read genomic region annotations
{
	vector<string> annoTypes;
	string s0("Intergenic");
	string s1("promoter-TSS");
	string s2("non-coding");
	string s3("intron");
	string s4("TTS");
	string s5("exon");
	string s6("5'UTR");
	string s7("3'UTR");
	annoTypes.clear();
	annoTypes.push_back(s0);
	annoTypes.push_back(s1);
	annoTypes.push_back(s2);
	annoTypes.push_back(s3);
	annoTypes.push_back(s4);
	annoTypes.push_back(s5);
	annoTypes.push_back(s6);
	annoTypes.push_back(s7);
	ifstream fin("GenomicRegions.txt");
	string line;
	vector<Region> regions;
	vector<int> types;
	if(fin.is_open())
	{
		while(getline(fin,line))
		{
			vector<string> words=splitString(line,'\t');
			string type=words[0];
			string chr=words[1];
			string begin=words[2];
			string end=words[3];
			//end=end.substr(0,end.size()-1);
			Region reg;
			reg.chr=chr;
			stringstream ss1(begin);
			stringstream ss2(end);
			int vbegin,vend,vtype;
			vtype=find(annoTypes.begin(),annoTypes.end(),type)-annoTypes.begin();
			ss1 >> vbegin;
			ss2 >> vend;
			reg.p1=vbegin;
			reg.p2=vend + 1;
			types.push_back(vtype);
			regions.push_back(reg);
		}
	}
	RegionList RL;
	RL.regions=regions;
	RL.types=types;
	return RL;
}
			


inline bool isOverlap(Region R1,Region R2)
//Check if two regions overlap with each other
{
	if(R1.chr.compare(R2.chr)==0 && max(R1.p2,R2.p2)-min(R1.p1,R2.p1)<=R1.p2-R1.p1+R2.p2-R2.p1)
		return true;
	else
		return false;
}

RegionList splitPeak(Region Peak,vector<Region> RegionLib,vector<int> RegionType,int *CurrentPos)
//Split a peak region to several subpeaks according to genomic elements covered
{
	int pos=*CurrentPos;
	while(isOverlap(Peak,RegionLib.at(pos++))==false && pos<RegionLib.size())
	{
		;
	}
	pos=pos-1;
	vector<Region> SubRegions;
	SubRegions.clear();
	vector<int> SubRegionTypes;
	SubRegionTypes.clear();
	bool foundTag=false;
	while(isOverlap(Peak,RegionLib.at(pos++))==true && pos<RegionLib.size())
	{
		foundTag=true;
		Region Intersect;
		Intersect.chr=Peak.chr;
		Intersect.p1=max(Peak.p1,RegionLib.at(pos-1).p1);
		Intersect.p2=min(Peak.p2,RegionLib.at(pos-1).p2);
		SubRegions.push_back(Intersect);
		SubRegionTypes.push_back(RegionType.at(pos-1));
	}
	if(foundTag==true)
		*CurrentPos=pos-2;
	RegionList SRI;
	SRI.regions=SubRegions;
	SRI.types=SubRegionTypes;
	return SRI;
}



int main(int argc, char *argv[])
{
	string RunID(argv[1]);
	string bdg("_sorted_pileup.bdg");
	string area("_Area_GenomeType.txt");
	string len("_Width_GenomeType.txt");
	string cnt("_Count_GenomeType.txt");
	string bdgFile=RunID+bdg;
	string areaFile=RunID+area;
	string lenFile=RunID+len;
	string cntFile=RunID+cnt;
	string posFile="MergedPeaks.bed";
	PosList PL=readPosInfo(bdgFile);
	RegionList RL=readRegionInfo();
	cout<<"Read Finished!"<<endl;
	int search_start=0;
	int search_end=PL.regions.size();
	ofstream farea(areaFile.c_str());
	ofstream flen(lenFile.c_str());
	ofstream fcnt(cntFile.c_str());
	int OldPos=0;
	ifstream fin(posFile.c_str());
	string line;
	int CurrentPos=0;
	if(fin.is_open())
	{
		while(getline(fin,line))
		{
			cout<<line<<endl;
			vector<string> words=splitString(line,'\t');
			string chro=words[0];
			int begin=str2int(words[1]);
			int end=str2int(words[2]);//.substr(0,words[2].size()-1));
			Region Peak;
			Peak.chr=chro;
			Peak.p1=begin;
			Peak.p2=end;
			RegionList SubRL=splitPeak(Peak,RL.regions,RL.types,&CurrentPos);
			vector<double> AreaType;
			vector<int> LenType;
			vector<int> CntType;
			for(int i=0;i<8;i++)
			{
				AreaType.push_back(0);
				LenType.push_back(0);
				CntType.push_back(0);
			}
			int nreg=SubRL.regions.size();
			for(int i=0;i<nreg;i++)
			{
				int sptype=SubRL.types[i];
				Region subpeak=SubRL.regions[i];
				int width=min(subpeak.p2,Peak.p2)-max(subpeak.p1,Peak.p1);
				LenType[sptype]=LenType[sptype]+width;
				CntType[sptype]=CntType[sptype]+1;
				for(int j=search_start;j<search_end;j++)
				{
					Region pos=PL.regions[j];
					bool foundTag=false;
					if(isOverlap(pos,subpeak)==true)
					{
						int pend=min(pos.p2,subpeak.p2);
						int pbegin=max(pos.p1,subpeak.p1);
						double pile=PL.pileups[j];
						AreaType[sptype]=AreaType[sptype]+(pend-pbegin)*pile;
						foundTag=true;
					}
					else if(foundTag==true)
					{
						search_start=j-1;
						break;
					}
				}
			}
			for(int i=0;i<8;i++)
			{
				farea<<AreaType[i]<<"\t";
				flen<<LenType[i]<<"\t";
				fcnt<<CntType[i]<<"\t";
			}
			farea<<endl;
			flen<<endl;
			fcnt<<endl;
		}
	}
	else
		cout<<"Failed opening peakpos file..."<<endl;
	fin.close();
	farea.close();
	fcnt.close();

	return 0;
}
