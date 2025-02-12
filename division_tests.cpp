#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <numeric>

using namespace std;

// Runs the interpreter and extracts cycle count, quotient, and remainder
int run_interpreter(const string& filename, const string& init_values, int expected_quotient, int expected_remainder) {
    string command = "./interpreter " + filename + " " + init_values + " > output.txt";
    system(command.c_str());

    ifstream fin("output.txt");
    string line;
    int total_cycles = -1, reg2 = -1, reg3 = -1;

    while (getline(fin, line)) {
        if (line.find("Your program took") != string::npos) {
            sscanf(line.c_str(), "Your program took %d cycles.", &total_cycles);
        } else if (line.find("r[2] =") != string::npos) {
            sscanf(line.c_str(), "r[2] = %d", &reg2);
        } else if (line.find("r[3] =") != string::npos) {
            sscanf(line.c_str(), "r[3] = %d", &reg3);
        }
    }
    fin.close();

    // Validate the output
    if (reg2 == expected_quotient && reg3 == expected_remainder) {
        cout << "PASS: " << init_values << " -> Quotient: " << reg2 << ", Remainder: " << reg3 << endl;
    } else {
        cout << "FAIL: " << init_values << " -> Got Quotient: " << reg2 << ", Remainder: " << reg3
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
    srand(time(0));
    const int num_tests = 40;
    vector<int> cycle_counts;

    for (int i = 0; i < num_tests; i++) {
        int dividend = rand() % 1000 + 1;  // Avoid zero dividend
        int divisor = rand() % 10 + 1;     // Avoid zero divisor
        int expected_quotient = dividend / divisor;
        int expected_remainder = dividend % divisor;
        string init_values = to_string(dividend) + "," + to_string(divisor);

        int cycles = run_interpreter(filename, init_values, expected_quotient, expected_remainder);
        if (cycles > 0) {
            cycle_counts.push_back(cycles);
        }
    }

    // Compute and display the average cycle count
    double average_cycles = cycle_counts.empty() ? 0 : 
                            (double)accumulate(cycle_counts.begin(), cycle_counts.end(), 0) / cycle_counts.size();

    cout << "Tested " << cycle_counts.size() << " valid cases." << endl;
    cout << "Average cycles: " << average_cycles << endl;

    return 0;
}
