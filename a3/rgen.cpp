//c++ rgen.cpp -std=c++11 -o rgen
#include <iostream>
#include <fstream>
#include<string>
#include <stdio.h>
#include <unistd.h>
#include<vector>
#include<algorithm>
using namespace std;


class Point {
public:
	int x;
	int y;
	Point() : x(0), y(0) {};
	Point(int x, int y) : x(x), y(y) {};
};


class Line {
public:
	Point src;
	Point dst;
	Line();
	Line(Point s, Point d) {
		src = s;
		dst = d;
	};
};
Line::Line()
{
	//
}


int* sort(int* record_ran, int* sorted_ran, int len)
{
	int tmp;
	for (int i = 0;i < len;i++)
	{
		for (int j = 0;j < len - i;j++)
		{
			if (record_ran[j] > record_ran[j + 1])
			{
				tmp = record_ran[j];
				record_ran[j] = record_ran[j + 1];
				record_ran[j + 1] = tmp;
			}
		}
	}
	for (int k = 0;k < len;k++)
	{
		sorted_ran[k] = record_ran[k];
	}
	return sorted_ran;
}


bool segment_not_exist(Line** streets, int L, int* record_num_n, Line* record_segment, int l)
{
	for (int i = 0;i < l;i++)
	{
		for (int j = 0;j < L;j++)
		{
			for (int k = 0;k < record_num_n[j];k++)
			{
				if (record_segment[i].src.x == streets[j][k].src.x && record_segment[i].src.y == streets[j][k].src.y &&record_segment[i].dst.x == streets[j][k].dst.x && record_segment[i].dst.y == streets[j][k].dst.y)
				{
					return false;
				}

			}
		}
	}
	return true;
}


int main(int argc, char **argv) {
	
	string svalue;
	string nvalue;
	string lvalue;
	string cvalue;

	int cmd;

	int s = 10;//number of streets [2,k]  [2,10]
	int n = 5;//number of line-segments in each street [1,k] [1,5]
	int l = 5;//wait a random number w seconds [5,k]  [5,5]
	int c = 20;//coordinate range [-c,c] [-20,20]

	opterr = 0;

	while ((cmd = getopt(argc, argv, "s:n:l:c:")) != -1)
	{
		switch (cmd)
		{
		case 's':
			svalue = optarg;
			s = atoi(svalue.c_str());
			if (s < 2)
			{
				cerr << "Error: s should be more than or equal to 2" << endl;
				return 1;
			}
			break;
		case 'n':
			nvalue = optarg;
			n = atoi(nvalue.c_str());
			if (n < 1)
			{
				cerr << "Error: n should be more than or equal to 1" << endl;
				return 1;
			}
			break;
		case 'l':
			lvalue = optarg;
			l = atoi(lvalue.c_str());
			if (l < 5)
			{
				cerr << "Error: l should be more than or equal to 5" << endl;
				return 1;
			}
			break;
		case 'c':
			cvalue = optarg;
			c = atoi(cvalue.c_str());
			break;
		case '?':
			if (optopt == 's' || optopt == 'n' || optopt == 'l' || optopt == 'c')
				cerr << "Error: option -" << optopt << " requires an argument." << endl;
			else
				cerr << "Error: unknown option" << endl;
			return 1;
		default:
			return 0;
		}
	}
  
	
	ifstream urandom("/dev/urandom");

	if (urandom.fail()) {
		cerr << "Error: cannot open /dev/urandom\n";
		return 1;
	}

	unsigned int num = 0;
	string command_add;
	string command_remove;
	bool flag = 0;
	int record_num_s = 0;//number of streets
	string* names;//array of names of streets

	while (true)
	{
		int num_attempts = 0;
		if (flag == 1)//flag=0 when fist step into while loop, don't need to remove streets
		{
			for (int i = 0;i < record_num_s;i++)
			{
				command_remove = "r \"";
				command_remove += names[i];
				command_remove += "\" ";
				cout << command_remove << endl;
			}
		}
		
		urandom.read((char*)&num, sizeof(int));
		int num_s = num % (s - 2 + 1) + 2;//number of streets
		record_num_s = num_s;//record the number of streets for e command
		names = new string[record_num_s];

		urandom.read((char*)&num, sizeof(int));
		int num_l = num % (l - 5 + 1) + 5;//sleep seconds

		
		Line** streets;
		streets = new Line*[num_s];
		int* record_num_n;//record number of line segments in each streets for one graph
		record_num_n = new int[num_s];

		for (int i = 0;i < num_s;i++)
		{
			string name;
			for (int j = 0;j < i + 1;j++)
			{
				name += "a";
			}
			names[i] = name;

			urandom.read((char*)&num, sizeof(int));
			int num_n = num % (n - 1 + 1) + 1;//number of line segment
			record_num_n[i] = num_n;


			vector<int> record_ran(2 * (num_n + 1));
			L1:for (int j = 0;j < 2 * (num_n + 1);j++)//n line segments, n+1 points, 2*(n+1) random numbers
			{
				urandom.read((char*)&num, sizeof(int));
				record_ran[j] = num % (2 * c + 1) - c;
			}
			sort(record_ran.begin(), record_ran.end());			

			Point* record_point;
			record_point = new Point[num_n + 1];
			Line* record_segment;
			record_segment = new Line[num_n];

			//combine every 2 random numbers to be a point
			for (int j = 0;j < num_n + 1;j++)//num_n+1 points
			{
				record_point[j].x = record_ran[2*j];
				record_point[j].y = record_ran[2*j + 1];
			}

			// determine whether there is zero length segment
			for (int j = 0;j < num_n;j++)
			{
				if(record_point[j].x==record_point[j+1].x && record_point[j].y == record_point[j + 1].y)
				{
						num_attempts += 1;
						if (num_attempts == 25)
					    {
							cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
							exit(0);
						}
						goto L1;
				}
			}

			//combine 2 points to be a line segment
			for (int j = 0;j < num_n;j++)
			{
				record_segment[j].src.x = record_point[j].x;
				record_segment[j].src.y = record_point[j].y;
				record_segment[j].dst.x = record_point[j+1].x;
				record_segment[j].dst.y = record_point[j+1].y;
			}
			
			//determing whether the line segments have already been generated
			if (segment_not_exist(streets, i, record_num_n, record_segment, num_n) == false)
			{
				num_attempts += 1;
				if (num_attempts == 25)
				{
					cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
					exit(0);
				}
				goto L1;
			}
			streets[i] = record_segment;//{{l1,l2,l3...},{},{},{}...}

			//generate command
			command_add = "a \"";
			command_add += names[i];
			command_add += "\" ";
			for (int j = 0;j < num_n+1;j++)
			{
				command_add += '(';
				command_add += to_string(record_point[j].x);
				command_add += ',';
				command_add += to_string(record_point[j].y);
				command_add += ')';
			}
			cout << command_add << endl;
		}
		
		cout << "g" << endl;
		flag = 1;
		sleep(num_l);
	}

	urandom.close();// close random stream
	return 0;
}