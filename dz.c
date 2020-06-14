#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS

typedef struct book_t {
    char isbn[16]; 
    char authors[40];
    char title[25];
    int total; 
    int available; 
} book_t;

typedef struct student_t {
    char id[16]; 
    char surname[16]; 
    char name[16]; 
    char patronymic[16]; 
    char faculty[25];
    char speciality[20]; 
} student_t;

typedef struct user_t { 
    char login[15]; 
    char password[15]; 
    int st; // доступ к БД со студентами
    int bk; // доступ к БД с книгами
} user_t;

//-----------------------------------ФУНКЦИИ ДЛЯ КНИГ-----------------------------

book_t read_book() {
book_t book; // создаём структуру

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
// вывод структуры
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

// вывод базы данных
void print_books(book_t* books, int n) {
    if (n == 0) { // если ещё нет записей
        printf("No book was added yet\n");
        return; // выходим
    }
    
    for (int k = n / 2; k > 0; k /= 2) {
        for (int i = k; i < n; i++) {
            book_t tmp = books[i]; // запоминаем текущий элемент
            int j = i;

            // ищем место для вставки
            while (j >= k && strcmp(tmp.isbn, books[j - k].isbn) < 0) {
                books[j] = books[j - k];
                j -= k;
            }

            books[j] = tmp; // вставляем элемент в найденное место
        }
    }

    print_book_header(); // выводим шапку таблицы

    for (int i = 0; i < n; i++)
        print_book(books[i]);

    print_book_footer(); // выводим низ таблицы
}

// получение индекса книги по номеру
int get_index_by_isbn(book_t* books, int n, char* isbn) {
    // ищем книгу по номеру
    for (int i = 0; i < n; i++)
        if (!strcmp(isbn, books[i].isbn))
            return i;

    return -1; // не нашли
}

// вывод книги по номеру
void print_book_by_isbn(book_t* books, int n) {
    char isbn[16];
    printf("Enter isbn: ");
    scanf("%s", isbn); 

    int index = get_index_by_isbn(books, n, isbn); // ищем книгу по номеру

    if (index == -1) { // если такой книги нет
        printf("No book with ISBN '%s'\n", isbn);
        return;
    }

    print_book_header(); // выводим шапку
    print_book(books[index]); // выводим книгу
    print_book_footer(); // выводим низ таблицы
}

// добавление новой записи
book_t* add_book(book_t* books, int* n) {
    book_t book = read_book(); // считываем запись

    if (get_index_by_isbn(books, *n, book.isbn) != -1) { // если книга с таким номером уже есть
        printf("Book with this ISBN already include\n");
        return books; 
    }

    books = (book_t*)realloc(books, (*n + 1) * sizeof(book_t)); // перераспределяем память
    printf("Enter information about new book\n");
    books[(*n)++] = book; // записываем книгу в массив

    return books; // возвращаем обновлённый массив
}

// удаление книги по isbn
void remove_book_by_isbn(book_t* books, int* n) {
    if (n == 0) { // если ещё нет записей
        printf("No book was added yet\n");
        return;
    }

    char isbn[16];
    printf("Enter isbn: ");
    scanf("%s", isbn);

    int i = get_index_by_isbn(books, *n, isbn); // ищем книгу по номеру

    // если не нашли
    if (i == -1) {
        printf("No book with isbn '%s'\n", isbn); 
        return;
    }

    (*n)--; // уменьшаем размер

    // сдвигаем книги влево
    for (; i < *n; i++)
        books[i] = books[i + 1];
}

// сохранение записей в файл
void save_books(char *books_path, book_t* books, int n) {
    FILE *fbook = fopen(books_path, "w"); 

    for (int i = 0; i < n; i++) {
        fprintf(fbook, "%s;%s;%s;%d;%d", books[i].isbn, books[i].authors, books[i].title, books[i].total, books[i].available);

        if (i < n - 1)
            fprintf(fbook, "\n");
    }

    fclose(fbook); 
}

// загрузка записей из файла
book_t* load_books(char *books_path, int* n) {
    FILE* fbook = fopen(books_path, "r"); 

    if (!fbook) { // если не удалось открыть файл
        *n = 0; // обнуляем количество
        return NULL; // возвращаем пустой указатель
    }

    book_t* books = (book_t*)malloc(sizeof(book_t)); // выделяем память под один элемент
    *n = 0; // изначально элементов нет

    while (!feof(fbook)) {
        fscanf(fbook, "%[^;]%*c", books[*n].isbn);
        fscanf(fbook, "%[^;]%*c", books[*n].authors);
        fscanf(fbook, "%[^;]%*c", books[*n].title);
        fscanf(fbook, "%d%*c", &books[*n].total);
        fscanf(fbook, "%d%*c", &books[*n].available);

        *n = *n + 1; // увеличиваем размер
        books = (book_t*)realloc(books, (*n + 1) * sizeof(book_t)); // перераспределяем память
    }

    fclose(fbook); 
    return books; // возвращаем считанный массив
}

//-------------------------------ФУНКЦИИ ДЛЯ СТУДЕНТОВ-----------------------------------

// считывание полей структуры
student_t read_student() {
    student_t student; // создаём структуру

    printf("Enter id: ");
    scanf("%s", student.id); 

    printf("Enter surname: ");
    scanf("%s", student.surname); 

    printf("Enter name: ");
    scanf("%s", student.name);

    printf("Enter patronymic: ");
    scanf("%s", student.patronymic); // отчество

    printf("Enter faculty: ");
    scanf("%s", student.faculty); 

    printf("Enter speciality: ");
    scanf("%s", student.speciality);

    return student; // возвращаем считанную структуру
}

// вывод шапки таблицы
void print_student_header() {
    printf("+------------------+------------------+------------------+------------------+---------------------------+----------------------+\n");
    printf("|        id        |     surname      |       name       |    patronymic    |          faculty          |      speciality      |\n");
    printf("+------------------+------------------+------------------+------------------+---------------------------+----------------------+\n");
}
// вывод структуры
void print_student(student_t student) {
    printf("| %16s ", student.id);
    printf("| %16s ", student.surname);
    printf("| %16s ", student.name);
    printf("| %16s ", student.patronymic);
    printf("| %25s ", student.faculty);
    printf("| %20s ", student.speciality);
    printf("|\n");
}
// вывод низа таблицы
void print_student_footer() {
    printf("+------------------+------------------+------------------+------------------+---------------------------+----------------------+\n");
}

// получение индекса студента по номеру зачётной книжки
int get_index_by_id(student_t* students, int n, char* id) {
    // ищем книгу по номеру
    for (int i = 0; i < n; i++)
        if (!strcmp(id, students[i].id))
            return i;

    return -1; // не нашли
}

// добавление новой записи
student_t* add_student(student_t* students, int* n) {
    student_t student = read_student(); 

    if (get_index_by_id(students, *n, student.id) != -1) { // если студент с таким номером уже есть
        printf("student with this id already include\n"); 
        return students; // возвращаем исходный массив
    }

    students = (student_t*)realloc(students, (*n + 1) * sizeof(student_t)); // перераспределяем память
    printf("Enter information about new student\n");
    students[(*n)++] = student; // записываем книгу в массив

    return students; // возвращаем обновлённый массив
}

// удаление студента по номеру
void remove_student_by_id(student_t* students, int* n) {
    if (n == 0) { // если ещё нет записей
        printf("No book was added yet\n");
        return; 
    }

    char id[16];
    printf("Enter id: ");
    scanf("%s", id); 

    int i = get_index_by_id(students, *n, id); // ищем студента по номеру

    // если не нашли
    if (i == -1) {
        printf("No students with id '%s'\n", id);
        return;
    }

    (*n)--; // уменьшаем размер

    // сдвигаем студентов влево
    for (; i < *n; i++)
        students[i] = students[i + 1];
}

// редактирование записи
void edit_students(student_t* students, int n) {
    if (n == 0) { // если ещё нет записей
        printf("No record was added yet\n");
        return; 
    }

    char id[16];
    printf("Enter id: ");
    scanf("%s", id);

    int index = get_index_by_id(students, n, id); // ищем студента

    if (index == -1) {
        printf("No such student\n");
        return;
    }

    printf("Enter new data for this student:\n");
    students[index] = read_student(); // считываем новые данные
}

// поиск записей по заданному полю
void find_students_by_field(student_t* students, int n) {
    if (n == 0) { // если ещё нет записей
        printf("No record was added yet\n");
        return; 
    }

    char id[16];
    printf("Enter id: ");
    scanf("%s", id);

    int index = get_index_by_id(students, n, id); // ищем студента

    if (index == -1) {
        printf("No such student\n");
        return;
    }

    print_student_header();
    print_student(students[index]); // выводим студента
    print_student_footer();
}

// сохранение записей в текстовый файл
void save_students(char *students_path, student_t* students, int n) {
    FILE* fstudent = fopen(students_path, "w"); 

    for (int i = 0; i < n; i++) {
        fprintf(fstudent, "%s;", students[i].id);
        fprintf(fstudent, "%s;", students[i].surname);
        fprintf(fstudent, "%s;", students[i].name);
        fprintf(fstudent, "%s;", students[i].patronymic);
        fprintf(fstudent, "%s;", students[i].faculty);
        fprintf(fstudent, "%s", students[i].speciality);

        if (i < n - 1)
            fprintf(fstudent, "\n");
    }

    fclose(fstudent); 
}

// загрузка записей из текстового файла
student_t* load_students(char *students_path, int* n) {
    FILE* fstudent = fopen(students_path, "r"); 

    if (!fstudent) { // если не удалось открыть файл
        *n = 0; // обнуляем количество
        return NULL; // возвращаем пустой указатель
    }

    student_t* students = (student_t*)malloc(sizeof(student_t)); // выделяем память под один элемент
    *n = 0; // изначально элементов нет

    while (!feof(fstudent)) {
        fscanf(fstudent, "%[^;]%*c", students[*n].id);
        fscanf(fstudent, "%[^;]%*c", students[*n].surname);
        fscanf(fstudent, "%[^;]%*c", students[*n].name);
        fscanf(fstudent, "%[^;]%*c", students[*n].patronymic);
        fscanf(fstudent, "%[^;]%*c", students[*n].faculty);
        fscanf(fstudent, "%[^\n]", students[*n].speciality);

        *n = *n + 1; // увеличиваем размер
        students = (student_t*)realloc(students, (*n + 1) * sizeof(student_t)); // перераспределяем память
    }

    fclose(fstudent); 
    return students; // возвращаем считанный массив
}

//-----------------------------------РАБОТА С ЛОГИНОМ И ПАРОЛЕМ-----------------------------

user_t* load_users(char* user_path, int* n) { // загрузка пользователей из файла
    FILE* fuser = fopen(user_path, "r");

    if (fuser == 0) { // если не удалось открыть файл
        printf("Couldn't open file '%s'\n", user_path);
        *n = 0;
        return NULL; // возвращаем пустой указатель
    }

    user_t* users = (user_t*)malloc(sizeof(user_t)); // выделяем память под один элемент
    *n = 0; // изначально элементов нет

    while (feof(fuser) == 0) {
        fscanf(fuser, "%[^;]%*c", users[*n].login);
        fscanf(fuser, "%[^;]%*c;", users[*n].password);
        fscanf(fuser, "%d%*c", &users[*n].st);
        fscanf(fuser, "%d%*c", &users[*n].bk);

        *n = *n + 1; // увеличиваем размер
        users = (user_t*)realloc(users, (*n + 1) * sizeof(user_t)); // перераспределяем память
    }

    fclose(fuser);
    return users; // возвращаем считанный массив
}


int main() {
    int m = 0, n = 0, k = 0; // количество элементов в массиве
    char log[15], pass[15]; 

    student_t* students = load_students("students.csv", &m); // массив студентов
    book_t* books = load_books("books.csv", &n); // массив книг
    user_t* users = load_users("users.csv", &k); // массив пользователей
    int lvl = 0; //уровень доступа к БД 
    int admlvl = 0; //уровень доступа к двум БД
    int punktst, punktb, punktadm; // выбираемый пункт меню

    printf("Enter login: ");
    scanf("%[^\n]%*c", log); 
    printf("Enter password: ");
    scanf("%[^\n]%*c", pass); 
    printf("\n");

    for (int i = 0; i < k; i++) {
        if ((strcmp(users[i].login, log) == 0) && (strcmp(users[i].password, pass) == 0) && (users[i].st == 1) && (users[i].bk == 0)) { // доступ только к БД со студентами
            lvl = 1;
            break;
        }

        else if ((strcmp(users[i].login, log) == 0) && (strcmp(users[i].password, pass) == 0) && (users[i].st == 0) && (users[i].bk == 1)) {
            lvl = 2;
            break;
        }

        else if ((strcmp(users[i].login, log) == 0) && (strcmp(users[i].password, pass) == 0) && (users[i].st == 1) && (users[i].bk == 1)) {
            admlvl = 1;
            break;
        }
    }

    if ((lvl == 0) && (admlvl == 0)) {
        printf("This user doesn't exist\n");

        free(users);
        free(students);
        free(books);
        return 0;
    }
    while (1) {
        if ((admlvl == 1) && (lvl == 0)) {
            printf("Menu\n");
            printf("1. Students\n");
            printf("2. Books\n");
            printf("Enter > ");
            scanf("%d%*c", &punktadm); 

            while ((punktadm < 1) || (punktadm > 2)) { 
                printf("Incorrect item, try again: ");
                scanf("%d%*c", &punktadm);
            }

            switch (punktadm) {
            case 1: 
                lvl = 1;
                break;

            case 2: 
                lvl = 2;
                break;
            }
            printf("\n");
        }
        if (lvl == 1) {
            do {
                printf("What do you want to do?\n");
                printf("1. Add student\n");
                printf("2. Remove by id\n");
                printf("3. Edit by id\n");
                printf("4. View by id\n");
                printf("5. Exit\n");
                if (admlvl == 1) {
                    printf("0. Exit to the main menu\n");
                }
                printf(">");
                scanf("%d%*c", &punktst); 

                // обрабатываем некорректный ввод пункта меню
                while (((punktst < 1 || punktst > 5)) && (punktst != 0)) {
                    printf("Incorrect item, try again: ");
                    scanf("%d%*c", &punktst); 
                }

                switch (punktst) {
                case 1:
                    students = add_student(students, &n); // добавляем новую запись
                    break;

                case 2:
                    remove_student_by_id(students, &n);
                    break;

                case 3:
                    edit_students(students, n);  // редактируем запись
                    break;

                case 4:
                    find_students_by_field(students, n);
                    break;

                case 5:
                    save_students("students.csv", students, m); // сохраняем студентов в файл
                    free(users);
                    free(students);
                    free(books);
                    return 0;
                }

                printf("\n");
            } while (punktst != 0);
            lvl = 0;
        }

        if (lvl == 2) {

            do {
                printf("What do you want to do?\n");
                printf("1. Add book\n");
                printf("2. Remove by id\n");
                printf("3. Print by id\n");
                printf("4. Print books\n");
                printf("5. Exit\n");
                if (admlvl == 1) {
                    printf("0. Exit to the main menu\n");
                }
                printf(">");
                scanf("%d%*c", &punktb); 

                // обрабатываем некорректный ввод пункта меню
                while (((punktb < 1 || punktb > 5)) && (punktb != 0)) {
                    printf("Incorrect item, try again: ");
                    scanf("%d%*c", &punktb); 
                }

                switch (punktb) {
                case 1:
                    books = add_book(books, &n); // добавляем новую запись
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
                        
                case 5:
                    save_books("books.csv", books, n); // сохраняем книги в файл
                    free(users);
                    free(students);
                    free(books);
                    return 0;
                }
                printf("\n");
            } while (punktb != 0);
            lvl = 0;
        }
    }
}
