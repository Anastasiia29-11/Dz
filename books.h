#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct book_t {
    char isbn[16]; 
    char authors[40];
    char title[25]; 
    int total; 
    int available; 
} book_t;

book_t read_book(); 
void print_book(book_t book); 

void print_books(book_t* books, int n); 
void print_book_by_isbn(book_t* books, int n); 

book_t* add_book(book_t* books, int* n); 
void remove_book_by_isbn(book_t* books, int* n); 

void save_books(book_t* books, int n); 
book_t* load_books(int* n); 
