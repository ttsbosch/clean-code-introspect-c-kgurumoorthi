#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "CSVToXMLTradeConversion.h"

bool ValidateTradeTokens(char* fields[CSV_FIELDS], int lineCount) {
    if (strlen(fields[0]) != CURRENCY_PAIR_LENGTH) {
        fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
        return false;
    }

    int tradeAmount;
    if (!TryConvertToIntegerFromString(fields[1], &tradeAmount)) {
        fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        return false;
    }

    double tradePrice;
    if (!TryConvertToDoubleFromString(fields[2], &tradePrice)) {
        fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        return false;
    }

    return true;
}

bool ConvertTokensToTradeRecord(char* fields[CSV_FIELDS], TradeRecord* tradeRecord) {
    int tradeAmount = strtol(fields[1], NULL, 10);
    double tradePrice = strtod(fields[2], NULL);

    strncpy(tradeRecord->SourceCurrency, fields[0], CURRENCY_CODE_LENGTH);
    tradeRecord->SourceCurrency[CURRENCY_CODE_LENGTH] = '\0';
    strncpy(tradeRecord->DestinationCurrency, fields[0] + CURRENCY_CODE_LENGTH, CURRENCY_CODE_LENGTH);
    tradeRecord->DestinationCurrency[CURRENCY_CODE_LENGTH] = '\0';
    tradeRecord->Lots = tradeAmount / (float)LOT_SIZE;
    tradeRecord->Price = tradePrice;

    return true;
}

bool ParseTradeLine(const char* line, TradeRecord* tradeRecord, int lineCount) {
    char* fields[CSV_FIELDS];
    if (!SplitLineIntoFields(line, fields)) {
        return false;
    }

    if (!ValidateTradeTokens(fields, lineCount)) {
        return false;
    }

    return ConvertTokensToTradeRecord(fields, tradeRecord);
}

void WriteTradesToXML(const TradeRecord* tradeRecords, int count, const char* outputFilePath) {
    FILE* outFile = fopen(outputFilePath, "w");
    if (outFile == NULL) {
        fprintf(stderr, "ERROR: Could not open output file for writing: %s\n", strerror(errno));
        return;
    }

    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < count; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", tradeRecords[i].SourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", tradeRecords[i].DestinationCurrency);
        fprintf(outFile, "\t\t<Lots>%.2f</Lots>\n", tradeRecords[i].Lots);
        fprintf(outFile, "\t\t<Price>%.4f</Price>\n", tradeRecords[i].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>\n");
    fclose(outFile);
}

void ConvertCSVToXML(FILE* inputFile, const char* outputFilePath) {
    char line[MAX_LINE_LENGTH];
    TradeRecord tradeRecords[MAX_TRADE_RECORDS];
    int tradeCount = 0;

    while (fgets(line, sizeof(line), inputFile) && tradeCount < MAX_TRADE_RECORDS) {
        if (ParseTradeLine(line, &tradeRecords[tradeCount], tradeCount)) {
            tradeCount++;
        }
    }

    WriteTradesToXML(tradeRecords, tradeCount, outputFilePath);
    printf("INFO: %d trades processed\n", tradeCount);
}

int main() {
    FILE* inputFile = fopen("trades.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "ERROR: Could not open input file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    ConvertCSVToXML(inputFile, "output.xml");
    fclose(inputFile);

    return EXIT_SUCCESS;
}
