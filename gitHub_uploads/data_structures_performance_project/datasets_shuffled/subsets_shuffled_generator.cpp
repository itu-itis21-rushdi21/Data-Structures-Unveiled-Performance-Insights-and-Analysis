#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<random>
#include<chrono>

using namespace std;

int main(void) {
    string output_files[10] = {"1k_shuffled.csv", "2k_shuffled.csv", "3k_shuffled.csv", "4k_shuffled.csv", "5k_shuffled.csv", "6k_shuffled.csv", "7k_shuffled.csv", "8k_shuffled.csv", "9k_shuffled.csv", "10k_shuffled.csv"};

    // Read all lines into a vector
    fstream read_file;
    read_file.open("dataset1.csv", ios::in);
    if (!read_file) {
        cout << "Cannot open read file!" << endl;
        return 1;
    }

    

    // Write shuffled lines to output files
    for (int i = 0; i < 10; i++) {
        string line;
        vector<string> lines;
        // push header line
        getline(read_file, line);
        lines.push_back(line);
        // push lines
        for(int j = 0; j < (i + 1)* 1000 ; j++) {
            getline(read_file, line);
            lines.push_back(line);
        }

        // Shuffle the lines except the header line
        auto rng = default_random_engine(chrono::steady_clock::now().time_since_epoch().count());
        shuffle(lines.begin() + 1, lines.end(), rng);
        
        fstream write_file;
        write_file.open(output_files[i], ios::out);
        if (!write_file) {
            cout << "Cannot open write file!" << endl;
            return 1;
        }
        // write lines to buffer
        stringstream ss;
        for (int j = 0; j < ((i + 1) * 1000) + 1; j++) {
            ss << lines[j] << endl;
        }

        // write buffer to file
        write_file << ss.rdbuf();
        // reset the read file pointer
        read_file.clear();
        read_file.seekg(0, ios::beg);
        
        write_file.close();
    }
    
    read_file.close();
    return 0;
}
