import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

# --- CONFIG ---
MAP_FILE = 'gen_map.txt'
RESULTS_FILE = 'results.csv'
ANIMATION_SPEED = 1000  # 1 sec

def load_map(filename):
    try:
        with open(filename, 'r') as f:
            header = f.readline().strip().split()
            if not header: return None
            rows, cols = int(header[0]), int(header[1])          

            values = []
            for line in f:
                values.extend([float(x) for x in line.split()])

            grid = np.array(values).reshape(rows, cols)
            return grid
    except FileNotFoundError:
        print(f"Błąd: Nie znaleziono pliku mapy {filename}")
        return None
    except Exception as err:
        print(f"Błąd podczas wczytywania mapy: {err}")
        return None

grid = load_map(MAP_FILE)

try:
    df = pd.read_csv(RESULTS_FILE, sep=';')
except FileNotFoundError:
    print(f"Błąd: Nie znaleziono pliku {RESULTS_FILE}. Uruchom najpierw program w C!")
    exit()

if df.empty:
    print("Plik CSV jest pusty!")
    exit()

fig, ax = plt.subplots(figsize=(8, 8))

# Map grid visualization
if grid is not None:
    masked_grid = np.ma.masked_less(grid, 0)
    
    ax.imshow(masked_grid, cmap='Reds', origin='upper', interpolation='nearest')
    
    rows, cols = grid.shape
    ax.set_xlim(-0.5, cols - 0.5)
    ax.set_ylim(rows - 0.5, -0.5)
else:
    x_min, x_max = df['X'].min(), df['X'].max()
    y_min, y_max = df['Y'].min(), df['Y'].max()
    margin = 1.0
    ax.set_xlim(x_min - margin, x_max + margin)
    ax.set_ylim(y_max + margin, y_min - margin)

# Particle visulization
scat = ax.scatter([], [], c='blue', s=80, edgecolors='white', linewidth=1.5, label='Drony')
title = ax.set_title("Oczekiwanie na start...")
ax.set_xlabel("Współrzędna X (Kolumna)")
ax.set_ylabel("Współrzędna Y (Wiersz)")
ax.grid(False)

iterations = sorted(df['Iteration'].unique())

def update(frame_number):
    current_iter = iterations[frame_number]
    drones = df[df['Iteration'] == current_iter]
    
    data = drones[['X', 'Y']].to_numpy()
    
    scat.set_offsets(data)
    
    title.set_text(f'Iteracja: {current_iter}')
    return scat, title

print(f"Uruchamianie wizualizacji (krok co {ANIMATION_SPEED}ms)...")
ani = animation.FuncAnimation(fig, update, frames=len(iterations), interval=ANIMATION_SPEED, blit=False)

plt.show()