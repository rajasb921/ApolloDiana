#include "mazeGraph.h"

// Vertex class methods
// Constructor
Vertex::Vertex(std::string _color, std::string _direction) : color(_color), direction(_direction) {}

// Getters
std::string Vertex::getColor() const { return color; }
std::string Vertex::getDirection() const { return direction; }
int Vertex::getX() const { return x_pos; }
int Vertex::getY() const { return y_pos; }
bool Vertex::isDiscovered() const { return discovered; }
std::vector<Vertex*> Vertex::getAdjacentVertices() const { return adjacentVertices; }

// Setters
void Vertex::markDiscovered() { discovered = true; }
void Vertex::setX(int x) { x_pos = x; }
void Vertex::setY(int y) { y_pos = y; }
void Vertex::addVertex(Vertex* v) { adjacentVertices.push_back(v); }

// Graph class methods

// Create mazeMatrix from file (Needed for Constructor)
void Graph::createmazeMatrix(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return;
    }

    // First line = Rows, columns
    file >> matrixRows >> matrixCols;

    // Create a 2D vector of Vertex pointers
    std::vector<std::vector<Vertex*>> data(matrixRows, std::vector<Vertex*>(matrixCols));

    // Read the data from the file
    for (int i = 0; i < matrixRows; i++) {
        for (int j = 0; j < matrixCols; j++) {
            std::string color, direction, string;
            file >> string;

            // Check whether string is target
            size_t hyphenPos = string.find('-');
            if (hyphenPos != std::string::npos) {
                direction = string.substr(hyphenPos + 1);
                color = string.substr(0, hyphenPos);
            } else {
                // If no hyphen, set direction to target
                color = string;
                direction = "Target";
            }

            // Create a new Vertex and assign its pointer to the 2D vector
            data[i][j] = new Vertex(color, direction);
            (data[i][j])->setX(i);
            (data[i][j])->setY(j);
        }
    }

    file.close();

    // Create mazeMatrix
    mazeMatrix = data;
}

