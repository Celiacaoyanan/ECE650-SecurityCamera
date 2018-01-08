// Compile with c++ ece650-a2.cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <string>
using namespace std;


class Graph 
{
    #define MAX    100
    #define INF    (~(0x1<<31))        
    public:
		int vNum;  //number of vertices
		int edgeNum;  //number of edges
		int mMatrix[MAX][MAX];  //mMatrix[i][j] store the distance from vertex i to j
		void Initialize(int E[MAX][2]);
		void Dijkstra(int source, int end, int prev[], int dist[],bool f);
};


void Graph::Initialize(int E[MAX][2]) 
{
	for (int i = 0; i < vNum; i++)
	{
		for (int j = 0; j < vNum; j++)
		{
			if (i == j) 
				mMatrix[i][j] = 0;  // the distance from one vertex to itself
			else
				mMatrix[i][j] = INF;
		}
	}

	for (int i = 0; i < edgeNum; i++)
	{
		int p1 = E[i][0];
		int p2 = E[i][1];
		mMatrix[p1][p2] = 1;
		mMatrix[p2][p1] = 1;
	}
}


/*
*     Dijkstra
*     source -- source vertex
*     prev -- array of previous vertex
*     dist -- array of distance, dist[i] is the distance from vertex i to source vertex
*/
void Graph::Dijkstra(int source, int end, int prev[], int dist[], bool f)
{
	int flag[MAX];      // flag[i]=1 means we have gotten the shortest path from source to vertex i 
	int k;
	int min;

	// Initialize
	for (int i = 0; i < vNum; i++)
	{
		flag[i] = 0;              
		dist[i] = mMatrix[source][i];  //dist[i] store the distance from source vertex to vertex i

		if (dist[i] == INF)
		{
			prev[i] = INF;
		}		
		else
		{
            prev[i] = source;
		}		
	}

	// Initialize source vertex
	flag[source] = 1;
	dist[source] = 0;

	for (int i = 1; i < vNum; i++)
	{
		min = INF;
		for (int j = 0; j < vNum; j++)
		{
			if (flag[j] == 0 && dist[j]<min)
			{
				min = dist[j]; 
				k = j;
			}
		}

		flag[k] = 1;

		for (int j = 0; j < vNum; j++)
		{
			int tmp;
			//int tmp = (mMatrix[k][j] == INF ? INF : (min + mMatrix[k][j]));
			if (mMatrix[k][j] != INF) //if other vertices is connected with vertex k
			{
				tmp = min + mMatrix[k][j];
			}

			if (mMatrix[k][j] != INF && flag[j] == 0 && (tmp  < dist[j]))
			{
				dist[j] = tmp;
				prev[j] = k;
			}
			
		}

		//if we find end point
		if (end == i)
		{			
			if (prev[end] == INF) // if end point has no previous vertex
			{
				cerr << "Error: path does not exist 1" << endl;
				break;
			}
			else
			{
				int* path = new int[vNum];
				int index = 0;
				path[index] = end;
				index++; 
				int tmp = prev[end];
				while (tmp != source)
				{
					path[index] = tmp;  
					index++; 
					tmp = prev[tmp];						
				}
				path[index] = source; // index is the total number of vertex in the path


				if (f == true)
				{
					for (int x = 0; x <index + 1; x++)
					{
						if (x != index)
							cout << path[x] << "-";
						else
							cout << path[x] << endl;

					}
				}
				else
				{
					for (int x = index; x >= 0; x--)
					{
						if (x != 0)
							cout << path[x] << "-";
						else
							cout << path[x] << endl;
					}
				}

				break;
			}
		}    				
	}
}


int main()
{
	int E[MAX][2];
	Graph g;
	//int last_command=0;
	char last_command='q';

	string line;

	// read from stdin until EOF
	L1:while (getline(cin, line))
	{

		istringstream input(line);

		while (!input.eof())
		{
			char command;
			input >> command;

			if (command == 'V')
			{
				//if (last_command == 1)
				if (last_command=='V')
				{
					cerr << "Error: expect E after V " << endl;
					break;
				}
				
				last_command = 'V';
				//last_command = 1;

				string content;
				input >> content;

				g.vNum = stoi(content);

			}

			else if (command == 'E')
			{
				//if (last_command == 2)
				if (last_command=='E')
				{
					cerr << "Error: E already exists" << endl;
					goto L1;
				}
				//last_command = 2;
				last_command = 'E';
				
				
				string content;
				input >> content;

				int l = content.find('{');
				int r = content.find('}');
				string content2 = content.substr(l + 1, r - l - 1);

				int l1 = content2.find('<');
				int r1 = content2.find('>');
				int comma = content2.find(',', l1);

				int edge = 0;

				while (r1 < content2.length() + 1)
				{
					string n1 = content2.substr(l1 + 1, comma-l1-1);
					int n1_i = stoi(n1);
					string n2 = content2.substr(comma+1, r1-comma-1);
					int n2_i = stoi(n2);

					if (n1_i >= g.vNum || n2_i>=g.vNum || n1_i<0 || n2_i<0)
					{
						cerr << "Error: index exceeds vertex number"<<endl;
						//last_command = 1;
						last_command = 'V';
						//break;
						goto L1;
					}

					E[edge][0] = n1_i;
					E[edge][1] = n2_i;
					edge++;

					l1 = content2.find('<', r1 + 1);
					r1 = content2.find('>', r1 + 1);
					comma = content2.find(',', l1);									
				}
				g.edgeNum = edge;
			}

			else if (command == 's')
			{	
				int u ;
				input >> u;
				int v;
				input >> v;

				if (u >= g.vNum || v >= g.vNum || u<0 || v<0)
				{
					cerr << "Error: index exceeds vertex number" << endl;
					break;
				}
				
				int prev[MAX] = { 0 };
				int dist[MAX] = { 0 };

				g.Initialize(E);

				bool f = true;
				int source;
				int end;
				if (u > v)
				{
					source = v;
					end = u;
					f = true;
				}
				else
				{
					source = u;
					end = v;
					f = false;
				}

				bool flag1 = false;
				bool flag2 = false;

				for (int i = 0;i < g.edgeNum; i++)
				{
					for (int j = 0;j < 2;j++)
					{
						if (E[i][j] == source)
						{
							flag1 = true;
						}
						if (E[i][j] == end)
						{
							flag2 = true;
						}
					}				
				}

				//cout << "flag " << flag1 << " " << flag2 << endl;
				if (source == end)
				{
					cout << source  <<endl;
				}
				else
				{
					if (flag1 == true && flag2 == true)
					{
						g.Dijkstra(source, end, prev, dist, f);
					}
					else
					{
						cerr << "Error: path does not exist 2" << endl;
						break;
					}
				}
			}
		}		
	}	
}