//Zachary Meadows
//Bryant Walley
//Intro to AI
//October 2017
//This program prompts the user for input: create starting state, use breadthfirst search, depthfirst search, A* search
	//using number of misplaced tiles, and A* search using the manhattan distance. Once a starting state is created and 
	//is printed to the screen, the user may select any of the four algorithms to check for a solution. If a solution is 
	//not found, the user is informed.
	//If there is a solution, the path to the solution is printed, along with statistics such as depth of solution,
	//the number of nodes generated, and the time it took to find the solution.

#include <iostream>
#include <map>
#include <array>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>
#include <stack>
#include <queue>
#include <iomanip>

using namespace std;

//node struct
struct node
{
	//current state
	string state;
	//current node count
	int nodeCount;
	//current depth
	int depth;
	//# of misplaced tiles
	int numOfMisplacedTiles;
	//manhattan distance of tiles
	int manhattanDistance;
	//path leading to and including the node
	list<string> pathToNode;
};
//function prototypes
string createStartingState();
string findPossibleMoves(int &counter, int &numOfMoves, int row, int column);
bool breadthFirst(int &nodeCountOfSolution, int &depthOfSolution);
bool depthFirst(int &nodeCountOfSolution, int &depthOfSolution);
bool aStarTiles(int &nodeCountOfSolution, int &depthOfSolution);
bool aStarDistance(int &nodeCountOfSolution, int &depthOfSolution);
void stringToArray(string state);
void printInitialState();
void findZero(int &row, int &column);
void getIndexOfValue(string state, char value, int &row, int &column);
void printPath();
void printStatistics(int nodeCountOfSolution, int depthOfSolution, double endTime);
char returnRandNum(string &poolOfChars);
node createNode(string state, int nodeCount, int depth, list<string> &pathToNode);
int findNumOfMissingTiles(string state);
int findManhattanDistance(string state);

//global vars

//struct compareTiles
struct compareTiles
{
	//boolean function to return which value is greater than or less than
	bool operator()(const node &l, const node &r)
	{

		return l.numOfMisplacedTiles > r.numOfMisplacedTiles;
	}
};

//struct compareDistance
struct compareDistance
{
	//boolean function to return which value is greater than or less than
	bool operator()(const node &l, const node &r)
	{

		return l.manhattanDistance > r.manhattanDistance;
	}
};

//holds our goal state
const string GOAL_STATE = "123456780";
//holds our initial state
string initialState = "0";
//holds our current state
string workingState;
//holds a child state of our current state
string tempState;
//array depiction of our puzzle
int puzzleArray[3][3] = { 0 };
//map to store visited states
map <string, int> puzzleMap;
//queue for breadthfirst search
queue <node> bfQueue;
//stack for depthfirst search
stack <node> dfStack;
//priority queue for A* using misplaced tiles
priority_queue <node, vector<node>, compareTiles > astQueue;
//priority queue for A* using misplaced tiles
priority_queue <node, vector<node>, compareDistance > asdQueue;
//create a temp list for current path
list<string> tempList;

