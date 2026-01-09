import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

# --- KONFIGURACJA ---
MAP_FILE = 'test_map.txt'
RESULTS_FILE = 'results.csv'
ANIMATION_SPEED = 1000  # 3000 ms = 3 sekundy

def load_map(filename):
    try:
        with open(filename, 'r') as f:
            # Czytamy wymiary (np. "7 7")
            header = f.readline().strip().split()
            if not header: return None
            rows, cols = int(header[0]), int(header[1])
            
            # Czytamy resztę liczb do jednej listy
            values = []
            for line in f:
                values.extend([float(x) for x in line.split()])
            
            # Zamieniamy na macierz numpy
            grid = np.array(values).reshape(rows, cols)
            return grid
    except FileNotFoundError:
        print(f"Błąd: Nie znaleziono pliku mapy {filename}")
        return None
    except Exception as e:
        print(f"Błąd podczas wczytywania mapy: {e}")
        return None

# 1. Wczytanie danych
grid = load_map(MAP_FILE)

try:
    df = pd.read_csv(RESULTS_FILE, sep=';')
except FileNotFoundError:
    print(f"Błąd: Nie znaleziono pliku {RESULTS_FILE}. Uruchom najpierw program w C!")
    exit()

if df.empty:
    print("Plik CSV jest pusty!")
    exit()

# 2. Ustawienia wykresu
fig, ax = plt.subplots(figsize=(8, 8))

# Rysowanie mapy (jeśli udało się wczytać)
if grid is not None:
    # Maskujemy wartości ujemne (zeby byly biale/przezroczyste)
    # np.ma.masked_less(grid, 0) ukrywa wszystko poniżej 0
    masked_grid = np.ma.masked_less(grid, 0)
    
    # Rysujemy mapę:
    # cmap='Reds' - wyższe wartości będą bardziej czerwone
    # origin='upper' - rysuje (0,0) w lewym górnym rogu (standard dla macierzy)
    # interpolation='nearest' - ostre krawędzie kwadratów
    ax.imshow(masked_grid, cmap='Reds', origin='upper', interpolation='nearest')
    
    # Ustawiamy granice wykresu zgodnie z wymiarami mapy
    rows, cols = grid.shape
    ax.set_xlim(-0.5, cols - 0.5)
    ax.set_ylim(rows - 0.5, -0.5) # Odwrócone Y, żeby pasowało do macierzy (0 na górze)
else:
    # Fallback jeśli brak mapy
    x_min, x_max = df['X'].min(), df['X'].max()
    y_min, y_max = df['Y'].min(), df['Y'].max()
    margin = 1.0
    ax.set_xlim(x_min - margin, x_max + margin)
    ax.set_ylim(y_max + margin, y_min - margin) # Też odwracamy Y dla spójności

# Elementy wykresu dla dronów
# edgecolors='black' dodaje obwódkę, żeby kropki były widoczne na czerwonym tle
scat = ax.scatter([], [], c='blue', s=80, edgecolors='white', linewidth=1.5, label='Drony')
title = ax.set_title("Oczekiwanie na start...")
ax.set_xlabel("Współrzędna X (Kolumna)")
ax.set_ylabel("Współrzędna Y (Wiersz)")
ax.grid(False) # Siatka może przeszkadzać przy mapie

iterations = sorted(df['Iteration'].unique())

# 3. Funkcja aktualizująca
def update(frame_number):
    current_iter = iterations[frame_number]
    drones = df[df['Iteration'] == current_iter]
    
    # Pobieramy pozycje
    data = drones[['X', 'Y']].to_numpy()
    
    scat.set_offsets(data)
    
    # Kolorowanie dronów (opcjonalne - teraz są niebieskie z białą obwódką dla kontrastu)
    # Możesz odkomentować poniższe, jeśli chcesz kolory zależne od Value:
    # scat.set_array(drones['Value'].to_numpy())
    
    title.set_text(f'Iteracja: {current_iter}')
    return scat, title

# 4. Uruchomienie animacji
# interval=3000 oznacza 3 sekundy
print(f"Uruchamianie wizualizacji (krok co {ANIMATION_SPEED}ms)...")
ani = animation.FuncAnimation(fig, update, frames=len(iterations), interval=ANIMATION_SPEED, blit=False)

plt.show()