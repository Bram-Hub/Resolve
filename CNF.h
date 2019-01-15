#ifndef CNF_H
#define CNF_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>
#include <list>

using std::string;
using std::cout;
using std::endl;
using std::find;
using std::vector;
using std::list;
using std::iterator;

bool debug=false;

bool parse(string& input,string& cons, int& place,int& erris);
string NNF(string input);
void Distribution(string& input);
void Distribution(string& input,vector<string>& clauses);
int findmainconnect(string input);

bool parse(string& input,string& cons, int& place,int& erris)
{
	erris=place=0;
	int iparc,iparo;
	iparc=iparo=0;

     //IPARO CONTAINS THE NUMBER OF OPENING PARENTHESIS THAT HAVE BEEN INPUT
     //IPARC CONTAINS THE NUMBER OF CLOSING PARENTHESIS THAT HAVE BEEN INPUT

     //CHANGES ALL TO UPPER CASE FOR PARSING
	  for(int i=0;i<input.size();i++)
	  {
		  if(isalpha(input[i]))
		  {
			  input[i]=char(toupper(input[i]));
			  if(find(cons.begin(),cons.end(),input[i])==cons.end())
			  {
				  cons.push_back(input[i]);
			  }
		  }
		  //increments () counters
		  else if(input[i] == '(')
		  {
			  iparo++;
		  }
		  else if(input[i] == ')')
		  {
			  iparc++;
		  }

		  //checks for too many closing parenthesis
		  if(iparo < iparc)
		  {
			  //cout<<"Too many closing parenthesis.\n";
			  place=i;
			  erris=1;
			  return false;
		  }

		  //checks to make sure a valid character follows a )
		  if(i > 0 && input[i-1] == ')' && (input[i] != ')' && input[i]!= '|'
		     && input[i] != '&' && input[i] != '>'))
		  {
			  //cout<<"Invalid character after closing parenthesis.\n";
			  place = i;
			  erris=2;
			  return false;
		  }

		  //checks to make sure a valid character follows a (
		  if(i > 0 && input[i-1] == '(' && (!isalpha(input[i]) && input[i] != '(' && input[i] != '~'))
		  {
			  //cout<<"Invalid character after opening parenthesis.\n";
			  place = i;
			  erris=3;
			  return false;
		  }

		  //checks to make sure a valid character follows a constant
		  if(i > 0 && isalpha(input[i-1]) && input[i] != ')' && input[i] != '|'
			  && input[i] != '&' && input[i] != '>')
		  {
			  //cout<<"Invalid character after constant.\n";
			  place=i;
			  erris=4;
			  return false;
		  }

		  //checks to make sure a constant of ( follows a negation
		  if(i>0 && input[i-1] == '~' && !(isalpha(input[i]) || input[i] == '(' || input[i] == '~'))
		  {
			  //cout<<"Invalid character after negation.\n";
			  place=i;
			  erris=5;
			  return false;
		  }

		  //checks to make sure a valid character follows an AND or and OR or an
		  //implication
		  if(i>0 && (input[i-1] == '&' || input[i-1] == '|' || input[i-1] == '>') && 
			  (!isalpha(input[i]) && input[i] != '(' && input[i] != '~'))
		  {
			  //cout<<"Invalid character after &, |, >.\n";
			  place=i;
			  erris=6;
			  return false;
		  }
	  }

	  //makes sure there is the correct amount of parenthesis
	  if(iparo != iparc)
	  {
		  //cout<<"Incorrect amount of parenthesis.\n";
		  place=-1;
		  erris=7;
		  return false;
	  }
	  return true;
}

