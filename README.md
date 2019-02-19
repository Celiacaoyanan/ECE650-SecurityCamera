# SecurityCamera
This project is to help the local police department with their installation of security cameras at traffic intersections. Based on the Vertex Cover problem, the idea is for the police to be able to minimize the number of cameras they need to install, and still be as effective as possible with their monitoring.
## A1
#### Summary
For this assignment, you need to: 1) Take as input a series of commands that describe streets. 2) Use that input to construct a particular kind of undirected graph.

#### Sample Input
The input comprises lines each of which species a command. There are 4 kinds of commands. (1) add a street, (2) change a street, (3) remove a street, and, (4) generate a graph. Here is an example of how your program should work.
```python
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
a "King Street S" (4,2) (4,8)
a "Davenport Road" (1,4) (5,8)
g
V = {
1: (2,2)
2: (4,2)
3: (4,4)
4: (5,5)
5: (1,4)
6: (4,7)
7: (5,6)
8: (5,8)
9: (3,8)
10: (4,8)
}
E = {
<1,3>,
<2,3>,
<3,4>,
<3,6>,
<7,6>,
<6,5>,
<9,6>,
<6,8>,
<6,10>
} 
```

#### Commands
- a is used to add a street. It is specied as: “a "Street Name" (x1, y1) (x2, y2) . . . (xn, yn)". Each (xi, yi) is a GPS coordinate. We interpret the coordinates as a poly-line segment. That is, we draw a line segment from (xi, yi) to (xi+1, yi+1). You are allowed to assume that each xi and yi is an integer. (Note, however, that the coordinates of an intersection may not be integer)
- c is used to change the specification of a street. Its format is the same as for a. It is a new specification for a street you've specified before 
- r is used to remove a street. It is specied as “r "Street Name" 
- g causes the program to output the corresponding graph 

#### Input and Output
Your program should take input from standard input. Your program should output to the standard output. Error should be output to standard error. You can use exceptions in your code to catch errors.

#### The Output Graph
There is a vertex corresponding to: (1) each intersection, and, (2) the end-point of a line segment of a street that intersects with another street. There is an edge between two vertices if: (1) at least one of them is an intersection, (2) both lie on the same street, and, (3) one is reachable from the other without traversing another vertex.

## A2 
#### Summary
For this assignment, you need to write a program that takes input till it sees an EOF. One of the kinds of input contributes towards the specification of an undirected graph. Another kind of input asks you to print out a shortest-path from one vertex to another in the current graph.

#### Sample Run
```python
V 15
E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}
s 2 10
2-8-10
V 5
E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}
s 4 0
4-1-2-0
```
#### Input, Output, and Error
Your program should take input from standard input, and output to standard output. Errors can also be output to standard output, but should always start with “Error:" followed by a brief description. Your program should terminate gracefully (and quietly) once it sees EOF. 


## A3
#### Summary
For this assignment, 1) Modify the output format of your Python script from Assignment 1 to match the input format of your C++ program from Assignment 2, 2) Modify your C++ program from Assignment 2 to output the input graph on standard output, 3) Write a program in C++ to generate random input for your Python script, 4) Write a driver program in C++ that uses Inter-Process Communication (IPC) to link the output of the random input generator to the input of the Python script, and the output of the Python script to the input of the C++ program from Assignment 2.

#### Sample Run
```python
$ cd build
$ cmake ../
$ make install
$ cd ./run/bin
$ ./ece650-a3 -s 5 -n 4 -l 5
V = 8
E = {<0,2>,<0,3>,<0,4>,<1,3>,<4,7>,<5,2>,<5,6>}
s 2 4
2-0-4
```
In the above, the first three lines make your executable, and run the driver program with some command-line arguments. Then, the lines “V = ...", “E = ...", and “2-0-4" are output. The input the user provided to your program via stdin is “s 2 4".

#### Python script
You should edit your Python script from Assignment 1 so that its output format for the specication of the graph matches the input format for your C++ program from Assignment 2. Think of it as though your Python script provides command-line input to the C++ program. The way to do this is to simply map each vertex to an index in [0, n-1] (where n is the number of vertices), and rename your edges accordingly. Also, the only output that your Python script should produce to stdout is in response to “g",for which it outputs the specication of the graph (i.e., V and E). Error output should go to stderr.

#### Cpp program from Assignment 2
You should edit it to print the graph that it has read before accepting an s command. This is necessary so that you know what graph has been produced and what vertexes are available.

