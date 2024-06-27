#include "Converter.h"
#include "StringHelpers.h"
#include "TradeRecord.h"

bool validateTradeLine(char* line, int lineCount);
void parseTradeLine(char* line, TradeRecord* tradeObjects, int* objectCount, int* lineCount);
void writeTradeRecordsToXML(const TradeRecord* tradeObjects, int objectCount);
void ConverTradeFromCSVToXML(FILE* stream);
