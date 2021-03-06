#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <time.h>


using namespace std;

struct Vertex {
	string name;
	bool blocked;
	double f; // DBL_MAX if not filled in yet
	double g;
	double h;
	int XCord;
	int YCord;
	int distance;
	Vertex * parent;
	vector<Vertex *> neighbours;//make vector, next to each other in memory 
	vector<double> distances;
	Vertex(string name_, int XCord_, int YCord_) : name(name_), blocked(false), distance(-1), f(DBL_MAX), g(DBL_MAX), h(DBL_MAX), XCord(XCord_), YCord(YCord_), parent(nullptr) {
		//can run code
	}
	/*
	bool operator< (const Vertex& other) const
	{
	return other.f > f;
	}
	*/
};

double findDistance(int XCordOne, int XCordTwo, int YCordOne, int YCordTwo)
{
	int x = abs(XCordOne - XCordTwo);
	int y = abs(YCordOne - YCordTwo);

	return sqrt((x*x) + (y*y));
}

void connect(Vertex& a, Vertex& b) {

	double result = findDistance(a.XCord, b.XCord, a.YCord, b.YCord);

	a.neighbours.push_back(&b);//passing by reference (passing memory value)
	a.distances.push_back(result);
	b.neighbours.push_back(&a);
	b.distances.push_back(result);

}

void otherDisplay(vector<Vertex *>& graph) {
	for (auto v : graph) {
		//cout << v->name << ": " << v->f << endl;
		//cout << "test" << endl;
		/*
		for (auto x : v->neighbours)
		{
		cout << x->distances << endl;
		}
		*/
		for (int i = 0; i < v->distances.size(); i++)
		{
			cout << v->name << " to " << v->neighbours[i]->name << ": " << v->distances[i] << endl;
		}
		cout << "--------" << endl;
	}
}

vector<Vertex *> makeGraph(int scale) {

	/*
	Vertex* va = new Vertex("A", 5, 5);//name, xcord, ycord
	Vertex* vb = new Vertex("B", 1, 1);
	Vertex* vc = new Vertex("C", 2, 5);
	Vertex* vd = new Vertex("D", 5, 1);
	connect(*va, *vb);
	connect(*vb, *vc);
	connect(*va, *vd);
	connect(*vd, *vc);
	//vd->blocked = true;
	vector<Vertex *> graph{ va, vb, vc, vd };
	//otherDisplay(graph);
	*/

	vector<Vertex *> randGraph;
	for (int i = 0; i < scale; i++)
	{
		int randX = (rand() % scale);
		int randY = (rand() % scale);

		randGraph.push_back(new Vertex(to_string(i), randX, randY));
	}

	for (auto item : randGraph)
	{
		for (auto itemTwo : randGraph)
		{
			int chance = (rand() % 100);
			if (chance < 1)//80% chance to connect
			{
				if (item != itemTwo)
				{
					connect(*item, *itemTwo);
				}
			}
		}

		int chance = (rand() % 100);
		if (chance < 1)//5% chance to block
		{
			item->blocked = true;
		}
	}




	



	return randGraph;
}

Vertex * findLowestF(set<Vertex *> open)
{
	double min_f = DBL_MAX;
	Vertex *lowf = nullptr;
	for (Vertex *v : open)
	{
		if (v->f < min_f)
		{
			min_f = v->f;
			lowf = v;
		}
	}
	return lowf;
}

list<Vertex *> findPathAStar(vector<Vertex *> &allVertex, Vertex &start, Vertex &end)//graph[3] at calling then Vertex *end then use end->name
{
	//cout << end.name << endl;

	start.blocked = false;//just making sure
	start.g = 0;//make number of vertexs moved
	start.h = findDistance(start.XCord, end.XCord, start.YCord, end.YCord);
	start.f = start.g + start.h;

	set<Vertex *> open;//storing pointers so cant use operator<, may use vextor instead, set not garinteed to store values next to each
	open.insert(&start);//other in memory so may be slower need to compare

	set<Vertex *> closed;


	while (!open.empty())
	{
		Vertex* current = findLowestF(open);
		if (current == &end)
		{

			list<Vertex *> path;


			Vertex* node = &end;

			while (node != &start)
			{
				path.push_back(node);
				node = node->parent;
			}
			path.push_back(&start);//adding start to final path vector


			return path;
		}

		open.erase(current);
		closed.insert(current);

		for (auto neighbour : current->neighbours)
		{
			if (closed.find(neighbour) != closed.end() || neighbour->blocked == true)
			{
				continue;
			}

			double new_g = current->g + findDistance(current->XCord, neighbour->XCord, current->YCord, neighbour->YCord);

			if (open.find(neighbour) == open.end())//if node not in open (not seen this node before)
			{
				open.insert(neighbour);
			}
			else if (new_g >= neighbour->g)
			{
				continue;
			}

			neighbour->g = new_g;
			neighbour->f = neighbour->g + findDistance(end.XCord, neighbour->XCord, end.YCord, neighbour->YCord);
			neighbour->parent = current;

			//const bool is_in = container.find(element) != container.end();
		}

	}

	list<Vertex *> path;
	cout << "No path found" << endl;

	return path;

}

