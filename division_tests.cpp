#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int run_interpreter(const string& filename, const string& init_values) {
    string command = "./interpreter " + filename + " " + init_values + " > output.txt";
    int result = system(command.c_str());
    
    ifstream fin("output.txt");
    string line;
    int total_cycles = -1;
    while (getline(fin, line)) {
        if (line.find("Your program took") != string::npos) {
            sscanf(line.c_str(), "Your program took %d cycles.", &total_cycles);
            break;
        }
    }
    fin.close();
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
        int dividend = rand() % 1000 + 1; // Avoid zero dividend
        int divisor = rand() % 10 + 1;   // Avoid zero divisor
        string init_values = to_string(dividend) + "," + to_string(divisor);
        
        int cycles = run_interpreter(filename, init_values);
        if (cycles > 0) {
            cycle_counts.push_back(cycles);
        }
    }
    
    // Compute average cycles
    int total_cycles = 0;
    for (int c : cycle_counts) {
        total_cycles += c;
    }
    double average_cycles = cycle_counts.empty() ? 0 : (double)total_cycles / cycle_counts.size();
    
    cout << "Tested " << cycle_counts.size() << " valid cases." << endl;
    cout << "Average cycles: " << average_cycles << endl;
    
    return 0;
}
