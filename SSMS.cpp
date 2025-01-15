#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <unordered_map>

using namespace std;

// Struct to represent a product
struct Product {
    int code = -1;             // Initialize with -1 to indicate an empty slot
    string name;
    float price = 0.0;
    float discount = 0.0;
    int quantity = 0;          
};

// Class to manage the supermarket system
class Shopping {
private:
    static const int TABLE_SIZE = 11;  // Prime number for hash table size
    Product products[TABLE_SIZE];      // Fixed-size array for the hash table
    const string filename = "products.txt";
    const string adminFile = "admin_users.txt";
    const string buyerFile = "buyer_users.txt";

    int hashFunction(int key) { return key % TABLE_SIZE; }  // Division hash method
    void saveToFile();   // Save products to file
    void loadFromFile(); // Load products from file

    bool login(const string& filename);
    void signup(const string& filename);

public:
    Shopping() { loadFromFile(); } // Constructor to load data
    ~Shopping() { saveToFile(); }  // Destructor to save data

    void welcome();          // Welcome menu
    void adminLogin();       // Admin login
    void buyerLogin();       // Buyer login
    void menu();             // Main menu
    void administrator();    // Administrator menu
    void buyer();            // Buyer menu
    void add();              // Add product
    void edit();             // Edit product
    void removeProduct();    // Remove product
    void lists();            // List products
    void receipt();          // Generate receipt
};

// Save products to file
void Shopping::saveToFile() {
    ofstream file(filename, ios::out);
    if (!file) {
        cerr << "Error saving to file.\n";
        return;
    }

    for (const auto& prod : products) {
        if (prod.code != -1) { // Only save non-empty slots
            file << prod.code << " " << prod.name << " "
                 << prod.price << " " << prod.discount << " "
                 << prod.quantity << "\n";
        }
    }

    file.close();
}

// Load products from file
void Shopping::loadFromFile() {
    ifstream file(filename, ios::in);
    if (!file) {
        cerr << "File not found. Starting with an empty product list.\n";
        return;
    }

    Product prod;
    while (file >> prod.code) {
        file.ignore();
        getline(file, prod.name, ' ');
        file >> prod.price >> prod.discount >> prod.quantity;

        int index = hashFunction(prod.code);
        while (products[index].code != -1) {
            index = (index + 1) % TABLE_SIZE; // Linear probing
        }
        products[index] = prod;
    }

    file.close();
}

// Authenticate user login
bool Shopping::login(const string& filename) {
    string username, password, storedUser, storedPass;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    ifstream file(filename);
    if (!file) {
        cerr << "Error accessing user database.\n";
        return false;
    }

    while (file >> storedUser >> storedPass) {
        if (storedUser == username && storedPass == password) {
            cout << "Login successful!\n";
            return true;
        }
    }

    cout << "Invalid credentials. Please try again.\n";
    return false;
}

// Register new user
void Shopping::signup(const string& filename) {
    string username, password;
    cout << "Choose a Username: ";
    cin >> username;
    cout << "Choose a Password: ";
    cin >> password;

    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error accessing user database.\n";
        return;
    }

    file << username << " " << password << "\n";
    cout << "Registration successful!\n";
}

