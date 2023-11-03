#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

const string FILENAME = "todo_list.txt";

class TodoItem {
public:
    string description;
    string dateAdded;

    TodoItem() : description("Dummy TODO"), dateAdded(getCurrentDate()) {}
    TodoItem(const string &desc) : description(desc), dateAdded(getCurrentDate()) {}
    TodoItem(const TodoItem &other) : description(other.description), dateAdded(other.dateAdded) {}

    TodoItem& operator=(const TodoItem &other) {
        if (this != &other) {
            description = other.description;
            dateAdded = other.dateAdded;
        }
        return *this;
    }

    friend ostream& operator<<(ostream &os, const TodoItem &item) {
        os << item.dateAdded << ": " << item.description;
        return os;
    }

    friend istream& operator>>(istream &is, TodoItem &item) {
        getline(is, item.description);
        item.dateAdded = getCurrentDate();
        return is;
    }

    static string getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return string(buffer);
    }

    void componentTest() const {
        cout << "Component Test for TodoItem: " << *this << endl;
    }
};

class TodoList {
private:
    TodoItem* items;
    int capacity;
    int count;

    void expandCapacity() {
        capacity *= 2;
        TodoItem* newList = new TodoItem[capacity];
        for (int i = 0; i < count; ++i) {
            newList[i] = items[i];
        }
        delete[] items;
        items = newList;
    }

public:
    TodoList() : items(new TodoItem[1]), capacity(1), count(0) {}

    ~TodoList() {
        delete[] items;
    }

    void addItem(const TodoItem &item) {
        if (count == capacity) {
            expandCapacity();
        }
        items[count++] = item;
    }

    void removeItem(int index) {
        if (index < 0 || index >= count) {
            cout << "Invalid index. No item removed." << endl;
            return;
        }
        for (int i = index; i < count - 1; ++i) {
            items[i] = items[i + 1];
        }
        count--;
    }

    void saveToFile() const {
        ofstream file(FILENAME);
        if (file.is_open()) {
            for (int i = 0; i < count; ++i) {
                file << items[i].description << endl;
            }
            file.close();
        }
    }

    void loadFromFile() {
        ifstream file(FILENAME);
        string line;
        while (getline(file, line)) {
            addItem(TodoItem(line));
        }
        file.close();
    }

    void displayAll() const {
        for (int i = 0; i < count; ++i) {
            cout << i << ". " << items[i] << endl;
        }
    }

    void componentTest() const {
        cout << "Component Test for TodoList: " << endl;
        displayAll();
    }
};

void printGreeting() {
    cout << "Welcome to the Todo List Manager" << endl;
    cout << "Current Date: " << TodoItem::getCurrentDate() << endl;
}

char getUserInput() {
    char input;
    cout << "Enter command (+ add, - remove, ? display, q quit): ";
    cin >> input;
    return input;
}

int main() {
    printGreeting();

    TodoList list;
    list.loadFromFile();
    list.componentTest();

    char command;
    while ((command = getUserInput()) != 'q') {
        switch (command) {
            case '+': {
                cout << "Enter a description for the new task: ";
                cin.ignore();
                TodoItem newItem;
                cin >> newItem;
                list.addItem(newItem);
                break;
            }
            case '-': {
                int index;
                cout << "Enter the index of the task to remove: ";
                cin >> index;
                list.removeItem(index);
                break;
            }
            case '?': {
                list.displayAll();
                break;
            }
            default:
                cout << "Invalid command." << endl;
        }
    }

    list.saveToFile();
    return 0;
}
