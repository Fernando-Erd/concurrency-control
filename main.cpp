#include <vector>
#include <stdio.h>

using namespace std;

typedef struct {
    int time;
    int id;
    char operation;
    char atribute;
} transition;

void readInput (vector<transition> *transitionList) {
    transition t;
    scanf("%d %d %c %c", &t.time, &t.id, &t.operation, &t.atribute);
    while (t.time != 0) {
        transitionList->push_back(t);
        scanf("%d %d %c %c", &t.time, &t.id, &t.operation, &t.atribute);
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

