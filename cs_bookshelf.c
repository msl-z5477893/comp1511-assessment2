// CS BOOKSHELF
//
// This program was written by Michael Stephen Lape (z5477893)
// on 2023-07-18
//
// Bookshelf software idk

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      CONSTANTS      ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define MAX_STR_LEN 32
#define MAX_RATING 5
#define MIN_RATING 1

// Provided enums
enum book_genre { CLASSICS, FANTASY, MYSTERY, NON_FICTION, SCI_FI, INVALID };

// TODO: Your #defines/enums can go here:
#define MAX_CLI_CHARS 256
#define CMD_BOOK_APPEND -999999

enum book_add_type { APPEND, INSERT };
enum shelf_find_total { TOTAL_PAGES, TOTAL_READ };
enum shelf_ptr_move { NEXT, PREVIOUS };

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct shelf {
    // the name of the shelf
    char name[MAX_STR_LEN];
    // pointer to the list of books on the shelf
    struct book *books;
    // pointer to the next shelf
    struct shelf *next;
};

struct book {
    // title of the book
    char title[MAX_STR_LEN];
    // author of the book
    char author[MAX_STR_LEN];
    // genre of the book
    enum book_genre genre;
    // rating of book out of 5
    int rating;
    // number of pages in the book
    int pages_count;
    // number of pages read
    int pages_read;

    struct book *next;
};

// TODO: Any additional structs you want to add can go here:

struct genre_grouping {
    enum book_genre genre;
    int count;
    struct genre_grouping *next;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void print_usage(void);
void print_book(struct book *book);
void scan_title_author(char title[MAX_STR_LEN], char author[MAX_STR_LEN]);
void scan_string(char string[MAX_STR_LEN]);
enum book_genre scan_genre(void);
void print_reading_stats(double average_rating, int total_read, int total_pages,
                         enum book_genre longest_grouping, int grouping_length);
void print_shelf_summary(int is_selected, int n, char name[MAX_STR_LEN],
                         int num_books);
void genre_to_string(enum book_genre genre, char genre_string[MAX_STR_LEN]);

// Additional provided function prototypes
// You won't need to use these functions!
// We use just them to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
enum book_genre string_to_genre(char *genre_str);

////////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct shelf *create_shelf(char name[MAX_STR_LEN]);
struct book *create_book(char title[MAX_STR_LEN], char author[MAX_STR_LEN],
                         enum book_genre genre, int rating, int pages_count);
// TODO: Put your function prototypes here
char *cli(char *);
int proc_cmd(char *, char *, struct shelf **, struct shelf **);

// function prototypes for handling commandline functions
void cmd_add_book(char *, enum book_add_type, struct shelf *);
void cmd_print_bookshelf(struct shelf *);
void cmd_shelf_count_books(struct shelf *);
void cmd_read_pages(char *, struct shelf *);
void cmd_show_read_stats(struct shelf *);
void cmd_add_shelf(struct shelf **, char *);
void cmd_switch_shelf(struct shelf **, struct shelf *, enum shelf_ptr_move);
void cmd_print_shelves(struct shelf *, struct shelf *);

// user defined helper functions
struct book *book_eol(struct book *);
void add_to_shelf(struct shelf *, struct book *, int);
struct book *check_book_in_shelf(struct shelf *, char *, char *);
int shelf_totals(struct book *, enum shelf_find_total, int);
double shelf_rating_avg(struct shelf *);
struct genre_grouping *get_book_groupings(struct shelf *);
struct genre_grouping *add_book_grouping(struct book *,
                                         struct genre_grouping *);
void free_genre_grouping(struct genre_grouping *);
struct shelf *get_shelf(struct shelf *, char *);
int shelf_book_count(struct shelf *);
void free_all(struct shelf *);
void free_book_in_shelves(struct book *);
void list_place_shelf(struct shelf **, struct shelf *);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    char *commandline, *loop_check;
    unsigned int buf_size;
    int running;
    struct shelf *shelf_list_head, *shelf_list_ptr;

    // init head shelf
    char sl_name[MAX_STR_LEN] = "tbr";
    shelf_list_head = create_shelf(sl_name);
    shelf_list_ptr = shelf_list_head;