string NNF(string input)
{
	while(input[0]=='~' && input[0]==input[1])
	{
		input=input.substr(2,input.size());
	}
	for(int i=1;i<input.size();i++)
	{
		if(input[i]=='~' && input[i]==input[i-1])
		{
			input=input.substr(0,i-1)+input.substr(i+1,input.size());
		}
	}
	bool parens=false;
	if(input[0]=='(' && input[input.size()-1]==')')
	{
		bool bad=false;
		int iparc=0,iparo=0;
		for(int i=1;i<input.size()-1;i++)
		{
			if(input[i]=='(')
			{
				iparo++;
			}
			else if(input[i]==')')
			{
				iparc++;
			}
			if(iparc > iparo)
			{
				bad=true;
				break;
			}
		}
		if(!bad)
		{
			input=input.substr(1,input.size()-2);
			parens=true;
		}
	}
	if(input.size()<=5)
	{
		if(input.size() <=3)
		{
			string ret;
			if(input[1]=='>')
			{
				ret='~';
				ret=ret+input[0]+'|'+input[2];
			}
			else ret=input;
			if(parens)
			{
				ret='(' + ret + ')';
			}
	        return ret;
		}
		//else it is something of the form ~p&~q ,etc.
		if(input[0]=='~')
		{
			if(input[2]=='&' || input[2] == '|')
			{
				if(parens)
				{
					return '(' + input + ')';
				}
				else
				{
                    return input;
				}
			}
			else if(input[2]=='>')
			{
				string ret;
				ret.clear();
				ret.push_back(input[1]);
				ret.push_back('|');
				if(input[3]=='~')
				{
					ret=ret+'~'+input[4];
				}
				else
				{
					ret=ret+input[3];
				}
				if(parens)
				{
					ret='('+ret+')';
				}
				return ret;
			}
		}
		else
		{
			if(input[1]=='&' || input[1] == '|')
			{
				if(parens)
				{
					input='('+input+')';
				}
				return input;
			}
			else if(input[1]=='>')
			{
				string ret;
				ret.clear();
				ret.push_back('~');
				ret.push_back(input[0]);
				ret.push_back('|');
				if(input[2]=='~')
				{
					ret.push_back('~');
					ret.push_back(input[3]);
				}
				else
				{
					ret.push_back(input[2]);
				}
				if(parens)
				{
					ret='('+ret+')';
				}
				return ret;
			}
		}
	}
	else if(input.size()==6)
	{
		if(input[0]=='~' && input[3]=='>')
		{
			input[0]='(';
			input[1]=input[2];
			input[2]='&';
			input[3]='~';
			if(parens)
			{
				input='('+input+')';
			}
			return input;
		}
	}
	int pos=findmainconnect(input);
	string ret;
	if(input[pos]=='&')
	{
		if(input[0]=='~' && input[1]=='(')
		{
			input[0]='(';
			input[1]='~';
			ret=NNF(input.substr(1,pos-1))+'|'+NNF('~'+input.substr(pos+1,input.size()-(pos+1)-1));
			parens=true;
		}
		else
		{
			ret=NNF(input.substr(0,pos)) + '&' + NNF(input.substr(pos+1,input.size()-pos-1));
			parens=true;
		}
	}
	else if(input[pos]=='|')
	{
		if(input[0]=='~' && input[1]=='(')
		{
			input[0]='(';
			input[1]='~';
			ret=NNF(input.substr(1,pos-1))+'&'+NNF('~'+input.substr(pos+1,input.size()-(pos+1)-1));
			parens=true;
		}
		else
		{
			ret=NNF(input.substr(0,pos)) + '|' + NNF(input.substr(pos+1,input.size()-pos-1));
			parens=true;
		}
	}
	else if(input[pos]=='>')
	{
		if(input[0]=='~' && input[1]=='(')
		{
			ret=NNF(input.substr(2,pos-2)) + '&' + NNF('~'+input.substr(pos+1,input.size()-(pos+1)-1));
			parens=true;
		}
		else
		{
			if(input[0]=='~')
			{
				ret=NNF(input.substr(1,pos-1))+'|'+NNF(input.substr(pos+1,(input.size()-pos-1)));
			}
			else ret=NNF('~'+input.substr(0,pos)) + '|' + NNF(input.substr(pos+1,(input.size()-pos-1)));
			parens=true;
		}
	}
	if(parens)
	{
		ret='('+ret+')';
	}
	return ret;
}

