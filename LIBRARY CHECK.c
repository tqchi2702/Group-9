#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
#ifndef HASHMAP_H
#define HASHMAP_H
typedef struct HashMapNode {
    void *key;
    void *value;
    struct HashMapNode *next;
} HashMapNode;

typedef struct {
    HashMapNode **buckets;
    size_t bucket_count;
    size_t (*hash_function)(void *key);
    int (*compare_function)(void *key1, void *key2);
} HashMap;

HashMap *hashmap_create(size_t bucket_count, size_t (*hash_function)(void *), int (*compare_function)(void *, void *));
void hashmap_put(HashMap *map, void *key, void *value);
void *hashmap_get(HashMap *map, void *key);
void hashmap_free(HashMap *map);

#endif // HASHMAP_H
//#define RESET 0 
//#define RED 1 
//#define YELLOW 2 
//#define BLUE 3 
//#define BOOK_FILE "books.csv"
#define RESET "\x1b[0m"     // Đ?t l?i màu v? m?c đ?nh
#define RED "\x1b[31m"       // Màu đ?
#define YELLOW "\x1b[33m"    // Màu vàng
#define BLUE "\x1b[34m"      // Màu xanh lam
#define BOOK_FILE "books.csv" // Tên file sách


#define TABLE_SIZE 100
int current_customer_id = 1; 
static HashMapNode *create_node(void *key, void *value) {
    HashMapNode *node = (HashMapNode *)malloc(sizeof(HashMapNode));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

HashMap *hashmap_create(size_t bucket_count, size_t (*hash_function)(void *), int (*compare_function)(void *, void *)) {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (HashMapNode **)calloc(bucket_count, sizeof(HashMapNode *));
    map->bucket_count = bucket_count;
    map->hash_function = hash_function;
    map->compare_function = compare_function;
    return map;
}

void hashmap_put(HashMap *map, void *key, void *value) {
    size_t bucket_index = map->hash_function(key) % map->bucket_count;
    HashMapNode *new_node = create_node(key, value);
    HashMapNode *current = map->buckets[bucket_index];

    if (current == NULL) {
        map->buckets[bucket_index] = new_node;
        return;
    }

    while (current->next != NULL) {
        if (map->compare_function(current->key, key) == 0) {
            current->value = value;
            free(new_node);
            return;
        }
        current = current->next;
    }

    if (map->compare_function(current->key, key) == 0) {
        current->value = value;
        free(new_node);
    } else {
        current->next = new_node;
    }
}

void *hashmap_get(HashMap *map, void *key) {
    size_t bucket_index = map->hash_function(key) % map->bucket_count;
    HashMapNode *current = map->buckets[bucket_index];

    while (current != NULL) {
        if (map->compare_function(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void hashmap_free(HashMap *map) {
	size_t i; 
    for ( i = 0; i < map->bucket_count; i++) {
        HashMapNode *current = map->buckets[i];
        while (current != NULL) {
            HashMapNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}

//void setColor(int color)
//{
//	void setColor(const char* color) {
//    printf("%s", color);
////    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
////    SetConsoleTextAttribute(hConsole, color);
//
//}


struct Book {
    int book_id;
    char title[50];
    char author[30];
    int quantity;
    struct Book* next;
};

struct Customer {
    int customer_id; 
    char customer_name[30];
    char address[50];
    char phone[15];
    struct Customer* next;
};

struct borrowBook {
    int borrow_id;
    int book_id;
    char customer_name[30];
    int quantity;
    struct borrowBook* next;
};

struct returnBook {
    int borrow_id;
    struct returnBook* next;
};

struct Admin {
	char username[20];
	char password[20]; 
}; 

struct Admin users[10]={
	{"admin1", "admin1"},
	{"1", "1"},
}; 

// Hash tables for books and customers
struct Book* BookTable[TABLE_SIZE];
struct Customer* CustomerTable[TABLE_SIZE];
struct borrowBook* borrowBookTable[TABLE_SIZE];
struct returnBook* returnBookTable[TABLE_SIZE];
// Global HashMap for books
HashMap *bookMap = NULL;
// Global HashMap for borrowBooks
HashMap *borrowBookMap = NULL;

const char* FILENAME = "cus_final.csv";
// Hash function
unsigned int hash(int key) {
    unsigned int hash = 0;
    hash = (hash * 31) ;
    return hash % TABLE_SIZE;
}

unsigned int hash_string(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % TABLE_SIZE;
}

// Dummy functions (to be implemented later)
//Manage Book 
size_t book_hash(void *key)
{
    int book_id = *((int *)key);
    return (size_t)book_id;
}

int book_compare(void *key1, void *key2)
{
    int book_id1 = *((int *)key1);
    int book_id2 = *((int *)key2);
    return book_id1 - book_id2;
}

size_t borrowBook_hash(void *key)
{
    int borrow_id = *((int *)key);
    return (size_t)borrow_id;
}

int borrowBook_compare(void *key1, void *key2)
{
    int borrow_id1 = *((int *)key1);
    int borrow_id2 = *((int *)key2);
    return borrow_id1 - borrow_id2;
}

// Function to save the data back to books.csv
void save_books_csv(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    // Iterate through each bucket in the HashMap
    size_t i; 
    for (i = 0; i < bookMap->bucket_count; i++)
    {
        HashMapNode *current = bookMap->buckets[i];
        while (current != NULL)
        {
            if (current->value != NULL){
     struct Book *book = (struct Book *)current->value;
            // Write the book data to the file
            fprintf(file, "%d,%s,%s,%d\n", book->book_id, book->title, book->author, book->quantity);
            }
       
            current = current->next;
        }
    }
    fclose(file);
}


void create_book(HashMap *map, struct Book *book)
{
    int *key = (int *)malloc(sizeof(int));
    *key = book->book_id;
    hashmap_put(map, key, book);
    save_books_csv(BOOK_FILE);
}
struct Book *read_book(HashMap *map, int book_id)
{
    int key = book_id;
    return (struct Book *)hashmap_get(map, &key);
}
void update_book(HashMap *map, struct Book *book)
{
    int *key = (int *)malloc(sizeof(int));
    *key = book->book_id;
    hashmap_put(map, key, book);
    save_books_csv(BOOK_FILE);
}
void delete_book(HashMap *map, int book_id)
{
    int key = book_id;
    hashmap_put(map, &key, NULL);
    save_books_csv(BOOK_FILE);
}


// Function to read the books.csv file and store the data in bookMap
void read_books_csv(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    char line[128];
    while (fgets(line, sizeof(line), file))
    {
        int book_id, quantity;
        char title[50], author[30];
        // Parse the line
        sscanf(line, "%d,%49[^,],%29[^,],%d", &book_id, title, author, &quantity);
        struct Book *new_book = (struct Book *)malloc(sizeof(struct Book));
        new_book->book_id = book_id;
        strcpy(new_book->title, title);
        strcpy(new_book->author, author);
        new_book->quantity = quantity;
        new_book->next = NULL;

        // Create a new Book struct
        create_book(bookMap, new_book);
    }
    fclose(file);
}
void displayBook()
{
//    printf(YELLOW "All books in the library:\n" RESET);
//    printf(YELLOW "------------------------------------------------------------------------------------------------------------------\n" RESET);
//    printf(YELLOW "| %-10s | %-50s | %-30s | %-10s |\n" RESET, "Book ID", "Title", "Author", "Quantity");
//    printf(YELLOW "------------------------------------------------------------------------------------------------------------------\n" RESET);
    size_t i; 
    for (i = 0; i < bookMap->bucket_count; i++)
    {
        HashMapNode *node = bookMap->buckets[i];
        while (node != NULL)
        {
            struct Book *book = (struct Book *)node->value;
            if (book != NULL)
            {
                printf(YELLOW "| %-10d | %-50s | %-30s | %-10d |\n" RESET, book->book_id, book->title, book->author, book->quantity);
            }
            node = node->next;
        }
    }
//    printf(YELLOW "------------------------------------------------------------------------------------------------------------------\n" RESET);
}

void addBook()
{
    struct Book *new_book = (struct Book *)malloc(sizeof(struct Book));
    printf("Enter book ID: ");
    scanf("%d", &new_book->book_id);
    printf("Enter book title: ");
    scanf(" %[^\n]", new_book->title);
    printf("Enter book author: ");
    scanf(" %[^\n]", new_book->author);
    printf("Enter book quantity: ");
    scanf("%d", &new_book->quantity);
    create_book(bookMap, new_book);
}

void editBook()
{
    int book_id;
    printf("Enter book ID to edit: ");
    scanf("%d", &book_id);
    struct Book *book = read_book(bookMap, book_id);
    if (book == NULL)
    {
        printf("Book not found.\n");
        return;
    }
    printf("Editing Book with ID %d:\n", book_id);
    printf("Enter new title (current: %s): ", book->title);
    scanf(" %[^\n]", book->title);
    printf("Enter new author (current: %s): ", book->author);
    scanf(" %[^\n]", book->author);
    printf("Enter new quantity (current: %d): ", book->quantity);
    scanf("%d", &book->quantity);
    update_book(bookMap, book);
}
void delete_book_from_map(int book_id)
{
    int key = book_id;
    hashmap_put(bookMap, &key, NULL);
}
void deleteBook()
{
    int book_id;
    printf("Enter book ID to delete: ");
    scanf("%d", &book_id);
    struct Book *book = read_book(bookMap, book_id);
    if (book == NULL)
    {
        printf("Book not found.\n");
        return;
    }
    delete_book_from_map(book_id);
    printf("Book with ID %d deleted.\n", book_id);
}

// Sort book by quantity
// split
void splitBook(struct Book* source, struct Book** left, struct Book** right) {
    struct Book* slow = source;
    struct Book* fast = source->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    *left = source;
    *right = slow->next;
    slow->next = NULL;
}

// merge
struct Book* mergeSortedBooks(struct Book* left, struct Book* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->quantity <= right->quantity) {
        left->next = mergeSortedBooks(left->next, right);
        return left;
    } else {
        right->next = mergeSortedBooks(left, right->next);
        return right;
    }
}

// merge sort
void sortBook(struct Book** headRef) {
    struct Book* head = *headRef;
    if (!head || !head->next) return;

    struct Book* left;
    struct Book* right;
    splitBook(head, &left, &right);
    sortBook(&left);
    sortBook(&right);
    *headRef = mergeSortedBooks(left, right);
}

void sortBookByQuantity() {

    struct Book* head = NULL;
    struct Book* tail = NULL;
    int i; 
	for (i = 0; i < TABLE_SIZE; i++) {
        HashMapNode* node = bookMap->buckets[i];
        while (node) {
            struct Book* book = (struct Book*)node->value;

            struct Book* newNode = (struct Book*)malloc(sizeof(struct Book));
            if (!newNode) {
                perror("Memory allocation failed");
                exit(1);
            }
            *newNode = *book;
            newNode->next = NULL;

            // Thêm vào danh sách liên ketket
            if (!head) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            node = node->next;
        }
    }
	
    sortBook(&head);

    // DisplayDisplay
    printf("Books sorted by quantity:\n");
    struct Book* temp = head;
    while (temp) {
        printf("ID: %d, Title: %s, Quantity: %d\n", temp->book_id, temp->title, temp->quantity);
        struct Book* freeNode = temp;
        temp = temp->next;
        free(freeNode);
    }
}

struct Book* findBookByID(int book_id) {
    int m = TABLE_SIZE;
    int hKey1 = book_id % m;
    int hKey2 = 7 - (book_id % (m - 3)); 
    int index = 1;
    int i=0; 
    while (i < TABLE_SIZE) {
        index = (index + i * hKey2) % m; 
        if (BookTable[index] != NULL) {
            struct Book* current = BookTable[index];
            while (current != NULL) {
                if (current->book_id == book_id) {
                    return current; 
                }
                current = current->next;
            }
        } 
        i++;
    }
    return NULL; // Customer not found
}
void searchBookID() {
    int book_id;
    printf("Enter the book ID to search: ");
    scanf("%d", &book_id);

    struct Book* book = findBookByID(book_id);
    if (book != NULL) {
        printf("Book found: ID = %d, Title = %s, Author = %s, Quantity = %d\n",
               book->book_id, book->title, book->author, book->quantity);
    } else {
        printf("Book not found.\n");
    }
}

struct Customer* findCustomerByID(int customer_id) {
    int m = TABLE_SIZE;
    int hKey1 = customer_id % m;
    int hKey2 = 7 - (customer_id % (m - 3)); 
    int index = 0;
    int i = 0;

    while (i < TABLE_SIZE) {
        index = (index + i * hKey2) % m; 
        if (CustomerTable[index] != NULL) {
            struct Customer* current = CustomerTable[index];
            while (current != NULL) {
                if (current->customer_id == customer_id) {
                    return current; 
                }
                current = current->next;
            }
        } 
        i++;
    }
    return NULL; // Customer not found
}
void searchCustomerByID() {
    int customer_id;
    printf("Enter the Customer ID to search: ");
    scanf("%d", &customer_id);

    struct Customer* customer = findCustomerByID(customer_id);
    if (customer != NULL) {
        printf("Customer found: ID = %d, Name = %s, Address = %s, Phone = %s\n",
               customer->customer_id, customer->customer_name, customer->address, customer->phone);
    } else {
        printf("Customer not found.\n");
    }
}


// Load customer data from file
void load_data() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("Error: Unable to open file %s for reading.\n", FILENAME);
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        int customer_id;
        char customer_name[30], address[50], phone[15];

        // Read data from the line
        if (sscanf(line, "%d,%29[^,],%49[^,],%14s", &customer_id, customer_name, address, phone) == 4) {
            // Remove leading single quote from phone number if it exists
            if (phone[0] == '\'') {
                memmove(phone, phone + 1, strlen(phone)); // Shift the string left
            }
            unsigned int index = hash(customer_id);
            struct Customer* newCustomer = (struct Customer*)malloc(sizeof(struct Customer));
            if (!newCustomer) {
                printf("Error: Memory allocation failed.\n");
                fclose(file);
                return;
            }

            newCustomer->customer_id = customer_id;
            strcpy(newCustomer->customer_name, customer_name);
            strcpy(newCustomer->address, address);
            strcpy(newCustomer->phone, phone); // Save the cleaned phone number
            newCustomer->next = CustomerTable[index];
            CustomerTable[index] = newCustomer;
        }
    }
    fclose(file);
    printf("Data loaded successfully from %s.\n", FILENAME);
}

// Save customer data to file
void save_data(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Unable to open file %s for writing.\n", filename);
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct Customer* current = CustomerTable[i];
        while (current) {
            fprintf(file, "%d,%s,%s,%s\n", current->customer_id, current->customer_name, current->address, current->phone);
            current = current->next;
        }
    }
    fclose(file);
    printf("Data saved successfully to %s.\n", filename);
}

int getMaxCustomerID() {
    int max_id = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct Customer* current = CustomerTable[i];
        while (current) {
            if (current->customer_id > max_id) {
                max_id = current->customer_id;
            }
            current = current->next;
        }
    }
    return max_id;
}
// Add a new customer
void addCustomer() {
    char customer_name[30], address[50], phone[15];

    // Get the maximum current customer ID and assign the next ID
    int customer_id = getMaxCustomerID() + 1;

    printf("Enter customer name: ");
    scanf(" %[^\n]", customer_name);

    printf("Enter address: ");
    scanf(" %[^\n]", address);

    int validPhone = 0;
    while (!validPhone) {
        printf("Enter phone number (10 digits only): ");
        scanf("%s", phone);

        if (strlen(phone) == 10) {
            validPhone = 1;
            for (int i = 0; i < 10; i++) {
                if (!isdigit(phone[i])) {
                    validPhone = 0;
                    break;
                }
            }
        }

        if (!validPhone) {
            printf("Invalid phone number! Please enter exactly 10 digits (no letters or special characters).\n");
        }
    }

    unsigned int index = hash(customer_id);

    struct Customer* newCustomer = (struct Customer*)malloc(sizeof(struct Customer));
    // if (!newCustomer) {
    //     printf("Error: Memory allocation failed.\n");
    //     return;
    // }
    newCustomer->customer_id = customer_id;
    strcpy(newCustomer->customer_name, customer_name);
    strcpy(newCustomer->address, address);
    strcpy(newCustomer->phone, phone);
    newCustomer->next = CustomerTable[index];
    CustomerTable[index] = newCustomer;

    save_data(FILENAME); // Save data after adding a new customer

    printf("Customer added successfully with ID: %d\n", customer_id);
}

// Display all customers
void displayCustomer() {
    printf("Customer List:\n");
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct Customer* current = CustomerTable[i];
        while (current) {
            printf("ID: %d, Name: %s, Address: %s, Phone: %s\n",
                   current->customer_id, current->customer_name, current->address, current->phone);
            current = current->next;
        }
    }
}

void editCustomer() {
    int customer_id;
    printf("Enter the customer ID to update: ");
    scanf("%d", &customer_id); 

    struct Customer* customer = findCustomerByID(customer_id);
    if (customer == NULL) {
        printf("Customer not found.\n");
    } else {
        printf("Enter new address: ");
        scanf(" %[^\n]", customer->address);
        printf("Enter new phone number: ");
        scanf("%s", customer->phone);

        save_data(FILENAME);
        printf("Customer updated successfully.\n");
    }
}

void deleteCustomer() {
    int customer_id;
    printf("Enter the customer ID to delete: ");
    scanf("%d", &customer_id);

    unsigned int index = hash(customer_id);
    struct Customer* current = CustomerTable[index];
    struct Customer* prev = NULL;
	
    while (current) {
        if (current->customer_id == customer_id) {
            if (prev) {
                prev->next = current->next;  
            } else {
                CustomerTable[index] = current->next;  
            }
            free(current);  
            printf("Customer deleted successfully.\n");
            save_data(FILENAME);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Customer not found.\n");
}

//Borrow Book 
void displayborrowBook()
{
//    printf(YELLOW "All borrowed books:\n" RESET);
//
//    printf(YELLOW "----------------------------------------------------------------------------------------------------------------------------------------\n" RESET);
//    printf(YELLOW "| %-10s | %-10s | %-30s | %-10s |\n" RESET, "Borrow ID", "Book ID", "Customer Name", "Quantity");
//    printf(YELLOW "----------------------------------------------------------------------------------------------------------------------------------------\n" RESET);
    size_t i; 
    for (i = 0; i < borrowBookMap->bucket_count; i++)
    {
        HashMapNode *node = borrowBookMap->buckets[i];
        while (node != NULL)
        {
            struct borrowBook *borrow = (struct borrowBook *)node->value;
            if (borrow != NULL)
            {
                printf(YELLOW "| %-10d | %-10d | %-30s | %-10d |\n" RESET, borrow->borrow_id, borrow->book_id, borrow->customer_name, borrow->quantity);
            }
            node = node->next;
        }
    }
    printf(YELLOW "----------------------------------------------------------------------------------------------------------------------------------------\n" RESET);
}
// Manage Borrow Book
void addborrowBook()
{
    struct borrowBook *new_borrow = (struct borrowBook *)malloc(sizeof(struct borrowBook));
    printf("Enter borrow ID: ");
    scanf("%d", &new_borrow->borrow_id);
    printf("Enter book ID: ");
    scanf("%d", &new_borrow->book_id);
    struct Book *book = read_book(bookMap, new_borrow->book_id);
    if (book == NULL)
    {
        printf("Book ID not found in bookMap.\n");
        free(new_borrow);
        return;
    }
    printf("Enter customer name: ");
    scanf(" %[^\n]", new_borrow->customer_name);
    printf("Enter quantity to borrow: ");
    scanf("%d", &new_borrow->quantity);
    if (new_borrow->quantity > book->quantity)
    {
        printf("Quantity to borrow exceeds available book quantity.\n");
        free(new_borrow);
        return;
    }
    create_book(bookMap, book); // Re-add book with updated quantity after borrowing
    book->quantity -= new_borrow->quantity;
    // Update book quantity in bookMap
    int *key = (int *)malloc(sizeof(int));
    *key = new_borrow->borrow_id;
    hashmap_put(borrowBookMap, key, new_borrow);
}

void editborrowBook()
{
    int borrow_id;
    printf("Enter borrow ID to edit: ");
    scanf("%d", &borrow_id);
    struct borrowBook *borrow = (struct borrowBook *)hashmap_get(borrowBookMap, &borrow_id);
    if (borrow == NULL)
    {
        printf("Borrow ID not found.\n");
        return;
    }
    struct Book *old_book = read_book(bookMap, borrow->book_id);
    printf("Editing Borrow with ID %d:\n", borrow_id);
    printf("Current Book ID: %d\n", borrow->book_id);
    printf("Enter new Book ID (or the same if no change): ");
    int new_book_id;
    scanf("%d", &new_book_id);
    printf("Current Quantity: %d\n", borrow->quantity);
    printf("Enter new Quantity (or the same if no change): ");
    int new_quantity;
    scanf("%d", &new_quantity);

    // Handle book_id change
    if (new_book_id != borrow->book_id)
    {
        struct Book *new_book = read_book(bookMap, new_book_id);
        if (new_book == NULL)
        {
            printf("New Book ID not found in bookMap.\n");
            return;
        }
        // Update quantities
        old_book->quantity += borrow->quantity; // Return old quantity to old book
        new_book->quantity -= new_quantity;     // Borrow new quantity from new book
        // Update borrowBook details
        borrow->book_id = new_book_id;
    }
    else
    {
        // Handle quantity change
        if (new_quantity != borrow->quantity)
        {
            if (new_quantity > old_book->quantity + borrow->quantity)
            {
                printf("New quantity exceeds available book quantity.\n");
                return;
            }
            // Update quantities
            old_book->quantity += borrow->quantity - new_quantity;
        }
    }

    // Update borrowBook details
    borrow->quantity = new_quantity;
    printf("BorrowBook with ID %d updated.\n", borrow_id);
}

void deleteborrowBook()
{
    int borrow_id;
    printf("Enter borrow ID to delete: ");
    scanf("%d", &borrow_id);
    struct borrowBook *borrow = (struct borrowBook *)hashmap_get(borrowBookMap, &borrow_id);
    if (borrow == NULL)
    {
        printf("Borrow ID not found.\n");
        return;
    }
    struct Book *book = read_book(bookMap, borrow->book_id);
    if (book != NULL)
    {
        book->quantity += borrow->quantity; // Return borrowed quantity to book quantity
        update_book(bookMap, book);         // Update the book quantity in bookMap
    }
    hashmap_put(borrowBookMap, &borrow_id, NULL); // Remove borrowBook from borrowBookMap
    printf("BorrowBook with ID %d deleted.\n", borrow_id);
}


//Manage Return Book 

void addreturnBook() {
    int borrow_id, book_id;
    int customer_id;

    printf("Enter borrow ID: ");
    scanf("%d", &borrow_id);
    printf("Enter book ID: ");
    scanf("%d", &book_id);
    printf("Enter customer ID: ");
    scanf("%d", &customer_id);

    struct Customer* customer = findCustomerByID(customer_id);  // Updated to get the pointer
    struct Book* book = findBookByID(book_id); 
    
    if (customer != NULL && book != NULL) {  // Check for valid customer pointer
        struct returnBook* newReturn = (struct returnBook*)malloc(sizeof(struct returnBook));
        newReturn->borrow_id = borrow_id;
        char customer_id_str[20];
        sprintf(customer_id_str, "%d", customer_id);  // Assuming customer_id is a valid integer
        
		unsigned int key = hash_string(customer_id_str);
        
        int index = key % TABLE_SIZE;
        newReturn->next = returnBookTable[index];
        returnBookTable[index] = newReturn;

         book->quantity += 1; ;
        printf("Return added successfully and book quantity updated.\n");
    } else {
        printf("Invalid customer ID or book ID.\n");
    }
}


void editreturnBook() {
    int borrow_id;
    printf("Enter the borrow ID to update: ");
    scanf("%d", &borrow_id);
    
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct returnBook* current = returnBookTable[i];
        while (current) {
            if (current->borrow_id == borrow_id) {
                printf("Enter new borrow ID: ");
                scanf("%d", &current->borrow_id);
                printf("Return book updated successfully.\n");
                return;
            }
            current = current->next;
        }
    }
    printf("Return book not found.\n");
}


void deletereturnBook() {
    int borrow_id;
    printf("Enter the borrow ID to delete: ");
    scanf("%d", &borrow_id);
    
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct returnBook* current = returnBookTable[i];
        struct returnBook* prev = NULL;
        while (current) {
            if (current->borrow_id == borrow_id) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    returnBookTable[i] = current->next;
                }
                free(current);
                printf("Return book deleted successfully.\n");
                return;
            }
            prev = current;
            current = current->next;
        }
    }
    printf("Return book not found.\n");
}

