#include "Converter.h"
#include "StringHelpers.h"
#include "TradeRecord.h"

#define MAX_LINE_LENGTH 1024
#define NUM_FIELDS 3
#define CURRENCY_LENGTH 6
#define MAX_TRADE_OBJECTS 1024
#define XML_FILENAME "output.xml"

bool validateTradeLine(char* line, int lineCount);
void parseTradeLine(char* line, TradeRecord* tradeObjects, int* objectCount, int* lineCount);
void writeTradeRecordsToXML(const TradeRecord* tradeObjects, int objectCount);
void ConverTradeFromCSVToXML(FILE* stream);
