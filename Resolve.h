#ifndef RESOLVE_H
#define RESOLVE_H

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cctype>

#include <fstream>
using std::ofstream;

using std::string;
using std::sort;
using std::vector;
using std::cout;
using std::endl;

class clauses
{
public:
	clauses(){already=false;}
	clauses(string& instr);
	clauses(vector<string>& invect);

	bool taut(const string& instr);
	void clear();
	void pop_back();
	void push_back(string& instr);
	void resolve();
	string print();
	string print(const int& index);
	bool noclauses(){return literals.empty();}
	bool UserResClau(int pos1, int pos2, int& err);
	int numclau()const;
	bool notaclause(const string& instr);
	bool notaclause(const int& pos1, const int& pos2);
	bool ContradReached();

private:
	vector<string> literals;
	vector<string> negations;
	vector<int> Cinput1;
	vector<int> Cinput2;
	bool already;

	bool ResClau(int pos1, int pos2, string& litstr, string& negstr, int& err);
};

clauses::clauses(string& instr)
{
	already=false;
	push_back(instr);
}

clauses::clauses(vector<string>& invect)
{
	already=false;
	for(int i=0;i<invect.size();i++)
	{
		push_back(invect[i]);
	}
}

bool clauses::notaclause(const int& pos1, const int& pos2)
{
	string tempL,tempN;
	tempL.clear();
	tempN.clear();
	tempL=literals[pos1]+literals[pos2];
	tempN=negations[pos1]+negations[pos2];
	sort(tempL.begin(),tempL.end());
	sort(tempN.begin(),tempN.end());
	//remove repeat letters
	for(int i=0;i+1<tempL.size();i++)
	{
		if(tempL[i]==tempL[i+1])
		{
			tempL=tempL.substr(0,i+1)+tempL.substr(i+2,tempL.size());
		}
	}
	for(int i=0;i+1<tempN.size();i++)
	{
		if(tempN[i]==tempN[i+1])
		{
			tempN=tempN.substr(0,i+1)+tempN.substr(i+2,tempN.size());
		}
	}
	for(int i=0;i<tempL.size();i++)
	{
		if(find(tempN.begin(),tempN.end(),tempL[i])!=tempN.end())
		{
			string temp;
			temp.clear();
			for(string::iterator it=tempN.begin();it!=tempN.end();it++)
			{
				if(it!=find(tempN.begin(),tempN.end(),tempL[i]))
				{
					temp.push_back(*it);
				}
			}
			tempN=temp;
			tempL=tempL.substr(0,i)+tempL.substr(i+1,tempL.size());
			break;
		}
	}
	for(int i=0;i<literals.size();i++)
	{
		if(literals[i]==tempL && negations[i]==tempN)
		{
			return false;
		}
	}
	return true;

}

bool clauses::notaclause(const string& instr)
{
	string tempL;
	string tempN;
	tempL.clear();
	tempN.clear();
	int i=0;
	while(i<instr.size())
	{
		if(instr[i]=='~')
		{
			tempN.push_back(instr[i+1]);
			i++;
			//i now = literal
			i++;
			//i now = |
			i++;
			//i now = next literal or > size
		}
		else if(isalpha(instr[i]))
		{
			tempL.push_back(instr[i]);
			i++;
			//i now = |
			i++;
			//i now = next literal or > size
		}
		else i++;
	}
	sort(tempL.begin(),tempL.end());
	sort(tempN.begin(),tempN.end());
	//remove repeat letters
	for(int i=0;i+1<tempL.size();i++)
	{
		if(tempL[i]==tempL[i+1])
		{
			tempL=tempL.substr(0,i+1)+tempL.substr(i+2,tempL.size());
		}
	}
	for(int i=0;i+1<tempN.size();i++)
	{
		if(tempN[i]==tempN[i+1])
		{
			tempN=tempN.substr(0,i+1)+tempN.substr(i+2,tempN.size());
		}
	}
	for(int i=0;i<literals.size();i++)
	{
		if(literals[i]==tempL && negations[i]==tempN)
		{
			return false;
		}
	}
	return true;
}
void clauses::push_back(string& instr)
{
	string tempL;
	string tempN;
	tempL.clear();
	tempN.clear();
	int i=0;
	while(i<instr.size())
	{
		if(instr[i]=='~')
		{
			tempN.push_back(instr[i+1]);
			i++;
			//i now = literal
			i++;
			//i now = |
			i++;
			//i now = next literal or > size
		}
		else if(isalpha(instr[i]))
		{
			tempL.push_back(instr[i]);
			i++;
			//i now = |
			i++;
			//i now = next literal or > size
		}
		else i++;
	}
	sort(tempL.begin(),tempL.end());
	sort(tempN.begin(),tempN.end());
	//remove repeat letters
	for(int i=0;i+1<tempL.size();i++)
	{
		if(tempL[i]==tempL[i+1])
		{
			tempL=tempL.substr(0,i+1)+tempL.substr(i+2,tempL.size());
		}
	}
	for(int i=0;i+1<tempN.size();i++)
	{
		if(tempN[i]==tempN[i+1])
		{
			tempN=tempN.substr(0,i+1)+tempN.substr(i+2,tempN.size());
		}
	}
	bool taut=false;
	for(int i=0;i<tempL.size();i++)
	{
		if(find(tempN.begin(),tempN.end(),tempL[i])!=tempN.end())
		{
			taut=true;
		}
	}
	if(!taut)
	{
		Cinput1.push_back(-1);
		Cinput2.push_back(-1);
		literals.push_back(tempL);
		negations.push_back(tempN);
	}
}

