#include "main.h"
#include "restaurant.cpp"

int main(int argc, char* argv[]) {
    auto start = chrono::high_resolution_clock::now();
    string fileName = "test.txt";
    simulate(fileName);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout<<"Duration: "<<duration.count()<<" seconds"<<endl;
    return 0;
}