int main()
{
	//create a value for user input
	int sentinelVal = -999;

	//solution nodeCount and depth initialized to 0
	int nodeCountOfSolution = 0;
	int depthOfSolution = 0;

	//start and end times for time elapsed
	double start = 0;
	double end = 0;

	//loop
	do
	{
		//print menu
		cout << '\t' << "   -------------------" << endl;
		cout << '\t' << "   | 8-Slider Puzzle |" << endl;
		cout << '\t' << "   -------------------" << endl << endl;

		cout << "   1) . . ." << "Create Starting State" << endl;
		cout << "   2) . . ." << "Breadth-First Search" << endl;
		cout << "   3) . . ." << "Depth-First Search" << endl;
		cout << "   4) . . ." << "A* Search Using Misplaced Tiles" << endl;
		cout << "   5) . . ." << "A* Search Using Manhattan Distance" << endl;
		cout << "-999) . . ." << "Exit" << endl << endl;

		//get user input
		cin >> sentinelVal;
		system("CLS");

		switch (sentinelVal)
		{
			//case to create a starting state

		case 1:
			//initial state = return value from createStartingState function
			initialState = createStartingState();

			//translate the string into an array
			stringToArray(initialState);

			//print the initial state
			printInitialState();
			cout << endl << endl;
			break;

			//case to perform a breadthFirst search on the puzzle
		case 2:
			//if they have already selected an initial state
			if (initialState != "0")
			{
				//print to screen
				cout << "Calculating Result . . ." << endl;

				//get current time
				start = clock();

				//if breadthfirst returns true
				if (breadthFirst(nodeCountOfSolution, depthOfSolution) == 1)
				{
					//print solution
					system("CLS");
					cout << "GOAL FOUND" << endl;

					//end time (in secs) = (current time - start time)
					end = (clock() - start) / (double)CLOCKS_PER_SEC;
					printPath();
					printStatistics(nodeCountOfSolution, depthOfSolution, end);
					cout << endl << endl;
				}
				//if it returns false
				else
				{
					//inform user there was no solution
					cout << "There Was No Solution" << endl << endl;
				}
				//clear map and list at end of search for next performed search
				puzzleMap.clear();
				tempList.clear();

				//clear queue
				while (!bfQueue.empty())
				{
					bfQueue.pop();
				}
			}

			//if starting state has not been created
			else
			{
				cout << "You Must Create A Starting State Before Attempting A Search!" << endl << endl;
			}
			break;

			//case for depthFirst search
		case 3:

			//if initial state has been set
			if (initialState != "0")
			{
				//print to screen
				cout << "Calculating Result . . ." << endl;

				//get current time
				start = clock();

				//if function returns true
				if (depthFirst(nodeCountOfSolution, depthOfSolution) == 1)
				{
					//print solution
					system("CLS");
					cout << "GOAL FOUND!" << endl;

					//end time (in secs) = (current time - start time)
					end = (clock() - start) / (double)CLOCKS_PER_SEC;
					printStatistics(nodeCountOfSolution, depthOfSolution, end);
					cout << endl << endl;
				}

				//if no solution
				else
				{
					//inform user that a solution was not found
					cout << "There Was No Solution" << endl << endl;
				}

				//clear map and list for next search
				puzzleMap.clear();
				tempList.clear();

				//clear stack
				while (!dfStack.empty())
				{
					dfStack.pop();
				}
			}

			//if starting state was not created
			else
			{
				cout << "You Must Create A Starting State Before Attempting A Search!" << endl << endl;
			}
			break;

		case 4:
			//if they have already selected an initial state
			if (initialState != "0")
			{
				//print to screen
				cout << "Calculating Result . . ." << endl;

				//get current time
				start = clock();
				//if breadthfirst returns true
				if (aStarTiles(nodeCountOfSolution, depthOfSolution) == 1)
				{
					//print solution
					system("CLS");
					cout << "GOAL FOUND" << endl;

					//end time (in secs) = (current time - start time)
					end = (clock() - start) / (double)CLOCKS_PER_SEC;
					printPath();
					printStatistics(nodeCountOfSolution, depthOfSolution, end);
					cout << endl << endl;
				}
				//if it returns false
				else
				{
					//inform user there was no solution
					cout << "There Was No Solution" << endl << endl;
				}
				//clear map and list at end of search for next performed search
				puzzleMap.clear();
				tempList.clear();

				//clear priority queue
				while (!astQueue.empty())
				{
					astQueue.pop();
				}
			}

			//if starting state has not been created
			else
			{
				cout << "You Must Create A Starting State Before Attempting A Search!" << endl << endl;
			}
			break;
		case 5:
			//if they have already selected an initial state
			if (initialState != "0")
			{
				//print to screen
				cout << "Calculating Result . . ." << endl;

				//get current time
				start = clock();

				//if breadthfirst returns true
				if (aStarDistance(nodeCountOfSolution, depthOfSolution) == 1)
				{
					//print solution
					system("CLS");
					cout << "GOAL FOUND" << endl;

					//end time (in secs) = (current time - start time)
					end = (clock() - start) / (double)CLOCKS_PER_SEC;
					printPath();
					printStatistics(nodeCountOfSolution, depthOfSolution, end);
					cout << endl << endl;
				}
				//if it returns false
				else
				{
					//inform user there was no solution
					cout << "There Was No Solution" << endl << endl;
				}
				//clear map and list at end of search for next performed search
				puzzleMap.clear();
				tempList.clear();

				//clear priority queue
				while (!asdQueue.empty())
				{
					asdQueue.pop();
				}
			}

			//if starting state has not been created
			else
			{
				cout << "You Must Create A Starting State Before Attempting A Search!" << endl << endl;
			}
			break;
		}
		//while user input is not exit code
	} while (sentinelVal != -999);
	return 0;
}

