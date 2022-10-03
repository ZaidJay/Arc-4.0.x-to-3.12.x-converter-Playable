#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<string>
#include<string.h>
using namespace std;

char tmp[1000];
char SceneType[1000];
int On6k1=0,Off6k1=480000,On6k2=0,Off6k2=480000;

double intnumber(double x)
{
	if(int(x*1000)%10>=5) return (x+0.01);
	else return x;
}

void fun(const string &str)
{
	for(int i=0;i<str.length();i++)
	{
		if(str[i]==',') tmp[i]=' ';
		else tmp[i]=str[i];
	}
	tmp[str.length()]='\0';
}

int arctap(string str)
{
	int i;
	for(i=0;i<str.length();i++)
	{
		if(str[i]=='[') return i;
	}
	return -1;
}

void check6k(string name)
{
	string in="input\\"+name+".aff";
	freopen(in.c_str(),"r",stdin);
	
	string str;
	while(cin>>str)
	{
		string object,code;
		int len;
		for(len=0;len<str.length();len++)
		{
			if(str[len]=='(') break;
		}
		
		object.assign(str,0,len);
		if(object=="scenecontrol")
		{
			code.assign(str,len+1,str.length()-len-3);
			int Time,state;
			double Duration;
			
			fun(code);
			sscanf(tmp,"%d %s %lf %d",&Time,SceneType,&Duration,&state);
			if(strcmp(SceneType,"enwidencamera")==0) 
			{

				if(state==1)
				{
					if(On6k1==0) On6k1=Time;
					else On6k2=Time;
				}
				else if(state==0)
				{
					if(On6k2==0) Off6k1=Time+int(Duration);
					else Off6k2=Time+int(Duration);
				}
				continue;
			}
		}
	}
	fclose(stdin);
	cin.clear(); 
}

void AFF_Read(string name)
{
	string input="input\\"+name+".aff";
	string output="output\\"+name+".aff";
	freopen(input.c_str(),"r",stdin);
	freopen(output.c_str(),"w",stdout);
	
	string str;
	while(cin>>str)
	{
		string object,code;
		int len;
		for(len=0;len<str.length();len++)
		{
			if(str[len]=='(') break;
		}
		
		object.assign(str,0,len);
		if(object=="scenecontrol")
		{
			code.assign(str,13,str.length()-15);
			int Time,state;
			double Duration;
			
			fun(code);
			sscanf(tmp,"%d %s %lf %d",&Time,SceneType,&Duration,&state);
			if(strcmp(SceneType,"enwidencamera")==0||strcmp(SceneType,"enwidenlanes")==0) continue;
		}
		else if(object.length()==0) 
		{ 
			code.assign(str,1,str.length()-3);
			int Time,Lane;
			
			fun(code);
			sscanf(tmp,"%d %d",&Time,&Lane);
			if(Lane==0||Lane==5) continue;	
		}
		else if(object=="hold")
		{
			code.assign(str,5,str.length()-7);
			int StartTime,EndTime,Lane;
			
			fun(code);
			sscanf(tmp,"%d %d %d",&StartTime,&EndTime,&Lane);
			if(Lane==0||Lane==5) continue;	
		}
		else if(object=="arc")
		{
			int tap=arctap(str);
			string arctaps;
			if(tap==-1)
			{
				code.assign(str,4,str.length()-6);
			}
			else
			{
				arctaps.assign(str,tap,str.length()-tap);
				code.assign(str,4,tap-5);
			}
			
			int StartTime,EndTime,Color;
			double StartX,EndX,StartY,EndY;
			char Type[10],Wav[100],State[10];
			
			fun(code);
			sscanf(tmp,"%d %d %lf %lf %s %lf %lf %d %s %s",&StartTime,&EndTime,&StartX,&EndX,Type,&StartY,&EndY,&Color,Wav,State);
			if((StartTime>=On6k1 && EndTime<=Off6k1 && On6k1!=0)||(StartTime>=On6k2 && EndTime<=Off6k2 && On6k2!=0))
			{
				StartX=0.5-(1-2*StartX)*1.0/3;
				EndX=0.5-(1-2*EndX)*1.0/3;
				StartY/=1.61;
				EndY/=1.61;
				cout<<"arc("<<StartTime<<","<<EndTime;
				cout<<","<<fixed<<setprecision(2)<<StartX;
				cout<<","<<fixed<<setprecision(2)<<EndX;
				cout<<","<<Type;
				cout<<","<<fixed<<setprecision(2)<<StartY;
				cout<<","<<fixed<<setprecision(2)<<EndY;
				cout<<","<<Color<<","<<Wav<<","<<State<<")";
				if(tap==-1)
				{
					cout<<";"<<endl;
					continue;
				}
				else
				{
					cout<<arctaps<<endl;
					continue;
				}
			}
		}
		cout<<str<<endl;
	}
	fclose(stdin);
	fclose(stdout);
	cin.clear();
}

int main()
{	check6k("3");
	AFF_Read("0");
	AFF_Read("1");
	AFF_Read("2");
	AFF_Read("3");
	return 0;
}
