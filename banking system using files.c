#include <stdio.h>
#include <string.h>
#define MAX_NAME_LENGTH 50
#define FILE_NAME "bank_customers.dat"

struct Customer {
    int accountNumber;
    char name[MAX_NAME_LENGTH];
    float balance;
};

void createAccount();
void deposit();
void withdraw();
void viewBalance();
void loadAccount(int accnum, struct Customer *customer);
void saveAccount(struct Customer *customer);

int main() {
    int ch;
    
    while (ch != 5) {
        printf("--Bank Management System--\n");
        printf("1. Create Account\n2. Deposit\n3. Withdraw\n4. View Balance\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                createAccount();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                viewBalance();
                break;
            case 5:
                printf("Exiting the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }
    return 0;
}

void createAccount() {
    struct Customer newCustomer;
    
    printf("\nEnter account number:\n");
    scanf("%d", &newCustomer.accountNumber);
    printf("Enter account holder's name:\n");
    scanf("%s", newCustomer.name);
    newCustomer.balance = 0.0; 
    saveAccount(&newCustomer);
    printf("Account created successfully!\n");
}

void deposit() {
    int accnum;
    struct Customer customer;
    float amt;
    
    printf("\nEnter account number to deposit to:\n");
    scanf("%d", &accnum);
    
    loadAccount(accnum, &customer);
    
    if (customer.accountNumber == 0) {
        printf("Account not found\n");
    } else {
        printf("Enter amount to deposit:\n");
        scanf("%f", &amt);
        
        if (amt <= 0) {
            printf("Invalid amount\n");
        } else {
            customer.balance += amt;
            printf("Deposited %.2f. New balance: %.2f\n", amt, customer.balance);
            
            saveAccount(&customer);
        }
    }
}

void withdraw() {
    int accnum;
    struct Customer customer;
    float amt;
    
    printf("\nEnter account number to withdraw from:\n");
    scanf("%d", &accnum);
    
    loadAccount(accnum, &customer);
    
    if (customer.accountNumber == 0) {
        printf("Account not found\n");
    } else {
        printf("Enter amount to withdraw:\n");
        scanf("%f", &amt);
        
        if (amt <= 0) {
            printf("Invalid amount\n");
        } else if (amt > customer.balance) {
            printf("Insufficient balance\n");
        } else {
            customer.balance -= amt;
            printf("Withdrew %.2f. New balance: %.2f\n", amt, customer.balance);
            saveAccount(&customer);
        }
    }
}

void viewBalance() {
    int accnum;
    struct Customer customer;
    
    printf("\nEnter account number to view balance:\n");
    scanf("%d", &accnum);
    
    loadAccount(accnum, &customer);
    
    if (customer.accountNumber == 0) {
        printf("Account not found\n");
    } else {
        printf("Account Number: %d\n", customer.accountNumber);
        printf("Account Holder: %s\n", customer.name);
        printf("Current Balance: %.2f\n", customer.balance);
    }
}

void loadAccount(int accnum, struct Customer *customer) {
    FILE *file = fopen(FILE_NAME, "rb");
    
    if (file == NULL) {
        printf("Error opening file\n");
        customer->accountNumber = 0;
        return;
    }
    
    int found = 0;
    while (fread(customer, sizeof(struct Customer), 1, file)) {
        if (customer->accountNumber == accnum) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        customer->accountNumber = 0; 
    }
    
    fclose(file);
}

void saveAccount(struct Customer *customer) {
    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL) {
        file = fopen(FILE_NAME, "wb+");
    }
    
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    
    struct Customer temp;
    int found = 0;
    while (fread(&temp, sizeof(struct Customer), 1, file)) {
        if (temp.accountNumber == customer->accountNumber) {
            found = 1;
            break;
        }
    }
    
    if (found) {
        fseek(file, -sizeof(struct Customer), SEEK_CUR);
        fwrite(customer, sizeof(struct Customer), 1, file);
    } else {
        fseek(file, 0, SEEK_END);
        fwrite(customer, sizeof(struct Customer), 1, file);
    }
    
    fclose(file);
}
