#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define TRUE 1
#define FALSE 0

class Proc
{
public:
    int num;
    int wait;
    int totDepProcs;
    int engagingProc;
    int depProc[50];        //Using dynamic size was causing issues. Set max 50 dependencies per process

    Proc()
    {
        num = 0;
        wait = FALSE;
        totDepProcs = 0;
        engagingProc = -1;
    }
};

struct queryArgs
{
    Proc *procArray;
    int init;
    int fromProc;
    int toProc;
};

void printGraph(int *matrix, int size)
{
    printf("TO ->\t");
    for (int i = 0; i < size; i++)
    {
        printf("P%d\t", i);
    }
    printf("\nFROM\n");
    for (int i = 0; i < size; i++)
    {
        printf("P%d\t", i);
        for (int j = 0; j < size; j++)
        {
            printf("%d\t", matrix[i * size + j]);
        }
        printf("\n");
    }
}

void printDependence(Proc *process, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("P%d\t-> ", i);
        for (int j = 0; j < process[i].totDepProcs; j++)
        {
            printf("P%d ", process[i].depProc[j]);
        }
        // printf("\n\tengaging process : P%d",process[i].engagingProc);
        printf("\n");
    }
}

void *reply(void *ptr) {
    struct queryArgs *replyIn;
    replyIn = (queryArgs *)ptr;
    replyIn->procArray[replyIn->toProc].num--;

    printf("REPLY : (%d,%d,%d) - num :%d\n",replyIn->init,replyIn->fromProc,replyIn->toProc,replyIn->procArray[replyIn->toProc].num);
    // printf("replyIn num : %d\n",replyIn->procArray[replyIn->toProc].num);
    if(replyIn->procArray[replyIn->toProc].num == 0) {
        if (replyIn->init == replyIn->toProc)
        {
            printf("DEADLOCK\n");
        }
        else
        {
            struct queryArgs *replyOut;
            replyOut = new queryArgs;
            replyOut->procArray=replyIn->procArray;
            replyOut->init=replyIn->init;
            replyOut->fromProc=replyIn->toProc;
            replyOut->toProc=replyOut->procArray[replyOut->fromProc].engagingProc;

            // printf("--------replyReply : ");
            usleep(1000000);
            reply((void*)replyOut);
        }
    }
    // else
    // {
    //     replyIn->procArray[replyIn->toProc].num--;
    // }
    
    return 0;

}

void *query(void *ptr)
{
    struct queryArgs *queryIn;
    queryIn = (queryArgs *)ptr;

    printf("QUERY : (%d,%d,%d)\n", queryIn->init, queryIn->fromProc, queryIn->toProc);
    if(queryIn->procArray[queryIn->toProc].engagingProc == -1){
        queryIn->procArray[queryIn->toProc].engagingProc = queryIn->fromProc;
    }

    if (queryIn->procArray[queryIn->toProc].wait == FALSE)
    {
        queryIn->procArray[queryIn->toProc].wait = TRUE;
        queryIn->procArray[queryIn->toProc].num = queryIn->procArray[queryIn->toProc].totDepProcs;
        printf("xxxxx : (%d,%d,%d) - num :%d\n", queryIn->init, queryIn->fromProc, queryIn->toProc,queryIn->procArray[queryIn->toProc].num);

        // usleep(2000000);
        pthread_t thread[queryIn->procArray[queryIn->toProc].totDepProcs];
        for (int i = 0; i < queryIn->procArray[queryIn->toProc].totDepProcs; i++)
        {
            queryArgs *queryOut;
            queryOut = new queryArgs;
            queryOut->procArray = queryIn->procArray;
            queryOut->init = queryIn->init;
            queryOut->fromProc = queryIn->toProc;
            queryOut->toProc = queryIn->procArray[queryIn->toProc].depProc[i];
            //query(procArray, init, toProc, procArray[toProc].depProc[i]);
            // printf("queryOut Stuff : %d,%d,%d\n",queryOut->init,queryOut->fromProc,queryOut->toProc);
            int rc;
            rc = pthread_create(&thread[i], NULL, query, (void *)queryOut);

            if (rc)
            {
                printf("Unable to create thread");
                exit(0);
            }
        }
        for (int i = 0; i < queryIn->procArray[queryIn->toProc].totDepProcs; i++)
        {
            pthread_join(thread[i], NULL);
        }
    }
    else
    {
        struct queryArgs *replyArgs;
        replyArgs = new queryArgs;
        replyArgs->procArray = queryIn->procArray;
        replyArgs->init = queryIn->init;
        replyArgs->fromProc = queryIn->toProc;
        replyArgs->toProc = queryIn->fromProc;

        // printf("--------queryReply : ");
        reply((void*)replyArgs);
    }

    return 0;
}

int deadLockDetect(Proc *process, int probeProcess)
{
    process[probeProcess].num = process[probeProcess].totDepProcs;
    process[probeProcess].wait = TRUE;
        // query(&process[0], probeProcess, probeProcess, process[probeProcess].depProc[0]);
        struct queryArgs *initiator;
        initiator = new queryArgs;
        initiator->procArray = process;
        initiator->init = probeProcess;
        initiator->fromProc = probeProcess;
        // initiator->toProc = process[probeProcess].depProc[0];

        for (int i = 0; i < process[probeProcess].totDepProcs; i++)
        {
            initiator->toProc = process[probeProcess].depProc[i];
            query((void *)initiator);
        }
    return 0;
}

int nProcs; //Number of Processes

int main()
{

    int probeProcess;

    printf("Enter the number of processes : ");
    scanf("%d", &nProcs);

    if (nProcs > 1)
    {
        int waitGraph[nProcs][nProcs]={0};
        Proc process[nProcs];

        printf("Input the wait graph : \n\n");
        for (int i = 0; i < nProcs; i++)
        {
            int depProcNum = 0;
            for (int j = 0; j < nProcs; j++)
            {
                if (i == j)
                    continue;
                int tmp;
                printf("Is process P%d waiting for P%d : ", i, j);
                scanf("%d", &tmp);
                if (tmp != 0)
                {
                    waitGraph[i][j] = 1;
                    process[i].depProc[depProcNum] = j;
                    depProcNum++;
                }
            }
            process[i].totDepProcs = depProcNum;
        }

        printf("Wait-for Graph matrix \n\n");
        // printGraph(&waitGraph[0],nProcs);
        printDependence(&process[0], nProcs);

        printf("Initiator process : P");
        scanf("%d", &probeProcess);
        deadLockDetect(&process[0], probeProcess);
        // for (int  i = 0; i < nProcs; i++)
        // {
        //     printf("process[%d].num\t: %d\n",i,process[i].num);
        //     printf("process[%d].wait\t: %d\n",i,process[i].wait);
        //     printf("process[%d].totDepProcs\t: %d\n",i,process[i].totDepProcs);
        // }
        
    }
    else
    {
        printf("No deadlock detected.\n");
    }
}