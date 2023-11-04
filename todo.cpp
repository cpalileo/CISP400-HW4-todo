#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

const string FILENAME = "todo_list.txt";

// TodoItem represents a single task with a description and date
class TodoItem {
public:
    string description;
    string dateAdded;

    // Default constructor with dummy data
    TodoItem() : description("Dummy Data"), dateAdded(getCurrentDate()) {}

    // Constructor with description parameter
    TodoItem(const string &desc) : description(desc), dateAdded(getCurrentDate()) {}

    // Copy constructor
    TodoItem(const TodoItem &other) : description(other.description), dateAdded(other.dateAdded) {}

    // Assignment operator overloading
    TodoItem& operator=(const TodoItem &other) {
        if (this != &other) {
            description = other.description;
            dateAdded = other.dateAdded;
        }
        return *this;
    }

    // Output stream operator overloading for easy printing
    friend ostream& operator<<(ostream &os, const TodoItem &item) {
        os << item.dateAdded << ": " << item.description;
        return os;
    }

    // Input stream operator overloading for easy input
    friend istream& operator>>(istream &is, TodoItem &item) {
        getline(is, item.description);
        item.dateAdded = getCurrentDate();
        return is;
    }

    // Static method to get the current date as a string
    static string getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return string(buffer);
    }

    // Method for testing components
    void componentTest() {
        cout << "Current Tasks (Component Test)" << *this << endl;
    }
};

// TodoList represents a list of TodoItem objects
class TodoList {
private:
    TodoItem* items;  // Array to store tasks
    int capacity;     // Current capacity of the array
    int count;        // Number of tasks in the array

    // Private method to double the capacity of the array when full
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
    // Constructor initializes an array with capacity of 1
    TodoList() : items(new TodoItem[1]), capacity(1), count(0) {}

    // Destructor to deallocate the dynamic array
    ~TodoList() {
        delete[] items;
    }

    // Adds a new item to the list
    void addItem(const TodoItem &item) {
        if (count == capacity) {
            expandCapacity();
        }
        items[count++] = item;
    }

    // Removes an item from the list by index
    void removeItem(int index) {
        // Adjust index to match array.
        index -= 1;

        if (index < 0 || index >= count) {
            cout << "Invalid index. No item removed." << endl;
            return;
        }
        for (int i = index; i < count - 1; ++i) {
            items[i] = items[i + 1];
        }
        count--;
    }

    // Saves the current list to a file
    void saveToFile() const {
        ofstream file(FILENAME);
        if (file.is_open()) {
            for (int i = 0; i < count; ++i) {
                file << items[i].description << endl;
            }
            file.close();
        }
    }

    // Loads a list from a file
    void loadFromFile() {
        ifstream file(FILENAME);
        string line;
        while (getline(file, line)) {
            addItem(TodoItem(line));
        }
        file.close();
    }

    // Displays all items in the list with numbering starting from 1
    void displayAll() const {
        for (int i = 0; i < count; ++i) {
            cout << (i + 1) << ". " << items[i] << endl;
        }
    }

    // Method for testing components
    void componentTest() const {
        cout << "Component Test for TodoList: " << endl;
        displayAll();
    }
};
void printGreeting() {
    cout << "====================-             -====================" << endl;
    cout << "===============.                       .===============" << endl;
    cout << "============.           :+****-           .============" << endl;
    cout << "==========        #@@@@@@@@@@@@@@@@@%        ==========" << endl;
    cout << "========       %@@@@@@@@@@@@@@@@@@@@@@@%       ========" << endl;
    cout << "======      +@@@@@@@@@@@%@#*#@@@@@@@@@@@@@*      ======" << endl;
    cout << "=====     :@@@@@@@@#               #@@@@@@@@+     =====" << endl;
    cout << "====     @@@@@@@#    :@@@@@@@@@@@-    #@@@@@%@     ====" << endl;
    cout << "===     @+--=@@   *@@@@@@@@@@@@@@@@@%   @@+--=@     ===" << endl;
    cout << "==     @-----.  -@@@@@@@@@@@@@@@@@@@@@#  .-----@     ==" << endl;
    cout << "=     @@#=--   :--=#@@@@@@@@@@@@@@@#+---   --+#@@     =" << endl;
    cout << "=    *@@@@@:  -----------=====-----------  :@@@@@%    =" << endl;
    cout << "     @@@@@#  @@%-----------------------@@@  %@@@@@     " << endl;
    cout << "    :@@@@@   @@@@@------------------=@@@@@   @@@@@=    " << endl;
    cout << "    =@@@@@   @@@@@@#---------------@@@@@@@-  @@@@@*    " << endl;
    cout << "    =@@@@@  .@@@@@@@@------------=@@@@@@@@-  @@@@@*    " << endl;
    cout << "    -@@@@@   @@@@@@@@@=---------+@@@@@@@@@   @@@@@=    " << endl;
    cout << "     @@@@@#  @@@@@@@@@@--------=@@@@@@@@@@  %@@@@@     " << endl;
    cout << "=    %@@@@@   @@@@@@@@@@-------@@@@@@@@@@   @@@@@@    =" << endl;
    cout << "=     @@@@@@   @@@@@@@@@+-----*@@@@@@@@@   @@@@@@     =" << endl;
    cout << "==    .@@@@@@.  *@@@@@@@@-----@@@@@@@@%  :@@@@@@-    ==" << endl;
    cout << "===    =@@@@@@@   @@@@@@@=---=@@@@@@@   @@@@@@@*    ===" << endl;
    cout << "====     @@@@@@@*    =@@@=---=@@@+    #@@@@@@@.    ====" << endl;
    cout << "=====     #@@@@@@@@*               *@@@@@@@@%     =====" << endl;
    cout << "======      %@@@@@@@@@@@@-...-@@@@@@@@@@@@%      ======" << endl;
    cout << "========      +@@@@@@@@@@-----@@@@@@@@@@*      ========" << endl;
    cout << "==========       =@@@@@@=-----=@@@@@@+       ==========" << endl;
    cout << "============:         -%@@@@@@@%=         :============" << endl;
    cout << "===============:                       :===============" << endl;
    cout << "====================:             :====================" << endl;
    cout << "                    Taskmaster Pro\n" << endl;
    cout << "                Current Date: " << TodoItem::getCurrentDate() << endl;
}
  
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