void Distribution(string & input,vector<string>& clauses)
{
	Distribution(input);
	list<string> myvect;
	clauses.clear();
	string temp1;
	string temp2;
	int pos;
	myvect.clear();
	///////////////////////////////////////////
	myvect.push_back(input);
	list<string>::iterator p=myvect.begin();
	int i=0;
	do{
		pos=findmainconnect(*p);
		if((*p).size()>2 && (*p)[pos]=='&')
		{
			temp1=p->substr(1,pos-1);
			temp2=p->substr(pos+1,p->size()-pos-2);
			p=myvect.erase(p);
			myvect.push_back(temp1);
			myvect.push_back(temp2);
			p=myvect.begin();
			while(p!=myvect.end() && 
				find((*p).begin(), (*p).end(), '&')==(*p).end())
			{
				p++;
			}
		}
		else p++;
	}while(p!=myvect.end());
	for(p=myvect.begin();p!=myvect.end();p++)
	{
		clauses.push_back(*p);
	}
}

void Distribution(string& input)
{
	bool parens=false;
	bool negs=false;
	if(input[0]=='~' && input[1]=='(')
	{
		negs=true;
		input=input.substr(1,input.size()-1);
	}
	if(input[0]=='(' && input[input.size()-1]==')')
	{
		parens=true;
		input=input.substr(1,input.size()-2);
	}
	int pos;
	string part1,part2;
	if(parens)
	{
		input='('+input+')';
		pos=findmainconnect(input);
		part1=input.substr(1,pos-1);
		part2=input.substr(pos+1,input.size()-pos-2);
	}
	else
	{
		pos=findmainconnect(input);
		part1=input.substr(0,pos);
		part2=input.substr(pos+1,input.size()-pos-2);
	}
	if(find(input.begin(),input.end(),'&')==input.end() || find(input.begin(),input.end(),'|')==input.end())
	{
		if(negs)
		{
			input='~'+input;
		}
		return;
	}
	if(input[pos]=='&')
	{
		if(negs)
		{
			input="(~"+part1+"|~"+part2+')';
			Distribution(input);//,clauses);
			return;
		}
		else
		{
			Distribution(part1);//,clauses);
			Distribution(part2);//,clauses);
		}
		return;
	}
	else
	{
		if(part1.size()<=2 && part2.size()<=2)
		{
			if(negs)
			{
				if(part1.size()==1)
				{
					part1='~'+part1;
				}
				else part1=part1[1];
				if(part2.size()==1)
				{
					part2='~'+part2;
				}
				else part2=part2[1];
				input=part1+'&'+part2;
				if(parens)
				{
					input='('+input+')';
				}
			}
			return;
		}
		else if(part1.size()<=2)//therefore should be a literal or its negation
		{
			if(negs)
			{
				if(part1.size()==1)
				{
					part1='~'+part1;
				}
				else part1=part1[1];
				Distribution('~'+part2);//,clauses);
				if(parens)
				{
					input='('+part1+'&'+part2+')';
				}
				else input=part1+'&'+part2;
				return;
			}
			else
			{
				Distribution(part2);//,clauses);
				input='('+part1+"|("+part2+"))";
				//Distribution(input,clauses);
				int p2pos=findmainconnect(part2);
				if(part2[p2pos]=='|')
				{
					return;
				}
				else
				{
					//not here
					input='('+part1+'|'+part2.substr(1,p2pos-1)+")&("+part1+'|'+part2.substr(p2pos+1,part2.size()-p2pos-2)+')';
					return;
				}
				return;
			}
		}
		else if(part2.size()<=2)//therefore should be a literal or its negation
		{
			if(negs)
			{
				if(part2.size()==1)
				{
					part2='~'+part2;
				}
				else part2=part2[1];
				Distribution('~'+part1);//,clauses);
				if(parens)
				{
					input='('+part1+'&'+part2+')';
				}
				else input=part1+'&'+part2;
				//not here either
				return;
			}
			else
			{

				Distribution(part1);//,clauses);
				input='('+part1+"|("+part2+"))";
				//Distribution(input,clauses);
				int p1pos=findmainconnect(part1);
				if(part1[p1pos]=='|')
				{
					return;
				}
				else
				{
					input='('+part1.substr(1,p1pos-1)+'|'+part2+")&("+part1.substr(p1pos+1,part1.size()-p1pos-2)+'|'+part2+')';
					return;
				}
				return;
			}
		}
		else
		{
			//if negs then it is ~p1 & ~p2
			if(negs)
			{
				Distribution('~'+part1);//,clauses);
				Distribution('~'+part2);//,clauses);
				input='('+part1+")&("+part2+')';
				return;
			}
			else
			{
				int p1pos=findmainconnect(part1);
				int p2pos=findmainconnect(part2);
				if(part1[p1pos]!=part2[p2pos])
				{
					if(part2[p2pos]==input[pos])
					{	
						string temp1='('+part1.substr(1,p1pos-1)+part2[p2pos]+part2+')';
						string temp2='('+part1.substr(p1pos+1,part1.size()-p1pos-2)+part2[p2pos]+part2+')';
						Distribution(temp1);//,clauses);
						Distribution(temp2);//,clauses);
						input='('+temp1+part1[p1pos]+temp2+')';
						return;
					}
					//if p1pos!=p2pos and p2pos!=input[pos] then p1pos must equal input[pos]
					else
					{
						string temp1='('+part2.substr(1,p2pos-1)+part1[p1pos]+part1+')';
						string temp2='('+part2.substr(p2pos+1,part2.size()-p2pos-2)+part1[p1pos]+part1+')';
						Distribution(temp1);//,clauses);
						Distribution(temp2);//,clauses);
						input='('+temp1+part2[p2pos]+temp2+')';
						return;
					}
				}
				else
				{
					string temp2='('+part1+input[pos]+part2.substr(1,p2pos-1)+')';
					string temp1='('+part1+input[pos]+part2.substr(p2pos+1,part2.size()-p2pos-2)+')';
					Distribution(temp1);//,clauses);
					Distribution(temp2);//,clauses);
					input='('+temp1+part1[p1pos]+temp2+')';
					return;
				}
			}
		}
	}
}