    // init cli buffer
    buf_size = MAX_CLI_CHARS * sizeof(char);
    commandline = malloc(buf_size);
    loop_check = NULL;

    // start command loop
    running = 1;
    printf("Welcome to Bookshelf Manager!\n");
    while (running) {
        printf("Enter command: ");
        loop_check = cli(commandline);
        running =
            proc_cmd(commandline, loop_check, &shelf_list_head, &shelf_list_ptr);
    }

    // free memory
    free(commandline);
    free_all(shelf_list_head);

    printf("\nGoodbye\n");

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// STAGE 1.1

// allocates a shelf in memory
//
// Parameters:
// void free_book_in_shelves(struct book *);
//      name (char[]): name of shelf
// Returns:
//      struct shelf *
//
struct shelf *create_shelf(char name[MAX_STR_LEN]) {

    // STAGE 1.1
    // TODO: malloc, initialise, and return a new shelf.

    // hint: you will have to replace NULL in this return statement.

    struct shelf *new_shelf;

    new_shelf = malloc(sizeof(struct shelf));
    strcpy(new_shelf->name, name);
    new_shelf->books = NULL;
    new_shelf->next = NULL;

    return new_shelf;
}

// STAGE 1.1

// allocates book in memory
//
// Parameters:
//      title (char[]): title of book
//      author (char[]): author of book
// Returns:
//      struct book *
//
struct book *create_book(char title[MAX_STR_LEN], char author[MAX_STR_LEN],
                         enum book_genre genre, int rating, int pages_count) {

    // STAGE 1.1
    // TODO: malloc, initialise, and return a new book.

    // hint: you will have to replace NULL in this return statement.

    struct book *new_book = malloc(sizeof(struct book));

    strcpy(new_book->title, title);
    strcpy(new_book->author, author);
    new_book->genre = genre;
    new_book->rating = rating;
    new_book->pages_read = 0;
    new_book->pages_count = pages_count;
    new_book->next = NULL;

    return new_book;
}

// STAGE 1.2

// get user input
// Returns:
//     char *
char *cli(char *i_buffer) {
    char *fgets_res;

    fgets_res = fgets(i_buffer, MAX_CLI_CHARS, stdin);
    return fgets_res;
}

// processes cli command
// Parameters:
//     cli_input (char *): command entered by user
//     head_shelf (struct shelf **): head of list of shelves
//     current_shelf (struct shelf **): shelf to operate on
int proc_cmd(char *cli_input, char *loopback, struct shelf **head_shelf,
             struct shelf **current_shelf) {
    char cmd_char, *args;

    // return false (0) if ctrl-d
    if (loopback == NULL) {
        return 0;
    }

    args = malloc(MAX_CLI_CHARS);

    sscanf(cli_input, "%c %[^\n]", &cmd_char, args);

    // if command is given do any of the ff.:
    if (cmd_char == '?') {
        print_usage();
    }

    // book operations
    if (cmd_char == 'a') {
        cmd_add_book(args, APPEND, *current_shelf);
    }
    if (cmd_char == 'p') {
        cmd_print_bookshelf(*current_shelf);
    }
    if (cmd_char == 'c') {
        cmd_shelf_count_books(*current_shelf);
    }
    if (cmd_char == 'i') {
        cmd_add_book(args, INSERT, *current_shelf);
    }
    if (cmd_char == 'r') {
        cmd_read_pages(args, *current_shelf);
    }
    if (cmd_char == 's') {
        cmd_show_read_stats(*current_shelf);
    }

    // shelf operations
    if (cmd_char == 'A') {
        cmd_add_shelf(head_shelf, args);
    }
    if (cmd_char == '>') {
        cmd_switch_shelf(current_shelf, *head_shelf, NEXT);
    }
    if (cmd_char == '<') {
        cmd_switch_shelf(current_shelf, *head_shelf, PREVIOUS);
    }
    if (cmd_char == 'P') {
        cmd_print_shelves(*head_shelf, *current_shelf);
    }