const string FILENAME = "todo_list.txt";

// TodoItem represents a single task with a description and date
class TodoItem {
public:
    string description;
    string dateAdded;

    // Default constructor with dummy data
    TodoItem() : description("Dummy Data"), dateAdded(getCurrentDate()) {}

    // Constructor with description parameter
    TodoItem(const string &desc) : description(desc), dateAdded(getCurrentDate()) {}

    // Copy constructor
    TodoItem(const TodoItem &other) : description(other.description), dateAdded(other.dateAdded) {}

    // Assignment operator overloading
    TodoItem& operator=(const TodoItem &other) {
        if (this != &other) {
            description = other.description;
            dateAdded = other.dateAdded;
        }
        return *this;
    }

    // Output stream operator overloading for easy printing
    friend ostream& operator<<(ostream &os, const TodoItem &item) {
        os << item.dateAdded << ": " << item.description;
        return os;
    }

    // Input stream operator overloading for easy input
    friend istream& operator>>(istream &is, TodoItem &item) {
        getline(is, item.description);
        item.dateAdded = getCurrentDate();
        return is;
    }

    // Static method to get the current date as a string
    static string getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return string(buffer);
    }

    // Method for testing components
    void componentTest() {
        cout << "Current Tasks (Component Test)" << *this << endl;
    }
};

// TodoList represents a list of TodoItem objects
class TodoList {
private:
    TodoItem* items;  // Array to store tasks
    int capacity;     // Current capacity of the array
    int count;        // Number of tasks in the array

    // Private method to double the capacity of the array when full
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
    // Constructor initializes an array with capacity of 1
    TodoList() : items(new TodoItem[1]), capacity(1), count(0) {}

    // Destructor to deallocate the dynamic array
    ~TodoList() {
        delete[] items;
    }

    // Adds a new item to the list
    void addItem(const TodoItem &item) {
        if (count == capacity) {
            expandCapacity();
        }
        items[count++] = item;
    }

    // Removes an item from the list by index
    void removeItem(int index) {
        // Adjust index to match array.
        index -= 1;

        if (index < 0 || index >= count) {
            cout << "Invalid index. No item removed." << endl;
            return;
        }
        for (int i = index; i < count - 1; ++i) {
            items[i] = items[i + 1];
        }
        count--;
    }

    // Saves the current list to a file
    void saveToFile() const {
        ofstream file(FILENAME);
        if (file.is_open()) {
            for (int i = 0; i < count; ++i) {
                file << items[i].description << endl;
            }
            file.close();
        }
    }

    // Loads a list from a file
    void loadFromFile() {
        ifstream file(FILENAME);
        string line;
        while (getline(file, line)) {
            addItem(TodoItem(line));
        }
        file.close();
    }

    // Displays all items in the list with numbering starting from 1
    void displayAll() const {
        for (int i = 0; i < count; ++i) {
            cout << (i + 1) << ". " << items[i] << endl;
        }
    }

    // Method for testing components
    void componentTest() const {
        cout << "Component Test for TodoList: " << endl;
        displayAll();
    }
};