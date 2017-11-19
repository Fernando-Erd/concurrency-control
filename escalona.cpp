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

int generateGraph(vector<t_operation> &transactionList,mn &graph){
    //first pass
	int nschedule = 0,atime,ntransaction = 0;
	char aop;
	for (int i = 0; i < (int) transactionList.size(); i++) {
		if(ntransaction == 0){
			nschedule++;
		}
		if(graph.find(transactionList[i].id) == graph.end()){
			t_node node;
			node.color = 0;
			node.schedule = nschedule;
			graph[transactionList[i].id] = node;
			ntransaction++;
		}
		if(transactionList[i].operation == 'C'){
			ntransaction--;
		}
	}

	for(int i=0;i<(int)transactionList.size();i++){
		aop = transactionList[i].operation;
		int aatrib = transactionList[i].atribute;
		if(aop == 'W'){
			for(int j=i+1;(j<(int)transactionList.size()) && ((transactionList[i].id != transactionList[j].id) || (transactionList[j].operation != 'C'));j++){
				if((transactionList[i].id != transactionList[j].id) && (transactionList[j].operation == 'R') && (transactionList[j].atribute == aatrib)){ // write i before read j
					graph[transactionList[i].id].neighbors.push_back(transactionList[j].id);
				}else if((transactionList[i].id != transactionList[j].id) && (transactionList[j].operation == 'W') && (transactionList[j].atribute == aatrib)){ // write i before write j
					graph[transactionList[i].id].neighbors.push_back(transactionList[j].id);
				}
			}
		}
		if(aop == 'R'){ // read i before write j
			for(int j=i+1;(j<(int)transactionList.size()) && ((transactionList[i].id != transactionList[j].id) || (transactionList[j].operation != 'C'));j++){
				if((transactionList[i].id != transactionList[j].id) && (transactionList[j].operation == 'W') && (transactionList[j].atribute == aatrib)){
					graph[transactionList[i].id].neighbors.push_back(transactionList[j].id);
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

void readInput (vector<t_operation> *transactionList) {
    t_operation t;
    while (scanf("%d %d %c %c", &t.time, &t.id, &t.operation, &t.atribute) != EOF) {
        transactionList->push_back(t);
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

int case1(vector<char> &atributes_list, vector<t_operation> &transactionList, vector<int> &active, map<int,vector<t_operation> > &map_transactions) {
    vector <int>::iterator it;
    vector <int>transactions_read;
    vector <t_operation>serial;
    for (int i =0; i <(int) atributes_list.size(); i++) {
        //transactions read init
        for (int j= 0; j <(int) transactionList.size(); j++) {
            it = find (active.begin(), active.end(), transactionList[j].id);
            if (it != active.end() && transactionList[j].atribute == atributes_list[i]) {
                if (transactionList[j].operation == 'R') {
                    transactions_read.push_back(transactionList[j].id);
                }
                else if (transactionList[j].operation == 'W') {
                    break;
                }
            }
        }
        //check map
        for (int j=0;j<(int)active.size(); j++) {
            serial = map_transactions[active[j]];
            for (int k=0; k<(int)serial.size(); k++) {
                if (serial[k].operation == 'W' && serial[k].atribute == atributes_list[i]) {
                    if ((int)transactions_read.size() > 1) return 0;
                }
            }
        }
        transactions_read.clear();
    }
    return 1;

}

int case2(vector<char> &atributes_list, vector<t_operation> &transactionList, vector<int> &active, map<int,vector<t_operation> > &map_transactions) {
    vector <int>::iterator it;
    vector <t_operation>serial;
    int write_time, read_time;
    map <int,vector <int> >depedency_time;
    int write_atribute = 0;
    for (int i =0; i <(int) atributes_list.size(); i++) {
				//search depedency write
        for (int j= 0; j <(int) transactionList.size(); j++) {
            it = find (active.begin(), active.end(), transactionList[j].id);
            if (it != active.end() && transactionList[j].atribute == atributes_list[i]) {
                if (transactionList[j].operation == 'W' && write_atribute == 1) {
                    write_atribute = 0;
                }
                if (transactionList[j].operation == 'W' && write_atribute == 0) {
                    write_atribute = 1;
                    write_time = transactionList[j].time;
                }
                if (transactionList[j].operation == 'R' && write_atribute == 1) {
                    depedency_time[write_time].push_back(transactionList[j].time);
                }
            }
        }
        write_atribute = 0;
        for(map<int, vector <int> >::const_iterator it_map = depedency_time.begin(); it_map != depedency_time.end(); it_map++) {
            for (int j=0; j < (int)it_map->second.size(); j++) { //loop vector de depedentes
                for (int k=0;k<(int)active.size(); k++) { //loop de transacoes id
                int exit_loop = 0;
                serial = map_transactions[active[k]];
                    for (int l=0; l<(int)serial.size(); l++) { //loop de transacoes operation
                        if (serial[l].time == it_map->first) { //se achei a transacao chave do map e n achei nenhum depedente acima tá ok
                            exit_loop = 1;
                            break;
                        }
                        if (serial[l].time == it_map->second[j]) {
                            return 0;
                        }
                    }
                    if (exit_loop == 1) break;
                }
            }
        }
        depedency_time.clear();
    }
    return 1;
}

int case3(vector<char> &atributes_list, vector<t_operation> &transactionList, vector<int> &active, map<int,vector<t_operation> > &map_transactions) {
    vector <int>::iterator it;
    int transaction_id_last_write;
    int transaction_id_last_write_serial = 0;
    vector <t_operation>serial;

    for (int i =0; i <(int) atributes_list.size(); i++) {
        //search the last id for atribute
        for (int j= 0; j <(int) transactionList.size(); j++) {
            it = find (active.begin(), active.end(), transactionList[j].id);
            if (it != active.end() && transactionList[j].atribute == atributes_list[i]) {
                if (transactionList[j].operation == 'W') {
                    transaction_id_last_write = transactionList[j].id;
                }
            }
        }
        //verify permute in map
        if (transaction_id_last_write != 0) {
            for (int j= (int)active.size() - 1;j >= 0 ; j--) {
                serial = map_transactions[active[j]];
                for (int k=(int)serial.size() - 1; k >= 0; k--) {
                    if(serial[k].operation == 'W' && serial[k].atribute == atributes_list[i] && transaction_id_last_write_serial == 0) {
                        transaction_id_last_write_serial = serial[k].id;
                        if (transaction_id_last_write != serial[k].id) return 0;
                    }
                }
            }
         }
    }
    return 1;
}

int vision(vector<t_operation> &transactionList, vector<int> &active) {
    map<int,vector<t_operation> > map_transactions;
    vector <int>::iterator it;
    vector <char>::iterator it_char;
    vector <char> atributes_list;
    int value_init = 0;
    //generate map
    for (int i= 0; i <(int) transactionList.size(); i++) {
        it = find (active.begin(), active.end(), transactionList[i].id);
        if (it != active.end() && transactionList[i].operation != 'C') {
            map_transactions[transactionList[i].id].push_back(transactionList[i]);
            it_char = find (atributes_list.begin(), atributes_list.end(), transactionList[i].atribute);
            if (it_char == atributes_list.end() && transactionList[i].operation != 'C')
                atributes_list.push_back(transactionList[i].atribute);
        }
    }

    sort(active.begin(), active.begin() + active.size());
    do {
        if(case1(atributes_list, transactionList, active, map_transactions) == 1 && case2(atributes_list, transactionList, active, map_transactions) == 1 &&case3(atributes_list, transactionList, active, map_transactions)) return 1;
    } while (next_permutation(active.begin(),active.begin() + active.size()));
    return 0;
}

int main () {
    int nschedule;
    vector <t_operation> transactionList;
    vector <int> transaction_list;
    mn graph;
    readInput(&transactionList);
    nschedule = generateGraph(transactionList,graph);
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
	for(int j=0;j<(int) transaction_list.size()-1;j++){
	    printf("%d,",transaction_list[j]);
	}
	if((int) transaction_list.size()) printf("%d ",transaction_list[transaction_list.size()-1]);
	if(answer) printf("SS ");
	else printf("NS ");
        if (vision(transactionList, transaction_list)) printf("SV");
        else printf("NV");
	transaction_list.clear();
	printf("\n");
    }
}