void clauses::resolve()
{
	if(already)
	{
		cout<<"Already resolved!"<<endl;
		if(literals[literals.size()-1].empty() && negations[negations.size()-1].empty())
		{
			cout<<"A contradiction was reached."<<endl;
		}
		else cout<<"A contradiction was not reached."<<endl;
		return;
	}
	else
	{
		already=true;
		int outer,inner;
		outer=0;
		inner=1;
		string newlit,newneg;
		newlit.clear();
		newneg.clear();
		bool startover=false;
		int error=0;
		while(outer<literals.size()-1)
		{
			startover=false;
			inner=outer+1;
			while(inner<literals.size())
			{
				newlit.clear();
				newneg.clear();
				if(ResClau(outer,inner,newlit,newneg,error))
				{
					startover=true;
					bool push=true;
					//checks to see if it has already been added
					for(int i=0;i<literals.size();i++)
					{
						if(literals[i]==newlit && negations[i]==newneg)
						{
							startover=false;
							push=false;
							break;
						}
					}
					if(push)
					{
						literals.push_back(newlit);
						negations.push_back(newneg);
						Cinput1.push_back(outer);
						Cinput2.push_back(inner);
						inner=literals.size();
					}
				}
				inner++;
			}
			if(startover)
			{
				outer=0;
			}
			else
			{
				outer++;
			}
			if(literals[literals.size()-1].empty() && negations[negations.size()-1].empty())
			{
				cout<<"Contradiction!"<<endl;
				return;
			}
		}
		cout<<"Unable to reach contradiction."<<endl;
	}
}