// Display all return books
void displayReturnBook() {
    printf("\nList of Return Books:\n");
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct returnBook* current = returnBookTable[i];
        while (current) {
            printf("Borrow ID: %d\n", current->borrow_id);
            current = current->next;
        }
    }
}
// Menu functions
void manageBook() {
    int choice;
    while (1) {
        printf("\n1. Display books\n");
        printf("2. Add book\n");
        printf("3. Edit book\n");
        printf("4. Delete book\n");
        printf("5. Sort books by quantity\n");
        printf("6. Search book by ID\n");
        printf("7. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBook();
                break;
            case 2:
                addBook();
                break;
            case 3:
                editBook();
                break;
            case 4:
                deleteBook();
                break;
            case 5:
                sortBookByQuantity();  
                break;
            case 6:
                searchBookID();  
                break;
            case 7:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void manageCustomer() {
    load_data(); // Load data from file before managing customers

    int choice;
    while (1) {
        printf("\n1. Display the customer\n");
        printf("2. Add customer information\n");
        printf("3. Edit customer information\n");
        printf("4. Delete customer\n");
        printf("5. Search customer information by name\n");
        printf("6. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayCustomer();
                break;
            case 2:
                addCustomer();
                break;
            case 3:
                editCustomer();
                break;
            case 4:
                deleteCustomer();
                break;
            case 5:
                searchCustomerByID();
                break;
            case 6:
                save_data(FILENAME); // Save data before exiting
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void manageBorrowBook() {
    int choice;
    while (1) {
        printf("\n1. Display list borrowed book\n");
        printf("2. Add new borrow book\n");
        printf("3. Edit borrow book\n");
        printf("4. Delete borrow book\n");
        printf("5. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayborrowBook();
                break;
            case 2:
                addborrowBook();
                break;
            case 3:
                editborrowBook();
                break;
            case 4:
                deleteborrowBook();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void manageReturnBook() {
    int choice;
    while (1) {
        printf("\n1. Display list returned book\n");
        printf("2. Add new return book\n");
        printf("3. Edit return book\n");
        printf("4. Delete return book\n");
        printf("5. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayReturnBook();
                break;
            case 2:
                addreturnBook();
                break;
            case 3:
                editreturnBook();
                break;
            case 4:
                deletereturnBook();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int login() {
	char username[20], password[20];
	 int isAuthenticated = 0;

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    
    int i; 
    for (i = 0; i < 3; i++) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            isAuthenticated = 1;
            break;
        }
    }
    return isAuthenticated;
}

// Main function
int main() {
//	 enableVirtualTerminalProcessing();
    bookMap = hashmap_create(TABLE_SIZE, book_hash, book_compare);
    // Read the books.csv file and store the data in bookMap
    read_books_csv(BOOK_FILE);
    // Initialize the HashMap
    borrowBookMap = hashmap_create(TABLE_SIZE, borrowBook_hash, borrowBook_compare);
    int choice;
    if (login()) {
        printf("Login successful!\n");
        while (1) {
            printf("\nLibrary Management System\n");
            printf("1. Manage Books\n");
            printf("2. Manage Customers\n");
            printf("3. Manage Borrowed Books\n");
            printf("4. Manage Returned Books\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            
//            setColor(BLUE);
//        printf("\nLibrary Management System\n");
//        setColor(RED);
//        printf("1. ");
//        setColor(YELLOW);
//        printf("Manage Books\n");
//        setColor(RED);
//        printf("2. ");
//        setColor(YELLOW);
//        printf("Manage Customers\n");
//        setColor(RED);
//        printf("3. ");
//        setColor(YELLOW);
//        printf("Manage Borrowed Books\n");
//        setColor(RED);
//        printf("4. ");
//        setColor(YELLOW);
//        printf("Manage Returned Books\n");
//        setColor(RED);
//        printf("5. ");
//        setColor(YELLOW);
//        printf("Exit\n");
//        setColor(BLUE);
        printf("Enter your choice: ");
        scanf("%d", &choice);
            switch (choice) {
                case 1:
                    manageBook();
                    break;
                case 2:
                    manageCustomer();
                    break;
                case 3:
                    manageBorrowBook();
                    break;
                case 4:
                    manageReturnBook();
                    break;
                case 5:
                    printf("Exiting system. Goodbye!\n");
                    return 0;
                default:
                    printf("Invalid choice. Try again.\n");
            }
        }
    } else {
        printf("Invalid username or password. Exiting...\n");
    }

    return 0;
}

