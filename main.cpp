// Algorithm Visualizer using C++ and SFML
// This program demonstrates sorting and pathfinding algorithms with visualization

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <string>

enum class AlgorithmType {
    BUBBLE_SORT,
    INSERTION_SORT,
    SELECTION_SORT,
    QUICK_SORT,
    MERGE_SORT,
    BFS,
    DFS,
    DIJKSTRA
};

class AlgorithmVisualizer {
private:
    sf::RenderWindow window;
    std::vector<int> data;
    AlgorithmType currentAlgorithm;
    bool isRunning;
    bool isPaused;
    int speed; // milliseconds delay
    int arraySize;
    int maxValue;
    
    // For pathfinding algorithms
    std::vector<std::vector<int>> grid;
    sf::Vector2i startPoint;
    sf::Vector2i endPoint;
    int gridSize;
    
    // UI elements
    sf::Font font;
    sf::Text algorithmText;
    sf::Text instructionText;
    
    void initializeWindow() {
        window.create(sf::VideoMode(1200, 800), "Algorithm Visualizer");
        window.setFramerateLimit(60);
    }
    
    void initializeFont() {
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            // Continue with default font
        }
        
        algorithmText.setFont(font);
        algorithmText.setCharacterSize(20);
        algorithmText.setFillColor(sf::Color::White);
        algorithmText.setPosition(10, 10);
        
