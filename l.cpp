#include <vector>
#include <map>
#include <stdio.h>

using namespace std;

typedef vector<int> vi;

typedef struct {
	int color; //0 - not visited, 1 - visited, 2 - visiting
	vi neighbors;
} t_node;

typedef map<int,t_node> mn;

/*typedef struct {
    vvi adj;
    vector <t_node> node;
} t_graph;
*/

typedef struct {
    int time;
    int id;
    char operation;
    char atribute;
} t_operation;

void generateGraph(vector<t_operation> &transitionList,mn *graph){
    //first pass
	
	for (int i = 0; i < (int) transitionList.size(); i++) {
		if(graph->find(transitionList[i].id) == graph->end()){
			t_node node;
			node.color = 0;
			graph->insert(make_pair(transitionList[i].id,node));
		}		
	}

	for(i=0;i<((int)transitionList.size()) && (transitionList[i].operation != 'C');i++){	
		
		for(j=0;j<(int)transitionList.size();j++){
			
		}
	}
	
};
//void addEdge();
//void addVertex();
//void searchCircle();
//void equivalence(vector<transition> &transitionList, int transitionsNumbers) {
//}

void readInput (vector<t_operation> *transitionList) {
    t_operation t;
    while (scanf("%d %d %c %c", &t.time, &t.id, &t.operation, &t.atribute) != EOF) {
        transitionList->push_back(t);
    }
}

void printInput (vector<t_operation> &transitionList) {
    for (int i = 0; i < (int) transitionList.size(); i++) {
          printf("%d %d %c %c\n", transitionList[i].time, transitionList[i].id, transitionList[i].operation, transitionList[i].atribute);
    }
}
void printGraph (mn &graph) {
    for (mn::iterator it = graph.begin(); it != graph.end(); ++it) {
        printf("%d -> c %d\n (",it->first,it->second.color);
		for (int i = 0; i < (((int) it->second.neighbors.size())-1); i++) {
			printf("%d ",it->second.neighbors[i]);
		}
		printf(")\n");
    }
}

int main () {
    vector <t_operation> transitionList;
	mn graph;
    readInput(&transitionList);
    printInput(transitionList);
	generateGraph(transitionList,&graph);
	printGraph(graph);
}

