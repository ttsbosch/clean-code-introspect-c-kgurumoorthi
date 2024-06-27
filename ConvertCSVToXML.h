#include "Converter.h"
#include "StringHelpers.h"
#include "TradeRecord.h"

#define MAX_LINE_LENGTH 1024
#define MAX_TRADE_RECORDS 1024
#define CURRENCY_CODE_LENGTH 3
#define CURRENCY_PAIR_LENGTH 6
#define LOT_SIZE 100000

bool ValidateTradeTokens(char* fields[CSV_FIELDS], int lineCount);
bool ConvertTokensToTradeRecord(char* fields[CSV_FIELDS], TradeRecord* tradeRecord);
bool ParseTradeLine(const char* line, TradeRecord* tradeRecord, int lineCount);
void WriteTradesToXML(const TradeRecord* tradeRecords, int count, const char* outputFilePath);
void ConvertCSVToXML(FILE* inputFile, const char* outputFilePath);
