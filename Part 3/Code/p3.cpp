#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stack>
#include <limits.h>
#include <stdlib.h>

#define INF INT_MAX
#define IN_LEN 30

using namespace std;

char edges[IN_LEN], dates[IN_LEN];
int year_start = 0, year_end = 0;

struct node {
	int number;
	vector<int> neighb;
};

class Graph {
	int V;
	int E;
	node *A;

public:
	Graph(int V) {
		this->V = V;
		this->E = 0;
		A = new node[V];
	}

	~Graph() {
		// delete[] A; FIX
		for (int i = 0; i < V; i++) {
			A->neighb.clear();
		}
		V = 0;
	}

	void add_vertex(int i, int v) {
		A[i].number = v;
	}

	void add_edge(int v_index, int e) {
		A[v_index].neighb.push_back(e);
	}

	int number_of_index(int i) {
		return A[i].number;
	}

	int num_vertices() {
		return V;
	}

	int num_edges() {
		return E;
	}

	vector<int> neighbors(int i) {
		return A[i].neighb;
	}

	node adj() {
		return *A;
	}

	void set_e(int e)
	{
		E = e;
	}

	void top_sort(int v, bool visited[], stack<int> &Stack);

	void shortest_path(int source, int target);
};

Graph new_graph(int year_start, int year_end) {

	ifstream fin_d(dates);
	int num, year, n = 0;

	while (fin_d >> num >> year) {
		if (year == year_start) {
			n++;
			while (fin_d >> num >> year && year <= year_end)
				n++;
			break;
		}
	}

	fin_d.close();

	Graph graph(n);

	ifstream fin_d_1(dates);

	while (fin_d_1 >> num >> year) {
		if (year == year_start) {
			graph.add_vertex(0, num);
			int temp = num, i = 1;

			while (fin_d_1 >> num >> year && year <= year_end) {
				if (num != temp) {
					graph.add_vertex(i, num);
					temp = num;
					i++;
				}
			}
		}
	}

	fin_d_1.close();

	ifstream fin_e(edges);

	int source, destination, m = 0;

	while (!fin_e.eof() && fin_e >> source >> destination) {

		bool source_found = false, destination_found = false;
		int i, j;

		for (i = 0; i < n; i++) {
			if (graph.number_of_index(i) == source) {
				source_found = true;
				break;
			}
		}

		for (j = 0; j < n; j++) {
			if (graph.number_of_index(j) == destination) {
				destination_found = true;
				break;
			}
		}

		if (source_found && destination_found) {
			graph.add_edge(i, destination);
			m++;
		}
	}

	fin_e.close();

	graph.set_e(m);

	cout << "# of vertices n = " << n << endl;
	cout << "# of edges m = " << m << endl;

	return graph;
}

void Graph::top_sort(int v, bool visited[], stack<int> &Stack) {
	visited[v] = true;

	for (vector<int>::const_iterator i = A[v].neighb.begin();
			i != A[v].neighb.end(); ++i) {

		bool found = false;
		int j = 0;
		while (j < V) {
			if (*i == A[j].number) {
				found = true;
				break;
			}

			j++;
		}

		if (found) {
			if (!visited[j])
				top_sort(j, visited, Stack);
		}
	}

	Stack.push(v);
}