int findmainconnect(string input)
{
	string::iterator firstCP=find(input.begin(), input.end(), ')');
	if(firstCP==input.end())
	{
		int i=0;
		while(i<input.size())
		{
			if(input[i]=='|' || input[i]=='&' || input[i]=='>')
			{
				return i;
			}
			i++;
		}
	}
	else
	{
		int pos=-1;
		int myabs=50;
		int mypos=-1;
		for(int i=0;i<input.size();i++)
		{
			if(input[i]=='&' || input[i]=='|' || input[i]=='>')
			{
				int part1,part2;
				part1=part2=0;
				for(int j=0;j<i;j++)
				{
					if(input[j]=='(')
					{
						part1++;
					}
					else if(input[j]==')')
					{
						part1--;
					}
				}
				for(int k=i+1;k<input.size();k++)
				{
					if(input[k]=='(')
					{
						part2++;
					}
					else if(input[k]==')')
					{
						part2--;
					}
				}
				if(part1==0 && part2==0)
				{
					return i;
				}
				else if(part1==1 && part2==-1)
				{
					pos=i;
				}
				else if(abs(part1+part2) < myabs)
				{
					myabs=abs(part1+part2);
					mypos=i;
				}
			}
		}
		if(pos!=-1)
		{
			return pos;
		}
		else 
		{
			return mypos;
		}
	}
	//is of length 1 or 2 therefore it must be at position 0
	return 0;
}

#endif