#include "mazeGraph.h"

int main() {

    // Default input filename 
    std::string filename = "input.txt";

    // Create graph
    Graph mazeGraph(filename);
    
    // Write path to output.txt file
    mazeGraph.writePath();

    return 0;

}
