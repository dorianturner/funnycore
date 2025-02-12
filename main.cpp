#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " filename [init_reg_values]\n";
        return 1;
    }
    
    // choose your file
    string filename = argv[1];
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error opening " << filename << "\n";
        return 1;
    }

    // init registers and values (if given), also pc
    vector<string> instructions;
    string line;
    while (getline(fin, line))
        if (!line.empty())
            instructions.push_back(line);
    fin.close();
    vector<int> r(16, 0);

    if (argc >= 3) {
        string initRegs = argv[2];
        stringstream ss(initRegs);
        string token;
        int regIndex = 0;
        while (getline(ss, token, ',') && regIndex < 16) {
            try {
                r[regIndex] = stoi(token);
            } catch (const invalid_argument&) {
                cerr << "Invalid initial register value: " << token << "\n";
            }
            regIndex++;
        }
    }

    int pc = 0;
    int total_cycles = 0;
    while (pc >= 0 && pc < instructions.size()) {
        istringstream iss(instructions[pc]);
        string op;
        iss >> op;

        // arithmetic / bitwise ops
        if (op == "add" || op == "sub" || op == "mul" ||
            op == "bor" || op == "band") {
            int d, a, b;
            if (!(iss >> d >> a >> b)) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            if (d < 0 || d >= 16 || a < 0 || a >= 16 || b < 0 || b >= 16) {
                cerr << "Register out-of-bound at instruction " << pc << "\n";
                pc++;
                continue;
            }
            if (op == "add") {
                r[d] = r[a] + r[b];
                total_cycles += 1;
            } else if (op == "sub") {
                r[d] = r[a] - r[b];
                total_cycles += 1;
            } else if (op == "mul") {
                r[d] = r[a] * r[b];
                total_cycles += 3;
            } else if (op == "bor") {
                r[d] = r[a] | r[b];
                total_cycles += 1;
            } else if (op == "band") {
                r[d] = r[a] & r[b];
                total_cycles += 1;
            }
        }
        // shift ops
        else if (op == "shl" || op == "shr") {
            int d, a;
            if (!(iss >> d >> a)) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            if (d < 0 || d >= 16 || a < 0 || a >= 16) {
                cerr << "Register out-of-bound at instruction " << pc << "\n";
                pc++;
                continue;
            }
            int shiftAmt = r[a];
            if (op == "shl") {
                r[d] = r[d] << shiftAmt;
                total_cycles += 1;
            } else { // "shr"
                r[d] = r[d] >> shiftAmt;
                total_cycles += 1;
            }
        }
        // load immediate
        else if (op == "li") {
            int d, imm;
            if (!(iss >> d >> imm)) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            if (d < 0 || d >= 16) {
                cerr << "Register out-of-bound at instruction " << pc << "\n";
                pc++;
                continue;
            }
            r[d] = imm;
            total_cycles += 1;
        }
        // conditional jumps
        else if (op == "jz" || op == "jp") {
            int a, offset;
            if (!(iss >> a >> offset)) {
                cerr << "Parse error at instruction " << pc << "\n";
                pc++;
                continue;
            }
            if (a < 0 || a >= 16) {
                cerr << "Register out-of-bound at instruction " << pc << "\n";
                pc++;
                continue;
            }
            bool doJump = (op == "jz" && r[a] == 0) ||
                          (op == "jp" && r[a] > 0);
            if (doJump) {
                total_cycles += 3;
                pc += offset;
                continue; // Skip the normal PC increment.
            }
        }
        else {
            cerr << "Unknown instruction '" << op << "' at line " << pc << "\n";
        }
        pc++;
    }

    // register output
    cout << "Your program took " << total_cycles << " cycles." << endl;
    for (int i = 0; i < 16; i++)
        cout << "r[" << i << "] = " << r[i] << "\n";

    return 0;
}
