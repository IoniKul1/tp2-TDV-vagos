import os
import subprocess
import re
import csv

# === CONFIGURACIÓN ===
BASE_DIR = "/Users/ionikullock/Desktop/tp2 2"
INSTANCES_DIR = os.path.join(BASE_DIR, "instancias", "2l-cvrp-0")
SOLUTIONS_DIR = os.path.join(INSTANCES_DIR, "soluciones")
EXECUTABLE = os.path.join(BASE_DIR, "src", "my_vrp_solver")
CSV_PATH = os.path.join(BASE_DIR, "src", "results.csv")

# === FUNCIONES AUXILIARES ===

def normalize(name):
    return ''.join(c.lower() for c in name if c.isalnum())

def extract_optimal_costs(solution_folder):
    print("🔍 Buscando archivos .HRE para óptimos...")
    opt_costs = {}
    for fname in os.listdir(solution_folder):
        if fname.endswith(".HRE"):
            path = os.path.join(solution_folder, fname)
            with open(path, 'r') as f:
                for line in f:
                    if "COST" in line:
                        match = re.search(r"COST\s*:\s*([\d.]+)", line)
                        if match:
                            cost = float(match.group(1))
                            key = normalize(fname.replace(".HRE", ""))
                            opt_costs[key] = cost
                            print(f"✅ {fname} → {cost}")
                            break
    return opt_costs

def extract_costs_from_output(output):
    lines = output.splitlines()
    cw_cost = None
    ni_cost = None

    for line in lines:
        line = line.strip()

        if re.match(r"Total cost\s*\(CW.*\):", line):
            match = re.search(r"([\d.]+)$", line)
            if match:
                cw_cost = float(match.group(1))

        if re.match(r"Total cost\s*\(NI.*\):", line):
            match = re.search(r"([\d.]+)$", line)
            if match:
                ni_cost = float(match.group(1))

    return cw_cost, ni_cost

# === EJECUCIÓN PRINCIPAL ===

print("🚀 Ejecutando instancias...")
results = []
instances = [f for f in os.listdir(INSTANCES_DIR) if f.endswith(".dat")]
opt_costs = extract_optimal_costs(SOLUTIONS_DIR)

for instance in sorted(instances):
    instance_path = os.path.join(INSTANCES_DIR, instance)
    sol_key = normalize(instance.replace(".dat", ""))  # para comparar con nombre .HRE

    print(f"\n⚙️  Ejecutando {instance}...")
    try:
        result = subprocess.run(
            [EXECUTABLE, instance_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=60
        )
        output = result.stdout
    except subprocess.TimeoutExpired:
        print("⏱️ Timeout.")
        output = ""

    # Extraer costos heurísticos
    cost_cw, cost_ni = extract_costs_from_output(output)

    # Buscar óptimo (por coincidencia parcial robusta)
    opt = ""
    for k in opt_costs:
        if k in sol_key or sol_key in k:
            opt = opt_costs[k]
            break

    gap_cw = round((cost_cw - opt) / opt * 100, 2) if cost_cw and opt else ""
    gap_ni = round((cost_ni - opt) / opt * 100, 2) if cost_ni and opt else ""

    if opt == "":
        print(f"❓ No se encontró óptimo para {instance}")
    else:
        print(f"📈 Óptimo usado: {opt}")

    results.append({
        "Instance": instance,
        "CW": cost_cw if cost_cw else "",
        "IN": cost_ni if cost_ni else "",
        "Optimal Cost": opt,
        "Gap CW (%)": gap_cw,
        "Gap IN (%)": gap_ni
    })

# === GUARDAR RESULTADOS ===

print(f"\n💾 Guardando resultados en {CSV_PATH}...")
with open(CSV_PATH, "w", newline="") as f:
    writer = csv.DictWriter(f, fieldnames=["Instance", "CW", "IN", "Optimal Cost", "Gap CW (%)", "Gap IN (%)"])
    writer.writeheader()
    writer.writerows(results)

print("✅ Evaluación finalizada.")