#define CSV_FIELDS 3

bool SplitLineIntoFields(const char* line, char* fields[CSV_FIELDS]) {
    int fieldCount = 0;
    char* lineCopy = strdup(line);
    char* token = strtok(lineCopy, ",");

    while (token != NULL && fieldCount < CSV_FIELDS) {
        fields[fieldCount++] = token;
        token = strtok(NULL, ",");
    }

    if (fieldCount != CSV_FIELDS) {
        fprintf(stderr, "WARN: Line malformed. Only %d field(s) found.\n", fieldCount);
        free(lineCopy);
        return false;
    }

    fields[CSV_FIELDS] = NULL;  // Null-terminate the array
    return true;
}
