#include<iostream>
#include<fstream>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <ctime>


using namespace std;

int compare(int,int);
class gate
{
	public:
		int vertexID;
		int delay;
		int arrival;
		int require;
		int slack;
		int longest;
		string type;  // in,out or mid
		vector<gate*> eat_gate;
		char o_eat[10];		
};




int main(int argc, char* argv[])
{
	clock_t time_init, time_end;
    time_init = clock();
    
	map<string,gate*> map_gate;
	vector<gate*> v_gate;
	vector<gate*> out_gate;
	vector<string> out_eat;
	vector<gate*> output;
	gate* temp_gate;
	
	
	string temp;
	char* part;
	char id[10] , eat_id[10] , line[50];
	char trash[10];
	char look;
	int i ,t , k , m , c;
	int count , temp_req , max_delay;
	count = 0;
	int in_num=0 , out_num=0;
	int edge=0;
	int test=0;
	
	ifstream infile;
	infile.open(argv[1]);
		
	if(!infile)
	{
		cout << "no input" << endl;
		system("pause");	
	}
	
	infile >> temp >> temp;
	infile >> ws;

////////////    read input   //////////////	
	while(1)
	{	
		infile.getline(trash,10,'(');
		infile.getline(id,10,')');
				
		map_gate[id]= new gate;
		map_gate[id]->vertexID = count;
		count = count + 1;
		in_num = in_num +1;	
		map_gate[id]->type = "in";
		map_gate[id]->delay = 0;
		map_gate[id]->require = 0;
		v_gate.push_back(map_gate[id]);
//cout << id << " " << map_gate[id]->vertexID <<" "<< map_gate[id]->type << endl;
		infile >> ws;
		look=infile.peek();
		if(look!='I')
		{
			break;
		}
	}
cout << "in sucess" <<endl;



////////////    read output   //////////////
	while(1)
	{	
		infile.getline(trash,10,'(');
		infile.getline(eat_id,10,')');		

		temp_gate = new gate;		
		temp_gate->vertexID = count; 
		count = count +1;
		out_num = out_num +1;
		temp_gate->type = "out";
		temp_gate->delay = 0;
		temp_gate->require = 0;
//cout << eat_id;
		strcpy(temp_gate->o_eat,eat_id);
//system("pause");
		v_gate.push_back(temp_gate);
		output.push_back(temp_gate);
		edge = edge+1;
//cout << eat_id <<" "<<v_gate[count-1]->vertexID <<" "<< v_gate[count-1]->type << endl;
		infile >> ws;
		look=infile.peek();
		if(look!='O')
		{
			break;
		}
	}
cout << "out sucess" <<endl;




////////////    read mid & find delay   //////////////
	infile.getline(id,10,' ');
	while(!infile.eof())
	{
//cout << "last :" << id << endl;
//		infile >> id;
		map_gate[id]= new gate;
		map_gate[id]->vertexID = count;
		count = count + 1;	
		map_gate[id]->type = "mid";
		map_gate[id]->delay = 0;
		map_gate[id]->require = 0;
		v_gate.push_back(map_gate[id]);
//cout << id << endl;


		infile.getline(trash,10,'(');
//system("pause");
		infile.getline(line,sizeof(line));
//system("pause");
//cout << line <<endl;
//cout << infile.gcount()<< endl;
		c=0;
		for(m=0;m<strlen(line);m++)
		{
			if(line[m]==',')
			{c=c+1;}
		}
		
		part=strtok (line , ", )");
	
		for(m=0;m<c+1;m++)
		{
//cout <<part<<endl;
//system("pause");
////cout << "0" <<endl;
			map_gate[id]->eat_gate.push_back(map_gate[part]);
//cout << "part: " <<part <<endl;
			map_gate[part]->delay = map_gate[part]->delay +1;
//cout << "success" <<endl;			
			edge = edge+1;
//cout << "left: " << line <<endl;		
			part = strtok(NULL , ", )");
//cout << "4" <<endl;									
		}

		infile.getline(id,10,' ');
//cout << "??" <<endl;					
	}
cout << "End of File !!!!" << endl;

	i=0;
	while(i < output.size())// find delay which cause by output
	{
		map_gate[output[i]->o_eat]->delay = map_gate[output[i]->o_eat]->delay +1;
		i = i+1;
	}

/*
i=0;
while(i<count)
{
	cout << v_gate[i]->delay << endl;
	i=i+1;
}
*/
cout << "delay end !!!" << endl;






////////////   arrival time    //////////////
	i=0;
	while(i < v_gate.size())
	{
		if(v_gate[i]->type=="in")
		{
			v_gate[i]->arrival = v_gate[i]->delay;
			v_gate[i]->longest = v_gate[i]->arrival;
		}
		else if(v_gate[i]->type=="mid")
		{
			if(v_gate[i]->eat_gate.size()==0)
			{
				v_gate[i]->arrival = 0;
				v_gate[i]->longest = -1;
			}
			else
			{
				t = v_gate[i]->eat_gate[0]->arrival;
				for(int j=1 ; j <v_gate[i]->eat_gate.size() ; j++)
				{
					t = compare ( t , v_gate[i]->eat_gate[j]->arrival );
				}
				if(t == 0)
				{
					v_gate[i]->arrival = 0;
					v_gate[i]->longest = -1;
				}
				else 
				{
					v_gate[i]->arrival = v_gate[i]->delay + t;
					v_gate[i]->longest = v_gate[i]->arrival;
				}
			}
		}
		else if(v_gate[i]->type=="out")
		{
			out_gate.push_back(v_gate[i]);
		}
		
		i = i+1;
	}
	i = 0;
	while(i < out_gate.size())
	{
		if(map_gate[out_gate[i]->o_eat]->arrival==0)
		{
			out_gate[i]->arrival = 0;
			out_gate[i]->longest = -1;
		}
		else
		{
			out_gate[i]->arrival = map_gate[out_gate[i]->o_eat]->arrival;
			out_gate[i]->longest = map_gate[out_gate[i]->o_eat]->arrival;
		}
				
		i = i+1;
	}
/*
i=0;
while(i < v_gate.size())
{
cout << v_gate[i]->arrival << endl;	
i=i+1;
}
*/
cout << "arrival time finish !!!" <<endl;	

////////////   require time    //////////////
	int tr;
	
	for(i=0;i<output.size()-1;i++)
	{
		tr = compare(output[i]-> arrival,output[i+1]-> arrival);
	}
	
	
	for(i=0;i<output.size();i++)
	{
		output[i]->require = tr;
		map_gate[output[i]->o_eat]->require = output[i]-> require;	
	}
	
	cout << "require: " << tr << endl;
	
	for(i=v_gate.size()-1;i>=0;i--)
	{
		if(v_gate[i]->type=="mid")
		{
			temp_req = v_gate[i]->require - v_gate[i]->delay;			
			for(k=0;k < v_gate[i]->eat_gate.size();k++)
			{
				if(v_gate[i]-> eat_gate[k]-> require==0)
				{
					v_gate[i]-> eat_gate[k]-> require = temp_req;
				}
				else if(v_gate[i]-> eat_gate[k]-> require!=0&&v_gate[i]-> eat_gate[k]-> require >= temp_req)
				{
					v_gate[i]-> eat_gate[k]-> require = temp_req;
				}
			}			
		}
	}
/*
i=0;
while(i < v_gate.size())
{
cout <<v_gate[i]->vertexID <<" "<<v_gate[i]->delay<<" "<<v_gate[i]->arrival<<" "<< v_gate[i]->require << endl;	
i=i+1;
}
*/
cout << "require finish !!!" << endl;

////////////   slack time    //////////////

	for(i=0;i<v_gate.size();i++)
	{
		v_gate[i]->slack = v_gate[i]->require - v_gate[i]->arrival;
	}
	for(i=0;i<v_gate.size()-1;i++)
	{
		max_delay = compare(v_gate[i]->arrival,v_gate[i+1]->arrival);
	}

	cout << "slack done!!" << endl;
	
	

/////////////    output     ////////////////////
	ofstream outfile;
	outfile.open(argv[2]);
	
	outfile << max_delay <<endl;
	i=0;
	outfile << in_num << " ";
	while(v_gate[i]->type=="in")
	{
		outfile << i << " ";
		i=i+1;
	}
	outfile << endl << out_num << " ";
	while(v_gate[i]->type=="out")
	{
		outfile << i << " ";
		i=i+1;
	}
	outfile << endl;
	i=0;
	while(i < v_gate.size())
	{
		outfile <<v_gate[i]->vertexID <<" "<<v_gate[i]->longest<<" "<< v_gate[i]->slack <<endl;	
		i=i+1;
	}
	cout << edge;
	
	time_end = clock();
    cout << "total CPU time = " << double(time_end - time_init)/CLOCKS_PER_SEC << endl;
	
////////////////////////////////////////////	
//	system("pause");
	return 0;
}

int compare(int a,int b)
{
	if(a>=b)
	{return a;}
	else if(a<b)
	{return b;}	
}



