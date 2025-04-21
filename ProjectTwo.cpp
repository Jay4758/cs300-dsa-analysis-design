/**
 * ProjectTwo.cpp
 * 
 * ABCU Advising Assistance Program
 * 
 * This program reads course data from a CSV file and allows academic advisors to:
 * - Load course data into a data structure
 * - Print an alphanumeric list of all courses
 * - Look up and display information about specific courses
 */

 #include <iostream>
 #include <fstream>
 #include <string>
 #include <vector>
 #include <algorithm>
 #include <sstream>
 #include <limits>
 
 using namespace std;
 
 // Course class to store course information
 class Course {
 private:
     string courseNumber;
     string courseTitle;
     vector<string> prerequisites;
 
 public:
     // Default constructor
     Course() {}
 
     // Parameterized constructor
     Course(string number, string title) {
         this->courseNumber = number;
         this->courseTitle = title;
     }
 
     // Setters
     void setCourseNumber(string number) {
         this->courseNumber = number;
     }
 
     void setCourseTitle(string title) {
         this->courseTitle = title;
     }
 
     void addPrerequisite(string prerequisite) {
         this->prerequisites.push_back(prerequisite);
     }
 
     // Getters
     string getCourseNumber() const {
         return this->courseNumber;
     }
 
     string getCourseTitle() const {
         return this->courseTitle;
     }
 
     vector<string> getPrerequisites() const {
         return this->prerequisites;
     }
 };
 
 // Binary Search Tree structure for organizing courses
 struct Node {
     Course course;
     Node* left;
     Node* right;
 
     // Default constructor
     Node() {
         left = nullptr;
         right = nullptr;
     }
 
     // Parameterized constructor
     Node(Course aCourse) : Node() {
         this->course = aCourse;
     }
 };
 
 // Binary Search Tree class 
 class CourseBST {
 private:
     Node* root;
 
     // Private helper method to add a node
     void addNode(Node* node, Course course);
     
     // Print courses in order (inorder traversal)
     void inOrder(Node* node);
     
     // Find a course by course number
     Course findCourse(Node* node, string courseNumber);
 
 public:
     // Default constructor
     CourseBST();
     
     // Destructor
     ~CourseBST();
     
     // Delete all nodes in the tree (used by destructor)
     void destroyTree(Node* node);
     
     // Public methods
     void Insert(Course course);
     void PrintCourseList();
     Course Search(string courseNumber);
     bool IsEmpty() const;
 };
 
 // Default constructor
 CourseBST::CourseBST() {
     // Initialize empty tree
     root = nullptr;
 }
 
 // Destructor
 CourseBST::~CourseBST() {
     // Call recursive helper function to delete all nodes
     destroyTree(root);
 }
 
 // Check if the tree is empty
 bool CourseBST::IsEmpty() const {
     return root == nullptr;
 }
 
 // Destroy tree nodes - recursive helper for destructor
 void CourseBST::destroyTree(Node* node) {
     if (node != nullptr) {
         // Recursively delete left and right subtrees
         destroyTree(node->left);
         destroyTree(node->right);
         
         // Delete current node
         delete node;
     }
 }
 
 // Add a course to the BST
 void CourseBST::Insert(Course course) {
     // If tree is empty, create new root
     if (root == nullptr) {
         root = new Node(course);
     }
     else {
         // Use the addNode helper method to find the right location
         this->addNode(root, course);
     }
 }
 
 // Recursive helper method to add a node in the correct position
 void CourseBST::addNode(Node* node, Course course) {
     // Compare course numbers to determine where to place the new course
     if (course.getCourseNumber() < node->course.getCourseNumber()) {
         // If left child is nullptr, create a new node here
         if (node->left == nullptr) {
             node->left = new Node(course);
         }
         // Otherwise, recursively call addNode on left subtree
         else {
             this->addNode(node->left, course);
         }
     }
     else {
         // If right child is nullptr, create a new node here
         if (node->right == nullptr) {
             node->right = new Node(course);
         }
         // Otherwise, recursively call addNode on right subtree
         else {
             this->addNode(node->right, course);
         }
     }
 }
 
 // Print course list in alphanumeric order
 void CourseBST::PrintCourseList() {
     cout << "\n-----------------------------------------" << endl;
     cout << "            COURSE LISTING              " << endl;
     cout << "-----------------------------------------" << endl;
     // Call recursive inOrder function starting from root
     this->inOrder(root);
     cout << "-----------------------------------------" << endl;
 }
 
 // In-order traversal for printing course list
 void CourseBST::inOrder(Node* node) {
     if (node != nullptr) {
         // Traverse left subtree
         inOrder(node->left);
         
         // Print current node's course information
         cout << node->course.getCourseNumber() << ", " 
              << node->course.getCourseTitle() << endl;
         
         // Traverse right subtree
         inOrder(node->right);
     }
 }
 
 // Search for a course by course number
 Course CourseBST::Search(string courseNumber) {
     // Trim leading and trailing spaces
     courseNumber.erase(0, courseNumber.find_first_not_of(" \t"));
     courseNumber.erase(courseNumber.find_last_not_of(" \t") + 1);
     
     // Convert search key to uppercase for case-insensitive search
     transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
     
     // Call recursive function to search starting from root
     return this->findCourse(root, courseNumber);
 }
 
 // Recursive function to find a course
 Course CourseBST::findCourse(Node* node, string courseNumber) {
     // Create an empty course to return if the course is not found
     Course course;
     
     // If reached the end of branch without finding the course
     if (node == nullptr) {
         return course;
     }
     
     // Convert current node's course number to uppercase for comparison
     string currentCourseNum = node->course.getCourseNumber();
     transform(currentCourseNum.begin(), currentCourseNum.end(), currentCourseNum.begin(), ::toupper);
     
     // Trim any leading/trailing spaces for comparison
     currentCourseNum.erase(0, currentCourseNum.find_first_not_of(" \t"));
     currentCourseNum.erase(currentCourseNum.find_last_not_of(" \t") + 1);
     
     // If found the course, return it
     if (currentCourseNum == courseNumber) {
         return node->course;
     }
     
     // Determine which subtree to search
     if (courseNumber < currentCourseNum) {
         // Search left subtree
         return this->findCourse(node->left, courseNumber);
     }
     else {
         // Search right subtree
         return this->findCourse(node->right, courseNumber);
     }
 }
 
 // Parse each line of the CSV file
 vector<string> parseCSVLine(string line) {
     vector<string> tokens;
     stringstream ss(line);
     string token;
     
     // Get tokens separated by commas
     while (getline(ss, token, ',')) {
         tokens.push_back(token);
     }
     
     return tokens;
 }
 
 // Load courses from CSV file into the BST
 bool loadCourses(string filename, CourseBST& courseBST) {
     // Open file
     ifstream file(filename);
     
     // Check if file is open
     if (!file.is_open()) {
         cout << "Error: Could not open file \"" << filename << "\"" << endl;
         cout << "Make sure the file exists and the name is typed correctly including spaces." << endl;
         return false;
     }
     
     // Read each line from the file
     string line;
     int courseCount = 0;
     
     while (getline(file, line)) {
         // Parse the line into tokens
         vector<string> tokens = parseCSVLine(line);
         
         // Check if we have at least course number and title
         if (tokens.size() >= 2) {
             // Create new course
             Course course(tokens[0], tokens[1]);
             
             // Add prerequisites if they exist (starting from index 2)
             for (size_t i = 2; i < tokens.size(); ++i) {
                 if (!tokens[i].empty()) {
                     course.addPrerequisite(tokens[i]);
                 }
             }
             
             // Insert course into BST
             courseBST.Insert(course);
             courseCount++;
         }
     }
     
     // Close file
     file.close();
     
     if (courseCount > 0) {
         cout << "File loaded successfully!" << endl;
         cout << "Loaded " << courseCount << " courses." << endl;
         return true;
     } else {
         cout << "Warning: No valid course data found in the file." << endl;
         return false;
     }
 }
 
 // Get course information including prerequisites
 void printCourseInformation(CourseBST& courseBST, string courseNumber) {
     // Search for the course
     Course course = courseBST.Search(courseNumber);
     
     // Display header with separating lines
     cout << "\n-----------------------------------------" << endl;
     cout << "          COURSE INFORMATION            " << endl;
     cout << "-----------------------------------------" << endl;
     
     // Check if course was found
     if (course.getCourseNumber().empty()) {
         cout << "Course \"" << courseNumber << "\" not found." << endl;
         cout << "-----------------------------------------" << endl;
         return;
     }
     
     // Print course information
     cout << course.getCourseNumber() << ", " << course.getCourseTitle() << endl;
     
     // Get prerequisites
     vector<string> prerequisites = course.getPrerequisites();
     
     // Print prerequisites if they exist
     if (!prerequisites.empty()) {
         cout << "Prerequisites: ";
         
         // Print all prerequisites except the last one
         for (size_t i = 0; i < prerequisites.size() - 1; ++i) {
             cout << prerequisites[i] << ", ";
         }
         
         // Print the last prerequisite
         cout << prerequisites.back();
     } else {
         cout << "No prerequisites";
     }
     cout << endl;
     cout << "-----------------------------------------" << endl;
 }
 
 // Display the menu and get user choice
 int displayMenu() {
     int choice = 0;
     
     // Display menu options with separating lines
     cout << "\n=========================================" << endl;
     cout << "               MAIN MENU                " << endl;
     cout << "=========================================" << endl;
     cout << "1. Load Data Structure" << endl;
     cout << "2. Print Course List" << endl;
     cout << "3. Print Course" << endl;
     cout << "9. Exit" << endl;
     cout << "=========================================" << endl;
     cout << "What would you like to do? ";
     
     // Get user input
     string input;
     getline(cin, input);
     
     // Convert string to integer
     try {
         choice = stoi(input);
     } catch (const exception&) {
         // If conversion fails, invalid option
         choice = 0;
     }
     
     return choice;
 }
 
 int main() {
     // Create the course binary search tree
     CourseBST courseBST;
     
     // Track if data has been loaded
     bool dataLoaded = false;
     
     // Welcome message with decorative borders
     cout << "\n=========================================" << endl;
     cout << "     WELCOME TO THE COURSE PLANNER      " << endl;
     cout << "=========================================" << endl;
     
     // Program loop
     int choice = 0;
     while (choice != 9) {
         // Display menu and get user choice
         choice = displayMenu();
         
         // Process user choice
         switch (choice) {
             case 1: { // Load Data Structure
                 cout << "\n-----------------------------------------" << endl;
                 cout << "            LOAD DATA FILE              " << endl;
                 cout << "-----------------------------------------" << endl;
                 
                 string filename;
                 cout << "Enter the file name containing course data: ";
                 getline(cin, filename); // Get the entire line including spaces
                 
                 // Load courses from file
                 dataLoaded = loadCourses(filename, courseBST);
                 cout << "-----------------------------------------" << endl;
                 break;
             }
             case 2: // Print Course List
                 if (!dataLoaded || courseBST.IsEmpty()) {
                     cout << "\n-----------------------------------------" << endl;
                     cout << "               WARNING                   " << endl;
                     cout << "-----------------------------------------" << endl;
                     cout << "Please load data first (option 1)." << endl;
                     cout << "-----------------------------------------" << endl;
                     break;
                 }
                 cout << "\nHere is a sample schedule:";
                 courseBST.PrintCourseList();
                 break;
             case 3: { // Print Course
                 if (!dataLoaded || courseBST.IsEmpty()) {
                     cout << "\n-----------------------------------------" << endl;
                     cout << "               WARNING                   " << endl;
                     cout << "-----------------------------------------" << endl;
                     cout << "Please load data first (option 1)." << endl;
                     cout << "-----------------------------------------" << endl;
                     break;
                 }
                 string courseNumber;
                 cout << "\n-----------------------------------------" << endl;
                 cout << "           COURSE LOOKUP                 " << endl;
                 cout << "-----------------------------------------" << endl;
                 cout << "What course do you want to know about? ";
                 getline(cin, courseNumber); // Use getline to handle potential spaces
                 
                 // Extract just the course number if full course info was entered
                 size_t commaPos = courseNumber.find(',');
                 if (commaPos != string::npos) {
                     courseNumber = courseNumber.substr(0, commaPos);
                 }
 
                 // Trim leading and trailing spaces
                 courseNumber.erase(0, courseNumber.find_first_not_of(" \t"));
                 courseNumber.erase(courseNumber.find_last_not_of(" \t") + 1);
                 
                 // Print information for the specified course
                 printCourseInformation(courseBST, courseNumber);
                 break;
             }
             case 9: // Exit
                 cout << "\n=========================================" << endl;
                 cout << "Thank you for using the course planner!" << endl;
                 cout << "=========================================" << endl;
                 break;
             default:
                 cout << "\n-----------------------------------------" << endl;
                 cout << "               ERROR                     " << endl;
                 cout << "-----------------------------------------" << endl;
                 if (choice != 0) { // Only print this if user entered a number
                     cout << choice << " is not a valid option." << endl;
                 } else {
                     cout << "Invalid input. Please enter a number." << endl;
                 }
                 cout << "-----------------------------------------" << endl;
                 break;
         }
     }
     
     return 0;
 }