#### Random input generator
Your random input generator rgen should generate random inputs of street specications for your Python script. It takes four command-line arguments. All are optional. Also, note that your random generator could go into an innite loop looking for a valid specification. You should disallow this from happening by limiting the number of tries. That is, if your random generator fails to generate a valid specication for a continuous A number of attempts, it should exit() with an error message reported on stderr. A reasonable A to adopt may be 25. Whatever A you adopt, your error message should mention it. That is, your error message should be something like, “Error: failed to generate valid input for 25 simultaneous attempts".
- -s k - where k is an integer>=2. The number of streets should be a random integer in [2, k]. If this option is not specified, you should use a default of k = 10; that is, the number of streets should be a random integer in [2, 10].
- -n k - where k is an integer>=1. The number of line-segments in each street should be a random integer in [1, k]. Default: k = 5.
- -l k - where k is an integer>=5. Your process should wait a random number w seconds, where w is in [5, k] before generating the next (random) input. Default: k = 5.
- -c k - where k is an integer>=1. Your process should generate (x, y) coordinates such that every x and y value is in the range [-k, k]. For example, if k = 15, all of your coordinate values should be integers between -15 and 15. Default: k = 20.
A typical interaction of the random generator is as follows:
1. issue enough r commands to clear any existing street database;
2. issue a commands to add new streets satisfying the specication;
3. issue a g command;
4. wait for specied number of seconds and repeat.

#### Driver
Your driver program has the overall control. You have at least three programs that run concurrently: (1) the random generator, (2) your Python script from Assignment 1, and, (3) your program from Assignment 2 that takes as input a graph-specification and computes shortest paths. Your driver program should fork() two processes and exec() two of those programs, and then exec() the third (so it turns into a process that corresponds to the third program). It should set up all IPC appropriately beforehand. It should send normal output to stdout, error output to stderr, and take input from stdin. As we mention above, the only input it takes are “s" commands, that ask for a shortest path between vertices.

## A4
#### Summary
For this assignment, you are to augment your code from Assignment 2 to solve the minimal Vertex Cover problem for the input graph. Your approach is based on a polynomial time reduction to CNF- SAT, and use of a SAT solver.

#### SAT Solver
We will be using MiniSat SAT solver available at https://github.com/agurfinkel/minisat MiniSat provides a CMake build system. You can compile it using the usual sequence: cd PROJECT && mkdir build && cd build && cmake ../ && make. The build process creates an executable minisat and a library libminisat.a. You will need link against the library in your assignment.

#### Incorporate SAT
Create a polynomial reduction of the decision version of VERTEX COVER to CNF-SAT. Implement the reduction and use minisat as a library to solve the minimum VERTEX COVER problem for the graphs that are input to your program (as in Assignment 2). As soon as you get an input graph via the 'V' and 'E' specification you should compute a minimum-sized Vertex Cover, and immediately output it. The output should just be a sequence of vertices in increasing order separated by one space each. You can use qsort(3) or std::sort for sorting. The following is a sample run of your program:
```python
$ ./ece650-a4
V 5
E {<0,4>,<4,1>,<0,3>,<3,4>,<3,2>,<1,3>}
3 4
```
The lines starting with V and E are the inputs to your program, and the last line is the output. Note that the minimum-sized vertex cover is not necessarily unique. You need to output just one of them.


## A5
#### Summary
You should augment your code from Assignment 4 in the following ways. Make it multithreaded. You should have at least 4 threads: one for I/O, and one each for the different approaches to solve the minimum vertex cover problem. Implement the following two additional ways to solve MIN-VERTEX-COVER, in addition to the REDUCTION-TO-CNF-SAT approach you had in Assignment 4. (We will call your approach from Assignment 4, CNF-SAT-VC.)
1. Pick a vertex of highest degree (most incident edges). Add it to your vertex cover and throw away all edges incident on that vertex. Repeat till no edges remain. We will call this algorithm APPROX-VC-1.
2. Pick an edge hu; vi, and add both u and v to your vertex cover. Throw away all edges attached to u and v. Repeat till no edges remain. We will call this algorithm APPROX- VC-2.

#### Input and Output
As input, use the output of /home/agurfink/ece650/graphGen/graphGen on ecelinux. That program generates graphs with the same number of edges for a particular number of vertices, but not necessarily the same edges. Given a graph as input, your program should output the vertex cover computed by each approach in sorted order. That is, give the following input:
```python
V 5
E {<2,1>,<2,0>,<2,3>,<1,4>,<4,3>}
The output from your program should be:
CNF-SAT-VC: 2,4
APPROX-VC-1: 2,4
APPROX-VC-2: 0,2,3,4
```
#### Analysis
You should analyze how efficient each approach is, for various inputs. An input is characterized by the number of vertices. Efficient is characterized in one of two ways: (1) running time, and (2) approximation ratio. We characterize the approximation ratio as the ratio of the size of the computed vertex cover to the size of an optimal (minimum-sized) vertex cover. For measuring the running time, use pthread getcpuclockid(). For measuring the approximation ratio, compare it to the output of CNF-SAT-VC, which is guaranteed to be optimal.