bool clauses::ResClau(int pos1, int pos2, string& litstr, string& negstr, int& err)
{
	string Lit1,Lit2,Neg1,Neg2;
	Neg1.clear();
	Neg2.clear();
	Lit1.clear();
	Lit2.clear();
	Lit1=literals[pos1];
	Lit2=literals[pos2];
	Neg1=negations[pos1];
	Neg2=negations[pos2];
	bool able=false;
	for(int i=0;i<Lit1.size();i++)
	{
		if(find(Neg2.begin(),Neg2.end(),Lit1[i])!=Neg2.end())
		{
			able=true;
			break;
		}
	}
	for(int i=0;i<Lit2.size();i++)
	{
		if(find(Neg1.begin(),Neg1.end(),Lit2[i])!=Neg1.end())
		{
			able=true;
			break;
		}
	}
	if(!able)
	{
		err=1;
		return false;
	}
	else
	{
		bool resolved=false;
		for(int i=0;i<Lit2.size();i++)
		{
			if(find(Neg1.begin(),Neg1.end(),Lit2[i])!=Neg1.end())
			{
				string temp;
				temp.clear();
				for(string::iterator it=Neg1.begin();it!=Neg1.end();it++)
				{
					if(it!=find(Neg1.begin(),Neg1.end(),Lit2[i]))
					{
						temp.push_back(*it);
					}
				}
				Neg1=temp;
				Lit2=Lit2.substr(0,i)+Lit2.substr(i+1,Lit2.size());
				resolved=true;
				break;
			}
		}
		if(!resolved)
		{
			for(int i=0;i<Lit1.size();i++)
			{
				if(find(Neg2.begin(),Neg2.end(),Lit1[i])!=Neg2.end())
				{
					string temp;
					temp.clear();
					for(string::iterator it=Neg2.begin();it!=Neg2.end();it++)
					{
						if(it!=find(Neg2.begin(),Neg2.end(),Lit1[i]))
						{
							temp.push_back(*it);
						}
					}
					Neg2=temp;
					Lit1=Lit1.substr(0,i)+Lit1.substr(i+1,Lit1.size());
					break;
				}
			}
		}
		litstr=Lit1+Lit2;
		negstr=Neg1+Neg2;
		sort(litstr.begin(),litstr.end());
		sort(negstr.begin(),negstr.end());
		//remove repeat letters
		for(int i=0;i+1<litstr.size();i++)
		{
			if(litstr[i]==litstr[i+1])
			{
				litstr=litstr.substr(0,i+1)+litstr.substr(i+2,litstr.size());
			}
		}
		for(int i=0;i+1<negstr.size();i++)
		{
			if(negstr[i]==negstr[i+1])
			{
				negstr=negstr.substr(0,i+1)+negstr.substr(i+2,negstr.size());
			}
		}
		for(int i=0;i<litstr.size();i++)
		{
			if(find(negstr.begin(),negstr.end(),litstr[i])!=negstr.end())
			{
				negstr.clear();
				litstr.clear();
				err=2;
				return false;
			}
		}
		return true;
	}
}


bool clauses::UserResClau(int pos1, int pos2, int & err)
{
	string Lit1,Lit2,Neg1,Neg2,litstr,negstr;
	litstr.clear();
	negstr.clear();
	Neg1.clear();
	Neg2.clear();
	Lit1.clear();
	Lit2.clear();
	Lit1=literals[pos1];
	Lit2=literals[pos2];
	Neg1=negations[pos1];
	Neg2=negations[pos2];
	bool able=false;
	for(int i=0;i<Lit1.size();i++)
	{
		if(find(Neg2.begin(),Neg2.end(),Lit1[i])!=Neg2.end())
		{
			able=true;
			break;
		}
	}
	for(int i=0;i<Lit2.size();i++)
	{
		if(find(Neg1.begin(),Neg1.end(),Lit2[i])!=Neg1.end())
		{
			able=true;
			break;
		}
	}
	if(!able)
	{
		err=1;
		return false;
	}
	else
	{
		bool resolved=false;
		for(int i=0;i<Lit2.size();i++)
		{
			if(find(Neg1.begin(),Neg1.end(),Lit2[i])!=Neg1.end())
			{
				string temp;
				temp.clear();
				for(string::iterator it=Neg1.begin();it!=Neg1.end();it++)
				{
					if(it!=find(Neg1.begin(),Neg1.end(),Lit2[i]))
					{
						temp.push_back(*it);
					}
				}
				Neg1=temp;
				Lit2=Lit2.substr(0,i)+Lit2.substr(i+1,Lit2.size());
				resolved=true;
				break;
			}
		}
		if(!resolved)
		{
			for(int i=0;i<Lit1.size();i++)
			{
				if(find(Neg2.begin(),Neg2.end(),Lit1[i])!=Neg2.end())
				{
					string temp;
					temp.clear();
					for(string::iterator it=Neg2.begin();it!=Neg2.end();it++)
					{
						if(it!=find(Neg2.begin(),Neg2.end(),Lit1[i]))
						{
							temp.push_back(*it);
						}
					}
					Neg2=temp;
					Lit1=Lit1.substr(0,i)+Lit1.substr(i+1,Lit1.size());
					break;
				}
			}
		}
		litstr=Lit1+Lit2;
		negstr=Neg1+Neg2;
		sort(litstr.begin(),litstr.end());
		sort(negstr.begin(),negstr.end());
		//remove repeat letters
		for(int i=0;i+1<litstr.size();i++)
		{
			if(litstr[i]==litstr[i+1])
			{
				litstr=litstr.substr(0,i+1)+litstr.substr(i+2,litstr.size());
			}
		}
		for(int i=0;i+1<negstr.size();i++)
		{
			if(negstr[i]==negstr[i+1])
			{
				negstr=negstr.substr(0,i+1)+negstr.substr(i+2,negstr.size());
			}
		}
		for(int i=0;i<litstr.size();i++)
		{
			if(find(negstr.begin(),negstr.end(),litstr[i])!=negstr.end())
			{
				//leads to tautologous clause
				err=2;
				negstr.clear();
				litstr.clear();
				return false;
			}
		}
		for(int i=0;i<literals.size();i++)
		{
			if(literals[i]==litstr && negations[i]==negstr)
			{
				//already resolved
				err=3;
				return false;
			}
		}
		literals.push_back(litstr);
		negations.push_back(negstr);
		Cinput1.push_back(pos1);
		Cinput2.push_back(pos2);
		return true;
	}
}