    // if command is successfully parsed return true (1)
    free(args);
    return 1;
}

// STAGE 1.3
// STAGE 2.1 -- modified
// STAGE 2.2 -- modified

// creates a book and appends it to head shelf.
// Parameters:
//     data (char *): provided data
//     position (int): book position, appended if -1
//     ptr_shelf (struct shelf *): shelf to operate on
void cmd_add_book(char *data, enum book_add_type method_type,
                  struct shelf *ptr_shelf) {
    struct book *new_book;
    enum book_genre genre;
    int rating, pages_count, pos;

    char title[MAX_STR_LEN] = "";
    char author[MAX_STR_LEN] = "";
    char str_genre[MAX_STR_LEN] = "";

    if (method_type == APPEND) {
        pos = 999999;
        sscanf(data, " %s %s %s %d %d", title, author, str_genre, &rating,
               &pages_count);
    }
    if (method_type == INSERT) {
        sscanf(data, " %d %s %s %s %d %d", &pos, title, author, str_genre,
               &rating, &pages_count);
    }

    genre = string_to_genre(str_genre);

    // error checking
    if (pos < 0) {
        printf("ERROR: n must be at least 0\n");
        return;
    }
    if (genre == INVALID) {
        printf("ERROR: Invalid book genre\n");
        return;
    }
    if (rating < 1 || rating > 5) {
        printf("ERROR: Rating should be between 1 and 5\n");
        return;
    }
    if (pages_count < 1) {
        printf("ERROR: Page count should be positive\n");
        return;
    }
    if (check_book_in_shelf(ptr_shelf, title, author) != NULL) {
        printf("ERROR: a book with title: '%s' by '%s' already exists in this "
               "shelf\n",
               title, author);
        return;
    }

