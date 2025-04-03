// Author: Thalia Ysassi
// Date: 3/31/25
// Function: Replicate an ALU with C++. Includes error messages for invalid cases. 

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

// Status Flags for operations
struct flags {
    int N_flag, Z_flag, C_flag, V_flag = 0;
};

// Function Prototypes
bool validOperation(const string& operation, uint32_t& numCount, bool& branch);
bool getNumbers(const string& operation, const vector<string>& numbers, uint32_t& num1, uint32_t& num2, uint32_t& num3, bool& isReg);
void computeOperation(const string& operation, uint32_t& num1, uint32_t& num2, uint32_t& num3, flags& statusFlags, uint32_t* registers, uint32_t* memory, bool& isReg);
void computeBranch(const string& operation, flags& statusFlags, const string& branchLabel);


int main () {
    uint32_t memory[5] = {0x0, 0x0, 0x0, 0x0, 0x0};  // initalize memory array to 0
    uint32_t registers[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};  // initialize register array to 0
    flags statusFlags;
    ifstream fin("pp2_input.txt");  // input file
    vector<string> fileLines;  // stores each individual line
    uint32_t num1, num2, num3 = 0;  // stores first, second, and third valid number
    uint32_t numCount = 0;  //stores amount of numbers needed
    bool branch;
    string branchLabel;
    bool isReg = false;
    
    if (!fin.eof()) {
        string line;  // stores individual line before stored into fileLines
        // read file input
        while (getline(fin, line)) {
            fileLines.push_back(line);
        }
    }
        fin.close();
    
        cout << "--- RESULTS ---" << endl;
        cout << endl;

        for (int i = 0; i < fileLines.size(); i++) {
            stringstream ss(fileLines[i]);  // Current line in file
            string operation;  // stores the valid operation
            string number;  // stores the next number in fileLine
            vector<string> numbers;  // stores the string of numbers
            ss >> operation;

            // prints original operation and results
            cout << fileLines[i] << endl;
            if (validOperation(operation, numCount, branch)) {  // checks if operation is valid
                ss.ignore();  // ignore extra whitespace char
                if (!branch) {
                    while (getline(ss, number, ',')) {  // get each number
                        if (ss.peek() == ' ') {  // ignores whitespace between commas
                            ss.ignore();
                        }
                        numbers.push_back(number);  // add to number list
                    }
                    if (getNumbers(operation, numbers, num1, num2, num3, isReg)) {
                        computeOperation(operation, num1, num2, num3, statusFlags, registers, memory, isReg);  // If all numbers are valid, compute the final operation
                    }
                } else {
                    getline(ss, branchLabel);
                    computeBranch(operation, statusFlags, branchLabel);
                }
            }
            cout << endl;
        }
        return 0;
    }
    
    
    //// Helper Functions Definitions ////
    bool validOperation(const string& operation, uint32_t& numCount, bool& branch) {
        bool valid = true;  // Used to check if operation is valid
        branch = false;
        if (operation == "ADD" || operation == "SUB" || operation == "AND" ||operation == "OR" 
            || operation == "XOR" ) {
            numCount = 3;
        } else if (operation == "MOV" || operation == "CMP" || operation == "LOAD" || operation == "STORE") {
            numCount = 2;
        } else if (operation == "BNE" || operation == "BEQ" || operation == "BAL") {
            numCount = 1;
            branch = true;  // Used in main to call computeBranch() function
        } else {
            cout << "Unsupported Operation" << endl;
            valid = false;
        }
        return valid;
    }
    
    bool getNumbers(const string& operation, const vector<string>& numbers, uint32_t& num1, uint32_t& num2, uint32_t& num3, bool& isReg) {
        isReg = false;
        bool valid = true; // Used to check if all numbers are valid
        stringstream ss;

        if (operation == "ADD" || operation == "SUB" || operation == "AND" ||operation == "OR" 
            || operation == "XOR") {
            if (numbers.size() == 3) {
                ss << numbers [0];
                if (ss.get() == 'R') {
                    ss >> hex >> num1;
                    ss.clear();

                    ss << numbers[1];
                    if (ss.get() == 'R') {
                        ss >> hex >> num2;  // According to input file, will always be an immediate value
                        ss.clear();

                        ss << numbers[2];
                        if (ss.get() == 'R') {
                            ss >> hex >> num3;  // According to input file, will always be an immediate value
                            isReg = true;
                        } else {
                            ss >> hex >> num3;
                        }
                    } else {
                        cout << "Invalid Instruction." << endl;
                        valid = false;
                    }
                }
            } else {
                cout << "Invalid Instruction. Invalid Operand Count." << endl;
                valid = false;
            }
         } else if (operation == "MOV" || operation == "CMP" || operation == "LOAD" || operation == "STORE") {
                if (numbers.size() == 2) {  // 2 numbers for binary or 1 number and 1 numShift for LSL & LSR
                    ss << numbers [0];
                    if (ss.get() == 'R') {
                        ss >> hex >> num1;
                        ss.clear();

                        ss << numbers[1];
                        if (ss.peek() == '#') {
                            ss.ignore();
                            isReg = true;  // sets as true because it is the memory index and not the value
                        }
                        ss >> hex >> num2;  // According to input file, will always be an immediate value

                        if (operation == "LOAD" || operation == "STORE")  {
                            if (num2 > 0x110 || num2 < 0x100) {  // checks if memory is valid
                                cout << "Invalid Instruction. Memory out-of-range." << endl;
                                valid = false;
                            }
                        }
                    }
                }
            }
        return valid;
}

    
    void computeOperation(const string& operation, uint32_t& num1, uint32_t& num2, 
         uint32_t& num3, flags& statusFlags, uint32_t* registers, uint32_t* memory, bool& isReg) {    
            if (operation != "MOV" && operation != "CMP" && operation != "LOAD" && operation != "STORE") {
                num2 = registers[num2];  // changes num2 to value at register[num2] instead of the index
                if (isReg) {
                    num3 = registers[num3];  // changes num3 to value at register[num3] instead of the index
                }
            } else if (operation == "LOAD" || operation == "STORE") {
                num2 = ((num2 - 0x100) / 4);  // get the correct index for the memory array
            }
        // Binary Operations
        if (operation == "ADD") {
            registers[num1] = num2 + num3;
        } else if (operation == "SUB") {
            registers[num1] = num2 - num3;
        } else if (operation == "AND") {
            registers[num1] = num2 & num3;
        } else if (operation == "OR") {
            registers[num1] = num2 | num3;
        } else if (operation == "XOR") {
            registers[num1] = num2 ^ num3;
        } else if (operation == "MOV" ) {
            registers[num1] = num2;
        } else if (operation == "CMP") {
            // perform CMP operation
            uint32_t unsigned_result = registers[num1] - num2;
            int32_t signed_result = registers[num1] = num2;

            // check for a zero result and set Z flag accordingly
            if (unsigned_result == 0) {
                statusFlags.Z_flag = 1;
            } else {
                statusFlags.Z_flag = 0;
            }

            // check for negative result and set N, V, and C flags
            if (signed_result < 0) {
                statusFlags.N_flag = 1;
                statusFlags.V_flag = 1;
                statusFlags.C_flag = 0;
            } else {
                statusFlags.N_flag = 0;
                statusFlags.V_flag = 0;
                statusFlags.C_flag = 1;
            }
        } else if (operation == "LOAD") {
            registers[num1] = memory[num2];
        } else if (operation == "STORE") {
            memory[num2] = registers[num1];
        }
    
        // print final results
        cout << "Register array: " << endl;  // print registers
        for (int i = 0; i < 8; i++) {
            cout << "R" << i << "=0x" << hex << registers[i] << " ";
        }
        cout << endl;
    
        cout << "Memory array: " << endl;  // print memory
        for (int i = 0; i < 5; i++) {
            if (memory[i] == 0) {
                cout << "___";
            } else {
                cout << "0x" << hex << memory[i];
            }
            
            if (i != 4) {
                cout << ",";
            }
        }
        cout << endl;

        // print flags if operation is CMP
        if (operation == "CMP") {
            cout << "Status flags: N = " << statusFlags.N_flag << ", Z = " << statusFlags.Z_flag
                << ", C = " << statusFlags.C_flag << ", V = " << statusFlags.V_flag << endl << endl;
        } else {
            cout << endl;
        }
    }

    void computeBranch(const string& operation, flags& statusFlags, const string& branchLabel) {
        if (operation == "BNE") {
            if (statusFlags.Z_flag == 0) {
                cout << "Branch will be taken to " << branchLabel << endl;
            } else {
                cout << "Branch will not be taken to " << branchLabel << endl;
            }
        } else if (operation == "BEQ") {
            if (statusFlags.Z_flag == 1) {
                cout << "Branch will be taken to " << branchLabel << endl;
            } else {
                cout << "Branch will not be taken to " << branchLabel << endl;
            }
        } else if (operation == "BAL") {
            cout << "Branch will be taken to " << branchLabel << endl;
        }
    }