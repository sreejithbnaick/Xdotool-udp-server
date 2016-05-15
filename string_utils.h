/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_utils.h
 * Author: sreejith
 *
 * Created on 2 April, 2016, 2:57 PM
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct SplitResult {
        char ** tokens;
        int count;
    }SplitResult;

    struct SplitResult str_split(char* a_str, const char a_delim);
    int str_contains_duplicate(char* a_str, const char a_delim);
    char *str_concat(char *str1, char *str2);


#ifdef __cplusplus
}
#endif

#endif /* STRING_UTILS_H */