//function to create a random starting state
string createStartingState()
{
	//value to be returned (starting state)
	string state;

	//temp var to hold randomly-generated chars
	char tempChar;

	//copy of GOAL_STATE to pull possible random chars from
	string poolOfChars = GOAL_STATE;

	//give rand() function the computer's current time
	srand((unsigned)time(0));

	//loop through all indices
	for (int i = 0; i < 9; i++)
	{

		//return random char and store in tempChar
		tempChar = returnRandNum(poolOfChars);

		//add the char to our state to be returned
		state += tempChar;
	}
	//return starting state
	return state;
}

//function to perform breadthFirst search on the puzzle
bool breadthFirst(int &nodeCountOfSolution, int &depthOfSolution)
{
	int row = 0;
	int column = 0;
	int numOfMoves = 0;
	int nodeCount = 1;
	int depth = 0;

	//create a node for initial state
	node startNode;

	//push starting state into queue
	bfQueue.push(createNode(initialState, nodeCount, depth, tempList));

	//add starting state to map
	puzzleMap[initialState] = nodeCount;

	//while queue has nodes
	while (!bfQueue.empty())
	{
		int counter = 0;

		//working state = front of queue's state
		workingState = bfQueue.front().state;

		//depth = front of queue's depth
		depth = bfQueue.front().depth;

		//path = front of queue's path
		tempList = bfQueue.front().pathToNode;

		//pop the node from the queue
		bfQueue.pop();

		//if goal state
		if (workingState == GOAL_STATE)
		{
			//set depth and nodeCount of solution
			depthOfSolution = depth;
			nodeCountOfSolution = nodeCount;
			return 1;
		}

		//increment depth
		depth++;

		//find 0 location
		findZero(row, column);

		//while number of possible moves != 0
		do
		{
			//update tempState to the next possible move
			tempState = findPossibleMoves(counter, numOfMoves, row, column);

			//if possible move is not in map
			if (!puzzleMap.count(tempState))
			{
				//increment nodeCount
				nodeCount++;

				//place into map
				puzzleMap[tempState] = nodeCount;

				//push onto queue
				bfQueue.push(createNode(tempState, nodeCount, depth, tempList));
			}
		} while (counter != numOfMoves);
		//	cout << endl << endl;
	}
	return 0;
}

bool depthFirst(int &nodeCountOfSolution, int &depthOfSolution)
{
	int row = 0;
	int column = 0;
	int numOfMoves = 0;
	int nodeCount = 1;
	int depth = 0;

	//create a node for initial state
	node startNode;

	//push starting state into queue
	dfStack.push(createNode(initialState, nodeCount, depth, tempList));

	//add starting state to map
	puzzleMap[initialState] = nodeCount;

	//while stack has nodes
	while (!dfStack.empty())
	{
		//counter for num of moves
		int counter = 0;

		//working state = top of stack's state
		workingState = dfStack.top().state;

		//depth = top of stack's depth
		depth = dfStack.top().depth;

		//pop the node from the stack
		dfStack.pop();

		//if goal state
		if (workingState == GOAL_STATE)
		{

			//set depth and nodecount of solution
			depthOfSolution = depth;
			nodeCountOfSolution = nodeCount;
			return 1;
		}
		//increment depth
		depth++;

		//find 0 location
		findZero(row, column);

		//while number of possible moves != number of moves
		do
		{
			//update tempState to the next possible move
			tempState = findPossibleMoves(counter, numOfMoves, row, column);

			//if possible move is not in map
			if (!puzzleMap.count(tempState))
			{
				//increment nodeCount
				nodeCount++;

				//place into map
				puzzleMap[tempState] = nodeCount;

				//push onto stack
				dfStack.push(createNode(tempState, nodeCount, depth, tempList));
			}
		} while (counter != numOfMoves);
		//	cout << endl << endl;
	}
	return 0;
}

