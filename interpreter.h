#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Executes instructions and updates registers
int execute_instructions(const vector<string>& instructions, vector<int>& r) {
    int pc = 0;
    int total_cycles = 0;
    
    while (pc >= 0 && pc < instructions.size()) {
        istringstream iss(instructions[pc]);
        string op;
        iss >> op;

        if (op == "add" || op == "sub" || op == "mul" || op == "bor" || op == "band") {
            int d, a, b;
            if (!(iss >> d >> a >> b) || d >= 16 || a >= 16 || b >= 16) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            if (op == "add") r[d] = r[a] + r[b], total_cycles += 1;
            else if (op == "sub") r[d] = r[a] - r[b], total_cycles += 1;
            else if (op == "mul") r[d] = r[a] * r[b], total_cycles += 3;
            else if (op == "bor") r[d] = r[a] | r[b], total_cycles += 1;
            else if (op == "band") r[d] = r[a] & r[b], total_cycles += 1;
        }
        else if (op == "shl" || op == "shr") {
            int d, a;
            if (!(iss >> d >> a) || d >= 16 || a >= 16) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            int shiftAmt = r[a];
            if (op == "shl") r[d] = r[d] << shiftAmt;
            else r[d] = r[d] >> shiftAmt;
            total_cycles += 1;
        }
        else if (op == "li") {
            int d, imm;
            if (!(iss >> d >> imm) || d >= 16) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            r[d] = imm;
            total_cycles += 1;
        }
        else if (op == "jz" || op == "jp") {
            int a, offset;
            if (!(iss >> a >> offset) || a >= 16) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            if ((op == "jz" && r[a] == 0) || (op == "jp" && r[a] > 0)) {
                total_cycles += 3;
                pc += offset;
                continue;
            }
        }
        else {
            cerr << "Unknown instruction '" << op << "' at line " << pc << "\n";
        }
        pc++;
    }
    return total_cycles;
}

#endif // INTERPRETER_H
