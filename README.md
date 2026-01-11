# 🐝 PSO Simulator

A C implementation of the **Particle Swarm Optimization (PSO)** algorithm designed to find the highest point (global maximum) on a generated or loaded 2D terrain. This project includes a Python script for real-time visualization of the swarm.

## 📦 Requirements

### C Simulation
* `gcc` compiler
* `make`

### Python Visualization
* Python 3.x
* Libraries: `pandas`, `matplotlib`, `numpy`

## 🚀 Quick Start

### 1. Compilation
Compile the project using the provided makefile:
```bash
make
```

### 2. Running the Simulation
You can either generate a new random map or load an existing one.
Option A: Generate a new map

```Bash
./ps -p 50 -i 200 -n 2 gen
```

Note: The program will prompt you to enter the map width and height in the console.

#### Option B: Load an existing map
```Bash
./pso -p 30 -i 100 -n 5 map_file.txt 
```

### 3. Visualization
After running the simulation (ensure you used the -n flag > 0 to generate data), run the Python script:

```Bash
python3 visualization_map.py
```
## ⚙️ Command Line Options

| Argument | Description | Default |
| :--- | :--- | :--- |
| `[file/gen]` | **Required.** Path to map file or `gen` to create a new one. | - |
| `-p <int>` | Number of particles in the swarm. | `30` |
| `-i <int>` | Total number of iterations. | `100` |
| `-n <int>` | Log data every N iterations (set to `0` to disable logging). | `0` |
| `-c <file>` | Path to config file for weights (`w`, `c1`, `c2`). | `config_file.txt` |

### 🧹 Clean Up
To remove compiled object files and the executable:
```Bash
make clean
```
