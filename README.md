# 🌀 Maze Runner – SFML Game

A simple yet engaging **maze game** built with **C++ and SFML**, featuring real-time movement, a countdown timer, and an automatic path reveal when time runs out.

---

## 🎮 Features

- 🧩 **Random Maze Generation**  
  Each run generates a new unique maze layout.

- 🚶 **Smooth Player Movement**  
  Move using **arrow keys (↑ ↓ ← →)**  of **W A S D keys** to find your way out.

- ⏱️ **Countdown Timer**  
  A sleek dark-mode timer box in the top-right corner tracks your time.

- 💀 **Timeout Path Reveal**  
  When time’s up, the game displays the **correct path** from start to finish.

- 🏁 **Winning Screen**  
  Reach the goal within time to see a celebratory "You Win!" message.

---

## 🛠️ Tech Stack

- **Language:** C++  
- **Library:** [SFML 2.6+](https://www.sfml-dev.org/)  
- **Build Tool:** CMake  
- **IDE Recommended:** Visual Studio Code / CLion

---

## ▶️ How to Run

```bash
# Clone the repository
git clone https://github.com/yourusername/maze-runner.git
cd maze-runner

# Create build folder
mkdir build && cd build

# Generate and build
cmake ..
make

# Run the game
./MazeGame
