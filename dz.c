#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "books.h"

void book_menu() {
    int n;
    book_t* books = load_books(&n); 
    int item; // пункт меню

    do {
        printf("What do you want to do?\n");
        printf("1. Add book\n");
        printf("2. Remove by id\n");
        printf("3. Print by id\n");
        printf("4. Print books\n");
        printf("5. Exit\n");
        printf(">");
        scanf("%d%*c", &item); 

        // некорректный ввод пункта меню
        while (item < 1 || item > 5) {
            printf("Incorrect item, try again: ");
            scanf("%d%*c", &item); 
        }

        switch (item) {
        case 1:
            books = add_book(books, &n); 
            break;
        case 2:
            remove_book_by_isbn(books, &n);
            break;
        case 3:
            print_book_by_isbn(books, n);  
            break;
        case 4:
            print_books(books, n);
            break;
        }
        printf("\n");
    } while (item != 5); 

    save_books(books, n); 
    free(books); 
}

int main() {
    char mode[100]; 
    book_menu();
    return 0;
}