    new_book = create_book(title, author, genre, rating, pages_count);
    add_to_shelf(ptr_shelf, new_book, pos);
    if (method_type == APPEND) {
        printf("Book: '%s' added!\n", new_book->title);
    }
    if (method_type == INSERT) {
        printf("Book: '%s' inserted!\n", new_book->title);
    }
}

// helper function for adding book to shelf
void add_to_shelf(struct shelf *used_shelf, struct book *added_book,
                  int position) {
    // struct book *temp;
    struct book *first_book = used_shelf->books;
    int node_index;

    // if there is no books in the shelf
    if (first_book == NULL) {
        first_book = added_book;
        // just to make sure
        used_shelf->books = first_book;
        return;
    }

    // put book on start of list
    if (position == 0 && first_book != NULL) {
        added_book->next = first_book;
        used_shelf->books = added_book;
        return;
    }
    // iter through shelf linked list till we either hit the end or we reach
    // position
    node_index = 0;
    while (node_index < position - 1) {
        if (first_book->next == NULL) {
            break;
        }
        first_book = first_book->next;
        node_index += 1;
    }

    if (first_book->next == NULL) {
        first_book->next = added_book;
        return;
    }

    // if we hit mid position
    added_book->next = first_book->next;
    first_book->next = added_book;
}

// helper function for checking if book exists
struct book *check_book_in_shelf(struct shelf *shelf_checked, char *title,
                                 char *author) {
    struct book *current_book;

    current_book = shelf_checked->books;

    while (current_book != NULL) {
        if (!strcmp(current_book->title, title) &&
            !strcmp(current_book->author, author)) {
            return current_book;
        }
        current_book = current_book->next;
    }

    return NULL;
}

// STAGE 1.4

// print all books in selected bookshelf
// Parameters:
//     ptr_shelf (struct shelf *): selected shelf
void cmd_print_bookshelf(struct shelf *ptr_shelf) {
    struct book *current_book;

    if (ptr_shelf->books == NULL) {
        printf("There are no books on this shelf!\n");
        return;
    }
    current_book = ptr_shelf->books;
    while (1) {
        if (current_book == NULL) {
            return;
        }
        print_book(current_book);
        current_book = current_book->next;
    }
}

// STAGE 1.5

// count books in selected shelf
// Parameters:
//     ptr_shelf (struct shelf *): selected shelf
void cmd_shelf_count_books(struct shelf *ptr_shelf) {
    unsigned int count;
    struct book *next_book;
    if (ptr_shelf->books == NULL) {
        printf("There are no books on this shelf!\n");
        return;
    }
    if (ptr_shelf->books->next == NULL) {
        printf("There is 1 book on this shelf!\n");
        return;
    }
    count = 1;
    next_book = ptr_shelf->books->next;
    while (1) {
        if (next_book == NULL) {
            printf("There are %d books on this shelf!\n", count);
            return;
        }
        next_book = next_book->next;
        count += 1;
    }
}

// STAGE 2.3

// read book
void cmd_read_pages(char *data, struct shelf *current_shelf) {
    int remaining_pages, pages_to_read;
    struct book *book_to_read;

    char title[MAX_STR_LEN] = "";
    char author[MAX_STR_LEN] = "";

    sscanf(data, " %s %s %d", title, author, &pages_to_read);

    if (pages_to_read < 1) {
        printf("ERROR: n must be a positive integer\n");
        return;
    }

    book_to_read = check_book_in_shelf(current_shelf, title, author);
    if (book_to_read == NULL) {
        printf("ERROR: No book '%s' by '%s' exists\n", title, author);
        return;
    }

    remaining_pages = book_to_read->pages_count - book_to_read->pages_read;
    if (remaining_pages < pages_to_read) {
        printf("ERROR: cannot read %d pages, there are only %d left to read\n",
               pages_to_read, remaining_pages);
        return;
    }

    book_to_read->pages_read += pages_to_read;
    printf("Pages read for '%s' has been increased by %d\n", title,
           pages_to_read);
}

// STAGE 2.4

// show read stats
void cmd_show_read_stats(struct shelf *current_shelf) {
    int total_read, total_pages;
    struct genre_grouping *group;
    double average_rating;

    if (current_shelf->books == NULL) {
        printf("No books on this shelf, so no stats to display!\n");
        return;
    }

    total_read = shelf_totals(current_shelf->books, TOTAL_READ, 0);
    total_pages = shelf_totals(current_shelf->books, TOTAL_PAGES, 0);
    average_rating = shelf_rating_avg(current_shelf);
    group = get_book_groupings(current_shelf);

    print_reading_stats(average_rating, total_read, total_pages, group->genre,
                        group->count);

    free(group);
}

// helper function for getting the average rating of books in a shelf
double shelf_rating_avg(struct shelf *current_shelf) {
    double sum;
    int count;
    struct book *current_book;

    sum = 0;
    count = 0;
    current_book = current_shelf->books;
    while (current_book != NULL) {
        sum += current_book->rating;
        count += 1;
        current_book = current_book->next;
    }

    return sum / count;
}

// helper function for getting either the total amount of pages or the total
// amount of pages read
// the function is recursive
int shelf_totals(struct book *current_book, enum shelf_find_total select_stat,
                 int value) {
    // if we finish going through the list return the final total
    if (current_book == NULL) {
        return value;
    }

    // traverse through the list a add all pages
    if (select_stat == TOTAL_PAGES) {
        return shelf_totals(current_book->next, TOTAL_PAGES,
                            value + current_book->pages_count);
    }
    if (select_stat == TOTAL_READ) {
        return shelf_totals(current_book->next, TOTAL_READ,
                            value + current_book->pages_read);
    }

    // if something weird comes up
    return value;
}

// return the biggest grouping
struct genre_grouping *get_book_groupings(struct shelf *s_shelf) {
    struct book *current_book;
    struct genre_grouping *biggest_group, *gl_head, *gl_ptr;

    current_book = s_shelf->books;
    biggest_group = malloc(sizeof(struct genre_grouping));
    biggest_group->next = NULL;
    gl_head = NULL;

    // create list of book groupings
    while (current_book != NULL) {
        gl_head = add_book_grouping(current_book, gl_head);
        current_book = current_book->next;
    }

    // get the biggest group in list
    biggest_group->genre = gl_head->genre;
    biggest_group->count = gl_head->count;
    gl_ptr = gl_head;
    while (gl_ptr != NULL) {
        if (gl_ptr->count > biggest_group->count) {
            biggest_group->genre = gl_ptr->genre;
            biggest_group->count = gl_ptr->count;
        }
        gl_ptr = gl_ptr->next;
    }

    free_genre_grouping(gl_head);

    return biggest_group;
}

// create a list of books
// return head of book group list
struct genre_grouping *add_book_grouping(struct book *sel_book,
                                         struct genre_grouping *head) {
    enum book_genre genre;
    struct genre_grouping *genre_group, *list_ptr;

