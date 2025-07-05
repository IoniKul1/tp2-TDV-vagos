import os
import re
import matplotlib.pyplot as plt
import textwrap

def graficar_rutas(rutas, coords, title="Rutas del VRP"):
    """
    Dibuja las rutas del VRP usando solo matplotlib.

    Parámetros:
    - rutas: lista de rutas (listas de nodos)
    - coords: diccionario de coordenadas {nodo: (x, y)}
    """
    colores = ['red', 'blue', 'green', 'orange', 'purple', 'cyan', 'brown', 'magenta', 'gray', 'olive']

    plt.figure(figsize=(10, 8))


    if not rutas:
        print("⚠️ No se graficaron rutas (lista vacía). Mostrando solo nodos.")

    for idx, ruta in enumerate(rutas):
        try:
            x = [coords[nodo][0] for nodo in ruta]
            y = [coords[nodo][1] for nodo in ruta]
            plt.plot(x, y, marker='o', label=f'Ruta {idx+1}')
        except KeyError as e:
            print(f"⚠️ Nodo faltante en coordenadas: {e}")

    for nodo, (x, y) in coords.items():
        plt.text(x, y, str(nodo), fontsize=9, ha='right')

    plt.title(title)
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.legend(loc='best')
    plt.grid(True)
    plt.axis('equal')
    plt.tight_layout()
    nombre_archivo = f"Experimentacion/solucion_E076-10e_{title.split('-')[-1].strip()}.png"
    plt.savefig(nombre_archivo, bbox_inches='tight')
    print(f"🖼️ Gráfico guardado como {nombre_archivo}")
    plt.show()

def extraer_rutas_desde_texto(texto):
    """
    Parsea las rutas desde el output textual del solver.
    Espera líneas como: 'Ruta 1 (demanda: 140): 1 27 18 4 52 7 69 5 76 1'
    """
    rutas = []
    for linea in texto.splitlines():
        print(f"📄 Línea leída: {repr(linea)}")
        if "Ruta" in linea and ":" in linea:
            numeros = re.findall(r'\d+', linea.split(":", 1)[1])
            nodos = list(map(int, numeros[1:]))
            rutas.append(nodos)
    print(f"\n🧪 Total de rutas encontradas: {len(rutas)}")
    for i, ruta in enumerate(rutas):
        print(f"Ruta {i+1}: {ruta}")
    return rutas

def leer_coords(path_archivo):
    """
    Lee las coordenadas de los nodos desde un archivo .dat (VRPLIB).
    """
    coords = {}
    leer = False
    with open(path_archivo, 'r') as f:
        for linea in f:
            if "NODE_COORD_SECTION" in linea:
                leer = True
                continue
            if leer:
                if "DEMAND_SECTION" in linea or "EOF" in linea:
                    break
                partes = linea.strip().split()
                if len(partes) == 3:
                    idx, x, y = map(float, partes)
                    coords[int(idx)] = (x, y)
    return coords

if __name__ == "__main__":
    nombres_salida = [
        "cw",
        "ni",
        "cw_relocate",
        "cw_swap",
        "cw_2opt",
        "cw_full",
        "ni_swap",
        "ni_relocate",
        "ni_2opt",
        "ni_full",
    ]

    os.makedirs("Experimentacion/salidas", exist_ok=True)

    for nombre_salida in nombres_salida:
        path_salida = f"Experimentacion/salidas/{nombre_salida}.txt"
        if not os.path.isfile(path_salida):
            print(f"❌ No se encontró el archivo: {path_salida}")
            continue

        with open(path_salida, "r") as f:
            salida = f.read()

        rutas = extraer_rutas_desde_texto(salida)
        coords = leer_coords("../instancias/2l-cvrp-0/E076-10e.dat")

        print(f"\n=== {nombre_salida.upper()} ===")
        print("Rutas extraídas:")
        for r in rutas:
            print(r)

        print("\nCoordenadas cargadas:")
        for k, v in coords.items():
            print(f"Nodo {k}: {v}")

        graficar_rutas(rutas, coords, title=f"Solución E076-10e - {nombre_salida}")