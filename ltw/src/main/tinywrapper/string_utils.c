#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "string_utils.h"

#pragma GCC visibility push(hidden)

const char* AllSeparators = " \t\n\r.,;()[]{}-<>+*/%&\\\"'^$=!:?";

char* gl4es_resize_if_needed(char* pBuffer, int *size, int addsize);

char* gl4es_inplace_replace(char* pBuffer, int* size, const char* S, const char* D)
{
    int lS = strlen(S), lD = strlen(D);
    pBuffer = gl4es_resize_if_needed(pBuffer, size, (lD-lS)*gl4es_count_string(pBuffer, S));
    char* p = pBuffer;
    while((p = strstr(p, S)))
    {
        // found an occurrence of S
        // check if good to replace, strchr also found '\0' :)
        if(strchr(AllSeparators, p[lS])!=NULL && (p==pBuffer || strchr(AllSeparators, p[-1])!=NULL)) {
            // move out rest of string
            memmove(p+lD, p+lS, strlen(p)-lS+1);
            // replace
            memcpy(p, D, strlen(D));
            // next
            p+=lD;
        } else p+=lS;
    }
    
    return pBuffer;
}


char * InplaceReplaceByIndex(char* pBuffer, int* size, const int startIndex, const int endIndex, const char* replacement)
{
    //SHUT_LOGD("BY INDEX: %s", replacement);
    //SHUT_LOGD("BY INDEX: %i", strlen(replacement));

    int length_difference;
    if(endIndex < startIndex)
        length_difference = strlen(replacement) + (endIndex - startIndex);
    else if(endIndex == startIndex)
        length_difference = strlen(replacement) - 1; // The initial char gets replaced
    else
        length_difference = strlen(replacement) - (endIndex - startIndex); // Can be negative if repl is smaller

    pBuffer = gl4es_resize_if_needed(pBuffer, size, length_difference);
    //SHUT_LOGD("BEFORE MOVING: \n%s", pBuffer);
    // Move the end of the string
    memmove(pBuffer + startIndex + strlen(replacement) , pBuffer + endIndex + 1, strlen(pBuffer) - endIndex + 1);
    //SHUT_LOGD("AFTER MOVING 1: \n%s", pBuffer);

    // Insert the replacement
    memcpy(pBuffer + startIndex, replacement, strlen(replacement));
    //strncpy(pBuffer + startIndex, replacement, strlen(replacement));
    //SHUT_LOGD("AFTER MOVING 2: \n%s", pBuffer);

    return pBuffer;
}

/**
 * Insert the string at the index, pushing "every chars to the right"
 * @param source The shader as a string
 * @param sourceLength The ALLOCATED length of the shader
 * @param insertPoint The index at which the string is inserted.
 * @param insertedString The string to insert
 * @return The shader as a string, maybe in a different memory location
 */
char * InplaceInsertByIndex(char * source, int *sourceLength, const int insertPoint, const char *insertedString){
    int insertLength = strlen(insertedString);
    source = gl4es_resize_if_needed(source, sourceLength, insertLength);
    memmove(source + insertPoint + insertLength,  source + insertPoint, strlen(source) - insertPoint + 1);
    memcpy(source + insertPoint, insertedString, insertLength);

    return source;
}

char* gl4es_inplace_insert(char* pBuffer, const char* S, char* master, int* size)
{
    char* m = gl4es_resize_if_needed(master, size, strlen(S));
    if(m!=master) {
        pBuffer += (m-master);
        master = m;
    }
    char* p = pBuffer;
    int lS = strlen(S), ll = strlen(pBuffer);
    memmove(p+lS, p, ll+1);
    memcpy(p, S, lS);

    return master;
}

char* gl4es_getline(char* pBuffer, int num)
{
    char *p = pBuffer;
    while(num-- && (p=strstr(p, "\n"))) p+=strlen("\n");
    return (p)?p:pBuffer;
}

int gl4es_countline(const char* pBuffer)
{
    int n=0;
    const char* p = pBuffer;
    while((p=strstr(p, "\n"))) {
        p+=strlen("\n");
        n++;
    }
    return n;
}

int gl4es_getline_for(const char* pBuffer, const char* S)
{
    int n=0;
    const char* p = pBuffer;
    const char* end = gl4es_find_string(pBuffer, S);
    if(!end)
        return 0;
    while((p=strstr(p, "\n"))) {
        p+=strlen("\n");
        n++;
        if(p>=end)
            return n;
    }
    return n;
}

int gl4es_count_string(const char* pBuffer, const char* S)
{
    const char* p = pBuffer;
    int lS = strlen(S);
    int n = 0;
    while((p = strstr(p, S)))
    {
        // found an occurrence of S
        // check if good to count, strchr also found '\0' :)
        if(strchr(AllSeparators, p[lS])!=NULL && (p==pBuffer || strchr(AllSeparators, p[-1])!=NULL))
            n++;
        p+=lS;
    }
    return n;
}

const char* gl4es_find_string(const char* pBuffer, const char* S)
{
    const char* p = pBuffer;
    int lS = strlen(S);
    while((p = strstr(p, S)))
    {
        // found an occurrence of S
        // check if good to count, strchr also found '\0' :)
        if(strchr(AllSeparators, p[lS])!=NULL && (p==pBuffer || strchr(AllSeparators, p[-1])!=NULL))
            return p;
        p+=lS;
    }
    return NULL;
}

