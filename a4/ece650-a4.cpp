// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// defined std::cout
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace Minisat;


int min_VC(int n,int** E,int edges)
{
	bool res;

	if (edges == 0)
	{
		//cout << endl;
		return 0;
	}

	for (int k = 1;k <= n;k++)   
	{
		unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
		Lit** X;
		X = new Lit*[n];
		for (int i = 0;i < n;i++)
		{
			X[i] = new Lit[k];
		}

		//initialize
		for (int i = 0;i < n;i++)
		{
			for (int j = 0;j <k;j++)//k=[0,n]
			{
				Lit l;
				l = Minisat::mkLit(solver->newVar());
				X[i][j] = l;
			}
		}

		//step1
		for (int j = 0;j <k;j++)
		{
			Minisat::vec<Minisat::Lit> constr1;
			for (int i = 0;i <n;i++)
			{
				constr1.push(X[i][j]);
			}
			solver->addClause(constr1);
		}


		//step2
		for (int m = 0;m <n;m++)
		{
			for (int p = 0;p <(k - 1);p++)
			{
				for (int q = p + 1;q <k;q++)
				{
					Minisat::vec<Minisat::Lit> constr2;
					constr2.push(~X[m][p]);
					constr2.push(~X[m][q]);
					solver->addClause(constr2);
				}
			}
		}

		//step3
		for (int m = 0;m <k;m++)
		{
			for (int p = 0;p <(n - 1);p++)
			{
				for (int q = p + 1;q <n;q++)
				{
					Minisat::vec<Minisat::Lit> constr3;
					constr3.push(~X[p][m]);
					constr3.push(~X[q][m]);
					solver->addClause(constr3);
				}
			}
		}

		//step4
		for (int e = 0;e <edges;e++)
		{
			int v1 = E[e][0];
			int v2 = E[e][1];
			Minisat::vec<Minisat::Lit> constr4;
			for (int j = 0;j <k;j++)
			{
				constr4.push(X[v1][j]);
				constr4.push(X[v2][j]);
			}
			solver->addClause(constr4);
		}


		res = solver->solve();//1-success   0-fail

		if (res == 1)
		{
			//cout << "k= " << k << endl;
			//cout << "res=" << res << endl;
			for (int i = 0;i < n;i++)
			{
				for (int j = 0;j < k;j++)
				{
					//cout << Minisat::toInt(solver->modelValue(X[i][j])) << " ";
					if (Minisat::toInt(solver->modelValue(X[i][j])) == 0)
					{
						cout << i << " ";
					}
				}
			}
			cout << endl;
			return 0;
		}
	}
}


int main(void)
{
	int n;
	int edges;
	int** E;
	int** E_tmp;

	while (!cin.eof())// read from stdin until EOF
	{

		char command;
		cin.clear();
		cin.sync();
		cin.get(command);

		switch (command)
		{
			case 'V':
			{
				string content;
				cin >> content;
				n = stoi(content);
				break;
			}

			case 'E':
			{
				string content;
				cin >> content;

				int size = content.size();
				E_tmp = new int*[size];//At first, we don't know there are how many edges
				for (int i = 0;i < size;i++)
				{
					E_tmp[i] = new int[2];
				}

				int l = content.find('{');
				int r = content.find('}');
				string content2 = content.substr(l + 1, r - l - 1);

				int l1 = content2.find('<');
				int r1 = content2.find('>');
				int comma = content2.find(',', l1);

				int edge_tmp = 0;

				while (r1 < content2.length() + 1)
				{
					string n1 = content2.substr(l1 + 1, comma - l1 - 1);
					int n1_i = stoi(n1);
					string n2 = content2.substr(comma + 1, r1 - comma - 1);
					int n2_i = stoi(n2);

					if (n1_i >= n || n2_i >= n || n1_i < 0 || n2_i < 0)
					{
						cerr << "Error: index exceeds vertex number" << endl;
					}

					if (n1_i != n2_i)
					{
						E_tmp[edge_tmp][0] = n1_i;
						E_tmp[edge_tmp][1] = n2_i;
						edge_tmp++;
					}

					l1 = content2.find('<', r1 + 1);
					r1 = content2.find('>', r1 + 1);
					comma = content2.find(',', l1);
				}
				edges = edge_tmp;
				E = new int*[edges];
				for (int i = 0;i < edges;i++)
				{
					E[i] = new int[2];
					E[i][0] = E_tmp[i][0];
					E[i][1] = E_tmp[i][1];
				}
				min_VC(n, E, edges);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	

	return 0;
}
