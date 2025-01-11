#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100


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
	{"admin2", "admin2"},
}; 

// Hash tables for books and customers
struct Book* BookTable[TABLE_SIZE];
struct Customer* CustomerTable[TABLE_SIZE];
struct borrowBook* borrowBookTable[TABLE_SIZE];
struct returnBook* returnBookTable[TABLE_SIZE];

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
void displayBook() {
}


void addBook() {
}


void editBook() {
}


void deleteBook() {
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

// sort and display
void sortBooksByQuantity() {
    struct Book* list = NULL, *tail = NULL;
    
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct Book* current = BookTable[i];
        while (current) {
            if (!list) list = tail = current;
            else { tail->next = current; tail = current; }
            current = current->next;
        }
    }
    if (tail) tail->next = NULL;

    sortBook(&list);

    printf("Books sorted by quantity:\n");
    struct Book* temp = list;
    while (temp) {
        printf("ID: %d, Title: %s, Quantity: %d\n", temp->book_id, temp->title, temp->quantity);
        temp = temp->next;
    }
}

// Search Book_id
int findBookByID(int book_id) {
    int m = TABLE_SIZE;
    int hKey1 = book_id % m;
    int hKey2 = 7 - (book_id % (m - 3));
    int index;
    int  i;
    for (i = 0; i < TABLE_SIZE; i++) {
        index = (hKey1 + i * hKey2) % m;
        // Tim sach bang ID
        if (BookTable[index] != NULL && BookTable[index]->book_id == book_id) {
            return index; 
        }
        if (BookTable[index] == NULL) {
            return -1; // Không tìm th?y
        }
    }
    return -1; // Không tìm thay
}
void searchBookID() {
    int book_id;
    printf("Enter the book ID to search: ");
    scanf("%d", &book_id);

    int index = findBookByIDDoubleHashing(book_id);
    if (index != -1) {
        struct Book* book = BookTable[index];
        printf("Book found: ID = %d, Title = %s, Author = %s, Quantity = %d\n",
               book->book_id, book->title, book->author, book->quantity);
    } else {
        printf("Book not found.\n");
    }
}

//Manage Customer 
// Search customer name
int findCustomerByName(const char* customer_name) {
    unsigned int key = hash_string(customer_name);
    int m = TABLE_SIZE;
    int hKey1 = key % m;
    int hKey2 = 7 - (key % (m - 3));
    int index;
    int  i;
    for (i = 0; i < TABLE_SIZE; i++) {
        index = (hKey1 + i * hKey2) % m;
        if (CustomerTable[index] != NULL &&
            strcmp(CustomerTable[index]->customer_name, customer_name) == 0) {
            return index; // Tìm th?y khách hàng
        }
        if (CustomerTable[index] == NULL) {
            return -1; // Không tìm th?y
        }
    }
    return -1; // Không tìm thay sau khi duyet
}
void searchCustomer() {
    char customer_name[30];
    printf("Enter the customer name to search: ");
    scanf(" %[^\n]", customer_name);

    int index = findCustomerByName(customer_name);
    if (index != -1) {
        struct Customer* customer = CustomerTable[index];
        printf("Customer found: Name = %s, Phone = %s\n",
               customer->customer_name, customer->phone);
    } else {
        printf("Customer not found.\n");
    }
}
// Hash function
//unsigned int hash(int customer_id) {
//    return customer_id % TABLE_SIZE;
//}


// Function to find customer by ID
int findCustomerByID(int customer_id) {
    unsigned int index = hash(customer_id);

    struct Customer* current = CustomerTable[index];
    while (current) {
        if (current->customer_id == customer_id) {
            return 1; // Customer found
        }
        current = current->next;
    }
    return 0; // Customer not found
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
        if (sscanf(line, "%d,%29[^,],%49[^,],%14s", &customer_id, customer_name, address, phone) == 4) {
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
            strcpy(newCustomer->phone, phone);
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
    
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct Customer* current = CustomerTable[i];
        while (current) {
            fprintf(file, "%d,%s,%s,%s\n", current->customer_id, current->customer_name, current->address, current->phone);
            current = current->next;
        }
    }

    fclose(file);
    printf("Data saved successfully to %s.\n", filename);
}

// Add a new customer
void addCustomer() {
    int customer_id;
    char customer_name[30];
    char address[50];
    char phone[15];

    printf("Enter customer ID: ");
    scanf("%d", &customer_id);

    if (findCustomerByID(customer_id)) {
        printf("Customer ID %d already exists in the system.\n", customer_id);
        return;
    }

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
            int i;
            for (i = 0; i < 10; i++) {
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
    if (!newCustomer) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
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
    
    int index = findCustomerByID(customer_id); 
    if (index != -1) {
        struct Customer* customer = CustomerTable[index];
        printf("Enter new address: ");
        scanf(" %[^'\n']s", customer->address);
        printf("Enter new phone number: ");
        scanf("%s", customer->phone);
        printf("Customer updated successfully.\n");
    } else {
        printf("Customer not found.\n");
    }
}

void deleteCustomer() {
    int customer_id;
    printf("Enter the customer ID to delete: ");
    scanf("%d", &customer_id);

    int index = findCustomerByID(customer_id);  // Find customer by ID instead of name
    if (index != -1) {
        struct Customer* current = CustomerTable[index];
        struct Customer* prev = NULL;
        
        // Search for the customer in the list
        while (current) {
            if (current->customer_id == customer_id) {
                if (prev) {
                    prev->next = current->next;  // Remove the customer from the list
                } else {
                    CustomerTable[index] = current->next;  // Remove head node
                }
                free(current);  // Free memory
                printf("Customer deleted successfully.\n");
                return;
            }
            prev = current;
            current = current->next;
        }
    } else {
        printf("Customer not found.\n");
    }
}

void displayborrowBook() {
}


//Manage Borrow Book 
void addborrowBook() {
}


void editborrowBook() {
}


void deleteborrowBook() {
}


void displayreturnBook() {
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

    int customerIndex = findCustomerByID(customer_id);
    int bookIndex = findBookByID(book_id);

    if (customerIndex != -1 && bookIndex != -1) {
        struct returnBook* newReturn = (struct returnBook*)malloc(sizeof(struct returnBook));
        newReturn->borrow_id = borrow_id;
        char customer_id_str[20];  
        sprintf(customer_id_str, "%d", customer_id);  
        
        unsigned int key = hash_string(customer_id_str);
        int index = key % TABLE_SIZE;
        newReturn->next = returnBookTable[index];
        returnBookTable[index] = newReturn;

        BookTable[bookIndex]->quantity += 1;
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
void displayReturnBooks() {
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
                sortBooksByQuantity();  
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
                searchCustomer();
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
                displayreturnBook();
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
