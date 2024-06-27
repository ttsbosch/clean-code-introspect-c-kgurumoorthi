#include "Converter.h"
#include "StringHelpers.h"
#include "TradeRecord.h"

#define MAX_LINE_LENGTH 1024
#define MAX_TRADE_RECORDS 1024
#define CURRENCY_CODE_LENGTH 3
#define CURRENCY_PAIR_LENGTH 6
#define LOT_SIZE 100000

bool validateTradeLine(char* line, int lineCount);
void parseTradeLine(char* line, TradeRecord* tradeObjects, int* objectCount, int* lineCount);
void writeTradeRecordsToXML(const TradeRecord* tradeObjects, int objectCount);
void ConverTradeFromCSVToXML(FILE* stream);
