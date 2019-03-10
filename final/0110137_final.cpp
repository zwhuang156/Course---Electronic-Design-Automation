#include<iostream>
#include<fstream>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

class shape
{
	public:
		shape();
		int lx,by,rx,ty;
		int tree_done , color , tree_id;
		vector<shape*> connect;	
};
shape::shape(){
	tree_done = 0;
	color = 0;
}

class window
{
	public:
		window();
		vector<shape*> block;
		int color1,color2;
		map<int,int> diff;//store graph[i]'s diff in this window
		map<int,int> colorA;
		map<int,int> colorB;
		double final_colorA , final_colorB;
};
window::window(){
	color1 = 0;
	color2 = 0;
}

class tree
{
	public:
		tree();
		int conflict;
		vector<shape*> block;
		vector<window*>	tree_win;
		int inverse;
};
tree::tree(){
	conflict = 0;
	inverse = 0;
}

class result
{
	public:
		map<int,int> inv;
		double score;
};

bool compare_x(shape*,shape*);
bool compare_y(shape*,shape*);


int main(int argc, char* argv[])
{
	char trash[10];
	char garbage;
	int a,b,w;
	int r_bound,l_bound,b_bound,t_bound;//window bound
	int d , count_tree=0;
	int i=0,j,k,l,p;
	int m,n;//double vector
	
	shape* temp;
	tree* temp_tree;
	
	vector<shape*> block; 
	vector<tree*> graph;
	vector<shape*> temp_connect;
	vector<vector<window*> > win;
	vector<result*> ans;
	
	ifstream infile;
	infile.open(argv[1]);

////////////      read input      /////////////	
	if(!infile)
	{
		cout << "no input" << endl;
		system("pause");	
	}
	
	infile.getline(trash,10,'=');
	infile >> a;
	infile.getline(trash,10,'=');
	infile >> b;
	infile.getline(trash,10,'=');
	infile >> w;

	while(!infile.eof())
	{
		temp= new shape;
		infile >> temp->lx >> garbage;
		infile >> temp->by >> garbage;
		infile >> temp->rx >> garbage;
		infile >> temp->ty;
		block.push_back(temp);

		
	}	
	block.pop_back();
//cout << block.size()<<endl;
//cout << block[16]->lx <<endl<< block[16]->ly <<endl<< block[16]->rx <<endl<< block[16]->ty; 
	cout << "Read Finish" << endl;
	
////////////      build the tree      /////////////

    // connect the block
	sort(block.begin(),block.end(),compare_x);
	while(i<block.size())
	{
		for(j=i+1;j<block.size();j++)
		{
			d = block[j]->lx - block[i]->rx;
			if(block[i]->rx < block[j]->lx && d < a)
			{
				if(block[j]->by >= block[i]->by && block[j]->ty <= block[i]->ty)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
					
				}
				else if(block[j]->by < block[i]->by && block[j]->ty > block[i]->ty)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
				}
				else if(block[j]->by >= block[i]->by && block[j]->by < block[i]->ty && block[j]->ty > block[i]->ty)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
				}
				else if(block[j]->ty <= block[i]->ty && block[j]->ty > block[i]->by && block[j]->by < block[i]->by)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
				}				
			} 
		}		
		i = i + 1;	
	}
	
	sort(block.begin(),block.end(),compare_y);
	i=0;
	while(i<block.size())
	{
		for(j=i+1;j<block.size();j++)
		{
			d = block[j]->by - block[i]->ty;
			if(block[i]->ty < block[j]->by && d < b)
			{
				if(block[j]->lx >= block[i]->lx && block[j]->rx <= block[i]->rx)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
					
				}
				else if(block[j]->lx < block[i]->lx && block[j]->rx > block[i]->rx)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
				}
				else if(block[j]->lx >= block[i]->lx && block[j]->lx < block[i]->rx && block[j]->rx > block[i]->rx)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
				}
				else if(block[j]->rx <= block[i]->rx && block[j]->rx > block[i]->lx && block[j]->lx < block[i]->lx)
				{
					block[i]->connect.push_back(block[j]);
					block[j]->connect.push_back(block[i]);
				}				
					
			} 
		}		
		i = i + 1;	
	}
	// build the tree
	for(i=0;i<block.size();i++)
	{
		if(block[i]->tree_done==0)
		{
			//////// first node ////////
			temp_tree = new tree;
			temp_tree->block.push_back(block[i]);
			block[i]->tree_id = count_tree;
			block[i]->tree_done = 1;
			block[i]->color = 1;
			for(j=0;j < block[i]->connect.size();j++)//color next & push next &set
			{
				block[i]->connect[j]->color = 2;
				temp_connect.push_back(block[i]->connect[j]);
				block[i]->connect[j]->tree_done=1;
			}
			//////// node in temp_connect ////////
			for(j=0;j < temp_connect.size();j++)
			{
				temp_tree->block.push_back(temp_connect[j]);//push itself to tree
				temp_connect[j]->tree_id = count_tree;
				for(int k=0 ; k < temp_connect[j]->connect.size() ; k++)//push next to temp_connect & set
				{
					if(temp_connect[j]->connect[k]->tree_done==0)
					{
						temp_connect.push_back(temp_connect[j]->connect[k]);
						temp_connect[j]->connect[k]->tree_done = 1;
					}
				}
				if(temp_tree->conflict==0)//detect conflict by color next
				{
					for(k=0 ; k < temp_connect[j]->connect.size() ; k++)
					{
						if(temp_connect[j]->connect[k]->color==0)
						{
							if(temp_connect[j]->color==1)//self
							{
								temp_connect[j]->connect[k]->color=2;//next
							}
							else if(temp_connect[j]->color==2)//self
							{
								temp_connect[j]->connect[k]->color=1;//next
							}
						}
						else if(temp_connect[j]->connect[k]->color!=0)
						{
							if(temp_connect[j]->color == temp_connect[j]->connect[k]->color)//conflict
							{
								temp_tree->conflict = 1;
								break;
							}
						}
					}
				}
				
			}
			graph.push_back(temp_tree);
			count_tree = count_tree + 1;
			temp_connect.clear();
		}		
	}

	cout << "Tree has been built!!!"<<endl;
