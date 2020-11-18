#include <stdio.h>

struct Proc
{
    int num;
    int wait;
    int dep;
};


void printGraph(struct Proc *matrix, int size) {
    printf("TO ->\t");
    for (int i = 0; i < size; i++)
    {
        printf("P%d\t",(i+1));
    }
    printf("\nFROM\n");
    for (int i = 0; i < size; i++)
    {
        printf("P%d\t", (i+1));
        for (int j = 0; j < size; j++)
        {
            printf("%d\t",matrix[i*size + j].dep);
        }
        printf("\n");
    }
}

int nProcs; //Number of Processes

void main() {
    int probeProcess;
    
    printf("Enter the number of processes : ");
    scanf("%d",&nProcs);

    if(nProcs > 1) {
        struct Proc waitGraph[nProcs][nProcs];
        // int i,j;

        printf("Input the wait graph : \n\n");
        for (int i=0;i<nProcs;i++){
            for(int j=0;j<nProcs;j++){
                int tmp;
                printf("From process P%d to P%d : ",i+1,j+1);
                scanf("%d",&waitGraph[i][j].dep);
            }
        }

        printf("Wait-for Graph matrix \n\n");
        printGraph(&waitGraph,nProcs);

        printf("Initiator process : P");
        scanf("%d",&probeProcess);

    }
    else {
        printf("No deadlock detected.\n");
    }
}