void display(vector<Vertex *>& graph) {
	for (auto v : graph) {
		cout << v->name << ": " << v->distance << endl;
	}
}

void flood(vector<Vertex*> &allVertex, Vertex &start, Vertex &end)
{

	int num = 0;
	bool run = true;

	start.distance = 0;
	while (run)
	{
		for (auto i : allVertex) {//not totally confident in flood. what if passing vertex that not got a distance that would have if it came later when one of its neghbours got it a distance
			if (i->distance == num)
			{
				for (auto neighbour : i->neighbours)
					if (neighbour->distance == -1)
					{
						if (neighbour->blocked == false)
						{
							neighbour->distance = num + 1;
						}
						else
						{
							neighbour->distance = -2;//-2 means blocked
						}

						//cout << neighbour << " " << &end << endl;

						if (neighbour == &end)
						{
							run = false;
							return;
						}
					}
			}
			

			//display(allVertex);//uncomment both to see flooding process
			//cout << endl;
		}
		num += 1;
	}
}

list<Vertex *> findPathLee(vector<Vertex*> &allVertex, Vertex &start, Vertex &end)
{
	int num = 1;
	list<Vertex *> path;
	path.push_back(&end);
	Vertex current = end;

	while (true)
	{
		for (auto i : current.neighbours)
		{
			if (i->distance == current.distance - 1)
			{
				path.push_front(i);
				current = *i;//dont change current while for loop is using it without break to reset for loop
				break;
			}
		}
		if (current.distance == 0)
		{
			break;
		}
	}

	//for (auto i : path) {
		//cout << i << endl;
	//}
	return path;
}


int main()
{
	srand(time(NULL));
	int scale = 10000;
	vector<Vertex *> graph = makeGraph(scale);

	int startNum = (rand() % scale);
	int endNum = (rand() % scale);


	while (startNum == endNum)
	{
		endNum = (rand() % scale);
	}





	Vertex* start = graph[startNum];
	Vertex* end = graph[endNum];

	cout << start->name << endl;
	cout << end->name << endl;



	//otherDisplay(graph);
	
	//cout << graph[0]->name << endl;
	//list<Vertex *> pathAStar = findPathAStar(graph, *graph[0], *graph[2]);//graph, start and end, graph[3] is blocked
	//graph[3]

																//g is distance currently taken block by block.

	list<Vertex *> pathAStar = findPathAStar(graph, *start, *end);
	//cout << "fgrggrgr";
	pathAStar.reverse();

	cout << "A* Path: " << endl;
	cout << pathAStar.size() << endl;
	for (auto item : pathAStar)
	{
		cout << "Name: " << item->name << endl;
	}

	//for (int i = 0; i < pathAStar.size(); i++)
	//{
		//double totalDistance = 0;
		//totalDistance += findDistance(pathAStar[i]->XCord, path);
	//}
	
	double totalDistance = 0;
	list<Vertex *>::iterator it;
	list<Vertex *>::iterator itNext = pathAStar.begin();
	for (it = pathAStar.begin(); it != pathAStar.end(); ++it) {
		++itNext;
		totalDistance += findDistance((*it)->XCord, (*itNext)->XCord, (*it)->YCord, (*itNext)->YCord);
		if ((*itNext)->name == end->name)
		{
			break;
		}
	}
	cout << "AStar Distance Traveled: " << totalDistance << endl;



	flood(graph, *start, *end);
	//display(graph);

	//list<Vertex *> pathLee = findPathLee(graph, *graph[0], *graph[2]);
	list<Vertex *> pathLee = findPathLee(graph, *start, *end);

	cout << "Lee Path:" << endl;
	for (auto item : pathLee)
	{
		cout << "Name: " << item->name << endl;
	}

	double totalDistanceTwo = 0;
	list<Vertex *>::iterator itTwo;
	list<Vertex *>::iterator itNextTwo = pathLee.begin();
	for (itTwo = pathLee.begin(); itTwo != pathLee.end(); ++itTwo) {
		++itNextTwo;
		totalDistanceTwo += findDistance((*itTwo)->XCord, (*itNextTwo)->XCord, (*itTwo)->YCord, (*itNextTwo)->YCord);
		if ((*itNextTwo)->name == end->name)
		{
			break;
		}
	}
	cout << "Lee Distance Traveled: " << totalDistanceTwo << endl;




	system("pause");

	return 0;
}

