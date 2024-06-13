#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char sourceCurrency[256]; 
    char destinationCurrency[256]; 
    float lots;
    double price;
} TradeRecords;


char** SplitString(const char* inputString, char delimiter) {
    int tokenCount = 0;
    const char* pointer = inputString;
    while (*pointer != '\0') {
        if (*pointer++ == delimiter) {
            tokenCount++;
        }
    }

    char** tokens = (char**)malloc(sizeof(char*) * (tokenCount + 2));
    int tokenIndex  = 0;
    pointer = inputString;
    char* token = (char*)malloc(strlen(inputString) + 1);
    int tokenLength = 0;
    while (*pointer != '\0') {
        if (*pointer == delimiter) {
            token[tokenLength] = '\0';
            tokens[tokenIndex ++] = strdup(token);
            tokenLength = 0;
        } else {
            token[tokenLength++] = *pointer;
        }
        pointer++;
    }
    token[tokenLength] = '\0';
    tokens[tokenIndex ++] = strdup(token);
    tokens[tokenIndex ] = NULL;
    free(token);
    return tokens;
}

bool TryParseIntFromString(const char* string, int* value) {
    char* endptr;
    *value = strtol(string, &endptr, 10);
    if (endptr == string) {
        return False; 
    }
    return True; 
}

bool TryParseDoubleFromString(const char* string, double* value) {
    char* endptr;
    *value = strtod(string, &endptr);
    if (endptr == string) {
        return False; 
    }
    return True; 
}

void ConverTradeFromCSVToXML(FILE* stream) {
    char line[1024];
    TradeRecords tradeObjects[1024];
    int lineCount = 0;
    int objectCount = 0;

    while (fgets(line, sizeof(line), stream)) {
        parseLine(line, tradeObjects, &objectCount, &lineCount);
    }

    writeXML(tradeObjects, objectCount);
    printf("INFO: %d trades processed\n", objectCount);
}

void parseLine(char* line, TradeRecords* tradeObjects, int* objectCount, int* lineCount) {
    char* fields[3];
    int fieldCount = 0;
    char* token = strtok(line, ",");
    while (token != NULL) {
        fields[fieldCount++] = token;
        token = strtok(NULL, ",");
    }

    if (fieldCount != 3) {
        fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", *lineCount + 1, fieldCount);
        (*lineCount)++;
        return;
    }

    if (strlen(fields[0]) != 6) {
        fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", *lineCount + 1, fields[0]);
        (*lineCount)++;
        return;
    }

    int tradeAmount;
    if (!parseIntFromString(fields[1], &tradeAmount)) {
        fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", *lineCount + 1, fields[1]);
    }

    double tradePrice;
    if (!parseDouble(fields[2], &tradePrice)) {
        fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", *lineCount + 1, fields[2]);
    }

    strncpy(tradeObjects[*objectCount].sourceCurrency, fields[0], 3);
    strncpy(tradeObjects[*objectCount].destinationCurrency, fields[0] + 3, 3);
    tradeObjects[*objectCount].lots = tradeAmount / LotSize;
    tradeObjects[*objectCount].price = tradePrice;
    (*objectCount)++;
    (*lineCount)++;
}

void writeXML(const TradeRecords* tradeObjects, int objectCount) {
    FILE* outFile = fopen("output.xml", "w");
    if (outFile == NULL) {
        fprintf(stderr, "ERROR: Unable to open output file\n");
        return;
    }
    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < objectCount; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", tradeObjects[i].sourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", tradeObjects[i].destinationCurrency);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", (int)tradeObjects[i].lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", tradeObjects[i].price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
}