// Constructor
Graph::Graph(const std::string& filename) {
    createmazeMatrix(filename);

    // Add adjacent vertices to each vertex in matrix
    for (int i = 0; i < matrixRows; i++) {
        for (int j = 0; j < matrixCols; j++) {
            Vertex* currVertex = mazeMatrix[i][j];

            // Find Vertices South
            if (currVertex->getDirection() == "S") {
                int iter_i = i + 1;
                while (iter_i < matrixRows) {
                    if ((mazeMatrix[iter_i][j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[iter_i][j]);
                    }
                    iter_i++;
                }
            }

            // Find Vertices North
            else if (currVertex->getDirection() == "N") {
                int iter_i = i - 1;
                while (iter_i >= 0) {
                    if ((mazeMatrix[iter_i][j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[iter_i][j]);
                    }
                    iter_i--;
                }
            }

            // Find Vertices East
            else if (currVertex->getDirection() == "E") {
                int iter_j = j + 1;
                while (iter_j < matrixCols) {
                    if ((mazeMatrix[i][iter_j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[i][iter_j]);
                    }
                    iter_j++;
                }
            }

            // Find Vertices West
            else if (currVertex->getDirection() == "W") {
                int iter_j = j - 1;
                while (iter_j >= 0) {
                    if ((mazeMatrix[i][iter_j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[i][iter_j]);
                    }
                    iter_j--;
                }
            }

            // Find Vertices NorthEast
            else if (currVertex->getDirection() == "NE") {
                int iter_i = i - 1;
                int iter_j = j + 1;
                while ((iter_i >= 0) && (iter_j < matrixCols)) {
                    if ((mazeMatrix[iter_i][iter_j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[iter_i][iter_j]);
                    }
                    iter_i--;
                    iter_j++;
                }
            }

            // Find Vertices NorthWest
            else if (currVertex->getDirection() == "NW") {
                int iter_i = i - 1;
                int iter_j = j - 1;
                while ((iter_i >= 0) && (iter_j >= 0)) {
                    if ((mazeMatrix[iter_i][iter_j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[iter_i][iter_j]);
                    }
                    iter_i--;
                    iter_j--;
                }
            }

            // Find Vertices SouthEast
            else if (currVertex->getDirection() == "SE") {
                int iter_i = i + 1;
                int iter_j = j + 1;
                while ((iter_i < matrixRows) && (iter_j < matrixCols)) {
                    if ((mazeMatrix[iter_i][iter_j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[iter_i][iter_j]);
                    }
                    iter_i++;
                    iter_j++;
                }
            }

            // Find Vertices SouthWest
            else if (currVertex->getDirection() == "SW") {
                int iter_i = i + 1;
                int iter_j = j - 1;
                while ((iter_i < matrixRows) && (iter_j >= 0)) {
                    if ((mazeMatrix[iter_i][iter_j])->getColor() != currVertex->getColor()) {
                        currVertex->addVertex(mazeMatrix[iter_i][iter_j]);
                    }
                    iter_i++;
                    iter_j--;
                }
            }
        }
    }
}

// Find path using BFS
std::vector<Vertex*> Graph::BFS_Path() {
    std::vector<Vertex*> path;
    std::queue<Vertex*> vertexQueue;
    std::unordered_map<Vertex*, Vertex*> parentMap;  // Map to keep track of parents
    Vertex* startVertex = mazeMatrix[0][0];
    vertexQueue.push(startVertex);
    startVertex->markDiscovered();

    while (!vertexQueue.empty()) {
        Vertex* curVertex = vertexQueue.front();

        for (const auto& newV : curVertex->getAdjacentVertices()) {
            if (!newV->isDiscovered()) {
                vertexQueue.push(newV);
                newV->markDiscovered();
                parentMap[newV] = curVertex;  // Record parent for backtracking
            }
        }

        if (curVertex->getColor() == "O") {
            // Reconstruct the path from the target to the start
            while (curVertex != startVertex) {
                path.insert(path.begin(), curVertex);
                curVertex = parentMap[curVertex];
            }
            path.insert(path.begin(), startVertex);
            return path;
        }
        vertexQueue.pop();
    }
    return path;
}

// Calculate distance between two vertices
int Graph::distance(Vertex* start, Vertex* end) {
    int start_X = start->getX();
    int start_Y = start->getY();
    int end_X = end->getX();
    int end_Y = end->getY();
    int distance = 0;
    std::string direction = start->getDirection();

    while ((start_X != end_X) || (start_Y != end_Y)) {
        if (direction == "N") {
            start_X--;
            distance++;
        } else if (direction == "S") {
            start_X++;
            distance++;
        } else if (direction == "E") {
            start_Y++;
            distance++;
        } else if (direction == "W") {
            start_Y--;
            distance++;
        } else if (direction == "NE") {
            start_X--;
            start_Y++;
            distance++;
        } else if (direction == "NW") {
            start_X--;
            start_Y--;
            distance++;
        } else if (direction == "SE") {
            start_X++;
            start_Y++;
            distance++;
        } else if (direction == "SW") {
            start_X++;
            start_Y--;
            distance++;
        }
    }

    return distance;
}

// Write Path to File
void Graph::writePath(const std::string &filename) {
    std::vector<Vertex*> path = BFS_Path();
    std::ofstream outfile;
    outfile.open(filename);

    std::cout << "Path: ";
    for (int i = 0; path[i]->getColor() != "O"; i++) {
        int d = distance(path[i], path[i + 1]);
        std::cout << d << path[i]->getDirection() << " ";
        outfile << d << path[i]->getDirection() << " ";
    }

    std::cout << std::endl << "Output written to file: " << filename << std::endl;
    outfile.close();
}

// Destructor
Graph::~Graph() {
    for (auto& row : mazeMatrix) {
        for (auto& vertexPtr : row) {
            delete vertexPtr;
        }
    }
}
