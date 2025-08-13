#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Contact {
private:
    string name;
    string phone;
    string email;
    Contact* next;

public:
    Contact(string n, string p, string e) : name(n), phone(p), email(e), next(nullptr) {}

    void display() const {
        cout << "Name: " << name << "\nPhone: " << phone << "\nEmail: " << email << "\n";
    }

    bool compareName(const string& otherName) const {
        return name == otherName;
    }

    bool operator<(const Contact& other) const {
        return name < other.name;
    }

    friend class ContactManager;
};

class ContactManager {
private:
    Contact* head;
    const string filename = "contacts.txt";

    void insertSorted(Contact* newContact) {
        if (head == nullptr || *newContact < *head) {
            newContact->next = head;
            head = newContact;
        } else {
            Contact* current = head;
            while (current->next != nullptr && !(*newContact < *current->next)) {
                current = current->next;
            }
            newContact->next = current->next;
            current->next = newContact;
        }
    }

public:
    ContactManager() : head(nullptr) {
        loadFromFile();
    }

    ~ContactManager() {
        saveToFile();
        clearContacts();
    }

    void addContact() {
        string name, phone, email;
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Phone: ";
        getline(cin, phone);
        cout << "Enter Email: ";
        getline(cin, email);

        Contact* newContact = new Contact(name, phone, email);
        insertSorted(newContact);
        cout << "Contact added successfully.\n";
        saveToFile();  // Auto-save after addition
    }

    void displayContacts() const {
        if (head == nullptr) {
            cout << "No contacts found.\n";
            return;
        }
        cout << "\n--- Contact List ---\n";
        Contact* current = head;
        while (current != nullptr) {
            current->display();
            cout << "-------------------\n";
            current = current->next;
        }
    }

    void searchContact() const {
        string name;
        cout << "Enter name to search: ";
        getline(cin, name);
        Contact* current = head;
        while (current != nullptr) {
            if (current->compareName(name)) {
                cout << "Contact Found:\n";
                current->display();
                return;
            }
            current = current->next;
        }
        cout << "Contact not found.\n";
    }

    void deleteContact() {
        string name;
        cout << "Enter name to delete: ";
        getline(cin, name);

        Contact *current = head, *previous = nullptr;
        while (current != nullptr && !current->compareName(name)) {
            previous = current;
            current = current->next;
        }
        if (current == nullptr) {
            cout << "Contact not found.\n";
            return;
        }
        if (previous == nullptr) {
            head = current->next;
        } else {
            previous->next = current->next;
        }
        delete current;
        cout << "Contact deleted successfully.\n";
        saveToFile();  // Auto-save after deletion
    }

    void updateContact() {
        string name;
        cout << "Enter name to update: ";
        getline(cin, name);
        Contact* current = head;
        while (current != nullptr) {
            if (current->compareName(name)) {
                cout << "Enter new phone: ";
                getline(cin, current->phone);
                cout << "Enter new email: ";
                getline(cin, current->email);
                cout << "Contact updated successfully.\n";
                saveToFile();  // Auto-save after update
                return;
            }
            current = current->next;
        }
        cout << "Contact not found.\n";
    }

    void saveToFile() {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error opening file for writing.\n";
            return;
        }

        Contact* current = head;
        while (current != nullptr) {
            outFile << current->name << "\n"
                    << current->phone << "\n"
                    << current->email << "\n";
            current = current->next;
        }
        outFile.close();
        cout << "Contacts saved to file.\n";
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (!inFile) {
            cout << "No existing contacts found (first run).\n";
            return;
        }

        string name, phone, email;
        while (getline(inFile, name) && getline(inFile, phone) && getline(inFile, email)) {
            Contact* newContact = new Contact(name, phone, email);
            insertSorted(newContact);
        }
        inFile.close();
        cout << "Contacts loaded from file.\n";
    }

    void clearContacts() {
        Contact* current = head;
        while (current != nullptr) {
            Contact* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }
};

void displayMenu() {
    cout << "\n--- Phone Contact Management System ---\n";
    cout << "1. Add Contact\n";
    cout << "2. Display All Contacts\n";
    cout << "3. Search Contact\n";
    cout << "4. Delete Contact\n";
    cout << "5. Update Contact\n";
    cout << "6. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    ContactManager manager;
    int choice;
    string input;

    do {
        displayMenu();
        getline(cin, input);
        try {
            choice = stoi(input);
        } catch (...) {
            choice = 0;
        }

        switch (choice) {
            case 1: manager.addContact(); break;
            case 2: manager.displayContacts(); break;
            case 3: manager.searchContact(); break;
            case 4: manager.deleteContact(); break;
            case 5: manager.updateContact(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