bool aStarTiles(int & nodeCountOfSolution, int & depthOfSolution)
{
	int row = 0;
	int column = 0;
	int numOfMoves = 0;
	int nodeCount = 1;
	int depth = 0;

	//create a node for initial state
	node startNode;

	//push starting state into queue
	astQueue.push(createNode(initialState, nodeCount, depth, tempList));

	//add starting state to map
	puzzleMap[initialState] = nodeCount;

	//while queue has nodes
	while (!astQueue.empty())
	{
		//counter for num of moves
		int counter = 0;

		//working state = top of queue's state
		workingState = astQueue.top().state;

		//depth = top of queue's depth
		depth = astQueue.top().depth;

		//path = top of queue's path
		tempList = astQueue.top().pathToNode;

		//pop the node from the queue
		astQueue.pop();

		//if goal state
		if (workingState == GOAL_STATE)
		{

			//set depth and nodecount of solution
			depthOfSolution = depth;
			nodeCountOfSolution = nodeCount;
			return 1;
		}
		//increment depth
		depth++;

		//find 0 location
		findZero(row, column);

		//while number of possible moves != number of moves
		do
		{
			//update tempState to the next possible move
			tempState = findPossibleMoves(counter, numOfMoves, row, column);

			//if possible move is not in map
			if (!puzzleMap.count(tempState))
			{
				//increment nodeCount
				nodeCount++;

				//place into map
				puzzleMap[tempState] = nodeCount;

				//push onto queue
				astQueue.push(createNode(tempState, nodeCount, depth, tempList));
			}
		} while (counter != numOfMoves);
		//	cout << endl << endl;
	}
	return 0;
}

bool aStarDistance(int & nodeCountOfSolution, int & depthOfSolution)
{
	int row = 0;
	int column = 0;
	int numOfMoves = 0;
	int nodeCount = 1;
	int depth = 0;

	//create a node for initial state
	node startNode;

	//push starting state into queue
	asdQueue.push(createNode(initialState, nodeCount, depth, tempList));

	//add starting state to map
	puzzleMap[initialState] = nodeCount;

	//while queue has nodes
	while (!asdQueue.empty())
	{
		//counter for num of moves
		int counter = 0;

		//working state = top of queue's state
		workingState = asdQueue.top().state;

		//depth = top of queue's depth
		depth = asdQueue.top().depth;

		//path = top of queue's path
		tempList = asdQueue.top().pathToNode;

		//pop the node from the queue
		asdQueue.pop();

		//if goal state
		if (workingState == GOAL_STATE)
		{

			//set depth and nodecount of solution
			depthOfSolution = depth;
			nodeCountOfSolution = nodeCount;
			return 1;
		}
		//increment depth
		depth++;

		//find 0 location
		findZero(row, column);

		//while number of possible moves != number of moves
		do
		{
			//update tempState to the next possible move
			tempState = findPossibleMoves(counter, numOfMoves, row, column);

			//if possible move is not in map
			if (!puzzleMap.count(tempState))
			{
				//increment nodeCount
				nodeCount++;

				//place into map
				puzzleMap[tempState] = nodeCount;

				//push onto queue
				asdQueue.push(createNode(tempState, nodeCount, depth, tempList));
			}
		} while (counter != numOfMoves);
		//	cout << endl << endl;
	}
	return 0;
}


//function to print the initial state to the user
void printInitialState()
{
	cout << "S T A R T" << endl;
	//for every index
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			//print out current element
			cout << '[' << puzzleArray[row][column] << "]";
		}
		cout << endl;
	}
	cout << endl;
}

//converts a string into a 2D array
void stringToArray(string state)
{
	int counter = 0;

	//for every index
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//element of index = corresponding char value
			puzzleArray[i][j] = (state[counter] - '0');
			counter++;
		}
	}
}

