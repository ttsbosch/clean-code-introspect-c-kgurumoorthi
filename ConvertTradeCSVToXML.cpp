#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ConvertTradeCSVToXML.h"

bool validateTradeLine(char* line, int lineCount) {
    char* fields[NUM_FIELDS];
    int fieldCount = 0;
    char* token = strtok(line, ",");
    while (token != NULL) {
        fields[fieldCount++] = token;
        token = strtok(NULL, ",");
    }

    if (fieldCount != NUM_FIELDS) {
        fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
        return false;
    }

    if (strlen(fields[0]) != CURRENCY_LENGTH) {
        fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
        return false;
    }

    int tradeAmount;
    if (!parseIntFromString(fields[1], &tradeAmount)) {
        fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        return false;
    }

    double tradePrice;
    if (!parseDouble(fields[2], &tradePrice)) {
        fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        return false;
    }

    return true;
}

void parseTradeLine(char* line, TradeRecord* tradeObjects, int* objectCount, int* lineCount) {
    if (!validateTradeLine(line, *lineCount)) {
        (*lineCount)++;
        return;
    }

    char* fields[NUM_FIELDS];
    int fieldCount = 0;
    char* token = strtok(line, ",");
    while (token != NULL) {
        fields[fieldCount++] = token;
        token = strtok(NULL, ",");
    }

    strncpy(tradeObjects[*objectCount].sourceCurrency, fields[0], 3);
    strncpy(tradeObjects[*objectCount].destinationCurrency, fields[0] + 3, 3);

    int tradeAmount;
    parseIntFromString(fields[1], &tradeAmount);
    tradeObjects[*objectCount].lots = tradeAmount / LotSize;

    double tradePrice;
    parseDouble(fields[2], &tradePrice);
    tradeObjects[*objectCount].price = tradePrice;

    (*objectCount)++;
    (*lineCount)++;
}

void writeTradeRecordsToXML(const TradeRecord* tradeObjects, int objectCount) {
    FILE* outFile = fopen(XML_FILENAME, "w");
    if (outFile == NULL) {
        fprintf(stderr, "ERROR: Unable to open output file '%s'\n", XML_FILENAME);
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

void ConvertTradeFromCSVToXML(FILE* stream) {
    char line[MAX_LINE_LENGTH];
    TradeRecord tradeObjects[MAX_TRADE_OBJECTS];
    int lineCount = 0;
    int objectCount = 0;

    while (fgets(line, sizeof(line), stream)) {
        parseTradeLine(line, tradeObjects, &objectCount, &lineCount);
    }

    writeTradeRecordsToXML(tradeObjects, objectCount);
    printf("INFO: %d trades processed\n", objectCount);
}

int main() {
    FILE* stream;
    stream = fopen("trades.csv", "r"); 

    if (stream == NULL) {
        perror("Error opening file");
        return 1;
    }
    ConvertTradeFromCSVToXML(stream);

    fclose(stream);

    return 0;
}
