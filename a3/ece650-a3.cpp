//c++ ece650-a3.cpp -std=c++11 -o ece650-a3
#include <sys/types.h>
#include <sys/wait.h>
#include<vector>
#include <unistd.h>
#include <signal.h>
#include<iostream>
#include <string>
using namespace std;


int get_s(void)
{
	string line;
	while (!cin.eof())
	{
		getline(cin, line);
		cout << line<<endl;
	}	
	return 0;
}


int main(int argc, char **argv)
{

	vector<pid_t> kids;

	// create pipe
	int Rto1[2];//1:write output  0:read input
	pipe(Rto1);

	int a1toa2[2];
	pipe(a1toa2);

	pid_t rgen_pid;
	rgen_pid = fork();
	if (rgen_pid == 0)//rgen write output to pipe
	{
		// redirect stdout to the pipe
		dup2(Rto1[1], STDOUT_FILENO);
		close(Rto1[0]);
		close(Rto1[1]);     

		char* arg_rgen[argc+1];
		arg_rgen[0] = (char*)"./rgen";
		for (int i = 1;i < argc;i++)
		{
			arg_rgen[i] = argv[i];
		}
		arg_rgen[argc] = NULL;

		execv("./rgen", arg_rgen);
	}
	else if (rgen_pid < 0) {
		cerr << "Error: could not fork\n";
		return 1;
	}

	kids.push_back(rgen_pid);

	
	pid_t a1_pid;
	a1_pid = fork();
	if (a1_pid == 0)//a1 read input from pipe
	{
		// redirect stdin from the pipe
		dup2(Rto1[0], STDIN_FILENO);	
		close(Rto1[1]);
		close(Rto1[0]);
	
		dup2(a1toa2[1], STDOUT_FILENO);
		close(a1toa2[0]);
		close(a1toa2[1]);
		
		char*arga1[3];
		arga1[0] = (char*)"python";
		arga1[1]= (char*)"ece650-a1.py";
		arga1[2] = NULL;
		execvp("python", arga1);
	}
	else if (a1_pid < 0) {
		cerr << "Error: could not fork\n";
		return 1;
	}

	kids.push_back(a1_pid);
	
	
	
	pid_t a2_pid;
	a2_pid = fork();
	if (a2_pid == 0)//a2 read input from a1 and keyboard
	{
		dup2(a1toa2[0], STDIN_FILENO);
		close(a1toa2[1]);
		close(a1toa2[0]);
		char*arga2[2];
		arga2[0] = (char*)"./ece650-a2";
		arga2[1] = NULL;
		execv("./ece650-a2", arga2);
	}
	else if (a2_pid < 0) {
		cerr << "Error: could not fork\n";
		return 1;
	}

	kids.push_back(a2_pid);
	
	dup2(a1toa2[1], STDOUT_FILENO);
	close(a1toa2[0]);
	close(a1toa2[1]);
	get_s();
	
	a1_pid = 0;
	rgen_pid = 0;
	a2_pid=0;

	// send kill signal to all children
	for (pid_t k : kids) {
		int status;
		kill(k, SIGTERM);
		waitpid(k, &status, 0);
	}

    return 0;
}