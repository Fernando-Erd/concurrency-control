#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;

typedef vector<int> vi;

typedef struct {
	int color; //0 - not visited, 1 - visited, 2 - visiting
	int schedule;
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

int generateGraph(vector<t_operation> &transitionList,mn &graph){
    //first pass
	int nschedule = 0,atime,ntransition = 0;
	char aop;
	for (int i = 0; i < (int) transitionList.size(); i++) {
		if(ntransition == 0){
			nschedule++;
		}
		if(graph.find(transitionList[i].id) == graph.end()){
			t_node node;
			node.color = 0;
			node.schedule = nschedule;
			graph[transitionList[i].id] = node;
			ntransition++;
		}
		if(transitionList[i].operation == 'C'){
			ntransition--;
		}
	}

	for(int i=0;i<(int)transitionList.size();i++){	
		aop = transitionList[i].operation;
		int aatrib = transitionList[i].atribute;
		if(aop == 'W'){ 
			for(int j=i+1;(j<(int)transitionList.size()) && ((transitionList[i].id != transitionList[j].id) || (transitionList[j].operation != 'C'));j++){
				if((transitionList[i].id != transitionList[j].id) && (transitionList[j].operation == 'R') && (transitionList[j].atribute == aatrib)){ // write i before read j
					graph[transitionList[i].id].neighbors.push_back(transitionList[j].id);
				}else if((transitionList[i].id != transitionList[j].id) && (transitionList[j].operation == 'W') && (transitionList[j].atribute == aatrib)){ // write i before write j
					graph[transitionList[i].id].neighbors.push_back(transitionList[j].id);
				}
			}
		}
		if(aop == 'R'){ // read i before write j
			for(int j=i+1;(j<(int)transitionList.size()) && ((transitionList[i].id != transitionList[j].id) || (transitionList[j].operation != 'C'));j++){
				if((transitionList[i].id != transitionList[j].id) && (transitionList[j].operation == 'W') && (transitionList[j].atribute == aatrib)){
					graph[transitionList[i].id].neighbors.push_back(transitionList[j].id);
				}
			}
		}
	}
	return nschedule;	
}

int dfs(mn &graph,int i){
	if(!graph[i].color)
		graph[i].color=2;
	else if(graph[i].color==1)
		return 1;
	else if(graph[i].color==2)
		return 0;
	for(int j=0;j<(int) graph[i].neighbors.size();j++){
		if(!dfs(graph,graph[i].neighbors[j]))
			return 0;
	}
	graph[i].color=1;
	return 1;
}

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
	int i;
    for (mn::iterator it = graph.begin(); it != graph.end(); ++it) {
        printf("%d -> sc %d\n (",it->first,it->second.schedule);
		for (i = 0; i < (((int) it->second.neighbors.size())-1); i++) {
			printf("%d ",it->second.neighbors[i]);
		}
		if((int) it->second.neighbors.size()) printf("%d)\n",it->second.neighbors[i]);
		else printf(")\n");
    }
}

//transicoes q leram o valor inicial
int case1(vector<char> &atributes_list, vector<t_operation> &transitionList, vector<int> &active, map<int,vector<t_operation> > &map_transitions) {
    vector <int>::iterator it;
    vector <int>transitions_read;
    vector <t_operation>serial;
    for (int i =0; i <(int) atributes_list.size(); i++) {
        //procura transicoes q le variavel inicial
        for (int j= 0; j <(int) transitionList.size(); j++) {
            it = find (active.begin(), active.end(), transitionList[j].id);
            if (it != active.end() && transitionList[j].atribute == atributes_list[i]) {
                if (transitionList[j].operation == 'R') {
                    transitions_read.push_back(transitionList[j].id);
                }
                else if (transitionList[j].operation == 'W') {
                    break;
                }
            }
        }
        //printf ("Transicoes q leram inicial %c: ", atributes_list[i]);
        //for (int j=0;j<(int)transitions_read.size(); j++)
        //    printf ("%d", transitions_read[j]);
        //printf("\n");
        //checa no map
        for (int j=0;j<(int)active.size(); j++) {
            serial = map_transitions[active[j]];
            for (int k=0; k<(int)serial.size(); k++) {
                printf("%d %d %c %c\n", serial[k].time, serial[k].id, serial[k].operation, serial[k].atribute);
                if (serial[k].operation == 'W' && serial[k].atribute == atributes_list[i]) {
                    printf("%d escreveu %c\n", serial[k].id, atributes_list[i]);
                    if ((int)transitions_read.size() > 1) return 0;
                }
            }
        }
        
        transitions_read.clear();
        printf("\n");
    }
    return 1;

}

void vision(vector<t_operation> &transitionList, vector<int> &active) {
    map<int,vector<t_operation> > map_transitions;
    vector <int>::iterator it;
    vector <char>::iterator it_char;
    vector <char> atributes_list;
    vector <int>read_init_value; //transicoes q leram valor inicial, caso 1
    int value_init = 0;
    //generate map
    for (int i= 0; i <(int) transitionList.size(); i++) {
        it = find (active.begin(), active.end(), transitionList[i].id);
        if (it != active.end() && transitionList[i].operation != 'C') {
            map_transitions[transitionList[i].id].push_back(transitionList[i]);
            it_char = find (atributes_list.begin(), atributes_list.end(), transitionList[i].atribute);
            if (it_char == atributes_list.end() && transitionList[i].operation != 'C')
                atributes_list.push_back(transitionList[i].atribute);
        }
    }

    printf ("\nVariaveis: ");
    for (int i=0;i<(int)atributes_list.size(); i++)
        printf ("%c ", atributes_list[i]);
    //generate permute and brute force
    printf("\n");
    //sort(active.begin(), active.end());
    //do {
    //    printf("Permutação: %d %d\n", active[0], active[1]);
        if(case1(atributes_list, transitionList, active, map_transitions) == 1) printf("Caso de Leitura Inicial é serializavel\n");
        else printf("Caso de Leitura Inicial não é serializavel\n");
        
    //} while (next_permutation(active.begin(),active.end())); 
}

int main () {
    int nschedule;
	vector <t_operation> transitionList;
	vector <int> transaction_list;
	mn graph;
    readInput(&transitionList);
    //printInput(transitionList);
	nschedule = generateGraph(transitionList,graph);
	//printGraph(graph);
	for(int i=1;i<=nschedule;i++){
		int answer = 1;
		printf("%d ",i);
		for(mn::iterator it = graph.begin(); it != graph.end(); ++it){
			if(it->second.schedule == i){
				if(answer && (it->second.color == 0)){
					answer = dfs(graph,it->first);	
				}
				transaction_list.push_back(it->first);
			}
		}
                vision(transitionList, transaction_list);
		for(int j=0;j<(int) transaction_list.size()-1;j++){
			printf("%d,",transaction_list[j]);
		}
		if((int) transaction_list.size()) printf("%d ",transaction_list[transaction_list.size()-1]);
		if(answer) printf("SS ");
		else printf("NS ");
		transaction_list.clear();
		printf("\n");
	}
	//printGraph(graph);
}

