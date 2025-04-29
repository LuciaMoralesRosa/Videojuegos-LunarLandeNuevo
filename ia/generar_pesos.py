import json
import numpy as np

# Archivo de entrada y salida
archivo_json = 'pesos.json'
archivo_salida = 'pesos.c'

# Cargar pesos
with open(archivo_json, 'r') as f:
    pesos = json.load(f)

def formatear_array(array, por_linea=8):
    """Convierte un numpy array en texto C bien formateado."""
    texto = ""
    array = np.array(array)
    plano = array.flatten()

    for i, val in enumerate(plano):
        texto += f"{val:.8f}f"
        if i != len(plano) - 1:
            texto += ", "
            if (i + 1) % por_linea == 0:
                texto += "\n"
    return texto

def formatear_matriz(matriz, filas, columnas):
    """Convierte una matriz 2D en C tipo pesos[x][y] = {...}."""
    matriz = np.array(matriz)
    texto = "{\n"
    for fila in range(filas):
        texto += "    { "
        texto += ", ".join(f"{matriz[fila, col]:.8f}f" for col in range(columnas))
        texto += " }"
        if fila != filas - 1:
            texto += ",\n"
        else:
            texto += "\n"
    texto += "};\n"
    return texto

# Crear el archivo pesos.c
with open(archivo_salida, 'w') as f:
    f.write('#include "pesos.h"\n\n')

    # Capa 1
    f.write('// --- CAPA 1 ---\n')
    f.write('float pesos1[512][300] = ')
    f.write(formatear_matriz(pesos['model.0.weight'], 512, 300))
    f.write('\n')
    f.write('float bias1[512] = {')
    f.write(formatear_array(pesos['model.0.bias']))
    f.write('};\n\n')

    # Capa 2
    f.write('// --- CAPA 2 ---\n')
    f.write('float pesos2[256][512] = ')
    f.write(formatear_matriz(pesos['model.2.weight'], 256, 512))
    f.write('\n')
    f.write('float bias2[256] = {')
    f.write(formatear_array(pesos['model.2.bias']))
    f.write('};\n\n')

    # Capa 3
    f.write('// --- CAPA 3 ---\n')
    f.write('float pesos3[32][256] = ')
    f.write(formatear_matriz(pesos['model.4.weight'], 32, 256))
    f.write('\n')
    f.write('float bias3[32] = {')
    f.write(formatear_array(pesos['model.4.bias']))
    f.write('};\n')

print(f"✅ Archivo '{archivo_salida}' generado correctamente.")
