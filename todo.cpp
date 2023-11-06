/********************************************************
* Author:      Christopher Palileo                      *
* Professor:   Caleb Fowler                             *
* Class:       CISP 400                                 *
* Assignment:  Homework 4: ToDo List (todo)             *
* Date:        11/05/2023                               *
* Description: A Todo list program (Nuff Said!)         *
********************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

string FILENAME = "todo_list.txt";

// TaskItem represents a single task with a description and date
class TaskItem {
public:
    string description;
    string dateAdded;

    // Default constructor: dummy data
    // Specification A4 - Overload Constructor
    TaskItem() : description("Dummy Data"), dateAdded(getCurrentDate()) {}

    // Constructor with description parameter
    // Specification A4 - Overload Constructor
    TaskItem(string &desc) : description(desc), dateAdded(getCurrentDate()) {}

    // Copy constructor
    // Specification A1 - Overload Copy Constructor
    TaskItem(TaskItem &otherItem) : description(otherItem.description), dateAdded(otherItem.dateAdded) {}

    // Assignment operator overloading
    // Specification A2 - Overload Assignment Operator
    TaskItem& operator=(const TaskItem &otherItem) {
        if (this != &otherItem) {
            description = otherItem.description;
            dateAdded = otherItem.dateAdded;
        }
        return *this;
    }

    // Output stream operator overloading for easy printing
    // Specification C1 - Overload «
    friend ostream& operator<<(ostream &output, TaskItem &todo) {
        output << todo.dateAdded << ": " << todo.description;
        return output;
    }

    // Input stream operator overloading for easy input
    //Specification C2 - Overload »
    friend istream& operator>>(istream &input, TaskItem &todo) {
        getline(input, todo.description);
        todo.dateAdded = getCurrentDate();
        return input;
    }

    // Static method to get current date as a string
    // Specification A3 - System Date.
    static string getCurrentDate() {
        time_t now = time(0);
        tm *localTime = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
        return string(buffer);
    }


    // Method for testing components
    // Specification C3 - Test TODO’s
    void runTests() {
        cout << "Current Tasks (Component Test)" << *this << endl;
    }
};

// TodoList represents a list of TaskItem objects
class TodoList {
private:
    TaskItem* items;  // Array to store tasks
    int capacity;     // Current capacity of the array
    int count;        // Number of tasks in the array

    // Private method to double the capacity of array when full
    //Specification C4 - TODO array
    void expandCapacity() {
        capacity *= 2;
        TaskItem* newList = new TaskItem[capacity];
        for (int i = 0; i < count; ++i) {
            newList[i] = items[i];
        }
        delete[] items;
        items = newList;
    }

public:
    // Constructor initializes an array with capacity of 1
    TodoList() : items(new TaskItem[1]), capacity(1), count(0) {}

    // Destructor to deallocate dynamic array
    ~TodoList() {
        delete[] items;
    }

    // Adds a new item to list
    void addItem(const TaskItem &todo) {
        if (count == capacity) {
            expandCapacity();
        }
        items[count++] = todo;
    }

    // Removes an item from list by index
    void removeItem(int index) {
        index -= 1; // Adjust for 0-based indexing

        if (index < 0 || index >= count) {
            cout << "Invalid index. No item removed." << endl;
            return;
        }
        for (int i = index; i < count - 1; ++i) {
            items[i] = items[i + 1];
        }
        count--;
    }

    // Saves current list to a file
    // Specification B4 - Persistence
    void saveToFile() {
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
            addItem(TaskItem(line));
        }
        file.close();
    }

    // Displays all items in list with numbers starting from 1
    void displayAll() const {
        if (count == 0) {
            cout << "The list is currently empty." << endl;
        } else {
            for (int i = 0; i < count; ++i) {
                cout << (i + 1) << ". " << items[i] << endl;
            }
        }
    }

    // Method for testing components
    void runTests() {
        cout << "Component Test for Todo List: " << endl;
        displayAll();
    }

    // Checks if input is empty
    bool isEmpty() {
        return count == 0;
    }

    // Returns number of todo items in the list
    int getCount() {
        return count;
    }
};


// Function prototypes
void printGreeting();
char getUserInput();
string getTaskDescription();
int getTaskIndexToRemove(int count);


// Main function
int main() {
    // Print the greeting message
    printGreeting();

    // Create and load the todo list from file
    TodoList list;
    list.loadFromFile();
    list.runTests();

    char command;
    // Main loop for user commands
    while ((command = getUserInput()) != 'q') {
        switch (command) {
            case '+': {
                // Add new item to list
                // Specification B1 - + Symbol
                string description = getTaskDescription();
                TaskItem newItem(description);
                list.addItem(newItem);
                break;
            }

            case '-': {
                // Remove an item from list
                // Specification B3 - - symbol
                if (list.isEmpty()) {
                    cout << "The list is empty. No items to remove." << endl;
                } else {
                    int index = getTaskIndexToRemove(list.getCount());
                    list.removeItem(index); // Pass the 1-based index directly
                }
                break;
            }
            
            case '?': {
                if (list.isEmpty()) {
                    cout << "The list is currently empty." << endl;
                } else {
                    list.displayAll();
                }
                break;
            }

            default:
                cout << "Invalid command." << endl;
        }
    }

    // Save list to the file before quitting
    list.saveToFile();
    return 0;
}


// ASCII Arty Generated at https://asciiart.club/
void printGreeting() {
    cout << "\n▓▓▓▓▓▓▓▓▓▓▓▓▓▀╙ `      ` ╙▀▀▓▓▓▓▓▓▓▓▓▓▓▓/========================================================\\" << endl;
    cout << "▓▓▓▓▓▓▓▓▓▀`   _,▄▄▄▄▄▄▄▄╓_   `╙▀▓▓▓▓▓▓▓▓|| _____         _                        _             ||" << endl;
    cout << "▓▓▓▓▓▓▀   ,▄▓██████████████▓▄▄   ╙▓▓▓▓▓▓|||_   _|       | |                      | |            ||" << endl;
    cout << "▓▓▓▓▀   ▄▓██████▀▀▀╙╙▀▀▀▓██████▌_  ╙▓▓▓▓||  | | __ _ ___| | ___ __ ___   __ _ ___| |_ ___ _ __  ||" << endl;
    cout << "▓▓▓`  ▄█████▀└,▄████████▄▄└▀█████▌   ▓▓▓||  | |/ _` / __| |/ / '_ ` _ \\ / _` / __| __/ _ \\ '__| ||" << endl;
    cout << "▓▓   ██╬╬╬▀ ▄███████████████_╙╬╬╬╬█_  ╫▓||  | | (_| \\__ \\   <| | | | | | (_| \\__ \\ ||  __/ |    ||" << endl;
    cout << "▓   ▓███╬ ╓▓╬╬████████████╬╬╬▄ ╠████_  ▓||  \\_/\\__,_|___/_|\\_\\_| |_| |_|\\__,_|___/\\__\\___|_|    ||" << endl;
    cout << "▌  ║████ ▐██╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬╬██▌ ████▌  ▌||                                                      ||" << endl;
    cout << "▌  ▓███▌ █████╬╬╬╬╬╬╬╬╬╬╬╬█████ ╟████  ▌||                                                      ||" << endl;
    cout << "▌  ████▌ ███████╬╬╬╬╬╬╬╬███████ ▐████  ▌||______                                                ||" << endl;
    cout << "▒  ╟████ ╟███████╬╬╬╬╬╬████████ ▓████  ▐||| ___ \\                                               ||" << endl;
    cout << "▓   ████▌ ▀███████╬╬╬╬████████ ╫████M  ▓||| |_/ / __ ___                                        ||" << endl;
    cout << "▓▌  ╙████▓_╙██████Ñ╬╬╬██████▀,▓████Ñ  ╫▓|||  __/ '__/ _ \\                                       ||" << endl;
    cout << "▓▓▓_  ▓████▓▄ ▀▀███╬╬╣███▀┘▄▓█████Γ  Æ▓▓||| |  | | | (_) |                                      ||" << endl;
    cout << "▓▓▓▓▄  ╙▓█████▓▌▄▄▄╓╓╓▄▄φ▓██████▀  ╓▓▓▓▓||\\_|  |_|  \\___/                                       ||" << endl;
    cout << "▓▓▓▓▓▓▄  `▀▓███████╬╬╬███████▀   ╓▓▓▓▓▓▓\\========================================================/" << endl;
    // Displays the current system date
    cout << "Current Date: " << TaskItem::getCurrentDate() << endl;
}
  
// Function to get user input for main menu
char getUserInput() {
    char input;
    cout << "\nEnter command (+) add, (-) remove, (?) display, (q) quit: ";
    cin >> input;
    return input;
}


// Utility function to get input for a new task
string getTaskDescription() {
    string description;
    cout << "\nEnter a description for the new task: ";
    cin.ignore(); // Clear input buffer
    getline(cin, description);
    while(description.empty()) {
        cout << "Cannot be empty. Please enter again: ";
        getline(cin, description);
    }
    return description;
}

// Utility function to get index for task removal
int getTaskIndexToRemove(int count) {
    int index;
    cout << "\nEnter the index of the task to remove: ";
    while(!(cin >> index) || index < 1 || index > count) {
        cout << "Please enter a number between 1 and " << count << ": ";
        cin.clear(); // Clear error flag
        cin.ignore(); // Clear input buffer
    }
    return index;
}