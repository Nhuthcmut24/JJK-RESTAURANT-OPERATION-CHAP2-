#include "main.h"
#include "restaurant.cpp"

int main(int argc, char* argv[]) {
    ofstream outFile;
    outFile.open("out.txt");
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());
    string fileName = "test.txt";
    simulate(fileName);
    cout.rdbuf(oldCoutStreamBuf);
    outFile.close();
    return 0;
}