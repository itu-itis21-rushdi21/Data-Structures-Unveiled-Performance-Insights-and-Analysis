#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

int main(void) {
    // output files name array

    string output_files[10] = {"1k.csv", "2k.csv", "3k.csv", "4k.csv", "5k.csv", "6k.csv", "7k.csv", "8k.csv", "9k.csv", "10k.csv"};

    fstream read_file;
    read_file.open("dataset1.csv", ios::in);
    if(!read_file) {
        cout << "Cannot open read file!" << endl;
        return 1;
    }

    for(int i = 0; i < 10; i++) {
        fstream write_file;
        write_file.open(output_files[i], ios::out);
        if(!write_file) {
            cout << "Cannot open write file!" << endl;
            return 1;
        }

        string line;
        stringstream ss;
        for(int j = 0; j < ((i + 1)* 1000) + 1; j++) {
            getline(read_file, line);
            ss << line << endl;
        }

        write_file << ss.rdbuf();
        write_file.close();

        // reset the read file pointer
        read_file.clear();
        read_file.seekg(0, ios::beg);
    }
    return 0;

}