    genre = sel_book->genre;

    // start the list if no list
    if (head == NULL) {
        genre_group = malloc(sizeof(struct genre_grouping));
        genre_group->genre = genre;
        genre_group->count = 1;
        genre_group->next = NULL;
        head = genre_group;
        return head;
    }

    // traverse to end
    list_ptr = head;
    while (list_ptr->next != NULL) {
        list_ptr = list_ptr->next;
    }

    // if same genre, add to book group
    if (list_ptr->genre == genre) {
        list_ptr->count += 1;
    } else {
        genre_group = malloc(sizeof(struct genre_grouping));
        genre_group->genre = genre;
        genre_group->count = 1;
        genre_group->next = NULL;
        list_ptr->next = genre_group;
    }

    return head;
}

// free genre_grouping list
void free_genre_grouping(struct genre_grouping *head) {
    struct genre_grouping *free_ptr;
    while (head != NULL) {
        free_ptr = head;
        head = head->next;
        free(free_ptr);
    }
}

// STAGE 3.1

// add a new shelf
void cmd_add_shelf(struct shelf **head_shelf, char *args) {
    struct shelf /**s_list_ptr,*/ *new_shelf;

    char name[MAX_STR_LEN] = "";
    sscanf(args, " %s", name);

    if (get_shelf(*head_shelf, name) != NULL) {
        printf("ERROR: a shelf with name '%s' already exists\n", name);
        return;
    }

    new_shelf = create_shelf(name);

    list_place_shelf(head_shelf, new_shelf);

}

// helper function for appending shelf to linked list
// the list must be arranged in alphabetical order
void list_place_shelf(struct shelf **pre_shelf, struct shelf *add_shelf) {

    // if add_shelf is lesser than pre_shelf in lexicographical order
    if (strcmp(add_shelf->name, (*pre_shelf)->name) < 0) {
        add_shelf->next = *pre_shelf;
        *pre_shelf = add_shelf;
        return;
    }

    // appending to the end of list
    if ((*pre_shelf)->next == NULL) {
        (*pre_shelf)->next = add_shelf;
        return;
    }

    // if neither applies iterate
    return list_place_shelf(&(*pre_shelf)->next, add_shelf);
}

// helper function for finding a shelf
struct shelf *get_shelf(struct shelf *head_shelf, char *name) {
    struct shelf *list_ptr;

    list_ptr = head_shelf;
    while (list_ptr != NULL) {
        if (!strcmp(list_ptr->name, name)) {
            return list_ptr;
        }
        list_ptr = list_ptr->next;
    }
    return NULL;
}

// STAGE 3.2

// go to next or previous shelf
void cmd_switch_shelf(struct shelf **current_shelf, struct shelf *head_shelf,
                      enum shelf_ptr_move direction) {
    struct shelf *list_ptr;

    if (direction == NEXT) {
        if ((*current_shelf)->next == NULL) {
            *current_shelf = head_shelf;
            return;
        }
        *current_shelf = (*current_shelf)->next;
        return;
    }

