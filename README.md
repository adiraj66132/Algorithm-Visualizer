# Algorithm Visualizer

An interactive tool for visualizing various algorithms implemented in C++ with SFML graphics.

![Algorithm Visualizer](https://via.placeholder.com/800x450.png?text=Algorithm+Visualizer)

## Overview

This project provides a visual representation of how different algorithms work in real-time. It's designed as an educational tool to help understand the inner workings of common sorting and pathfinding algorithms through intuitive visual feedback.

## Features

### Sorting Algorithms
- **Bubble Sort**: Simple comparison-based algorithm that repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order.
- **Insertion Sort**: Builds the final sorted array one item at a time by comparing with previous elements.
- **Selection Sort**: Divides the input into a sorted and unsorted region, repeatedly selecting the smallest element from the unsorted region.
- **Quick Sort**: Efficient divide-and-conquer sorting algorithm that works by selecting a 'pivot' element and partitioning the array around it.
- **Merge Sort**: Divide-and-conquer algorithm that divides the array into halves, sorts them separately, and then merges them.

### Pathfinding Algorithms
- **Breadth-First Search (BFS)**: Explores all neighbor nodes at the present depth before moving to nodes at the next depth level.
- **Depth-First Search (DFS)**: Explores as far as possible along each branch before backtracking.
- **Dijkstra's Algorithm**: Finds the shortest paths between nodes in a graph, accounting for weighted edges.

## Controls

| Key | Action |
|-----|--------|
| Space | Play/Pause the algorithm visualization |
| R | Reset with new random data |
| 1-5 | Select sorting algorithms (1=Bubble, 2=Insertion, 3=Selection, 4=Quick, 5=Merge) |
| 6-8 | Select pathfinding algorithms (6=BFS, 7=DFS, 8=Dijkstra) |
| +/- | Adjust animation speed |
| Esc | Exit the application |

## Requirements

- C++11 or higher
- SFML 2.5.x or higher
- Compatible C++ compiler (GCC, Clang, or Visual C++)

## Installation

### Prerequisites
Make sure you have SFML installed on your system. Visit [SFML's official website](https://www.sfml-dev.org/download.php) for installation instructions for your platform.

### Building from Source

#### On Linux/macOS
```bash
# Clone the repository
git clone https://github.com/yourusername/algorithm-visualizer.git
cd algorithm-visualizer

# Build the project
mkdir build && cd build
cmake ..
make

# Run the visualizer
./algorithm_visualizer
```

#### On Windows with Visual Studio
1. Clone the repository
2. Open the project in Visual Studio
3. Configure the project to include SFML libraries
4. Build and run the solution

#### With G++ directly
```bash
g++ -std=c++11 main.cpp -o algorithm_visualizer -lsfml-graphics -lsfml-window -lsfml-system
```

## Project Structure

```
algorithm-visualizer/
├── src/
│   ├── main.cpp
│   ├── AlgorithmVisualizer.cpp
│   └── AlgorithmVisualizer.h
├── resources/
│   └── fonts/
│       └── arial.ttf
├── CMakeLists.txt
└── README.md
```

## How It Works

The visualizer represents:
- **Sorting algorithms**: As an array of bars with varying heights
- **Pathfinding algorithms**: As a grid with start point (green), end point (red), obstacles (gray), visited nodes (blue), and final path (yellow)

As the algorithm executes, the visualization updates in real-time, allowing you to see how data is manipulated at each step.

## Contributing

Contributions are welcome! Here are some ways you can contribute:

1. Implement additional algorithms
2. Improve visualization effects
3. Add performance metrics for algorithm comparison
4. Fix bugs or improve code quality

Please feel free to submit a pull request or open an issue to discuss potential improvements.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by various algorithm visualization tools and educational resources
- Built with [SFML](https://www.sfml-dev.org/) for graphics rendering
