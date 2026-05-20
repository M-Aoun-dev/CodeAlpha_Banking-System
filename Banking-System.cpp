// |---------------------- Banking System ----------------------|

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

// |---------------------- Structures ----------------------|

struct Customer {

    int customerID;

    char fullName[50];
    char phone[20];
    char address[100];
};

struct Account {
    int accountNumber;
    int customerID;
    char accountType[20];
    double balance;
};

struct Transaction {
    int accountNumber;
    char type[30];
    double amount;
    double remainingBalance;
};

// |---------------------- Helper Functions ----------------------|

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {

    cout << endl;
    cout << "Press Enter to continue...";
    cin.get();
}

void printLine(char ch = '-', int len = 70) {

    for (int i = 0; i < len; i++) {
        cout << ch;
    }
    cout << endl;
}

void printHeader(const char title[]) {
    cout << endl;
    printLine();
    cout << "  " << title << endl;
    printLine();
}

int getIntInput(const char message[], int min, int max) {

    int value;
    while (true) {
        cout << message;

        if (cin >> value && value >= min && value <= max) {
            cin.ignore(1000, '\n');
            return value;
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid Input!" << endl;
    }
}

double getDoubleInput(const char message[], double min) {

    double value;
    while (true) {
        cout << message;

        if (cin >> value && value >= min) {
            cin.ignore(1000, '\n');
            return value;
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid Amount!" << endl;
    }
}

// |---------------------- Validation Functions ----------------------|

bool customerExists(int customerID) {

    ifstream file("customers.txt");
    Customer c;

    while (file >> c.customerID
        >> c.fullName
        >> c.phone
        >> c.address) {

        if (c.customerID == customerID) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool accountExists(int accountNumber) {

    ifstream file("accounts.txt");
    Account a;

    while (file >> a.accountNumber
        >> a.customerID
        >> a.accountType
        >> a.balance) {

        if (a.accountNumber == accountNumber) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// |---------------------- Transaction Storage ----------------------|

void saveTransaction(
    int accountNumber,
    const char type[],
    double amount,
    double remainingBalance
) {

    ofstream file("transactions.txt", ios::app);

    file << accountNumber << " "
        << type << " "
        << amount << " "
        << remainingBalance
        << endl;

    file.close();
}

// |---------------------- Create Customer ----------------------|

void createCustomer() {

    printHeader("CREATE CUSTOMER");
    Customer c;

    while (true) {
        c.customerID = getIntInput(
            "Enter Customer ID: ",
            1,
            99999
        );

        if (customerExists(c.customerID)) {
            cout << "Customer ID already exists."
                << endl;
        }

        else {
            break;
        }
    }

    cout << "Enter Full Name: ";
    cin.getline(c.fullName, 50);

    cout << "Enter Phone Number: ";
    cin.getline(c.phone, 20);

    cout << "Enter Address: ";
    cin.getline(c.address, 100);

    // Remove spaces
    for (int i = 0; c.fullName[i] != '\0'; i++) {
        if (c.fullName[i] == ' ') {
            c.fullName[i] = '_';
        }
    }

    for (int i = 0; c.address[i] != '\0'; i++) {
        if (c.address[i] == ' ') {
            c.address[i] = '_';
        }
    }

    ofstream file("customers.txt", ios::app);

    file << c.customerID << " "
        << c.fullName << " "
        << c.phone << " "
        << c.address
        << endl;

    file.close();

    cout << endl;
    cout << "Customer Created Successfully!" << endl;
    pauseScreen();
}

// |---------------------- Create Account ----------------------|

void createAccount() {

    printHeader("CREATE ACCOUNT");
    Account a;

    a.customerID = getIntInput(
        "Enter Customer ID: ",
        1, 99999
    );

    if (!customerExists(a.customerID)) {
        cout << endl;
        cout << "Customer Not Found." << endl;
        pauseScreen();
        return;
    }

    while (true) {
        a.accountNumber = getIntInput(
            "Enter Account Number: ",
            1, 999999
        );

        if (accountExists(a.accountNumber)) {
            cout << "Account Number already exists."
                << endl;
        }

        else {
            break;
        }
    }

    cout << "Enter Account Type (Saving/Current): ";
    cin.getline(a.accountType, 20);

    a.balance = getDoubleInput(
        "Enter Initial Deposit: ",
        0
    );

    ofstream file("accounts.txt", ios::app);

    file << a.accountNumber << " "
        << a.customerID << " "
        << a.accountType << " "
        << fixed << setprecision(2)
        << a.balance
        << endl;

    file.close();

    saveTransaction(
        a.accountNumber,
        "Deposit",
        a.balance,
        a.balance
    );

    cout << endl;
    cout << "Account Created Successfully!" << endl;

    pauseScreen();
}

// |---------------------- Deposit ----------------------|

void depositMoney() {

    printHeader("DEPOSIT MONEY");
    int accountNumber;

    accountNumber = getIntInput(
        "Enter Account Number: ",
        1, 999999
    );

    ifstream file("accounts.txt");
    ofstream temp("temp.txt");

    Account a;

    bool found = false;

    while (file >> a.accountNumber
        >> a.customerID
        >> a.accountType
        >> a.balance) {

        if (a.accountNumber == accountNumber) {
            found = true;
            double amount;

            amount = getDoubleInput(
                "Enter Deposit Amount: ",
                1
            );

            a.balance += amount;

            saveTransaction(
                accountNumber,
                "Deposit",
                amount,
                a.balance
            );
        }

        temp << a.accountNumber << " "
            << a.customerID << " "
            << a.accountType << " "
            << a.balance
            << endl;
    }

    file.close();
    temp.close();

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    cout << endl;

    if (found) {
        cout << "Deposit Successful!"
            << endl;
    }

    else {
        cout << "Account Not Found."
            << endl;
    }

    pauseScreen();
}

// |---------------------- Withdraw ----------------------|

void withdrawMoney() {

    printHeader("WITHDRAW MONEY");
    int accountNumber;

    accountNumber = getIntInput(
        "Enter Account Number: ",
        1, 999999
    );

    ifstream file("accounts.txt");
    ofstream temp("temp.txt");

    Account a;

    bool found = false;

    while (file >> a.accountNumber
        >> a.customerID
        >> a.accountType
        >> a.balance) {

        if (a.accountNumber == accountNumber) {
            found = true;
            double amount;

            amount = getDoubleInput(
                "Enter Withdraw Amount: ",
                1
            );

            if (amount > a.balance) {
                cout << endl;
                cout << "Insufficient Balance!" << endl;
            }

            else {
                a.balance -= amount;
                saveTransaction(
                    accountNumber,
                    "Withdraw",
                    amount,
                    a.balance
                );

                cout << endl;
                cout << "Withdrawal Successful!" << endl;
            }
        }

        temp << a.accountNumber << " "
            << a.customerID << " "
            << a.accountType << " "
            << fixed << setprecision(2)
            << a.balance
            << endl;
    }

    file.close();
    temp.close();

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    if (!found) {
        cout << "Account Not Found." << endl;
    }
    pauseScreen();
}

// |---------------------- Transfer Funds ----------------------|

void transferFunds() {

    printHeader("TRANSFER FUNDS");

    int sender;
    int receiver;

    sender = getIntInput(
        "Enter Sender Account Number: ",
        1, 999999
    );

    receiver = getIntInput(
        "Enter Receiver Account Number: ",
        1, 999999
    );

    if (sender == receiver) {
        cout << "Cannot Transfer To Same Account." << endl;
        pauseScreen();
        return;
    }

    double amount;

    amount = getDoubleInput(
        "Enter Transfer Amount: ",
        1
    );

    ifstream file("accounts.txt");
    ofstream temp("temp.txt");

    Account a;

    bool senderFound = false;
    bool receiverFound = false;
    bool transferSuccess = false;

    double senderBalance = 0;

    while (file >> a.accountNumber
        >> a.customerID
        >> a.accountType
        >> a.balance) {

        if (a.accountNumber == sender) {
            senderFound = true;

            if (a.balance >= amount) {
                a.balance -= amount;
                senderBalance = a.balance;
                transferSuccess = true;
            }
        }

        temp << a.accountNumber << " "
            << a.customerID << " "
            << a.accountType << " "
            << a.balance
            << endl;
    }

    file.close();
    temp.close();

    if (!senderFound || !transferSuccess) {

        remove("temp.txt");

        cout << endl;
        cout << "Transfer Failed."
            << endl;

        pauseScreen();
        return;
    }

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    ifstream file2("accounts.txt");
    ofstream temp2("temp2.txt");

    while (file2 >> a.accountNumber
        >> a.customerID
        >> a.accountType
        >> a.balance) {

        if (a.accountNumber == receiver) {
            receiverFound = true;
            a.balance += amount;

            saveTransaction(
                receiver,
                "Received",
                amount,
                a.balance
            );
        }

        temp2 << a.accountNumber << " "
            << a.customerID << " "
            << a.accountType << " "
            << a.balance
            << endl;
    }

    file2.close();
    temp2.close();

    remove("accounts.txt");
    rename("temp2.txt", "accounts.txt");

    if (!receiverFound) {

        cout << "Receiver Account Not Found." << endl;
    }

    else {

        saveTransaction(
            sender,
            "Transfer",
            amount,
            senderBalance
        );

        cout << endl;
        cout << "Fund Transfer Successful!" << endl;
    }

    pauseScreen();
}

// |---------------------- View Account Info ----------------------|

void viewAccountInfo() {

    printHeader("ACCOUNT INFORMATION");
    int accountNumber;

    accountNumber = getIntInput(
        "Enter Account Number: ",
        1, 999999
    );

    ifstream file("accounts.txt");
    Account a;

    bool found = false;

    while (file >> a.accountNumber
        >> a.customerID
        >> a.accountType
        >> a.balance) {

        if (a.accountNumber == accountNumber) {
            found = true;
            cout << endl;
            cout << "Account Number : "
                << a.accountNumber << endl;

            cout << "Customer ID    : "
                << a.customerID << endl;

            cout << "Account Type   : "
                << a.accountType << endl;

            cout << fixed << setprecision(2);

            cout << "Balance        : "
                << a.balance << endl;
        }
    }

    file.close();

    if (!found) {
        cout << "Account Not Found." << endl;
    }
    pauseScreen();
}

// |---------------------- View Transactions ----------------------|

void viewTransactions() {

    printHeader("TRANSACTION HISTORY");
    int accountNumber;

    accountNumber = getIntInput(
        "Enter Account Number: ",
        1, 999999
    );

    ifstream file("transactions.txt");
    Transaction t;

    bool found = false;

    cout << left
        << setw(15) << "Type"
        << setw(15) << "Amount"
        << setw(20) << "Remaining Balance"
        << endl;

    printLine('-', 60);

    while (file >> t.accountNumber
        >> t.type
        >> t.amount
        >> t.remainingBalance) {

        if (t.accountNumber == accountNumber) {
            found = true;
            cout << left
                << setw(15) << t.type
                << setw(15) << t.amount
                << setw(20) << t.remainingBalance
                << endl;
        }
    }

    file.close();

    if (!found) {
        cout << "No Transactions Found." << endl;
    }

    pauseScreen();
}

// |---------------------- View Customers ----------------------|

void viewCustomers() {

    printHeader("ALL CUSTOMERS");

    ifstream file("customers.txt");
    Customer c;

    bool empty = true;

    cout << left
        << setw(15) << "ID"
        << setw(20) << "Name"
        << setw(20) << "Phone"
        << endl;

    printLine('-', 60);

    while (file >> c.customerID
        >> c.fullName
        >> c.phone
        >> c.address) {
        empty = false;

        cout << left
            << setw(15) << c.customerID
            << setw(20) << c.fullName
            << setw(20) << c.phone
            << endl;
    }

    file.close();

    if (empty) {
        cout << "No Customers Found." << endl;
    }

    pauseScreen();
}

// |---------------------- Main Menu ----------------------|

void showMenu() {

    clearScreen();
    cout << endl;
    cout << "|--------------------------------------------------------------|" << endl;
    cout << "                 BANKING MANAGEMENT SYSTEM" << endl;
    cout << "|--------------------------------------------------------------|" << endl;
    cout << endl;

    cout << "[1] Create Customer" << endl;
    cout << "[2] Create Account" << endl;
    cout << "[3] Deposit Money" << endl;
    cout << "[4] Withdraw Money" << endl;
    cout << "[5] Transfer Funds" << endl;
    cout << "[6] View Account Information" << endl;
    cout << "[7] View Transaction History" << endl;
    cout << "[8] View Customers" << endl;
    cout << "[0] Exit" << endl;

    printLine('-', 70);
}

// |---------------------- Main Function ----------------------|

int main() {

    int choice;
    while (true) {
        showMenu();

        choice = getIntInput(
            "Enter Your Choice: ",
            0, 8
        );

        switch (choice) {

        case 1:
            createCustomer();
            break;
        case 2:
            createAccount();
            break;
        case 3:
            depositMoney();
            break;
        case 4:
            withdrawMoney();
            break;
        case 5:
            transferFunds();
            break;
        case 6:
            viewAccountInfo();
            break;
        case 7:
            viewTransactions();
            break;
        case 8:
            viewCustomers();
            break;
        case 0:
            clearScreen();

            cout << endl;
            cout << "Thank You For Using Banking System!" << endl;
            return 0;
        }
    }

    system("pause");
    return 0;
}