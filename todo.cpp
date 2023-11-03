#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

const string FILENAME = "todo_list.txt"; // File for saving and loading todos

// Utility function to get current system date as a string in "YYYY-MM-DD" format
// Specification A1 - Get current date
string getCurrentSystemDate() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime); // Get the current time
    timeinfo = localtime(&rawtime); // Convert the time to local time

    // Format the time to a string
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);

    return string(buffer);
}

// Class representing a single Todo item
// Specification B1 - TodoItem class definition
class TodoItem {
private:
    string task; // Task description
    int id; // Unique ID for the todo item
    string dateAdded; // Date when the task was added

public:
    // Parameterized constructor
    // Specification C1 - Parameterized constructor
    TodoItem(string taskInput, int idInput, string dateInput)
        : task(taskInput), id(idInput), dateAdded(dateInput) {}

    // Constructor for creating new TodoItem with a task
    // Specification C2 - Constructor with task input
    TodoItem(string taskInput) 
        : task(taskInput), id(rand() % 9999 + 1), dateAdded(getCurrentSystemDate()) {
        // Initialization is done via member initializer list
    }

    // Copy constructor
    // Specification C3 - Copy constructor
    TodoItem(const TodoItem& other)
        : task(other.task), id(other.id), dateAdded(other.dateAdded) {}

    // Assignment operator
    // Specification C4 - Assignment operator
    TodoItem& operator=(const TodoItem& other) {
        if (this != &other) { // Self-assignment check
            task = other.task;
            id = other.id;
            dateAdded = other.dateAdded;
        }
        return *this; // Enable chaining
    }

    // Getters
    int getId() const { return id; }
    string getTask() const { return task; }
    string getDateAdded() const { return dateAdded; }

    // Method to print the todo item
    void print() const {
        cout << "TODO ID: " << id << " | Task: " << task << " | Date Added: " << dateAdded << endl;
    }

    // Setter for task, also generates a new ID
    void setTask(const string& newTask) {
        task = newTask;
        id = rand() % 9999 + 1; // Generate a new random ID
        // If you want to update the date when the task changes:
        // dateAdded = getCurrentSystemDate();
    }

    // Destructor
    ~TodoItem() {
        // Currently empty as there's no dynamic allocation
    }
};


// Function Prototypes
void saveTodos(TodoItem** todoList, int todoCount);
void loadTodos(TodoItem** &todoList, int &todoCount, int &capacity);
void generateTestTodos(TodoItem** &todoList, int &todoCount);
void expandTodoList(TodoItem** &todoList, int &capacity);
void listTodos(TodoItem** todoList, int todoCount);
void removeTodoById(TodoItem** &todoList, int &todoCount, int id);


int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initial capacity for the todo list
    int capacity = 10;
    // Number of todos currently in the list
    int todoCount = 0;
    // Allocate the array for storing todo items
    TodoItem** todoList = new TodoItem*[capacity];

    // Specification A3 - Load existing todos from a file on startup
    loadTodos(todoList, todoCount, capacity);

    // User command input
    char command;
    while (true) {
        cout << "Enter command (+ to add, ? to list, - to remove, q to quit): ";
        cin >> command;

        // Error handling for user input
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            // Ignore invalid input
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "You can only enter single character commands." << endl;
            continue; // Skip the rest of the loop
        }

        // Clear the buffer after reading a character
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Add a new todo item
        if (command == '+') {
            string task;
            cout << "Enter the task: ";
            getline(cin, task);

            // Check if there is a need to expand the array
            if (todoCount == capacity) {
                expandTodoList(todoList, capacity);
            }

            // Add the new TodoItem to the list
            todoList[todoCount++] = new TodoItem(task);
            cout << "Item added." << endl;
        } 
        // List all todo items
        else if (command == '?') {
            // Specification B2 - List all todos
            listTodos(todoList, todoCount);
        } 
        // Remove a todo item by ID
        else if (command == '-') {
            int idToRemove;
            cout << "Enter TODO ID to remove: ";
            cin >> idToRemove;
            // Specification B3 - Remove the todo with given ID
            removeTodoById(todoList, todoCount, idToRemove);
        } 
        // Quit the program
        else if (command == 'q') {
            cout << "Exiting program." << endl;
            break; // Break out of the loop to end the program
        } 
        // Handle invalid commands
        else {
            cout << "Invalid command, try again." << endl;
        }
    }

    // Specification A2 - Save todos to a file before quitting
    saveTodos(todoList, todoCount);

    // Cleanup before exiting
    for (int i = 0; i < todoCount; ++i) {
        delete todoList[i]; // Delete individual TodoItems
    }
    delete[] todoList; // Delete the array of pointers

    return 0; // End of program
}