//function to find possible moves based on the current state
string findPossibleMoves(int &counter, int &numOfMoves, int row, int column)
{

	//initialize our move to the current state
	string possibleMove = workingState;

	//if index [0][0]
	if (row == 0 && column == 0)
	{
		//counter = num of possible moves
		numOfMoves = 2;
		counter++;
		switch (counter)
		{
		case 1:
			//move right
			//zero location = location to right
			possibleMove[0] = workingState[1];

			//location to right = 0
			possibleMove[1] = '0';

			break;
		case 2:
			//move down
			//zero location = location beneath it
			possibleMove[0] = workingState[3];

			//location beneath it = 0
			possibleMove[3] = '0';

			break;
		}
	}

	//if index [0][1]
	if (row == 0 && column == 1)
	{
		//counter = num of possible moves
		numOfMoves = 3;
		counter++;
		switch (counter)
		{
		case 1:
			//move right
			//zero location = location to right
			possibleMove[1] = workingState[2];

			//location to right = 0
			possibleMove[2] = '0';

			break;
		case 2:
			//move down
			//zero location = location beneath it
			possibleMove[1] = workingState[4];

			//location beneath it = 0
			possibleMove[4] = '0';

			break;

		case 3:
			//move left
			//zero location = location to the left
			possibleMove[1] = workingState[0];

			//location to the left = 0
			possibleMove[0] = '0';

			break;
		}
	}

	//if index [0][2]
	if (row == 0 && column == 2)
	{
		//counter = num of possible moves
		numOfMoves = 2;
		counter++;
		switch (counter)
		{
		case 1:
			//move below
			//zero location = location beneath it
			possibleMove[2] = workingState[5];

			//location beneath it = 0
			possibleMove[5] = '0';

			break;

		case 2:
			//move left
			//zero location = location to the left
			possibleMove[2] = workingState[1];

			//location to the left = 0
			possibleMove[1] = '0';

			break;
		}
	}

	//if index [1][0]
	if (row == 1 && column == 0)
	{
		//counter = num of possible moves
		numOfMoves = 3;
		counter++;
		switch (counter)
		{
		case 1:
			//move up
			//zero location = location above it
			possibleMove[3] = workingState[0];

			//location above = 0
			possibleMove[0] = '0';

			break;

		case 2:
			//move right
			//zero location = location to the right
			possibleMove[3] = workingState[4];

			//location to the right = 0
			possibleMove[4] = '0';

			break;

		case 3:
			//move below
			//zero location = location beneath
			possibleMove[3] = workingState[6];

			//location beneath = 0
			possibleMove[6] = '0';

			break;
		}
	}

	//if index [1][1]
	if (row == 1 && column == 1)
	{
		//counter = num of possible moves
		numOfMoves = 4;
		counter++;
		switch (counter)
		{
		case 1:
			//move up
			//zero location = location above it
			possibleMove[4] = workingState[1];

			//location above = 0
			possibleMove[1] = '0';

			break;

		case 2:
			//move right
			//zero location = location to the right
			possibleMove[4] = workingState[5];

			//location to the right = 0
			possibleMove[5] = '0';

			break;

		case 3:
			//move below
			//zero location = location beneath
			possibleMove[4] = workingState[7];

			//location beneath = 0
			possibleMove[7] = '0';

			break;

		case 4:
			//move left
			//zero location = location to the left
			possibleMove[4] = workingState[3];

			//location to the left = 0;
			possibleMove[3] = '0';

			break;
		}
	}

	//if index [1][2]
	if (row == 1 && column == 2)
	{
		//counter = num of possible moves
		numOfMoves = 3;
		counter++;
		switch (counter)
		{
		case 1:
			//move up
			//zero location = location above
			possibleMove[5] = workingState[2];

			//location above = 0
			possibleMove[2] = '0';

			break;

		case 2:
			//move below
			//zero location = location beneath
			possibleMove[5] = workingState[8];

			//location beneath = 0
			possibleMove[8] = '0';

			break;

		case 3:
			//move left
			//zero location = location to the left
			possibleMove[5] = workingState[4];

			//location to the left = 0;
			possibleMove[4] = '0';

			break;
		}
	}

	//if index [2][0]
	if (row == 2 && column == 0)
	{
		//counter = num of possible moves
		numOfMoves = 2;
		counter++;
		switch (counter)
		{
		case 1:
			//move up
			//zero location = location above
			possibleMove[6] = workingState[3];

			//location above = 0
			possibleMove[3] = '0';

			break;

		case 2:
			//move right
			//zero location = location to the right
			possibleMove[6] = workingState[7];

			//location to the right = 0;
			possibleMove[7] = '0';

			break;
		}
	}

	//if index [2][1]
	if (row == 2 && column == 1)
	{
		//numOfPossibleMoves = num of possible moves
		numOfMoves = 3;
		counter++;;
		switch (counter)
		{
		case 1:
			//move up
			//zero location = location above
			possibleMove[7] = workingState[4];

			//location above = 0
			possibleMove[4] = '0';

			break;

		case 2:
			//move right
			//zero location = location to the right
			possibleMove[7] = workingState[8];

			//location to the right = 0
			possibleMove[8] = '0';

			break;

		case 3:
			//move left
			//zero location = location to the left
			possibleMove[7] = workingState[6];

			//location to the left = 0;
			possibleMove[6] = '0';

			break;
		}
	}

	//if index [2][2]
	if (row == 2 && column == 2)
	{
		//counter = num of possible moves
		numOfMoves = 2;
		counter++;
		switch (counter)
		{
		case 1:
			//move up
			//zero location = location above
			possibleMove[8] = workingState[5];

			//location above = 0
			possibleMove[5] = '0';

			break;

		case 2:
			//move left
			//zero location = location to the left
			possibleMove[8] = workingState[7];

			//location to the left = 0;
			possibleMove[7] = '0';

			break;
		}
	}
	return possibleMove;
}

