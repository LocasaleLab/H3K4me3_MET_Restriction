#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>


//Calculate peak height, area, width, entropy, skewness, kurtosis and 5th moment


using namespace std;

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

vector<double> getCurve(string line)
{
	vector<double> values;
	values.clear();
	vector<string> words=splitString(line,'\t');
	for(int i=0;i<words.size();i++)
	{
		double v=str2double(words[i]);
		values.push_back(v);
	}
	return values;
}

double calArea(vector<double> vec)
{
	double a=0;
	for(int i=0;i<vec.size();i++)
		a+=vec[i];
	return a;
}

double calCenter(vector<double> vec,double area)
{
	double a=0;
	for(int i=0;i<vec.size();i++)
		a+=i*vec[i]/area;
	return a;
}

double calStd(vector<double> vec,double area,double center)
{
	double s=0;
	for(int i=0;i<vec.size();i++)
		s+=(vec[i]/area)*(i-center)*(i-center);
	s=sqrt(s);
	return s;
}

double calMoment(vector<double> v,double area,double center,double std,int n)
{
	double m=0;
	for(int i=0;i<v.size();i++)
		m+=pow((i-center)/std,n)*v[i]/area;
	return m;
}

double calEntropy(vector<double> v,double area,double std)
{
	double e=0;
	for(int i=0;i<v.size();i++)
	{
		double p=std*v[i]/area;
		if(p!=0)
			e-=p*log(p)/std;
	}
	return e;
}

double calHeight(vector<double> v)
{
	double hmax=-1;
	for(int i=0;i<v.size();i++)
	{
		if(v[i]>hmax)
			hmax=v[i];
	}
	return hmax;
}

vector<double> peakFeatures(vector<double> v)
{
	vector<double> pf;
	pf.clear();
	double area=calArea(v);
	double h=calHeight(v);
	double center=calCenter(v,area);
	double std=calStd(v,area,center);
	double entropy=calEntropy(v,area,std);
	pf.push_back(h);
	pf.push_back(area);
	pf.push_back(std);
	pf.push_back(entropy);
	for(int i=3;i<6;i++)
	{
		double moment=calMoment(v,area,center,std,i);
		pf.push_back(moment);
	}
	return pf;
}

int main(int argc,char *argv[])
{
	string RunID(argv[1]);
	string curve("_PeakCurves.txt");
	string feature("_PeakFeatures.txt");
	string curveFile=RunID+curve;
	string outFile=RunID+feature;
	ifstream fin(curveFile.c_str());
	ofstream fout(outFile.c_str());
	if(fin.is_open())
	{
		string line;
		while(getline(fin,line))
		{
			vector<double> v=getCurve(line);
			vector<double> pf=peakFeatures(v);
			for(int i=0;i<pf.size();i++)
				fout<<pf[i]<<"\t";
			fout<<endl;
			v.clear();
			pf.clear();
		}
		fout.close();
	}
	else
		cout<<"Error in opening file!"<<endl;
	fin.close();
	fout.close();

	return 0;
}