        instructionText.setFont(font);
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(10, 40);
    }
    
    void generateRandomData() {
        data.clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(10, maxValue);
        
        for (int i = 0; i < arraySize; i++) {
            data.push_back(distrib(gen));
        }
    }
    
    void initializeGrid() {
        grid.clear();
        grid.resize(gridSize, std::vector<int>(gridSize, 0));
        
        // Random obstacles (1 = obstacle)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 100);
        
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                if (distrib(gen) < 20) { // 20% chance of obstacle
                    grid[i][j] = 1;
                }
            }
        }
        
        // Set start and end points
        startPoint = sf::Vector2i(0, 0);
        endPoint = sf::Vector2i(gridSize - 1, gridSize - 1);
        
        // Ensure start and end points are not obstacles
        grid[startPoint.x][startPoint.y] = 0;
        grid[endPoint.x][endPoint.y] = 0;
    }
    
    void drawArray() {
        window.clear(sf::Color(30, 30, 30));
        
        float barWidth = static_cast<float>(window.getSize().x) / data.size();
        float heightScale = static_cast<float>(window.getSize().y - 100) / maxValue;
        
        for (size_t i = 0; i < data.size(); i++) {
            sf::RectangleShape bar(sf::Vector2f(barWidth - 1, data[i] * heightScale));
            bar.setPosition(i * barWidth, window.getSize().y - bar.getSize().y - 50);
            bar.setFillColor(sf::Color(100, 180, 255));
            window.draw(bar);
        }
        
        // Draw UI text
        std::string algorithmName;
        switch (currentAlgorithm) {
            case AlgorithmType::BUBBLE_SORT: algorithmName = "Bubble Sort"; break;
            case AlgorithmType::INSERTION_SORT: algorithmName = "Insertion Sort"; break;
            case AlgorithmType::SELECTION_SORT: algorithmName = "Selection Sort"; break;
            case AlgorithmType::QUICK_SORT: algorithmName = "Quick Sort"; break;
            case AlgorithmType::MERGE_SORT: algorithmName = "Merge Sort"; break;
            default: algorithmName = "Unknown Algorithm";
        }
        
        algorithmText.setString(algorithmName);
        instructionText.setString("Controls: R-Reset | Space-Play/Pause | 1-5-Algorithm Selection | +/- Speed Adjustment");
        
        window.draw(algorithmText);
        window.draw(instructionText);
        
        window.display();
    }
    
    void drawGrid() {
        window.clear(sf::Color(30, 30, 30));
        
        float cellSize = std::min(
            static_cast<float>(window.getSize().x) / gridSize,
            static_cast<float>(window.getSize().y - 100) / gridSize
        );
        
        float offsetX = (window.getSize().x - cellSize * gridSize) / 2;
        float offsetY = ((window.getSize().y - 100) - cellSize * gridSize) / 2 + 100;
        
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
                cell.setPosition(offsetX + i * cellSize, offsetY + j * cellSize);
                
                // Color based on cell state
                if (i == startPoint.x && j == startPoint.y) {
                    cell.setFillColor(sf::Color::Green);
                } else if (i == endPoint.x && j == endPoint.y) {
                    cell.setFillColor(sf::Color::Red);
                } else if (grid[i][j] == 1) {
                    cell.setFillColor(sf::Color(100, 100, 100)); // Obstacle
                } else if (grid[i][j] == 2) {
                    cell.setFillColor(sf::Color(100, 180, 255)); // Visited
                } else if (grid[i][j] == 3) {
                    cell.setFillColor(sf::Color(255, 255, 0)); // Path
                } else {
                    cell.setFillColor(sf::Color(40, 40, 40)); // Empty
                }
                
                window.draw(cell);
            }
        }
        
        // Draw UI text
        std::string algorithmName;
        switch (currentAlgorithm) {
            case AlgorithmType::BFS: algorithmName = "Breadth-First Search"; break;
            case AlgorithmType::DFS: algorithmName = "Depth-First Search"; break;
            case AlgorithmType::DIJKSTRA: algorithmName = "Dijkstra's Algorithm"; break;
            default: algorithmName = "Unknown Pathfinding Algorithm";
        }
        
        algorithmText.setString(algorithmName);
        instructionText.setString("Controls: R-Reset | Space-Play/Pause | 6-8-Algorithm Selection | +/- Speed Adjustment");
        
        window.draw(algorithmText);
        window.draw(instructionText);
        
        window.display();
    }
    
    void delay() {
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }
    
    // Sorting Algorithms
    void bubbleSort() {
        for (size_t i = 0; i < data.size() - 1 && isRunning && !isPaused; i++) {
            for (size_t j = 0; j < data.size() - i - 1 && isRunning && !isPaused; j++) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                    drawArray();
                    delay();
                }
                
                // Handle window events
                handleEvents();
                
                if (isPaused) {
                    while (isPaused && isRunning) {
                        handleEvents();
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
            }
        }
    }
    
    void insertionSort() {
        for (size_t i = 1; i < data.size() && isRunning && !isPaused; i++) {
            int key = data[i];
            int j = i - 1;
            
            while (j >= 0 && data[j] > key && isRunning && !isPaused) {
                data[j + 1] = data[j];
                j--;
                drawArray();
                delay();
                
                // Handle window events
                handleEvents();
                
                if (isPaused) {
                    while (isPaused && isRunning) {
                        handleEvents();
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
            }
            
            data[j + 1] = key;
            drawArray();
            delay();
        }
    }
    
    void selectionSort() {
        for (size_t i = 0; i < data.size() - 1 && isRunning && !isPaused; i++) {
            size_t min_idx = i;
            
            for (size_t j = i + 1; j < data.size() && isRunning && !isPaused; j++) {
                if (data[j] < data[min_idx]) {
                    min_idx = j;
                }
                
                // Handle window events
                handleEvents();
                
                if (isPaused) {
                    while (isPaused && isRunning) {
                        handleEvents();
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
            }
            
            if (min_idx != i) {
                std::swap(data[i], data[min_idx]);
                drawArray();
                delay();
            }
        }
    }
    
    void quickSort(int low, int high) {
        if (low < high && isRunning && !isPaused) {
            // Partition the array
            int pivot = data[high];
            int i = low - 1;
            
            for (int j = low; j <= high - 1 && isRunning && !isPaused; j++) {
                if (data[j] < pivot) {
                    i++;
                    std::swap(data[i], data[j]);
                    drawArray();
                    delay();
                }
                
                // Handle window events
                handleEvents();
                
                if (isPaused) {
                    while (isPaused && isRunning) {
                        handleEvents();
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
            }
            
            std::swap(data[i + 1], data[high]);
            drawArray();
            delay();
            
            int partition = i + 1;
            
            // Recursively sort the subarrays
            quickSort(low, partition - 1);
            quickSort(partition + 1, high);
        }
    }
    
    void merge(int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        std::vector<int> leftArray(n1);
        std::vector<int> rightArray(n2);
        
        for (int i = 0; i < n1; i++) {
            leftArray[i] = data[left + i];
        }
        
        for (int j = 0; j < n2; j++) {
            rightArray[j] = data[mid + 1 + j];
        }
        
        int i = 0, j = 0, k = left;
        
        while (i < n1 && j < n2 && isRunning && !isPaused) {
            if (leftArray[i] <= rightArray[j]) {
                data[k] = leftArray[i];
                i++;
            } else {
                data[k] = rightArray[j];
                j++;
            }
            
            k++;
            drawArray();
            delay();
            
            // Handle window events
            handleEvents();
            
            if (isPaused) {
                while (isPaused && isRunning) {
                    handleEvents();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        
        while (i < n1 && isRunning && !isPaused) {
            data[k] = leftArray[i];
            i++;
            k++;
            drawArray();
            delay();
            
            handleEvents();
            
            if (isPaused) {
                while (isPaused && isRunning) {
                    handleEvents();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        
        while (j < n2 && isRunning && !isPaused) {
            data[k] = rightArray[j];
            j++;
            k++;
            drawArray();
            delay();
            
            handleEvents();
            
            if (isPaused) {
                while (isPaused && isRunning) {
                    handleEvents();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
    }
    
    void mergeSort(int left, int right) {
        if (left < right && isRunning && !isPaused) {
            int mid = left + (right - left) / 2;
            
            mergeSort(left, mid);
            mergeSort(mid + 1, right);
            
            merge(left, mid, right);
        }
    }
    
    // Pathfinding Algorithms
    void bfs() {
        std::vector<std::vector<bool>> visited(gridSize, std::vector<bool>(gridSize, false));
        std::vector<std::vector<sf::Vector2i>> parent(gridSize, std::vector<sf::Vector2i>(gridSize, sf::Vector2i(-1, -1)));
        
        std::queue<sf::Vector2i> q;
        q.push(startPoint);
        visited[startPoint.x][startPoint.y] = true;
        
        // Directions: up, right, down, left
        std::vector<sf::Vector2i> directions = {
            sf::Vector2i(0, -1), sf::Vector2i(1, 0),
            sf::Vector2i(0, 1), sf::Vector2i(-1, 0)
        };
        
        bool foundPath = false;
        
        while (!q.empty() && isRunning && !isPaused) {
            sf::Vector2i current = q.front();
            q.pop();
            
            if (current == endPoint) {
                foundPath = true;
                break;
            }
            
            // Mark as visited in the grid
            if (current != startPoint && current != endPoint) {
                grid[current.x][current.y] = 2;
            }
            
            for (const auto& dir : directions) {
                sf::Vector2i next(current.x + dir.x, current.y + dir.y);
                
                // Check if next position is valid
                if (next.x >= 0 && next.x < gridSize && next.y >= 0 && next.y < gridSize &&
                    !visited[next.x][next.y] && grid[next.x][next.y] != 1) {
                    
                    q.push(next);
                    visited[next.x][next.y] = true;
                    parent[next.x][next.y] = current;
                }
            }
            
            drawGrid();
            delay();
            
            // Handle window events
            handleEvents();
            
            if (isPaused) {
                while (isPaused && isRunning) {
                    handleEvents();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        
        // Reconstruct path
        if (foundPath) {
            sf::Vector2i current = endPoint;
            while (current != startPoint) {
                sf::Vector2i prev = parent[current.x][current.y];
                if (prev != startPoint) {
                    grid[prev.x][prev.y] = 3; // Path
                }
                current = prev;
                
                drawGrid();
                delay();
            }
        }
    }
    
    void dfs() {
        std::vector<std::vector<bool>> visited(gridSize, std::vector<bool>(gridSize, false));
        std::vector<std::vector<sf::Vector2i>> parent(gridSize, std::vector<sf::Vector2i>(gridSize, sf::Vector2i(-1, -1)));
        
        std::stack<sf::Vector2i> s;
        s.push(startPoint);
        
        // Directions: up, right, down, left
        std::vector<sf::Vector2i> directions = {
            sf::Vector2i(0, -1), sf::Vector2i(1, 0),
            sf::Vector2i(0, 1), sf::Vector2i(-1, 0)
        };
        
        bool foundPath = false;
        
        while (!s.empty() && isRunning && !isPaused) {
            sf::Vector2i current = s.top();
            s.pop();
            
            if (current == endPoint) {
                foundPath = true;
                break;
            }
            
            if (visited[current.x][current.y]) {
                continue;
            }
            
            visited[current.x][current.y] = true;
            
            // Mark as visited in the grid
            if (current != startPoint && current != endPoint) {
                grid[current.x][current.y] = 2;
            }
            
            for (const auto& dir : directions) {
                sf::Vector2i next(current.x + dir.x, current.y + dir.y);
                
                // Check if next position is valid
                if (next.x >= 0 && next.x < gridSize && next.y >= 0 && next.y < gridSize &&
                    !visited[next.x][next.y] && grid[next.x][next.y] != 1) {
                    
                    s.push(next);
                    parent[next.x][next.y] = current;
                }
            }
            
            drawGrid();
            delay();
            
            // Handle window events
            handleEvents();
            
            if (isPaused) {
                while (isPaused && isRunning) {
                    handleEvents();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        
        // Reconstruct path
        if (foundPath) {
            sf::Vector2i current = endPoint;
            while (current != startPoint) {
                sf::Vector2i prev = parent[current.x][current.y];
                if (prev != startPoint) {
                    grid[prev.x][prev.y] = 3; // Path
                }
                current = prev;
                
                drawGrid();
                delay();
            }
        }
    }
    
    void dijkstra() {
        // Priority queue with min-heap, stores pairs of (distance, position)
        std::priority_queue<std::pair<int, sf::Vector2i>, 
                             std::vector<std::pair<int, sf::Vector2i>>,
                             std::greater<std::pair<int, sf::Vector2i>>> pq;
        
        std::vector<std::vector<int>> distance(gridSize, std::vector<int>(gridSize, INT_MAX));
        std::vector<std::vector<sf::Vector2i>> parent(gridSize, std::vector<sf::Vector2i>(gridSize, sf::Vector2i(-1, -1)));
        
        // Directions: up, right, down, left + diagonals
        std::vector<sf::Vector2i> directions = {
            sf::Vector2i(0, -1), sf::Vector2i(1, 0),
            sf::Vector2i(0, 1), sf::Vector2i(-1, 0),
            sf::Vector2i(1, -1), sf::Vector2i(1, 1),
            sf::Vector2i(-1, 1), sf::Vector2i(-1, -1)
        };
        
        // Start with distance 0 for the start point
        pq.push({0, startPoint});
        distance[startPoint.x][startPoint.y] = 0;
        
        bool foundPath = false;
        
        while (!pq.empty() && isRunning && !isPaused) {
            auto [dist, current] = pq.top();
            pq.pop();
            
            if (current == endPoint) {
                foundPath = true;
                break;
            }
            
            // Skip if we've found a better path already
            if (dist > distance[current.x][current.y]) {
                continue;
            }
            
            // Mark as visited in the grid
            if (current != startPoint && current != endPoint) {
                grid[current.x][current.y] = 2;
            }
            
            for (const auto& dir : directions) {
                sf::Vector2i next(current.x + dir.x, current.y + dir.y);
                
                // Check if next position is valid
                if (next.x >= 0 && next.x < gridSize && next.y >= 0 && next.y < gridSize && grid[next.x][next.y] != 1) {
                    // Calculate weight (1 for cardinal directions, sqrt(2) for diagonals)
                    int weight = (std::abs(dir.x) + std::abs(dir.y) == 1) ? 1 : 14; // Using 14 for sqrt(2)*10
                    
                    int newDist = distance[current.x][current.y] + weight;
                    
                    if (newDist < distance[next.x][next.y]) {
                        distance[next.x][next.y] = newDist;
                        parent[next.x][next.y] = current;
                        pq.push({newDist, next});
                    }
                }
            }
            
            drawGrid();
            delay();
            
            // Handle window events
            handleEvents();
            
            if (isPaused) {
                while (isPaused && isRunning) {
                    handleEvents();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        
        // Reconstruct path
        if (foundPath) {
            sf::Vector2i current = endPoint;
            while (current != startPoint) {
                sf::Vector2i prev = parent[current.x][current.y];
                if (prev != startPoint) {
                    grid[prev.x][prev.y] = 3; // Path
                }
                current = prev;
                
                drawGrid();
                delay();
            }
        }
    }
    
public:
    AlgorithmVisualizer() :
        isRunning(false),
        isPaused(false),
        speed(50),
        arraySize(100),
        maxValue(500),
        currentAlgorithm(AlgorithmType::BUBBLE_SORT),
        gridSize(20)
    {
        initializeWindow();
        initializeFont();
        generateRandomData();
        initializeGrid();
    }
    
    ~AlgorithmVisualizer() {
        window.close();
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                isRunning = false;
            }
            
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        window.close();
                        isRunning = false;
                        break;
                    
                    case sf::Keyboard::R:
                        generateRandomData();
                        initializeGrid();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Space:
                        isPaused = !isPaused;
                        break;
                    
                    case sf::Keyboard::Add:
                    case sf::Keyboard::Equal:
                        speed = std::max(10, speed - 10);
                        break;
                    
                    case sf::Keyboard::Subtract:
                    case sf::Keyboard::Dash:
                        speed = std::min(200, speed + 10);
                        break;
                    
                    case sf::Keyboard::Num1:
                        currentAlgorithm = AlgorithmType::BUBBLE_SORT;
                        generateRandomData();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Num2:
                        currentAlgorithm = AlgorithmType::INSERTION_SORT;
                        generateRandomData();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Num3:
                        currentAlgorithm = AlgorithmType::SELECTION_SORT;
                        generateRandomData();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Num4:
                        currentAlgorithm = AlgorithmType::QUICK_SORT;
                        generateRandomData();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Num5:
                        currentAlgorithm = AlgorithmType::MERGE_SORT;
                        generateRandomData();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Num6:
                        currentAlgorithm = AlgorithmType::BFS;
                        initializeGrid();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Num7:
                        currentAlgorithm = AlgorithmType::DFS;
                        initializeGrid();
                        isPaused = true;
                        break;
                    
                    case sf::Keyboard::Num8:
                        currentAlgorithm = AlgorithmType::DIJKSTRA;
                        initializeGrid();
                        isPaused = true;
                        break;
                }
            }
        }
    }
    
    void runVisualization() {
        isRunning = true;
        isPaused = true;  // Start paused
        
        while (window.isOpen() && isRunning) {
            handleEvents();
            
            if (currentAlgorithm == AlgorithmType::BFS || 
                currentAlgorithm == AlgorithmType::DFS || 
                currentAlgorithm == AlgorithmType::DIJKSTRA) {
                drawGrid();
                
                if (!isPaused) {
                    // Reset grid to initial state before running algorithm
                    for (int i = 0; i < gridSize; i++) {
                        for (int j = 0; j < gridSize; j++) {
                            if (grid[i][j] == 2 || grid[i][j] == 3) {
                                grid[i][j] = 0;
                            }
                        }
                    }
                    
                    switch (currentAlgorithm) {
                        case AlgorithmType::BFS:
                            bfs();
                            break;
                        case AlgorithmType::DFS:
                            dfs();
                            break;
                        case AlgorithmType::DIJKSTRA:
                            dijkstra();
                            break;
                        default:
                            break;
                    }
                    
                    isPaused = true;
                }
            } else {
                drawArray();
                
                if (!isPaused) {
                    switch (currentAlgorithm) {
                        case AlgorithmType::BUBBLE_SORT:
                            bubbleSort();
                            break;
                        case AlgorithmType::INSERTION_SORT:
                            insertionSort();
                            break;
                        case AlgorithmType::SELECTION_SORT:
                            selectionSort();
                            break;
                        case AlgorithmType::QUICK_SORT:
                            quickSort(0, data.size() - 1);
                            break;
                        case AlgorithmType::MERGE_SORT:
                            mergeSort(0, data.size() - 1);
                            break;
                        default:
                            break;
                    }
                    
                    isPaused = true;
                }
            }
            
            // Ensure window doesn't freeze when paused
            if (isPaused) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    }
};

int main() {
    AlgorithmVisualizer visualizer;
    visualizer.runVisualization();
    
    return 0;
}
