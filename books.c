#define _CRT_SECURE_NO_WARNINGS
#include "books.h"

const char* books_path = "books.csv"; 

// считывание полей структуры
book_t read_book() {
    book_t book; 
    
    printf("Enter isbn: ");
    scanf("%s", book.isbn); 

    printf("Enter authors: ");
    scanf("%s", book.authors); 

    printf("Enter title: ");
    scanf("%s", book.title); 

    printf("Enter total: ");
    scanf("%d", &book.total); 

    printf("Enter available: ");
    scanf("%d", &book.available); 

    return book; 
}


void print_book_header() {
    printf("+------------------+------------------------------------------+---------------------------+-----------+-----------+\n");
    printf("|       isbn       |                 authors                  |           title           |   total   | available |\n");
    printf("+------------------+------------------------------------------+---------------------------+-----------+-----------+\n");
}

void print_book(book_t book) {
    printf("| %16s ", book.isbn);
    printf("| %40s ", book.authors);
    printf("| %25s ", book.title);
    printf("| %9d ", book.total);
    printf("| %9d ", book.available);
    printf("|\n");
}

void print_book_footer() {
    printf("+------------------+------------------------------------------+---------------------------+-----------+-----------+\n");
}


void print_books(book_t* books, int n) {
    if (n == 0) { 
        printf("No book was added yet\n"); 
        return; 
    }

    // алгоритм Шелла
    for (int k = n / 2; k > 0; k /= 2) {
        for (int i = k; i < n; i++) {
            book_t tmp = books[i]; // запоминаем текущий элемент
            int j = i;

            // поиск места для вставки
            while (j >= k && strcmp(tmp.isbn, books[j - k].isbn) < 0) {
                books[j] = books[j - k];
                j -= k;
            }

            books[j] = tmp; // вставка элемента в найденное место
        }
    }

    print_book_header(); 

    for (int i = 0; i < n; i++)
        print_book(books[i]);

    print_book_footer(); 
}

// получение индекса книги по номеру
int get_index_by_isbn(book_t* books, int n, char* isbn) {
    for (int i = 0; i < n; i++)
        if (!strcmp(isbn, books[i].isbn))
            return i;

    return -1; // не найдено
}

// вывод книги по номеру
void print_book_by_isbn(book_t* books, int n) {
    char isbn[16];
    printf("Enter isbn: ");
    scanf("%s", isbn); 

    int index = get_index_by_isbn(books, n, isbn); 

    if (index == -1) { 
        printf("No book with ISBN '%s'\n", isbn);
        return;
    }

    print_book_header(); 
    print_book(books[index]); 
    print_book_footer(); 
}

// добавление новой книги
book_t* add_book(book_t* books, int* n) {
    book_t book = read_book(); 

    if (get_index_by_isbn(books, *n, book.isbn) != -1) { 
        printf("Book with this ISBN already include\n"); 
        return books; 
    }

    books = (book_t*)realloc(books, (*n + 1) * sizeof(book_t)); 
    printf("Enter information about new book\n");
    books[(*n)++] = book; // запись книги в массив

    return books; 
}

// удаление книги по isbn
void remove_book_by_isbn(book_t* books, int* n) {
    if (n == 0) { 
        printf("No book was added yet\n");
        return; 
    }

    char isbn[16];
    printf("Enter isbn: ");
    scanf("%s", isbn); 

    int i = get_index_by_isbn(books, *n, isbn);
    if (i == -1) {
        printf("No book with isbn '%s'\n", isbn); 
        return; 
    }

    (*n)--; // уменьшение размера

    // сдвиг книг влево
    for (; i < *n; i++)
        books[i] = books[i + 1];
}

// сохранение записей в файл
void save_books(book_t* books, int n) {
    FILE* f = fopen(books_path, "w"); 

    for (int i = 0; i < n; i++) {
        fprintf(f, "%s;%s;%s;%d;%d", books[i].isbn, books[i].authors, books[i].title, books[i].total, books[i].available);
        if (i < n - 1)
            fprintf(f, "\n");
    }
    fclose(f); 
}

// загрузка записей из файла
book_t* load_books(int* n) {
    FILE* f = fopen(books_path, "r"); 
    if (!f) { 
        *n = 0; 
        return NULL; 
    }
    
    book_t* books = (book_t*)malloc(sizeof(book_t)); 
    *n = 0; 

    while (!feof(f)) {
        fscanf(f, "%[^;]%*c", books[*n].isbn);
        fscanf(f, "%[^;]%*c", books[*n].authors);
        fscanf(f, "%[^;]%*c", books[*n].title);
        fscanf(f, "%d%*c", &books[*n].total);
        fscanf(f, "%d%*c", &books[*n].available);

        *n = *n + 1; // увеличиваем размер
        books = (book_t*)realloc(books, (*n + 1) * sizeof(book_t)); 
    }

    fclose(f); 
    return books; 
}