string clauses::print()
{
	string retstr;
	retstr.clear();
	for(int i=0;i<literals.size();i++)
	{
		retstr=retstr+print(i)+"\n";
		if(literals[i].empty() && negations[i].empty())
		{
			break;
		}
	}
	return retstr;
}

string clauses::print(const int& index)
{
	string retstr;
	char buffer[33];
	retstr=retstr+itoa(index,buffer,10)+string(": {");
	int litisize=literals[index].size();
	int negisize=negations[index].size();
	ofstream out("debug2.txt");
	out<<retstr<<endl;
	for(int l=0;l<litisize-1;l++)
	{
		retstr=retstr+literals[index][l]+",";
	}
	out<<retstr<<endl;
	if(litisize > 0)
	{
		if(negisize > 0)
		{
			retstr=retstr+literals[index][litisize-1]+",";
		}
		else retstr=retstr+literals[index][litisize-1]+"} ";
	}
	out<<retstr<<endl;
	for(int n=0;n<negisize-1;n++)
	{
		retstr=retstr+"~"+negations[index][n]+",";
	}
	if(negisize>0)
	{
		retstr=retstr+"~"+negations[index][negisize-1]+"} ";
	}
	else if(litisize==0)
	{
		retstr=retstr+"} ";
	}
	if(Cinput1[index]==-1)
	{
		retstr=retstr+"Given\n";
	}
	else
	{
		retstr=retstr+"From: "+itoa(Cinput1[index],buffer,10)+","+itoa(Cinput2[index],buffer,10);
	}
	return retstr;
}

int clauses::numclau() const
{
	return literals.size();
}

bool clauses::ContradReached()
{
	if(literals.size()==0)
	{
		return false;
	}
	else if(literals[literals.size()-1].empty() && 
		negations[negations.size()-1].empty())
	{
		return true;
	}
	else return false;
}

void clauses::pop_back()
{
	literals.pop_back();
	negations.pop_back();
	Cinput1.pop_back();
	Cinput2.pop_back();
}

void clauses::clear()
{
	literals.clear();
	negations.clear();
	Cinput1.clear();
	Cinput2.clear();
	already=false;
}

bool clauses::taut(const string& instr)
{
	string tempL;
	string tempN;
	tempL.clear();
	tempN.clear();
	int i=0;
	while(i<instr.size())
	{
		if(instr[i]=='~')
		{
			tempN.push_back(instr[i+1]);
			i++;
			//i now = literal
			i++;
			//i now = |
			i++;
			//i now = next literal or > size
		}
		else if(isalpha(instr[i]))
		{
			tempL.push_back(instr[i]);
			i++;
			//i now = |
			i++;
			//i now = next literal or > size
		}
		else i++;
	}
	sort(tempL.begin(),tempL.end());
	sort(tempN.begin(),tempN.end());
	//remove repeat letters
	for(int i=0;i+1<tempL.size();i++)
	{
		if(tempL[i]==tempL[i+1])
		{
			tempL=tempL.substr(0,i+1)+tempL.substr(i+2,tempL.size());
		}
	}
	for(int i=0;i+1<tempN.size();i++)
	{
		if(tempN[i]==tempN[i+1])
		{
			tempN=tempN.substr(0,i+1)+tempN.substr(i+2,tempN.size());
		}
	}
	for(int i=0;i<tempL.size();i++)
	{
		if(find(tempN.begin(),tempN.end(),tempL[i])!=tempN.end())
		{
			return true;
		}
	}
	return false;
}
#endif