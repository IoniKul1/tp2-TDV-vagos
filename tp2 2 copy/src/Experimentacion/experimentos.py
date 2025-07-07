import subprocess
import time
import csv
import os

INSTANCIAS = ["E045-04f.dat", "E051-05e.dat", "E072-04f.dat", "E076-07u.dat", "E076-08u.dat", "E076-14u.dat", "E101-08e.dat", "E101-10c.dat", "E101-14u.dat", "E121-07c.dat", "E135-07f.dat", "E151-12c.dat", "E200-17b.dat","E200-17c.dat" ]
HEURISTICAS = ["cw", "ni"]
OPERADORES = ["", "relocate", "swap", "2opt", "relocate,swap", "relocate,2opt", "swap,2opt", "relocate,swap,2opt"]

OUTPUT = os.path.join(os.path.dirname(__file__), "resultados_experimentos.csv")

with open(OUTPUT, mode='w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["Instancia", "Heuristica", "Operadores", "Costo", "Valido", "Tiempo(ms)"])

    for instancia in INSTANCIAS:
        for heur in HEURISTICAS:
            for ops in OPERADORES:
                ruta_instancia = f"../instancias/2l-cvrp-0/{instancia}"
                cmd = ["./my_vrp_solver", ruta_instancia, heur]
                if ops:
                    cmd += ops.split(',')

                start_time = time.time()
                # Run the command without input
                process = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

                # Responder 's' para que el solver valide la solución y así poder registrar si es válida
                try:
                    output_cmd, error_cmd = process.communicate(input="s\n", timeout=10)
                except subprocess.TimeoutExpired:
                    process.kill()
                    output_cmd, error_cmd = process.communicate()
                end_time = time.time()
                elapsed_time = (end_time - start_time) * 1000

                costo = ""
                for line in output_cmd.splitlines():
                    if "Costo total:" in line:
                        parts = line.split()
                        if len(parts) >= 3:
                            costo = parts[2]
                        break

                valido = "SI" if "✅" in output_cmd else "NO"

                writer.writerow([instancia, heur, ops, costo, valido, f"{elapsed_time:.3f}"])

print(f"✅ Experimentación completada. Resultados en {OUTPUT}")