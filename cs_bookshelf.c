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
int proc_cmd(char *, char *, struct shelf *);

// function prototypes for handling commandline functions
void cmd_add_book(char *, int, struct shelf *);
void cmd_print_bookshelf(struct shelf *);
void cmd_shelf_count_books(struct shelf *);

// user defined helper functions
struct book *book_eol(struct book *);
void add_to_shelf(struct shelf *, struct book *, int);
int check_book_in_shelf(struct shelf *, char *, char *);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    char *commandline, *loop_check;
    unsigned int buf_size;
    int running;
    struct shelf *head_shelf;

    // init head shelf
    char init_shelf_name[MAX_STR_LEN] = "tbr";
    head_shelf = create_shelf(init_shelf_name);

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
        running = proc_cmd(commandline, loop_check, head_shelf);
    }

    // free memory
    free(commandline);
    free(head_shelf);

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
//     shelf (struct shelf *): shelf to operate on
int proc_cmd(char *cli_input, char *loopback, struct shelf *current_shelf) {
    char cmd_char, *args;

    // return false (0) if ctrl-d
    if (loopback == NULL) {
        return 0;
    }

    args = malloc(MAX_CLI_CHARS);

    sscanf(cli_input, "%c %[^\n]", &cmd_char, args);
    // printf("%s\n", args);
    if (cmd_char == '?') {
        print_usage();
    }
    if (cmd_char == 'a') {
        cmd_add_book(args, CMD_BOOK_APPEND, current_shelf);
    }
    if (cmd_char == 'p') {
        cmd_print_bookshelf(current_shelf);
    }
    if (cmd_char == 'c') {
        cmd_shelf_count_books(current_shelf);
    }
    if (cmd_char == 'i') {
        cmd_add_book(args, 0, current_shelf);
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
void cmd_add_book(char *data, int position, struct shelf *ptr_shelf) {
    struct book *new_book;
    enum book_genre genre;
    int rating, pages_count, pos;

    char title[MAX_STR_LEN] = "";
    char author[MAX_STR_LEN] = "";
    char str_genre[MAX_STR_LEN] = "";

    if (position == CMD_BOOK_APPEND) {
        sscanf(data, " %s %s %s %d %d", title, author, str_genre, &rating,
               &pages_count);
    } else {
        sscanf(data, " %d %s %s %s %d %d", &pos, title, author, str_genre,
               &rating, &pages_count);
    }

    genre = string_to_genre(str_genre);

    // error checking
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
    if (check_book_in_shelf(ptr_shelf, title, author)) {
        printf("ERROR: a book with title: '%s' by '%s' already exists in this "
               "shelf\n",
               title, author);
        return;
    }

    new_book = create_book(title, author, genre, rating, pages_count);
    add_to_shelf(ptr_shelf, new_book, pos);
    printf("Book: '%s' added!\n", new_book->title);
}

// helper function for adding book to shelf
void add_to_shelf(struct shelf *used_shelf, struct book *added_book,
                  int position) {
    struct book *temp;
    struct book *first_book = used_shelf->books;

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
    }
    /*
    while (1) {
        if (first_book->next == NULL) {
            first_book->next = added_book;
            return;
        }
        first_book = first_book->next;
    }
    */

    // iter through shelf linked list till we either hit the end or we reach
    // position
    for (int index = 0; index < position; index++) {
        if (first_book->next == NULL) {
            first_book->next = added_book;
            return;
        }
        first_book = first_book->next;
    }

    // if we hit mid position
    if (first_book->next != NULL) {
        temp = first_book->next;
        added_book->next = temp;
        first_book = added_book;
    }
}

// helper function for checking if book exists
int check_book_in_shelf(struct shelf *shelf_checked, char *title,
                        char *author) {
    struct book *current_book;

    current_book = shelf_checked->books;

    while (current_book != NULL) {
        if (!strcmp(current_book->title, title) &&
            !strcmp(current_book->author, author)) {
            return 1;
        }
        current_book = current_book->next;
    }

    return 0;
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
