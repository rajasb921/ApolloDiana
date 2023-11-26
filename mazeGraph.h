#ifndef MAZE_GRAPH_H
#define MAZE_GRAPH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <unordered_map>

// Vertex class
class Vertex {
private:
    std::string color;
    std::string direction;
    int x_pos;
    int y_pos;
    bool discovered = false;
    std::vector<Vertex*> adjacentVertices;

public:
    // Constructor
    Vertex(std::string _color, std::string _direction);

    // Getters
    std::string getColor() const;
    std::string getDirection() const;
    int getX() const;
    int getY() const;
    bool isDiscovered() const;
    std::vector<Vertex*> getAdjacentVertices() const;

    // Setters
    void markDiscovered();
    void setX(int x);
    void setY(int y);
    void addVertex(Vertex* v);
};

// Graph class
class Graph {
private:
    std::vector<std::vector<Vertex*>> mazeMatrix;
    int matrixRows;
    int matrixCols;

public:
    // Create mazeMatrix from file (Needed for Constructor)
    void createmazeMatrix(const std::string& filename);

    // Constructor
    Graph(const std::string& filename);

    // Find path using BFS
    std::vector<Vertex*> BFS_Path();

    // Calculate distance between two vertices (Helper function)
    int distance(Vertex* start, Vertex* end);

    // Write Path to File
    void writePath(const std::string &filename = "output.txt");

    // Destructor
    ~Graph();
};

#endif