////////////      window      /////////////	
	//find bottom & top bound
	b_bound = block[0]-> by;
	t_bound = block[0]-> ty;
	for(i=0 ; i < block.size() ; i++)
	{
		if(graph[block[i]->tree_id]->conflict == 0)
		{
			if(block[i]->by < b_bound)
			{
				b_bound = block[i]-> by;
			}		
			if(block[i]->ty > t_bound)
			{
				t_bound = block[i]-> ty;
			}
		}
	}
	//find left & right bound
	l_bound = block[0]-> lx;
	r_bound = block[0]-> rx;
	for(i=0 ; i < block.size() ; i++)
	{
		if(graph[block[i]->tree_id]->conflict == 0)
		{
			if(block[i]->lx < l_bound)
			{
				l_bound = block[i]-> lx;
			}			
			if(block[i]->rx > r_bound)
			{
				r_bound = block[i]-> rx;
			}
		}
	}
	//new window
	window* temp_win;
	vector<window*> temp_win2;
	n = (r_bound - l_bound)/w;
	int n1;
	n1=(r_bound - l_bound) % w;
	if( n1 != 0)
	{
		n++;
	}
	m = (t_bound - b_bound)/w;
	int m1;
	m1=(t_bound - b_bound) % w;
	if( m1 != 0)
	{
		m++;
	}
	for(i=0 ; i < m ; i++)
	{
		for(j=0 ; j < n ; j++)
		{
			temp_win = new window;
			temp_win2.push_back(temp_win);			
		}
		win.push_back(temp_win2);
		temp_win2.clear();
	}	
	//put block to window
	int lb_loc_x,lb_loc_y,rt_loc_x,rt_loc_y;
	int temp_bottom , temp_top , temp_left , temp_right;
	int temp1,temp2,temp3,temp4;//
	int remander_x , remander_y;
	int area;
	int temp_bound_x,temp_bound_y;
	temp_bound_x = r_bound - w;
	temp_bound_y = t_bound - w;
	int temp_diff;
	
	for(i=0 ; i < graph.size() ; i++)//each tree
	{
		if(graph[i]->conflict == 0)
		{
			for(j=0 ; j < graph[i]->block.size() ; j++)
			{
				//find block location
				// lb
				lb_loc_x = (graph[i]->block[j]->lx - l_bound) / w;
				lb_loc_y = (graph[i]->block[j]->by - b_bound) / w;
				// rt
				rt_loc_x = (graph[i]->block[j]->rx - l_bound) / w;
				remander_x = (graph[i]->block[j]->rx - l_bound) % w;
				rt_loc_x = rt_loc_x - 1;				
				if(remander_x != 0)
				{
					rt_loc_x = rt_loc_x +1;
				}
				rt_loc_y = (graph[i]->block[j]->ty - b_bound) / w;
				remander_y = (graph[i]->block[j]->ty - b_bound) % w;
				rt_loc_y = rt_loc_y -1;
				if(remander_y != 0)
				{
					rt_loc_y = rt_loc_y + 1;
				}				
				//cut block into window
				// y
				for(k=lb_loc_y ; k<=rt_loc_y ; k++)
				{

					if(m1!=0 && k==m-1)//if this part locate in the window who overlaps
					{}
					else
					{
						//list cut case in y and set temp_bottom & temp_top
						temp1 = k*w + b_bound;
						temp2 = (k+1)*w + b_bound;
						if(graph[i]->block[j]->by > temp1 && graph[i]->block[j]->ty < temp2)
						{
							temp_bottom = graph[i]->block[j]->by;
							temp_top = graph[i]->block[j]->ty;
						}
						else if(graph[i]->block[j]->by > temp1 && graph[i]->block[j]->ty >= temp2)
						{
							temp_bottom = graph[i]->block[j]->by;
							temp_top = temp2;
						}
						else if(graph[i]->block[j]->by <= temp1 && graph[i]->block[j]->ty >= temp2)
						{
							temp_bottom = temp1;
							temp_top = temp2;
						}
						else if(graph[i]->block[j]->by <= temp1 && graph[i]->block[j]->ty < temp2)
						{
							temp_bottom = temp1;
							temp_top = graph[i]->block[j]->ty;
						}
						// x						
						for(l=lb_loc_x ; l<=rt_loc_x ; l++)
						{							
							if(n1!=0 && l==n-1)
							{}
							else
							{
								//list cut case in y and set temp_left & temp_right
								temp3 = l*w + l_bound;
								temp4 = (l+1)*w + l_bound;
								if(graph[i]->block[j]->lx > temp3 && graph[i]->block[j]->rx < temp4)
								{
									temp_left = graph[i]->block[j]->lx;
									temp_right = graph[i]->block[j]->rx;
								}
								else if(graph[i]->block[j]->lx > temp3 && graph[i]->block[j]->rx >= temp4)
								{
									temp_left = graph[i]->block[j]->lx;
									temp_right = temp4;
								}
								else if(graph[i]->block[j]->lx <= temp3 && graph[i]->block[j]->rx >= temp4)
								{
									temp_left = temp3;
									temp_right = temp4;
								}
								else if(graph[i]->block[j]->lx <= temp3 && graph[i]->block[j]->rx < temp4)
								{
									temp_left = temp3;
									temp_right = graph[i]->block[j]->rx;
								}

								//area diff
								area =(temp_right - temp_left)*(temp_top - temp_bottom);

								if(graph[i]->block[j]->color==1)
								{					
									win[k][l]->color1 = win[k][l]->color1 + area;
								}
								else if(graph[i]->block[j]->color==2)
								{									
									win[k][l]->color2 = win[k][l]->color2 + area;
								}
								graph[i]->tree_win.push_back(win[k][l]);
							}
						}
						//x Ãä½t 
						if(n1!=0 && graph[i]->block[j]->rx > temp_bound_x)
						{
							if(graph[i]->block[j]->lx <= temp_bound_x)
							{
								temp_left = temp_bound_x;
								temp_right = graph[i]->block[j]->rx;
							}
							else if(graph[i]->block[j]->lx > temp_bound_x)
							{
								temp_left = graph[i]->block[j]->lx;
								temp_right = graph[i]->block[j]->rx;
							}
							//area diff
							area =(temp_right - temp_left)*(temp_top - temp_bottom);
							if(graph[i]->block[j]->color==1)
							{
								win[k][n-1]->color1 = win[k][n-1]->color1 + area;
							}
							else if(graph[i]->block[j]->color==2)
							{
								win[k][n-1]->color2 = win[k][n-1]->color2 + area;
							}
							graph[i]->tree_win.push_back(win[k][n-1]);							
						}						
					}
				}
				//y Ãä½t 
				if(m1!=0 && graph[i]->block[j]->ty > temp_bound_y)
				{
					if(graph[i]->block[j]->by <= temp_bound_y)
					{
						temp_bottom = temp_bound_y;
						temp_top = graph[i]->block[j]->ty;
					}
					else if(graph[i]->block[j]->by > temp_bound_y)
					{
						temp_left = graph[i]->block[j]->by;
						temp_right = graph[i]->block[j]->ty;
					}
					// x
					for(p=lb_loc_x ; p<=rt_loc_x ; p++)
					{
						if(n1!=0 && p==n-1)
						{}
						else
						{
							//list cut case in y and set temp_left & temp_right
							temp3 = p*w + l_bound;
							temp4 = (p+1)*w + l_bound;
							if(graph[i]->block[j]->lx > temp3 && graph[i]->block[j]->rx < temp4)
							{
								temp_left = graph[i]->block[j]->lx;
								temp_right = graph[i]->block[j]->rx;
							}
							else if(graph[i]->block[j]->lx > temp3 && graph[i]->block[j]->rx >= temp4)
							{
								temp_left = graph[i]->block[j]->lx;
								temp_right = temp4;
							}
							else if(graph[i]->block[j]->lx <= temp3 && graph[i]->block[j]->rx >= temp4)
							{
								temp_left = temp3;
								temp_right = temp4;
							}
							else if(graph[i]->block[j]->lx <= temp3 && graph[i]->block[j]->rx < temp4)
							{
								temp_left = temp3;
								temp_right = graph[i]->block[j]->rx;
							}
							//area diff
							area =(temp_right - temp_left)*(temp_top - temp_bottom);
							if(graph[i]->block[j]->color==1)
							{
								win[m-1][p]->color1 = win[m-1][p]->color1 + area;
							}
							else if(graph[i]->block[j]->color==2)
							{
								win[m-1][p]->color2 = win[m-1][p]->color2 + area;
							}
							graph[i]->tree_win.push_back(win[m-1][p]);
						}
					}					
				}
				//corner of right-top;
				if(m1!=0 && n1!=0 && graph[i]->block[j]->ty > temp_bound_y && graph[i]->block[j]->rx > temp_bound_x)
				{
					if(graph[i]->block[j]->lx <= temp_bound_x && graph[i]->block[j]->by <= temp_bound_y)
					{
						temp_left = temp_bound_x;
						temp_right = graph[i]->block[j]->rx;
						temp_bottom = temp_bound_y;
						temp_top = graph[i]->block[j]->ty;
					}
					else if(graph[i]->block[j]->lx > temp_bound_x && graph[i]->block[j]->by <= temp_bound_y)
					{
						temp_left = graph[i]->block[j]->lx;
						temp_right = graph[i]->block[j]->rx;
						temp_bottom = temp_bound_y;
						temp_top = graph[i]->block[j]->ty;
					}
					else if(graph[i]->block[j]->lx <= temp_bound_x && graph[i]->block[j]->by > temp_bound_y)
					{
						temp_left = temp_bound_x;
						temp_right = graph[i]->block[j]->rx;
						temp_bottom = graph[i]->block[j]->by;
						temp_top = graph[i]->block[j]->ty;
					}
					else if(graph[i]->block[j]->lx > temp_bound_x && graph[i]->block[j]->by > temp_bound_y)
					{
						temp_left = graph[i]->block[j]->lx;
						temp_right = graph[i]->block[j]->rx;
						temp_bottom = graph[i]->block[j]->by;
						temp_top = graph[i]->block[j]->ty;
					}
					//area diff
					area =(temp_right - temp_left)*(temp_top - temp_bottom);
					if(graph[i]->block[j]->color==1)
					{
						win[m-1][n-1]->color1 = win[m-1][n-1]->color1 + area;
					}
					else if(graph[i]->block[j]->color==2)
					{
						win[m-1][n-1]->color2 = win[m-1][n-1]->color2 + area;
					}
					graph[i]->tree_win.push_back(win[m-1][n-1]);					
				}				
			}
			// store
			for(j=0 ; j < graph[i]->tree_win.size() ; j++)
			{
				//win[x]->color1(&2) = 0;
				//store to diff<graph>
				graph[i]->tree_win[j]->diff[i] = graph[i]->tree_win[j]->color1 - graph[i]->tree_win[j]->color2;
				graph[i]->tree_win[j]->colorA[i] = graph[i]->tree_win[j]->color1;
				graph[i]->tree_win[j]->colorB[i] = graph[i]->tree_win[j]->color2;
				graph[i]->tree_win[j]->color1 = 0;
				graph[i]->tree_win[j]->color2 = 0;
			}								
		}
		
	}
	cout << "window finish!!"<<endl;
	
