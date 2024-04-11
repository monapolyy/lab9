#include <stdio.h>
#include <stdlib.h>

#define HASH_TABLE_SIZE 10

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType* next;
};

// Fill out this structure
struct HashType
{
    struct RecordType* head;
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_TABLE_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);

        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;
    struct RecordType* pRecord;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        pRecord = pHashArray[i].head;
        while (pRecord != NULL)
        {
            printf("%d, %c, %d -> ", pRecord->id, pRecord->name, pRecord->order);
            pRecord = pRecord->next;
        }
        printf("NULL\n");
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    struct HashType hashTable[HASH_TABLE_SIZE];
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        hashTable[i].head = NULL;
    }

    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        struct RecordType* pNewRecord = (struct RecordType*)malloc(sizeof(struct RecordType));
        *pNewRecord = pRecords[i];
        pNewRecord->next = hashTable[index].head;
        hashTable[index].head = pNewRecord;
    }

    printf("Records in Hash Table:\n");
    displayRecordsInHash(hashTable, HASH_TABLE_SIZE);

    // Free allocated memory
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        struct RecordType* pRecord = hashTable[i].head;
        while (pRecord != NULL)
        {
            struct RecordType* pTemp = pRecord;
            pRecord = pRecord->next;
            free(pTemp);
        }
    }
    free(pRecords);

    return 0;
}