//find zero location in state; sets row and column respectively
void findZero(int &row, int &column)
{
	//@ [0][0]
	if (workingState[0] == '0')
	{
		row = 0;
		column = 0;
	}
	//@ [0][1]
	if (workingState[1] == '0')
	{
		row = 0;
		column = 1;
	}
	//@ [0][2]
	if (workingState[2] == '0')
	{
		row = 0;
		column = 2;
	}
	//@ [1][0]
	if (workingState[3] == '0')
	{
		row = 1;
		column = 0;
	}
	//@ [1][1]
	if (workingState[4] == '0')
	{
		row = 1;
		column = 1;
	}
	//@ [1][2]
	if (workingState[5] == '0')
	{
		row = 1;
		column = 2;
	}
	//@ [2][0]
	if (workingState[6] == '0')
	{
		row = 2;
		column = 0;
	}
	//@ [2][1]
	if (workingState[7] == '0')
	{
		row = 2;
		column = 1;
	}
	//@ [2][2]
	if (workingState[8] == '0')
	{
		row = 2;
		column = 2;
	}
}

//function to return a random char based on the goal state
char returnRandNum(string &poolOfChars)
{
	//set value as a random char from the Goal State (123456780)
	char value = poolOfChars[rand() % (poolOfChars.length())];

	//delete char from the pool so that it is not selected again

	poolOfChars.erase(remove(poolOfChars.begin(), poolOfChars.end(), value), poolOfChars.end());
	//return the value
	return value;
}

node createNode(string currentState, int currentNodeCount, int currentDepth, list<string> &currentPathToNode)
{
	//create a new node
	node newNode;

	//node's state = passed in state
	newNode.state = currentState;

	//node's count = passed in count
	newNode.nodeCount = currentNodeCount;

	//node's depth = passed in depth
	newNode.depth = currentDepth;

	//num of missing tiles = return value from function
	newNode.numOfMisplacedTiles = (findNumOfMissingTiles(newNode.state) + currentDepth);

	//manhattan distance = return value from function
	newNode.manhattanDistance = (findManhattanDistance(newNode.state) + currentDepth);

	//node's path = passed in path
	newNode.pathToNode = currentPathToNode;

	//push back the current node into list
	newNode.pathToNode.push_back(newNode.state);
	//return new node
	return newNode;
}