// Function to save the list of TodoItems to a file
// Specification A2 - Save todos to a file
void saveTodos(TodoItem** todoList, int todoCount) {
    ofstream file(FILENAME);
    for (int i = 0; i < todoCount; ++i) {
        if (todoList[i] != nullptr) {
            // Write the todo item's details to the file
            file << todoList[i]->getId() << endl
                 << todoList[i]->getTask() << endl
                 << todoList[i]->getDateAdded() << endl;
        }
    }
    file.close(); // Close the file after writing
}

// Function to load the list of TodoItems from a file
// Specification A3 - Load todos from a file
void loadTodos(TodoItem** &todoList, int &todoCount, int &capacity) {
    ifstream file(FILENAME);
    if (file) {
        string task, dateAdded;
        int id;
        // Read todo items until the end of file is reached
        while (file >> id && getline(file.ignore(), task) && getline(file, dateAdded)) {
            if (todoCount == capacity) {
                // Specification E1 - Expand the list if capacity is reached
                // Logic to expand the todoList if needed (not shown in this snippet)
            }
            // Create new TodoItem and add it to the list
            todoList[todoCount++] = new TodoItem(task, id, dateAdded);
        }
    }
    file.close(); // Close the file after loading
}

// Function to generate test TodoItems
// Specification A4 - Generate test todos
void generateTestTodos(TodoItem** &todoList, int &todoCount) {
    for (int i = 0; i < 5; ++i) {
        string testTask = "Test Task " + to_string(i + 1);
        // Add new TodoItem with a test task to the list
        todoList[todoCount++] = new TodoItem(testTask);
    }
}

// Function to expand the TodoList array when the capacity is reached
void expandTodoList(TodoItem** &todoList, int &capacity) {
    int newCapacity = capacity * 2; // Double the capacity
    TodoItem** newList = new TodoItem*[newCapacity];

    // Copy existing todo items to the new list
    for (int i = 0; i < capacity; ++i) {
        newList[i] = todoList[i];
    }

    delete[] todoList; // Free old list memory
    todoList = newList; // Point to the new list
    capacity = newCapacity; // Update the capacity
}

// Function to list all TodoItems
// Specification B2 - List todos
void listTodos(TodoItem** todoList, int todoCount) {
    for (int i = 0; i < todoCount; ++i) {
        if (todoList[i] != nullptr) {
            // Print details of the todo item
            todoList[i]->print();
        }
    }
}

// Function to remove a TodoItem by ID
// Specification B3 - Remove todo by ID
void removeTodoById(TodoItem** &todoList, int &todoCount, int id) {
    for (int i = 0; i < todoCount; ++i) {
        if (todoList[i] != nullptr && todoList[i]->getId() == id) {
            delete todoList[i]; // Delete the todo item
            for (int j = i; j < todoCount - 1; ++j) {
                // Shift items down to fill the gap
                todoList[j] = todoList[j + 1];
            }
            todoList[todoCount - 1] = nullptr; // Clear the last element
            --todoCount; // Decrease count
            cout << "TODO with ID " << id << " has been removed." << endl;
            return;
        }
    }
    cout << "TODO with ID " << id << " not found." << endl;
}