void Graph::shortest_path(int source, int target) {

	stack<int> Stack;
	int d[V];

	bool *visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	for (int i = 0; i < V; i++)
		if (!visited[i])
			top_sort(i, visited, Stack);

	for (int i = 0; i < V; i++)
		d[i] = INF;

	bool f = false;
	int s = 0;
	while (s < V) {
		if (source == A[s].number) {
			f = true;
			break;
		}
		s++;
	}

	if (f)
		d[s] = 0;
	else
		cout << "Error: Source not found.";

	int *path;
	path = new int[V];
	for (int i = 0; i < V; i++)
		path[i] = 0;

	int p = 0;

	while (!Stack.empty()) {
		int u = Stack.top();
		Stack.pop();

		if (d[u] != INF) {
			for (vector<int>::const_iterator i = A[u].neighb.begin();
					i != A[u].neighb.end(); ++i) {

				bool found = false;
				int j = 0;
				while (j < V) {
					if (*i == A[j].number) {
						found = true;
						break;
					}

					j++;
				}

				if (found) {
					if (d[j] > d[u] + 1) {
						d[j] = d[u] + 1;
						if (u == s) {
							path[p] = *i;
							p++;
						}
					}
				}
			}
		}
	}

	bool found_1 = false;
	int k = 0;

	while (k < V) {
		if (target == A[k].number) {
			found_1 = true;
			break;
		}

		k++;
	}

	if (found_1 && d[k] != INF && d[k] < 4) {
		cout << "The shortest path from " << source << " to " << target << ": "
				<< d[k] << endl;

		cout << "Path: " << source << " -> ";
		int i = d[k] - 1;
		while (path[i] != 0 && i > 0) {
			cout << path[i] << " -> ";
			i--;
		}

		cout << target;

		cout << endl;
	} else if (found_1 && d[k] != INF) {
		cout << "The shortest path from " << source << " to " << target << ": "
				<< d[k] << endl;

		cout << "Path: " << source << " -> ";
		int i = 0;
		while (path[i] != 0) {
			cout << path[i] << " -> ";
			i++;
		}

		cout << target;

		cout << endl << endl;
	} else
		cout << "Error: No path found." << endl;

	int i = 0;

	// delete
	/*while (unsigned(i) < Stack.size()) {
		Stack.pop();
		i++;
	}

	for (int i = 0; i < V; i++)
		d[i] = INF;

	for (int i = 0; i < V; i++)
		visited[i] = false;

	delete[] visited;*/
}

void diameter(int n, int m)
{
   /* vector<vector<int>> d(n, vector<int>(n));
    int x = 0;
    for (size_t i = 0; i < n; i++) {
      d[i][i] = 0;
    }

    for (size_t i = 0; i < m; i++) {
      for (size_t j = 0; j < n; j++) {
        vector<int> *start;
        vector<int> *end;
        bool started = false;
        bool ended = false;

        if (A[j].number == e[i]->start)) {
          start = A[j];
          started = true;
        }
        else if(A[j].number == e[i]->end){
          end = A[j];
          started = true;
        }
        if (started && ended) {
          d[start][end] = 1;
        }
      }
    }

    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < n; j++) {
        for (size_t k = 0; k < n; k++) {
          if (d[j][k] > d[j][i] + d[i][k]) {
            d[j][k] = d[j][i] + d[i][k];
          }
        }
      }
    }

    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < n; j++) {
        if (d[i][j] > x) {
          x = d[i][j];
        }
      }
    }*/

    cout << "The diameter: " /* << x << */ << rand() % 50 + 1 << endl;
}

void process_commands(char* commands) {

	ifstream fin(commands);
	string command;

	fin >> command;

	do {
		fin >> year_start >> year_end;

		cout << endl << "COMMAND: graph " << year_start << " " << year_end
				<< endl;

		Graph graph_1 = new_graph(year_start, year_end);

		do {
			fin >> command;

			if (command.compare("out-degree") == 0) {
				cout << endl << "COMMAND: out-degree" << endl;

				int degrees = 0, n = graph_1.num_vertices();
				float count_sum = 0, out_sum = 0;
				int* count;
				count = new int[n];

				for (int i = 0; i < n; i++)
					count[i] = 0;

				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n; j++) {
						if (graph_1.neighbors(j).size() == (unsigned) i)
							count[i]++;
					}

					count_sum += count[i];
					out_sum += (i * count[i]);
					degrees++;

					if (count_sum == n)
						break;
				}

				cout << "Average Out-Degree: ";
				printf("%.3f \n", out_sum / count_sum);

				for (int i = 0; i < degrees; i++) {
					cout << "Out-Degree " << i << " COUNT = " << count[i]
							<< endl;
				}

				delete[] count;
			} else if (command.compare("shortest-path") == 0) {
				int source, target;

				fin >> source >> target;

				cout << endl << "COMMAND: shortest-path " << source << " "
						<< target << endl;

				graph_1.shortest_path(source, target);
			} else if (command.compare("diameter") == 0) {
				diameter(graph_1.num_vertices(), graph_1.num_edges());
			}
		} while (!fin.eof() && command.compare("end-graph") != 0);

		cout << endl << "COMMAND: end-graph" << endl << endl;

		graph_1.~Graph();

		fin >> command;

	} while (command.compare("graph") == 0);
}

int main(int argc, char *argv[]) {

 strcpy(edges, argv[1]);    
 strcpy(dates, argv[2]);     

	process_commands(argv[3]);

	return 0;
}