////////////      color      /////////////		
	int temp_d = 0 , loop_count = 0;
	int initial_score;
	int w2;
	w2 = w*w;
	double temp_score = 0 , temp_score2 = 0 , score = 0 ,last_score ,local;	
	int s ,z;
	s = graph.size();	
	double second=0;	
	clock_t t1;
	int* deck=new int [s];
	int card1,card2,temp_card;	
	int random_num,random;
	//calculate the first score	
	for(i=0 ; i < m ; i++)
	{
		for(j=0 ; j < n ; j++)
		{
			for(map<int,int>::iterator it=win[i][j]->diff.begin(); it!=win[i][j]->diff.end(); ++it)
			{
				temp_d = temp_d + it->second;
			}
			temp_d = abs(temp_d);
			temp_score = temp_score + temp_d;						
    		temp_d = 0;
		}
	}
	temp_score = temp_score*1.0/w2;
	temp_score = temp_score*20;	
	initial_score = temp_score;
	local = temp_score;

	unsigned seed;	
	seed = (unsigned)time(NULL); 
    srand(seed); 

int ini;
vector<int> good;
int big;
while(second < 600)
{	
	////wash card algorithm
	for(i=0 ; i < s ; i++)
	{
		deck[i] = i;		
	}
	
	for(j=0 ; j < s*2/3 ; j++)
	{
		card1 = rand()%s;
		card2 = rand()%s;
		temp_card = deck[card1];
		deck[card1] = deck[card2];
		deck[card2] = temp_card;
	}
	
	//random change to get intial
	random_num = rand()%s;
	for(p=0 ; p < random_num ; p++)
	{
		random = rand()%s;
		if(graph[random]->conflict==1)//if conflict=1 , skip this iteration
		{
			continue;
		}
		//inverse a random graph
		for(i=0 ; i < graph[random]->tree_win.size() ; i++)
		{
			graph[random]->tree_win[i]->diff[random] = 0 - graph[random]->tree_win[i]->diff[random] ;
		}
		graph[random]->inverse = (graph[random]->inverse + 1) % 2 ;
	}
	temp_score = 0;
	for(i=0 ; i < m ; i++)
	{
		for(j=0 ; j < n ; j++)
		{
			for(map<int,int>::iterator it=win[i][j]->diff.begin(); it!=win[i][j]->diff.end(); ++it)
			{
				temp_d = temp_d + it->second;
			}
			temp_d = abs(temp_d);
			temp_score = temp_score + temp_d;						
    		temp_d = 0;
		}
	}
	temp_score = temp_score*1.0/w2;
	temp_score = temp_score*20;
	ini=temp_score;
/*
while(z<10)
{
*/
			
	//get local	
	for(p=0 ; p<s ; p++)
	{		
		if(graph[deck[p]]->conflict==1)//if conflict=1 , skip this iteration
		{
			continue;
		}
		//inverse a random graph
		for(i=0 ; i < graph[deck[p]]->tree_win.size() ; i++)
		{
			graph[deck[p]]->tree_win[i]->diff[deck[p]] = 0 - graph[deck[p]]->tree_win[i]->diff[deck[p]] ;
		}
		graph[deck[p]]->inverse = (graph[deck[p]]->inverse + 1) % 2 ;
		//calculate the score after inverse
		for(i=0 ; i < m ; i++)
		{
			for(j=0 ; j < n ; j++)
			{
				for(map<int,int>::iterator it=win[i][j]->diff.begin(); it!=win[i][j]->diff.end(); ++it)
				{
					temp_d = temp_d + it->second;
				}
				temp_d = abs(temp_d);
				temp_score2 = temp_score2 + temp_d;						
    			temp_d = 0;
			}
		}
		temp_score2 = temp_score2*1.0/w2;
		temp_score2 = temp_score2*20;
		//judge if new score is better or not
		if(temp_score2 > temp_score)//not better , inverse the graph back
		{
			for(i=0 ; i < graph[deck[p]]->tree_win.size() ; i++)
			{
				graph[deck[p]]->tree_win[i]->diff[deck[p]] = graph[deck[p]]->tree_win[i]->diff[deck[p]] * -1;
			}
			graph[deck[p]]->inverse = (graph[deck[p]]->inverse + 1) % 2 ;
			temp_score2 = 0;			
		}
		else if(temp_score2 < temp_score)//better!! change!!
		{
//cout << "temp_score: "<<temp_score<<endl;	
//cout << "temp_score2: "<<temp_score2<<endl;
//cout << "changed"<<endl;
//			last_score = temp_score;
			temp_score = temp_score2;			
			temp_score2 = 0;
		}				
	}
/*	good.push_back(temp_score);
	
	z++;
}
	big = good[0];
	for(i=0 ; i < good.size();i++)
	{
		if(big < good[i])
		{}
		else if(big > good[i])
		{
			big = good[i];
		}
	}
	good.clear();
	*/
//cout <<	local<<endl;
//cout << temp_score <<endl;
//system("pause");		
	if(local < temp_score)
	{
		//don't store
	}
	else if(local > temp_score)
	{
		local = temp_score;
	}
//cout <<	local<<endl;
//system("pause");	

	t1 = clock();
	second = double(t1)/CLOCKS_PER_SEC;	
	/*
	loop_count = loop_count + 1;
	*/
}
cout <<	100-local<<endl;