    // when direction == PREVIOUS
    list_ptr = head_shelf;
    while (list_ptr->next != NULL) {
        // if list_ptr points to the shelf before the current shelf
        // set current_shelf as list_ptr
        //
        // note that if current_shelf == head_shelf
        // this condition wont be fulfilled
        if (!strcmp(list_ptr->next->name, (*current_shelf)->name)) {
            *current_shelf = list_ptr;
            return;
        }
        list_ptr = list_ptr->next;
    }
    // set tail as current_shelf if we PREVIOUS from head_shelf
    *current_shelf = list_ptr;
}

// STAGE 3.3

// print all shelves
void cmd_print_shelves(struct shelf *head_shelf, struct shelf *current_shelf) {
    struct shelf *ptr;
    unsigned int count;
    ptr = head_shelf;
    count = 0;
    while (1) {
        if (ptr == NULL) {
            return;
        }
        print_shelf_summary(!strcmp(ptr->name, current_shelf->name), count,
                            ptr->name, shelf_book_count(ptr));
        count += 1;
        ptr = ptr->next;
    }
}

// helper function for counting books
int shelf_book_count(struct shelf *current_shelf) {
    struct book *book_ptr;
    int count;

    if (current_shelf->books == NULL) {
        return 0;
    }

    count = 1;
    book_ptr = current_shelf->books;
    while (book_ptr->next != NULL) {
        count += 1;
        book_ptr = book_ptr->next;
    }

    return count;
}

// free functions
// for freeing all used memory after running the program

void free_all(struct shelf *head_shelf) {
    struct shelf *ptr, *trash;
    ptr = head_shelf;
    while (ptr != NULL) {
        free_book_in_shelves(ptr->books);
        trash = ptr;
        ptr = ptr->next;
        free(trash);
    }
}
void free_book_in_shelves(struct book *head) {
    struct book *ptr, *trash;
    ptr = head;
    while (ptr != NULL) {
        trash = ptr;
        ptr = ptr->next;
        free(trash);
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////  PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Prints the Bookshelf Manager usage instructions,
// displaying the different commands and their arguments.
//
void print_usage(void) {
    printf(
        "=====================[  Bookshelf Manager ]=====================\n"
        "      ===============[     Usage Info     ]===============      \n"
        "  ?                                                             \n"
        "    Show help                                                   \n"
        "  a [title] [author] [genre] [rating] [pages_count]             \n"
        "    Add a book to the shelf                                     \n"
        "  p                                                             \n"
        "    Print out all of the books on the shelf                     \n"
        "  c                                                             \n"
        "    Count all of the books on the shelf                         \n"
        "                                                                \n"
        "  i [n] [title] [author] [genre] [rating] [page_count]          \n"
        "    Insert a book into the shelf at position `n`                \n"
        "  r [title] [author] [n]                                        \n"
        "    Read `n` pages of the book with matching `title` and        \n"
        "    `author`                                                    \n"
        "  s                                                             \n"
        "    Display the reading stats                                   \n"
        "                                                                \n"
        "  A [name]                                                      \n"
        "    Add a new empty shelf in alphabetical order                 \n"
        "  >                                                             \n"
        "    Select the next shelf in the shelf list.                    \n"
        "  <                                                             \n"
        "    Select the previous shelf in the shelf list.                \n"
        "  P                                                             \n"
        "    Print out the shelves list                                  \n"
        "  d [title] [author]                                            \n"
        "    Delete book with matching `title` and `author` from         \n"
        "    the selected shelf.                                         \n"
        "  D                                                             \n"
        "    Remove the selected shelf.                                  \n"
        "                                                                \n"
        "  G [genre]                                                     \n"
        "    Create a new shelf with all the books of `genre`            \n"
        "  S [pattern]                                                   \n"
        "    Search for titles that match `pattern` across all shelves   \n"
        "================================================================\n");
}

// Formats and prints out a book struct,
//
// Parameters:
//      book - The struct book to print.
//
void print_book(struct book *book) {
    char genre_string[MAX_STR_LEN];
    genre_to_string(book->genre, genre_string);

    printf(" ---------------------------------- \n");
    printf("| %-32s |\n", book->title);
    printf("| %-32s |\n", book->author);
    printf("| %-32s |\n", genre_string);
    printf("| Rating: %d/%d     Pages: %4d/%4d |\n", book->rating, MAX_RATING,
           book->pages_read, book->pages_count);
    printf(" -----------------V---------------- \n");
}

// Scan in the title and authors strings into the provided char array, placing
// '\0' at the end.
//
// Parameters:
//      title  - a char array of length MAX_STR_LEN, which will be used
//              to store the title.
//      author - a char array of length MAX_STR_LEN, which will be used
//              to store the author.
//
// Usage:
// ```
//      char title[MAX_STR_LEN];
//      char author[MAX_STR_LEN];
//      scan_title_author(title, author);
// ```
void scan_title_author(char title[MAX_STR_LEN], char author[MAX_STR_LEN]) {
    scan_token(title, MAX_STR_LEN);
    scan_token(author, MAX_STR_LEN);
}

// Scan in a string into the provided char array, placing '\0' at the end.
//
// Parameters:
//      string  - a char array of length MAX_STR_LEN, which will be used
//              to store the string.
//
// Usage:
// ```
//      char string[MAX_STR_LEN];
//      scan_string(string);
// ```
void scan_string(char string[MAX_STR_LEN]) { scan_token(string, MAX_STR_LEN); }

// Scans a string and converts it to a genre.
//
// Returns:
//      The corresponding genre, if the string was valid,
//      Otherwise, returns INVALID.
//
// Usage:
// ```
//      enum genre genre = scan_genre();
// ```
//
enum book_genre scan_genre(void) {
    char genre[MAX_STR_LEN];
    scan_token(genre, MAX_STR_LEN);
    return string_to_genre(genre);
}

// Formats and prints out the reading statistics
//
// Parameters:
//      average_rating  - The average rating of all books in selected shelf
//      total_read      - The total pages read for books on selected shelf.
//      total_pages     - The total pages for books on selected shelf.
//      longest_grouping  - The longest grouping of adjacent genres
//      grouping_length  - The length of the longest genre grouping
//
void print_reading_stats(double average_rating, int total_read, int total_pages,
                         enum book_genre longest_grouping,
                         int grouping_length) {
    char genre_name[MAX_STR_LEN];
    genre_to_string(longest_grouping, genre_name);
    printf("  Average Rating    :  %.2lf / %d\n", average_rating, MAX_RATING);
    printf("  Pages Read        : %5d / %5d\n", total_read, total_pages);
    printf("  Longest Grouping  : %s of length %d\n", genre_name,
           grouping_length);
}

// Formats and prints out various information about a given shelf.
//
// Parameters:
//      is_selected - 1, if this shelf is the currently selected shelf,
//                    0, otherwise.
//      n           - The position of the given shelf in the list of shelves,
//                    starting from 0.
//      name        - The name of the given shelf.
//      num_books   - The number of books in the given shelf.
//
void print_shelf_summary(int is_selected, int n, char name[MAX_STR_LEN],
                         int num_books) {
    printf("     ---------------------------------- \n");
    printf("    | Shelf: %-25d |\n", n);

    if (is_selected) {
        printf("--->");
    } else {
        printf("    ");
    }

    printf("| %-32s |\n", name);
    printf("    | Books: %-25d |\n", num_books);
    printf("     -----------------V---------------- \n");
}

// Get a string from an enum book_genre
//
// Parameters:
//      genre           - the genre to be converted to a string
//      genre_string    - the char array the name of genre should be written to
//
void genre_to_string(enum book_genre genre, char genre_string[MAX_STR_LEN]) {
    if (genre == CLASSICS) {
        strcpy(genre_string, "CLASSICS");
    } else if (genre == FANTASY) {
        strcpy(genre_string, "FANTASY");
    } else if (genre == MYSTERY) {
        strcpy(genre_string, "MYSTERY");
    } else if (genre == NON_FICTION) {
        strcpy(genre_string, "NON_FICTION");
    } else if (genre == SCI_FI) {
        strcpy(genre_string, "SCI_FI");
    } else {
        strcpy(genre_string, "INVALID");
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////// ADDITIONAL PROVIDED FUNCTIONS ///////////////////////////
////////////////////////////////////////////////////////////////////////////////
// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

// Convert a string into an enum book_genre
//
// Parameters:
//      genre_str   - a string version of the genre
// Returns:
//      enum book_genre
//
enum book_genre string_to_genre(char *genre_str) {
    int len = strlen(genre_str);

    if (strncasecmp(genre_str, "classics", len) == 0) {
        return CLASSICS;
    }
    if (strncasecmp(genre_str, "fantasy", len) == 0) {
        return FANTASY;
    }
    if (strncasecmp(genre_str, "mystery", len) == 0) {
        return MYSTERY;
    }
    if (strncasecmp(genre_str, "non_fiction", len) == 0) {
        return NON_FICTION;
    }
    if (strncasecmp(genre_str, "sci_fi", len) == 0) {
        return SCI_FI;
    }

    return INVALID;
}

// Scan a token into the provided buffer of buffer_size
//
// Parameters:
//      buffer      - the buffer being scanned into
//      buffer_size - the size of buffer
// Returns:
//      the number of chars scanned
//
int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    // consume all leading whitespace
    scanf(" ");

    // Scan in characters until whitespace
    while (i < buffer_size - 1 && (num_scanned = scanf("%c", &c)) == 1 &&
           !isspace(c)) {

        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}
