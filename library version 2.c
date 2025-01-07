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
    char customer_name[30];
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
	{"user", "123"}
}; 

// Hash tables for books and customers
struct Book* BookTable[TABLE_SIZE];
struct Customer* CustomerTable[TABLE_SIZE];
struct borrowBook* borrowBookTable[TABLE_SIZE];
struct returnBook* returnBookTable[TABLE_SIZE];

// Hash function
unsigned int hash(const char* key) {
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

//Manage Customer 
void displayCustomer() {
}


void editCustomer() {
}


void deleteCustomer() {
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
}


void editreturnBook() {
}


void deletereturnBook() {
}

// Menu functions
void manageBook() {
    int choice;
    while (1) {
        printf("\n1. Display books\n");
        printf("2. Add book\n");
        printf("3. Edit book\n");
        printf("4. Delete book\n");
        printf("5. Back to main menu\n");
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
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void manageCustomer() {
    int choice;
    while (1) {
        printf("\n1. Display the customer\n");
        printf("2. Edit customer information\n");
        printf("3. Delete customer\n");
        printf("4. Back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayCustomer();
                break;
            case 2:
                editCustomer();
                break;
            case 3:
                deleteCustomer();
                break;
            case 4:
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

