#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <limits>  // Required for numeric_limits

namespace my_namespace {
    using std::cout;
    using std::cin;
    using std::endl;
    using std::string;
    using std::vector;
    using std::ifstream;
    using std::stringstream;
    using std::unique_ptr;
    using std::make_unique;
    using std::transform;
    using std::numeric_limits;
    using std::getline;
}

// Function to display the menu
void DisplayMenu() {
    my_namespace::cout << "1. Load Data Structure" << my_namespace::endl;
    my_namespace::cout << "2. Print Course List" << my_namespace::endl;
    my_namespace::cout << "3. Print Course" << my_namespace::endl;
    my_namespace::cout << "9. Exit" << my_namespace::endl << my_namespace::endl;
    my_namespace::cout << "What would you like to do? ";
}

// Struct to hold course information
struct Course {
    my_namespace::string courseId;
    my_namespace::string courseName;
    my_namespace::vector<my_namespace::string> preRequisite;
};

// Node structure for tree node
struct Node {
    Course course;
    my_namespace::unique_ptr<Node> left;
    my_namespace::unique_ptr<Node> right;

    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Define a class for the Binary Search Tree (BST)
class BinarySearchTree {

private:
    my_namespace::unique_ptr<Node> root;

    void Destruct(Node* node);
    void insert(my_namespace::unique_ptr<Node>& node, Course course);
    const Node* search(const Node* node, const my_namespace::string& courseId) const;
    void printCourse(const Node* node) const;

public:
    BinarySearchTree() : root(nullptr) {}
    void Insert(Course course);
    void Search(const my_namespace::string& courseId) const;
    void PrintCourse() const;
};

// Insert a course into the BST
void BinarySearchTree::Insert(Course course) {
    insert(root, course);
}

void BinarySearchTree::insert(my_namespace::unique_ptr<Node>& node, Course course) {
    if (!node) {
        node = my_namespace::make_unique<Node>(course);
    } else if (course.courseId < node->course.courseId) {
        insert(node->left, course);
    } else {
        insert(node->right, course);
    }
}

// Search for a course in the BST
void BinarySearchTree::Search(const my_namespace::string& courseId) const {
    const Node* node = search(root.get(), courseId);
    if (node) {
        my_namespace::cout << node->course.courseId << ", " << node->course.courseName << my_namespace::endl;
        my_namespace::cout << "Prerequisites: ";
        if (node->course.preRequisite.empty()) {
            my_namespace::cout << "None" << my_namespace::endl;
        } else {
            for (size_t i = 0; i < node->course.preRequisite.size(); ++i) {
                my_namespace::cout << node->course.preRequisite[i];
                if (i < node->course.preRequisite.size() - 1) my_namespace::cout << ", ";
            }
            my_namespace::cout << my_namespace::endl;
        }
    } else {
        my_namespace::cout << "Course " << courseId << " not found." << my_namespace::endl;
    }
}

const Node* BinarySearchTree::search(const Node* node, const my_namespace::string& courseId) const {
    if (!node || node->course.courseId == courseId) {
        return node;
    } else if (courseId < node->course.courseId) {
        return search(node->left.get(), courseId);
    } else {
        return search(node->right.get(), courseId);
    }
}

// Print all courses in alphanumeric order
void BinarySearchTree::PrintCourse() const {
    printCourse(root.get());
}

void BinarySearchTree::printCourse(const Node* node) const {
    if (!node) return;
    printCourse(node->left.get());
    my_namespace::cout << node->course.courseId << ", " << node->course.courseName << my_namespace::endl;
    printCourse(node->right.get());
}

// Load courses from file and insert them into the BST
void loadCourse(const my_namespace::string& filename, BinarySearchTree& bst) {
    my_namespace::ifstream file(filename);
    if (!file.is_open()) {
        my_namespace::cout << "File error, please try again." << my_namespace::endl;
        return;
    }
    my_namespace::cout << "File loaded." << my_namespace::endl;

    my_namespace::string line;
    while (my_namespace::getline(file, line)) {
        my_namespace::stringstream str(line);
        my_namespace::string word;
        Course course;
        int index = 0;

        // Read the course ID and name
        while (index < 2 && my_namespace::getline(str, word, ',')) {
            if (index == 0) {
                course.courseId = word;
            } else if (index == 1) {
                course.courseName = word;
            }
            index++;
        }

        // Read the prerequisites
        while (my_namespace::getline(str, word, ',')) {
            course.preRequisite.push_back(word);
        }

        // Insert the course into the BST
        bst.Insert(course);
    }
}

int main() {
    BinarySearchTree bst;  // Create a binary search tree to hold courses

    my_namespace::string fileChoice;     // Variable to store the file name input by the user
    my_namespace::string courseChoice;   // Variable to store the course ID input by the user

    int userInput = 0;     // Variable to store the user's menu choice

    my_namespace::cout << "Welcome to the course planner." << my_namespace::endl << my_namespace::endl;

    // Main loop for the program
    while (userInput != 9) {
        DisplayMenu();     // Display the menu options
        my_namespace::cin >> userInput;  // Get the user's menu choice

        // Clear the input buffer to handle leftover input
        if(my_namespace::cin.fail()) {
            my_namespace::cin.clear();  // Clear error flags
            my_namespace::cin.ignore(my_namespace::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
            my_namespace::cout << "Invalid input. Please enter a number corresponding to the menu options." << my_namespace::endl;
            continue;  // Skip the rest of the loop and prompt the user again
        }

        // Handle the user's menu choice
        switch (userInput) {
        case 1:
            my_namespace::cout << "Enter the file name: ";
            my_namespace::cin >> fileChoice;  // Get the file name from the user
            loadCourse(fileChoice, bst);  // Load the courses from the file into the BST
            break;
        case 2:
            my_namespace::cout << "Course List:" << my_namespace::endl;
            bst.PrintCourse();  // Print all courses in the BST
            break;
        case 3:
            my_namespace::cout << "Enter the course ID: ";
            my_namespace::cin >> courseChoice;  // Get the course ID from the user
            my_namespace::transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);  // Convert to uppercase
            bst.Search(courseChoice);  // Search and display the course details
            break;
        case 9:
            my_namespace::cout << "Thank you for using the course planner!" << my_namespace::endl;
            break;
        default:
            my_namespace::cout << userInput << " is not a valid option." << my_namespace::endl;
            break;
        }
    }

    return 0;  // Exit the program
}