// Welcome screen
void Shopping::welcome() {
    while (true) {
        int choice;
        cout << "\n========== Welcome to the Supermarket ==========\n";
        cout << "1) Admin Login\n";
        cout << "2) Buyer Login\n";
        cout << "3) Admin Signup\n";
        cout << "4) Buyer Signup\n";
        cout << "5) Exit\n";
        cout << "===============================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            adminLogin();
            break;
        case 2:
            buyerLogin();
            break;
        case 3:
            signup(adminFile);
            break;
        case 4:
            signup(buyerFile);
            break;
        case 5:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Admin login
void Shopping::adminLogin() {
    if (login(adminFile)) {
        administrator();
    }
}

// Buyer login
void Shopping::buyerLogin() {
    if (login(buyerFile)) {
        buyer();
    }
}

// Main menu
void Shopping::menu() {
    while (true) {
        int choice;
        cout << "\n================= Supermarket Main Menu =================\n";
        cout << "1) Administrator\n";
        cout << "2) Buyer\n";
        cout << "3) Exit\n";
        cout << "=========================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            administrator();
            break;
        case 2:
            buyer();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Administrator menu
void Shopping::administrator() {
    while (true) {
        int choice;
        cout << "\n========== Administrator Menu ==========\n";
        cout << "1) Add Product\n";
        cout << "2) Edit Product\n";
        cout << "3) Remove Product\n";
        cout << "4) List Products\n";
        cout << "5) Back to Main Menu\n";
        cout << "=========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            add();
            break;
        case 2:
            edit();
            break;
        case 3:
            removeProduct();
            break;
        case 4:
            lists();
            break;
        case 5:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Buyer menu
void Shopping::buyer() {
    while (true) {
        int choice;
        cout << "\n============== Buyer Menu ==============\n";
        cout << "1) Show All Products\n";
        cout << "2) Buy Product\n";
        cout << "3) Back to Main Menu\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            lists();  // Show all products
            break;
        case 2:
            receipt();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Add a product
void Shopping::add() {
    Product newProduct;
    cout << "Enter Product Code: ";
    cin >> newProduct.code;

    int index = hashFunction(newProduct.code);
    int startIndex = index;

    // Linear probing to find an empty slot
    while (products[index].code != -1) {
        if (products[index].code == newProduct.code) {
            cout << "Product code already exists. Please use a unique code.\n";
            return;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex) { // Table is full
            cout << "Error: Product table is full.\n";
            return;
        }
    }

    cin.ignore();
    cout << "Enter Product Name: ";
    getline(cin, newProduct.name);
    cout << "Enter Product Price: ";
    cin >> newProduct.price;
    cout << "Enter Product Discount (%): ";
    cin >> newProduct.discount;
    cout << "Enter Product Quantity: ";
    cin >> newProduct.quantity;

    products[index] = newProduct;
    cout << "Product added successfully.\n";
}

// Edit an existing product
void Shopping::edit() {
    int code;
    cout << "Enter Product Code to Edit: ";
    cin >> code;

    int index = hashFunction(code);
    int startIndex = index;

    while (products[index].code != -1) {
        if (products[index].code == code) {
            cout << "Editing Product: " << products[index].name << "\n";
            cout << "Enter New Name: ";
            cin.ignore();
            getline(cin, products[index].name);
            cout << "Enter New Price: ";
            cin >> products[index].price;
            cout << "Enter New Discount (%): ";
            cin >> products[index].discount;
            cout << "Enter New Quantity: ";
            cin >> products[index].quantity;
            cout << "Product updated successfully.\n";
            return;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex) break; // Table fully searched
    }

    cout << "Product not found.\n";
}

// Remove a product
void Shopping::removeProduct() {
    int code;
    cout << "Enter Product Code to Remove: ";
    cin >> code;

    int index = hashFunction(code);
    int startIndex = index;

    while (products[index].code != -1) {
        if (products[index].code == code) {
            products[index] = Product(); // Reset to default (empty slot)
            cout << "Product removed successfully.\n";
            return;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == startIndex) break; 
    }

    cout << "Product not found.\n";
}

// List all products
void Shopping::lists() {
    cout << "\n====================== Product List =========================\n";
    cout << setw(10) << "Code" << setw(20) << "Name" << setw(10) << "Price"
         << setw(10) << "Discount" << setw(20) << "Quantity\n";
    cout << "=============================================================\n";

    for (const auto& prod : products) {
        if (prod.code != -1) { // Only display non-empty slots
            cout << setw(10) << prod.code << setw(20) << prod.name
                 << setw(10) << prod.price << setw(9) << prod.discount << "%"
                 << setw(10) << prod.quantity << "\n";
        }
    }
}

// Generate a receipt
void Shopping::receipt() {
    int code, quantity;
    float total = 0;
    char choice;

    cout << "\n=================== Receipt ===================\n";
    do {
        cout << "Enter Product Code: ";
        cin >> code;

        int index = hashFunction(code);
        int startIndex = index;

        while (products[index].code != -1) {
            if (products[index].code == code) {
                cout << "Enter Quantity: ";
                cin >> quantity;

                if (quantity > products[index].quantity) {
                    cout << "Insufficient stock! Available quantity: " << products[index].quantity << "\n";
                    break;
                }

                float amount = products[index].price * quantity;
                float discount = amount * products[index].discount / 100;
                total += amount - discount;

                products[index].quantity -= quantity;

                cout << "Product: " << products[index].name << ", Quantity: " << quantity
                     << ", Total: " << amount - discount << "\n";
                break;
            }
            index = (index + 1) % TABLE_SIZE;
            if (index == startIndex) {
                cout << "Product not found.\n";
                break;
            }
        }

        cout << "Add more products? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "===============================================\n";
    cout << "Total Amount: " << total << "\n";
}


int main() {
    Shopping shop;
    shop.welcome();
    return 0;
}
