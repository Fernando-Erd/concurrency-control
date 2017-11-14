#include <vector>
#include <stdio.h>

using namespace std;

typedef vector<int> vi;

typedef vector<vi> vvi;

typedef struct {
    vvi adj;
    vector <t_node> node;
} t_graph;

typedef struct {
    int id;
		int color;
} t_node;

typedef struct {
    int time;
    int id;
    char operation;
    char atribute;
} t_operation;

void inicGraph();
void addEdge();
void addVertex();
void searchCircle();
void equivalence(vector<transition> &transitionList, int transitionsNumbers) {
}

void readInput (vector<transition> *transitionList) {
    transition t;
       while (scanf("%d %d %c %c", &t.time, &t.id, &t.operation, &t.atribute) != EOF) {
        transitionList->push_back(t);
    }
}

void printInput (vector<transition> &transitionList) {
    for (int i = 0; i < transitionList.size(); i++) {
          printf("%d %d %c %c\n", transitionList[i].time, transitionList[i].id, transitionList[i].operation, transitionList[i].atribute);
    }
}

int main () {
    vector <transition> transitionList;
    readInput(&transitionList);
    printInput(transitionList);
}