//function to find num of missing tiles
int findNumOfMissingTiles(string state)
{
	//declare and define our var
	int numOfMissingTiles = 0;

	//loop through all indices of string
	for (int i = 0; i < 9; i++)
	{
		//if tiles is misplaced
		if (state[i] != GOAL_STATE[i])
		{
			//increment numOfMissingTiles
			numOfMissingTiles++;
		}
	}
	//return num of missing tiles
	return numOfMissingTiles;
}

//function to find the manhattan distance of the current state
int findManhattanDistance(string state)
{
	//int for manhattan distance value
	int manhattanDistance = 0;
	//string for our current state
	string currentState = state;
	//holds row of current state
	int currentRow = 0;
	//holds column of current state
	int currentColumn = 0;
	//holds row of goal state
	int goalRow = 0;
	//holds column of goal state
	int goalColumn = 0;

	//loop through string
	for (int i = 0; i < 9; i++)
	{
		//find row and column location of GOAL_STATE[i] in currentState and set row/column respectively
		getIndexOfValue(currentState, GOAL_STATE[i], currentRow, currentColumn);

		//find row and column location of GOAL_STATE[i] in GOAL_STATE and set row/column respectively
		getIndexOfValue(GOAL_STATE, GOAL_STATE[i], goalRow, goalColumn);

		//manhattan distance = sum of the total differnces in the rows and columns
		manhattanDistance += abs(currentRow - goalRow) + abs(currentColumn - goalColumn);
	}
	//return the manhattan distance
	return manhattanDistance;
}


//function to manipulate row and column to the index of the value passed in
void getIndexOfValue(string state, char value, int &row, int &column)
{
	//loop through string
	for (int i = 0; i < 9; i++)
	{
		//if current value in state == value passed in
		if (state[i] == value)
		{
			//switch on i
			switch (i)
			{
				//if string[0]
			case 0:

				//2D value = [0][0]
				row = 0;
				column = 0;
				break;

				//if string[1]
			case 1:

				//2D value = [0][1]
				row = 0;
				column = 1;
				break;

				//if string[2]
			case 2:

				//2D value = [0][2]
				row = 0;
				column = 2;
				break;

				//if string[3]
			case 3:

				//2D value = [1][0]
				row = 1;
				column = 0;
				break;

				//if string[4]
			case 4:

				//2D value = [1][1]
				row = 1;
				column = 1;
				break;

				//if string[5]
			case 5:

				//2D value = [1][2]
				row = 1;
				column = 2;
				break;

				//if string[6]
			case 6:

				//2D value = [2][0]
				row = 2;
				column = 0;
				break;

				//if string[7]
			case 7:

				//2D value = [2][1]
				row = 2;
				column = 1;
				break;

				//if string[8]
			case 8:

				//2D value = [2][2]
				row = 2;
				column = 2;
				break;
			}
		}
	}
}

//function to print the solution's path
void printPath()
{
	//move count = 0
	int moveCount = 0;

	//loop through list of moves 
	for (string i : tempList)
	{
		//if last move
		if (i == tempList.back())
		{
			//output goal
			cout << "     GOAL: ";
		}
		else if (i == tempList.front())
		{
			//start state
			cout << "    START: ";
			moveCount++;
		}

		//if not last move
		else
		{
			//output move number
			cout << "MOVE #" << setfill('0') << setw(3) << moveCount << ": ";
			moveCount++;
		}
		//output the move
		cout << i << endl;
	}
}

//function to print statistics for the search
void printStatistics(int nodeCountOfSolution, int depthOfSolution, double endTime)
{
	//set mins
	int mins = (endTime / 60);
	//subtract mins from endTime
	endTime = (endTime - (endTime / 60));
	//int secs = endtime
	int secs = endTime;
	//milliseconds = decimal portion of endTime * 1000
	int milliSeconds = (endTime - secs) * 1000;
	

	//print depth of solution
	cout << endl << "Depth Of Solution (Number Of Moves Made): ";
	cout << depthOfSolution << endl;

	//print num of nodes generated
	cout << "Number Of Nodes Generated: ";
	cout << nodeCountOfSolution << endl;

	//print time took to find a solution
	cout << "Time Elapsed: " << (int)mins << "m " << (int)secs << "s " << (int)milliSeconds << "ms " << endl;
}