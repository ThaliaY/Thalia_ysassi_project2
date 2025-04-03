# Thalia_ysassi_project2 Report
CS 219 Programming Assignment 2 (Spring 2025)

# Extra Credit
CMP performs the SUB operation but only updates the program's status flags.

N (Negative): N is set to the signed bit of the result (Bit 31). N = 1 if negative, N = 0 otherwise \
Z (Zero): Z = 1 if CMP results in a 0. Z = 0 otherwise \
C (Carry): C = 1 if CMP has no borrow. C = 0 otherwise \
V (Overflow): V = 1 when CMP results in an overflow (The subtracting number is larger than the number being subtracted) V = 0 otherwise

These flags are required to perform the branch operations. In this assignment, only the Z flag is needed to decide if the branch is taken. For BNE, the branch is taken if Z = 0, while for BEQ the branch is only taken if Z = 1. BAL doesn't depend on any flags, as it is always taken.

# How to Run Program | Makefile Commands
**make main**: executable function \
**make clean**: removes executable 

# How the Code Works
My code is split into a main function with four helper functions. The three helper functions are linked together to move onto the next operation as soon as possible, as it doesn't call the next function if the operands or operations are invalid. 

## main()
Declares arrays for both the memory and the registers. Reads each line from input file, first storing the operation. ValidOperation is called to check if valid, and if it returns as true, the program then parsing the rest ofthe remaining line into individual numbers or the branch label. If the operation is not a branch, GetNumbers() is called to check if the operands are valid. If the function returns as true, computeOperation() is called to perform the operation and print the results. If the operation is a branch, the rest of the file line is read as the branch label and computeBranch() is called. 

### validOperation()
Checks if operation is one of the supported operations (ADD, SUB, AND, OR, XOR, MOV, CMP, LOAD, STORE, BNE, BEQ, BAL) and sets numCount to required amount of operands. if operation isn't supported, valid is set to false

If operation is BNE, BEQ, or BAL, variable branch is set to true (used in main)

function returns valid

### getNumbers()
If operation needs 3 operands, function individually reads each number to check if the first two values are registers and is stored into num1 and num2. Then checks if num3 is a register, if so, isReg is set to true. Otherwise, the value is stored into num3

If operation needs 2 operands, function individually reads first char to check if it is a register. If so, the value is stored into num1. Then checks if num2 is a register, if so, isReg is set to true. Otherwise, the value is stored into num2

Otherwise, if numCount != numbers.size(), error message is printed

### computeOperation()
Uses the operation and num1, num2, and num3 variables to compute and store the correct values in either the registers or memory arrays.

The function then prints the register and memory arrays after performing the operation.

If the operation is CMP, the status flags are also updated and printed

### computeBranch()
Uses the operation and Z flag to decide if the branch is taken. Prints if the branch is or isn't taken to the branch label.

# Logic and Functionality 

## Three Operands (Arithmetic and Logical Operations)

1. ADD - Adds two numbers (num2 and num3), to be stored in the first given operand
2. SUB - Subtracts two numbers (num2 and num3), to be stored in the first given operand
3. AND - Compares two numbers, (num2 and num3), results in a 1 for every identical bit. Final answer is stored in the first given operand
4. OR - Compares two numbers, (num2 and num3), results in a 1 for every bit equal to 1. Final answer is stored in the first given operand
5. XOR - Compares two numbers, (num2 and num3), results in a 1 if only one bit is equal to 1. Final answer is stored in the first given operand

## Two Operands
6. MOV - Stores a value (num2) into the first given operand (num1)
7. CMP - Performs the SUB operation, but only changes the status flags
8. LOAD - Stores the value of what is in the memory array at index num2 into the register at index num1
9. STORE - Stores the value of what is in  the register at index num1 into the memory array at index num2

## Branch Operations
10. BNE - Takes the branch if the Zero Flag (Z) is set to 0
11. BEQ - Takes the branch if the Zero Flag (Z) is set to 1
12. BAL - Will always take the branch


# Test Cases
1. MOV R2,#0x20 \
R2 = #0x20
2. MOV R3,#0x40 \
R2 = #0x20, R3 = #0x40
3. ADD R1, R2, R3 \
R1 = #0x60 (#0x20 + #0x40), R2 = #0x20, R3 = #0x40
4. ADD R1,#0,R6 \
Invalid -> R1 = #0x60, R2 = #0x20, R3 = #0x40 (stays the same)
5. SUB R4, R1, #0x30 \
R1 = #0x60, R2 = #0x20, R3 = #0x40, R4 = #0x30 (#0x60 - #0x30)
6. AND R7, R4, R3 \
R1 = #0x60, R2 = #0x20, R3 = #0x40, R4 = #0x30, R7 = #0x00 (#0x30 & #0x40)
7. OR R7, R1 \
Invalid -> R1 = #0x60, R2 = #0x20, R3 = #0x40, R4 = #0x30, R7 = #0x00 (stays the same)
8. XOR R5, R4, R1 \
R1 = #0x60, R2 = #0x20, R3 = #0x40, R4 = #0x30, R5 = #0x50 (#0x30 | #0x60), R7 = #0x00
9. LOAD R6, 0x300 \
Invalid -> R1 = #0x60, R2 = #0x20, R3 = #0x40, R4 = #0x30, R5 = #0x50, R7 = #0x00 (stays the same)
10. STORE R1, 0x104 \
R1 = #0x60, R2 = #0x20, R3 = #0x40, R4 = #0x30, R5 = #0x50, R7 = #0x00 \
Memory: ____, #0x60, ____ , ____ , ____
11. LOAD R6, 0x104
R1 = #0x60, R2 = #0x20, R3 = #0x40, R4 = #0x30, R5 = #0x50, R6 = #0x60, R7 = #0x00 \
Memory: ____, #0x60, ____ , ____ , ____
12. CMP R1,#0x60 \
R1 = #0x60 \
(#0x60 == #0x60) True \
 Z flag = 1
13. BEQ LABEL_EQ \
 Z flag = 1 \
 Branch is taken
14. CMP R1,#0x60 \
R1 = #0x60 \
(#0x60 == #0x60) True \
 Z flag = 1
15. BNE LABEL_NEQ
 Z flag = 1 \
 Branch is not taken
16. BAL LABEL_ALWAYS \
Branch is taken


# Invalid Cases in File
4. ADD R1,#0,R6 \
Invalid. Only the third operand can be an immediate value

7. OR R7, R1 \
Invalid. Doesn't contain the correct amount of operands (3)

9. LOAD R6, 0x300 \
Invalid. 0x300 is out of our memory's 0x100 to 0x110 range

# Input Text
```
MOV R2,#0x20
MOV R3,#0x40
ADD R1, R2, R3
ADD R1,#0,R6
SUB R4, R1, #0x30
AND R7, R4, R3
OR R7, R1
XOR R5, R4, R1
LOAD R6, 0x300
STORE R1, 0x104
LOAD R6, 0x104
CMP R1,#0x60
BEQ LABEL_EQ
CMP R1,#0x60
BNE LABEL_NEQ
BAL LABEL_ALWAYS
```