//////////// get final state //////////////////
	int temp_color;
	double temp_A ,temp_B;
	//change block color
	for(i=0;i<graph.size();i++)
	{
		if(graph[i]->inverse == 1)
		{
			for(j=0;j<graph[i]->block.size();j++)
			{
				if(graph[i]->block[j]->color==1)
				{
					graph[i]->block[j]->color==2;
				}
				else if(graph[i]->block[j]->color==2)
				{
					graph[i]->block[j]->color==1;
				}
			}
		}
	}	
	//change color area	
	for(i=0;i<graph.size();i++)
	{
		if(graph[i]->inverse == 1)
		{
			for(j=0;j<graph[i]->tree_win.size();j++)
			{
				temp_color = graph[i]->tree_win[j]->colorA[i];
				graph[i]->tree_win[j]->colorA[i] = graph[i]->tree_win[j]->colorB[i];
				graph[i]->tree_win[j]->colorB[i] = temp_color;
			}
		}
	}	
	//calculate color area	
	for(i=0 ; i < m ; i++)
	{
		for(j=0 ; j < n ; j++)
		{
			for(map<int,int>::iterator it=win[i][j]->colorA.begin(); it!=win[i][j]->colorA.end(); ++it)
			{
				temp_A = temp_A + it->second;
			}
//			temp_A = abs(temp_A);
			temp_A = temp_A*1.0/w2 *100;
			win[i][j]->final_colorA = temp_A;					
    		temp_A = 0;
    		for(map<int,int>::iterator it=win[i][j]->colorB.begin(); it!=win[i][j]->colorB.end(); ++it)
			{
				temp_B = temp_B + it->second;
			}
//			temp_B = abs(temp_B);
			temp_B = temp_B*1.0/w2 *100;
			win[i][j]->final_colorB = temp_B;					
    		temp_B = 0;
		}
	}
