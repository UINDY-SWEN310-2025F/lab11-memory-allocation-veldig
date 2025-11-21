#include <stdio.h>
#define MAX 100

typedef struct {
    int partition;
    int start;
    int end;
    int size;
} AllocationInfo;

void computePartitionAddresses(int n, int segSize[], int segStart[], int firstAddr) {
    if(n <= 0) return;
    segStart[0] = firstAddr;
    for(int i=1;i<n;i++){
        segStart[i] = segStart[i-1] + segSize[i-1];
    }
}

/* Print allocations */
void printSummary(int nSeg, int nProc, int segSize[], int segUsed[], int segStart[], int occupied[], AllocationInfo alloc[]) {
    printf("\n=== SEGMENTS INFO ===\n");
    for(int i=0;i<nSeg;i++){
        printf("Partition %d: Start=%d  Size=%d  Occupied=%s\n",
            i, segStart[i], segSize[i], occupied[i] ? "YES" : "NO");
    }

    printf("\nAllocated Processes:\n");
    int anyAlloc = 0;
    for(int i=0;i<nProc;i++){
        if(alloc[i].partition != -1){
            anyAlloc =1;
            printf("Process %d (size=%d) -> Partition %d | Start=%d End=%d\n",
                i, alloc[i].size, alloc[i].partition, alloc[i].start, alloc[i].end);
        }
    }
    if(!anyAlloc) printf("  (no allocations)\n");

    printf("\nUnallocated Processes:\n");
    int anyUnalloc=0;
    for(int i=0;i<nProc;i++){
        if(alloc[i].partition == -1){
            anyUnalloc=1;
            printf("Process %d (size=%d) could NOT be allocated\n", i, alloc[i].size);
        }
    }
    if(!anyUnalloc) printf("  (none)\n");
}

/* First-Fit */
void firstFit(int nSeg, int nProc, int segSize[], int segUsed[], int segStart[], AllocationInfo alloc[]) {
    printf("\n--- FIRST FIT ---\n");

    int occupied[MAX] = {0};
    for (int i = 0; i < nSeg; i++)
        if (segUsed[i] > 0) occupied[i] = 1;

    for (int i = 0; i < nProc; i++)
        alloc[i].partition = -1;

    for (int p = 0; p < nProc; p++) {
        for (int s = 0; s < nSeg; s++) {
            if (!occupied[s] && segSize[s] >= alloc[p].size) {
                alloc[p].partition = s;
                alloc[p].start = segStart[s];
                alloc[p].end   = segStart[s] + alloc[p].size;
                occupied[s] = 1;
                segUsed[s] = alloc[p].size;
                break;              // first-fit: stop at first suitable partition
            }
        }
    }

    printSummary(nSeg, nProc, segSize, segUsed, segStart, occupied, alloc);
}

/* Best-Fit */
void bestFit(int nSeg, int nProc, int segSize[], int segUsed[], int segStart[], AllocationInfo alloc[]) {
    printf("\n--- BEST FIT ---\n");

    int occupied[MAX] = {0};
    for (int i = 0; i < nSeg; i++)
        if (segUsed[i] > 0) occupied[i] = 1;

    for (int i = 0; i < nProc; i++)
        alloc[i].partition = -1;

    for (int p = 0; p < nProc; p++) {
        int bestS   = -1;
        int bestDiff = -1;

        for (int s = 0; s < nSeg; s++) {
            if (!occupied[s] && segSize[s] >= alloc[p].size) {
                int diff = segSize[s] - alloc[p].size;
                if (bestS == -1 || diff < bestDiff) {
                    bestDiff = diff;
                    bestS = s;
                }
            }
        }

        if (bestS != -1) {
            alloc[p].partition = bestS;
            alloc[p].start = segStart[bestS];
            alloc[p].end   = segStart[bestS] + alloc[p].size;
            occupied[bestS] = 1;
            segUsed[bestS] = alloc[p].size;
        }
    }

    printSummary(nSeg, nProc, segSize, segUsed, segStart, occupied, alloc);
}

/* Worst-Fit */
void worstFit(int nSeg, int nProc, int segSize[], int segUsed[], int segStart[], AllocationInfo alloc[]) {
    printf("\n--- WORST FIT ---\n");
    int occupied[MAX]={0};
    for(int i=0;i<nSeg;i++) if(segUsed[i]>0) occupied[i]=1;
    for(int i=0;i<nProc;i++) alloc[i].partition=-1;

    for(int p=0;p<nProc;p++){
        int worstS=-1;
        int worstDiff=-1;
        for(int s=0;s<nSeg;s++){
            if(!occupied[s] && segSize[s]>=alloc[p].size){
                int diff=segSize[s]-alloc[p].size;
                if(diff>worstDiff){
                    worstDiff=diff;
                    worstS=s;
                }
            }
        }
        if(worstS!=-1){
            alloc[p].partition=worstS;
            alloc[p].start=segStart[worstS];
            alloc[p].end=segStart[worstS]+alloc[p].size;
            occupied[worstS]=1;
            segUsed[worstS]=alloc[p].size;
        }
    }
    printSummary(nSeg, nProc, segSize, segUsed, segStart, occupied, alloc);
}

int main(){
    int nSeg,nProc;
    int segSize[MAX],segUsedInit[MAX],segUsedForFirst[MAX],segUsedForBest[MAX],segUsedForWorst[MAX];
    int segStart[MAX];
    AllocationInfo allocFirst[MAX],allocBest[MAX],allocWorst[MAX];

    printf("Enter number of partitions: "); scanf("%d",&nSeg);
    printf("Enter partition sizes:\n");
    for(int i=0;i<nSeg;i++) scanf("%d",&segSize[i]);

    int firstAddr;
    printf("Enter starting memory address for the FIRST partition: "); scanf("%d",&firstAddr);

    printf("Enter pre-allocated memory (0=available, >0=occupied) for each partition:\n");
    for(int i=0;i<nSeg;i++) scanf("%d",&segUsedInit[i]);

    printf("Enter number of processes: "); scanf("%d",&nProc);
    printf("Enter process sizes:\n");
    for(int i=0;i<nProc;i++){
        int sz; scanf("%d",&sz);
        allocFirst[i].size=sz;
        allocBest[i].size=sz;
        allocWorst[i].size=sz;
    }

    computePartitionAddresses(nSeg, segSize, segStart, firstAddr);

    for(int i=0;i<nSeg;i++){
        segUsedForFirst[i]=segUsedInit[i];
        segUsedForBest[i]=segUsedInit[i];
        segUsedForWorst[i]=segUsedInit[i];
    }

    firstFit(nSeg,nProc,segSize,segUsedForFirst,segStart,allocFirst);
    bestFit(nSeg,nProc,segSize,segUsedForBest,segStart,allocBest);
    worstFit(nSeg,nProc,segSize,segUsedForWorst,segStart,allocWorst);

    return 0;
}