char* gl4es_find_string_nc(char* pBuffer, const char* S)
{
    char* p = pBuffer;
    int lS = strlen(S);
    while((p = strstr(p, S)))
    {
        // found an occurrence of S
        // check if good to count, strchr also found '\0' :)
        if(strchr(AllSeparators, p[lS])!=NULL && (p==pBuffer || strchr(AllSeparators, p[-1])!=NULL))
            return p;
        p+=lS;
    }
    return NULL;
}

char* gl4es_resize_if_needed(char* pBuffer, int *size, int addsize) {
    char* p = pBuffer;
    int current_size = *size;
    int str_len = (int)strlen(pBuffer);
    
    // 检查 addsize 是否会导致溢出
    if (addsize < 0 || str_len < 0) {
        fprintf(stderr, "LTW: Invalid size parameters in gl4es_resize_if_needed\n");
        return NULL;
    }
    
    // 检查 newsize 是否溢出
    if (str_len > INT_MAX - addsize - 1) {
        fprintf(stderr, "LTW: Integer overflow in gl4es_resize_if_needed (newsize calculation)\n");
        return NULL;
    }
    int newsize = str_len + addsize + 1;
    
    if (newsize > current_size) {
        // Optimize: Pre-allocate extra space to reduce frequent realloc calls
        // Use exponential growth strategy (1.5x or 2x) for better amortized performance
        int growth_size = current_size + (current_size >> 1); // 1.5x growth
        if (growth_size < current_size) {
            // 溢出检查失败，使用更保守的增长策略
            growth_size = current_size + 256;
        }
        if (growth_size < newsize) {
            growth_size = newsize;
        }
        // Ensure minimum growth of 256 bytes to avoid tiny allocations
        if (growth_size - current_size < 256) {
            if (current_size > INT_MAX - 256) {
                fprintf(stderr, "LTW: Integer overflow in gl4es_resize_if_needed (minimum growth)\n");
                return NULL;
            }
            growth_size = current_size + 256;
        }
        // 最终溢出检查
        if (growth_size < current_size || growth_size < newsize) {
            fprintf(stderr, "LTW: Integer overflow in gl4es_resize_if_needed (final check)\n");
            return NULL;
        }
        char* new_ptr = (char*)realloc(pBuffer, growth_size);
        if (!new_ptr) {
            // realloc failed, log error and return NULL to indicate failure
            fprintf(stderr, "LTW: Failed to reallocate buffer in gl4es_resize_if_needed (requested %d bytes)\n", growth_size);
            return NULL;
        }
        p = new_ptr;
        *size = growth_size;
    }
    return p;
}

char* gl4es_append(char* pBuffer, int* size, const char* S) {
    char* p =pBuffer;
    p = gl4es_resize_if_needed(pBuffer, size, strlen(S));
    strcat(p, S);
    return p;
}

static int gl4es_is_blank(char c)  {
    switch(c) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case ':':
        case ',':
        case ';':
        case '/':
            return 1;
        default:
            return 0;
    }
}

int isDigit(char value){
    return (value >= '0' && value <= '9');
}

int isValidFunctionName(char value){
    return ((value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z') || (value == '_'));
}

char* gl4es_str_next(char *pBuffer, const char* S) {
    if(!pBuffer) return NULL;
    char *p = strstr(pBuffer, S);
    return p ? p : NULL;
}

char* gl4es_next_str(char* pBuffer) {
    if(!pBuffer) return NULL;
    while(gl4es_is_blank(*pBuffer))
        ++pBuffer;
    return pBuffer;
}

char* gl4es_prev_str(char* Str, char* pBuffer) {
    if(!pBuffer) return NULL;
    if(pBuffer == Str)
        return Str;
    // go to previous non blank
    do {
        --pBuffer;
    } while(gl4es_is_blank(*pBuffer) && (pBuffer!=Str));
    // go to blank
    while((pBuffer!=Str) && !gl4es_is_blank(*(pBuffer-1)))
        --pBuffer;
    return pBuffer;
}

char* gl4es_next_blank(char* pBuffer) {
    if(!pBuffer) return NULL;
    while(!gl4es_is_blank(*pBuffer))
        ++pBuffer;
    return pBuffer;
}

char* gl4es_next_line(char* pBuffer) {
    if(!pBuffer) return NULL;
    while(*pBuffer && *pBuffer!='\n')
        ++pBuffer;
    return pBuffer;
}

const char* gl4es_get_next_str(char* pBuffer) {
    static char buff[100] = {0};
    buff[0] = '\0';
    if(!pBuffer) return NULL;
    char* p1 = gl4es_next_str(pBuffer);
    if(!p1) return buff;
    char* p2 = gl4es_next_blank(p1);
    if(!p2) return buff;
    int i=0;
    while(p1!=p2 && i<99)
        buff[i++] = *(p1++);
    buff[i] = '\0';
    return buff;
}

int gl4es_countstring_simple(char* pBuffer, const char* S)
{
    char* p = pBuffer;
    int lS = strlen(S);
    int n = 0;
    while((p = strstr(p, S)))
    {
        // found an occurrence of S
        n++;
        p+=lS;
    }
    return n;
}

char* gl4es_inplace_replace_simple(char* pBuffer, int* size, const char* S, const char* D)
{
    int lS = strlen(S), lD = strlen(D);
    pBuffer = gl4es_resize_if_needed(pBuffer, size, (lD-lS)*gl4es_countstring_simple(pBuffer, S));
    char* p = pBuffer;
    while((p = strstr(p, S)))
    {
        // found an occurrence of S
        // move out rest of string
        memmove(p+lD, p+lS, strlen(p)-lS+1);
        // replace
        memcpy(p, D, strlen(D));
        // next
        p+=lD;
    }
    
    return pBuffer;
}
#pragma GCC visibility pop(hidden)