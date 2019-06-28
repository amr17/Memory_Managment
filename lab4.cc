#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int framesCount;
int *framesArray;
int *goblinArray;
int *useArray;
int pageFaultCount = 0;
char policyName[5];

int isFramesArrayContainsPage(int pageNumber)
{
    for(int i = 0; i < framesCount; i++)
    {
        if(framesArray[i] == pageNumber)
            return i;
    }
    return -1;
}

void printFramesContent()
{
    for(int i = 0; i < framesCount; i++)
    {
        if(framesArray[i] != -1)
            printf("%02d ",framesArray[i]);
    }
    printf("\n");
}

void startFIFO()
{
    int index = 0;
    int pageNumber;
    scanf("%d", &pageNumber);
    while(pageNumber != -1)
    {
        int isPageAvilable = isFramesArrayContainsPage(pageNumber);
        if(isPageAvilable != -1)
        {
            printf("%02d     ",pageNumber);
        }
        else
        {
            if(framesArray[index % framesCount] != -1)
            {
                printf("%02d F   ",pageNumber);
                pageFaultCount++;
            }
            else
                printf("%02d     ",pageNumber);

            framesArray[index++ % framesCount] = pageNumber;
        }
        printFramesContent();
        scanf("%d", &pageNumber);

    }
}

void updateGoblinArray(int index, int referenceNumber)
{
    goblinArray[index] = referenceNumber;
}
int LRUPageIndex()
{
    int min = 10000000, minIndex;
    for(int i = 0; i< framesCount; i++)
    {
        if(goblinArray[i] < min)
        {
            min = goblinArray[i];
            minIndex = i;
        }
    }
    return minIndex;
}
void startLRU()
{
    int pageNumber, nextAvilableFrameIndex = 0, referenceNumber = 0;
    scanf("%d", &pageNumber);
    while(pageNumber != -1)
    {

        int isPageAvilable = isFramesArrayContainsPage(pageNumber);
        if(isPageAvilable != -1)
        {
            updateGoblinArray(isPageAvilable, referenceNumber++);
            printf("%02d     ",pageNumber);
        }
        else
        {
            if(nextAvilableFrameIndex < framesCount)
            {
                framesArray[nextAvilableFrameIndex] = pageNumber;
                updateGoblinArray(nextAvilableFrameIndex++, referenceNumber++);
                printf("%02d     ",pageNumber);
            }
            else
            {
                int index = LRUPageIndex();
                framesArray[index] = pageNumber;
                updateGoblinArray(index, referenceNumber++);
                pageFaultCount++;
                printf("%02d F   ",pageNumber);
            }

        }
        printFramesContent();
        scanf("%d", &pageNumber);
    }
}

void startCLOCK()
{
    int pageNumber, useArrayIndex = 0;
    scanf("%d", &pageNumber);
    while(pageNumber != -1)
    {
        int isPageAvilable = isFramesArrayContainsPage(pageNumber);
        //checks if the page exists in resident set if so return it's location
    //resident set is full (begin replacing pages)

        if(isPageAvilable != -1)
        {
            useArray[isPageAvilable] = 1;

            printf("%02d     ",pageNumber);
        }
        else
        {

            while(useArray[useArrayIndex % framesCount] != 0)
            {
                useArray[useArrayIndex % framesCount] = 0;
                useArrayIndex++;
            }
            framesArray[useArrayIndex % framesCount] = pageNumber;
            useArray[useArrayIndex % framesCount] = 1;
            useArrayIndex++;
    //count number of page faults excluding ones when resident set is not yet full

            if(useArrayIndex > 3)
            {
                printf("%02d F   ",pageNumber);
                pageFaultCount++;
            }
            else printf("%02d     ",pageNumber);

        }
        printFramesContent();
        scanf("%d", &pageNumber);

    }
}

void initializeByNegativeOne()
{
    for(int i = 0; i < framesCount; i++)
    {
        framesArray[i] = -1;
        goblinArray[i] = -1;
    }
}

int main()
{
    //read number of available frames for this process
    scanf("%d",&framesCount);
    //read which page replacement policy we should use
    scanf("%s", policyName);

    printf("Replacement Policy = %s\n",policyName);
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    //read all page references
    //initialize resident set for the process

    framesArray = (int*) calloc(framesCount,sizeof(int));
    goblinArray = (int*) calloc(framesCount,sizeof(int));
    useArray = (int*) calloc(framesCount,sizeof(int));
    initializeByNegativeOne();
    if(strcmp(policyName, "FIFO") == 0)
        startFIFO();

    else if (strcmp(policyName, "LRU") == 0)
        startLRU();
    else
        startCLOCK();

    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",pageFaultCount);

    return 0;
}
