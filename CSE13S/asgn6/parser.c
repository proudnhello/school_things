#include "parser.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

int once;

struct Parser {
    FILE *f;
    char current_line[MAX_PARSER_LINE_LENGTH + 1];
    uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
    Parser *p = (Parser *)malloc(sizeof(Parser));
    once = true;
    p->f = f;
    p->line_offset = 0;
    fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f);

    return p;
}

void parser_delete(Parser **p) {
    free(*p);
    *p = NULL;
    return;
}

bool next_word(Parser *p, char *word) {

    char next_char = p->current_line[p->line_offset];
    // If the first character is \0, return immidiately
    if(next_char == '\0'){
        return false; 
    }
    
    // If the first character is \n, reduce the line offset. This is so we don't
    // skip it, like we would if it were any other unacceptable character
    if (next_char == '\n') {
        p->line_offset -= 1;
    }

    while(!isalnum(next_char) && next_char != '\'' && next_char != '-' &&
             next_char != '\0') {
        // Get the next character
        p->line_offset += 1;
        next_char = p->current_line[p->line_offset];
        // If the character is \0, there are no more acceptable characters,
        // break out of the lool
        if (next_char == '\0') {
            break;
        }
        // If the next character is \n, get the next line, then set the line
        // offset to -1. This is so we don't skip the first char
        if (next_char == '\n') {
            char* tester = fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f);
            if(tester == NULL){
                return false;
            }
            p->line_offset = -1;
        }
    } 

    char c = p->current_line[p->line_offset];
    uint32_t buffer = 0;

    // Get the next character while the next character is acceptable
    while (isalnum(c) || c == '\'' || c == '-') {
        word[buffer] = tolower(c);
        buffer += 1;
        p->line_offset += 1;
        c = p->current_line[p->line_offset];
    }

    // Set add a null terminator to the returned string
    word[buffer] = '\0';

    return true;
}
