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
