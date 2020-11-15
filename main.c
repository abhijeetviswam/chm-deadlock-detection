#include <stdio.h>

void printGraph(int *matrix, int size) {
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
            printf("%d\t",matrix[i*size + j]);
        }
        printf("\n");
    }
}


int nProcs; //Number of processes

void main() {
    int probe_pid;
    
    printf("Enter the number of processes : ");
    scanf("%d",&nProcs);

    if(nProcs > 1) {
        int waitGraph[nProcs][nProcs];
        // int i,j;

        printf("Input the wait graph : \n\n");
        for (int i=0;i<nProcs;i++){
            for(int j=0;j<nProcs;j++){
                int tmp;
                printf("From process P%d to P%d : ",i+1,j+1);
                scanf("%d",&waitGraph[i][j]);
            }
        }

        printf("Input wait graph is : \n\n");
        printGraph(&waitGraph,nProcs);
    }
    else {
        printf("No deadlock detected.\n");
    }
}