#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>
#include <fstream>
#include "interpreter.h"

using namespace std;

// Runs the interpreter in-memory and extracts cycle count, quotient, and remainder
int run_interpreter(vector<string> instructions, const string& init_values, int expected_quotient, int expected_remainder) {
    vector<int> r(16, 0);
    stringstream ss(init_values);
    string token;
    int regIndex = 0;
    while (getline(ss, token, ',') && regIndex < 16) {
        try {
            r[regIndex] = stoi(token);
        } catch (const invalid_argument&) {
            cerr << "Invalid initial register value: " << token << "\n";
            return -1;
        }
        regIndex++;
    }

    int total_cycles = execute_instructions(instructions, r);
    
    if (r[2] == expected_quotient && r[3] == expected_remainder) {
        cout << "PASS: " << init_values << " -> Quotient: " << r[2] << ", Remainder: " << r[3] << endl;
    } else {
        cout << "FAIL: " << init_values << " -> Got Quotient: " << r[2] << ", Remainder: " << r[3]
             << " (Expected: " << expected_quotient << ", " << expected_remainder << ")" << endl;
    }
    return total_cycles;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error opening " << filename << "\n";
        return 1;
    }

    vector<string> instructions;
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) {
            instructions.push_back(line);
        }
    }
    fin.close();

    srand(time(0));
    const int num_tests = 40;
    vector<int> cycle_counts;

    for (int i = 0; i < num_tests; i++) {
        int dividend = rand() % 1000 + 1;
        int divisor = rand() % 10 + 1;
        int expected_quotient = dividend / divisor;
        int expected_remainder = dividend % divisor;
        string init_values = to_string(dividend) + "," + to_string(divisor);

        int cycles = run_interpreter(instructions, init_values, expected_quotient, expected_remainder);
        if (cycles > 0) {
            cycle_counts.push_back(cycles);
        }
    }

    double average_cycles = cycle_counts.empty() ? 0 : 
                            (double)accumulate(cycle_counts.begin(), cycle_counts.end(), 0) / cycle_counts.size();

    cout << "Tested " << cycle_counts.size() << " valid cases." << endl;
    cout << "Average cycles: " << average_cycles << endl;
    
    return 0;
}