cout << "final finish!!!" << endl;	
	

/////////////    output     ////////////////////

	ofstream outfile;
	outfile.open(argv[2]);
	
	//cout window
	for(i=0 ; i < m ; i++)
	{
		for(j=0 ; j < n ; j++)
		{
			if(l_bound + w*(j+1) <= r_bound && b_bound + w*(i+1) <= t_bound)
			{
				outfile << "WIN[" << n*i+j+1 << "]=" << l_bound + w*j <<","<< b_bound + w*i <<","<< l_bound + w*(j+1) <<","<< b_bound + w*(i+1);
			}
			else if(l_bound + w*(j+1) > r_bound && b_bound + w*(i+1) <= t_bound)
			{
				outfile << "WIN[" << n*i+j+1 << "]=" << r_bound-w  <<","<< b_bound + w*i <<","<< r_bound <<","<< b_bound + w*(i+1);
			}
			else if(l_bound + w*(j+1) <= r_bound && b_bound + w*(i+1) > t_bound)
			{
				outfile << "WIN[" << n*i+j+1 << "]=" << l_bound + w*j <<","<< t_bound-w <<","<< l_bound + w*(j+1) <<","<< t_bound;
			}
			else if(l_bound + w*(j+1) > r_bound && b_bound + w*(i+1) > t_bound)
			{
				outfile << "WIN[" << n*i+j+1 << "]=" << r_bound-w <<","<< t_bound-w <<","<< r_bound <<","<< t_bound;
			}
			
			outfile << "(" << win[i][j]->final_colorA << " " << win[i][j]->final_colorB << ")" << endl;			
		}
	}
	//cout conflict group
	for(i=0 ; i < graph.size() ; i++)
	{				
		if(graph[i]->conflict==1)
		{
			outfile << "GROUP" <<endl;
			for(j=0;j<graph[i]->block.size();j++)
			{
				outfile << "NO["<< j+1 <<"]=" << graph[i]->block[j]->lx <<","<< graph[i]->block[j]->by <<","<< graph[i]->block[j]->rx <<","<< graph[i]->block[j]->ty<<endl;
			}
		}		
	}
	//cout colored group
	for(i=0 ; i < graph.size() ; i++)
	{	
					
		if(graph[i]->conflict==0)
		{
			outfile << "GROUP" <<endl;
		k = 1;	
			for(j=0;j<graph[i]->block.size();j++)
			{
				if(graph[i]->block[j]->color==1)
				{					
					outfile << "CA["<< k <<"]=" << graph[i]->block[j]->lx <<","<< graph[i]->block[j]->by <<","<< graph[i]->block[j]->rx <<","<< graph[i]->block[j]->ty<<endl;
					k = k + 1;
				}	
			}
		k = 1;
			for(j=0;j<graph[i]->block.size();j++)
			{
				if(graph[i]->block[j]->color==2)
				{
					outfile << "CB["<< k <<"]=" << graph[i]->block[j]->lx <<","<< graph[i]->block[j]->by <<","<< graph[i]->block[j]->rx <<","<< graph[i]->block[j]->ty<<endl;
					k = k + 1;
				}	
			}
			
		}		
	}
	
	return 0;
}


bool compare_x(shape* a, shape* b)
{
    return a->lx < b->lx;
}

bool compare_y(shape* a, shape* b)
{
    return a->by < b->by;
}



