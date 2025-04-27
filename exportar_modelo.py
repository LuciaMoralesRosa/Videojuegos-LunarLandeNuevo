import torch
import torch.nn as nn
import json
import os
import numpy as np

# Definir la arquitectura de la red neuronal
class DQN(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(DQN, self).__init__()
        self.model = nn.Sequential(
            nn.Linear(input_dim, 512),
            nn.ReLU(),
            nn.Linear(512, 256),
            nn.ReLU(),
            nn.Linear(256, output_dim)
        )

    def forward(self, x):
        return self.model(x)

# Función para obtener el archivo más reciente en un directorio
def obtener_archivo_mas_reciente(directorio):
    archivos = [f for f in os.listdir(directorio) if f.endswith('.pth')]
    
    if not archivos:
        raise FileNotFoundError("No se encontraron archivos .pth en el directorio especificado.")
    
    archivos_completos = [os.path.join(directorio, f) for f in archivos]
    archivo_mas_reciente = max(archivos_completos, key=os.path.getmtime)
    
    return archivo_mas_reciente

# Directorio de modelos
directorio_modelos = 'modelos'

# Obtener el archivo .pth más reciente
archivo_modelo = obtener_archivo_mas_reciente(directorio_modelos)

# Definir el modelo
input_dim = 300
output_dim = 32  # El tamaño de salida (combinaciones posibles de acción)

# Cargar el modelo
modelo = DQN(input_dim, output_dim)
estado_modelo = torch.load(archivo_modelo)
modelo.load_state_dict(estado_modelo)
modelo.eval()

# Función para obtener los pesos de la red
def obtener_pesos(modelo):
    pesos = {}
    
    # Recorrer todas las capas del modelo y extraer sus pesos
    for name, param in modelo.named_parameters():
        # Convertir el ndarray de los pesos a una lista para poder serializarla en JSON
        pesos[name] = param.detach().cpu().numpy().tolist()
        
    return pesos

# Guardar los pesos en un archivo JSON
pesos = obtener_pesos(modelo)
with open('pesos.json', 'w') as f:
    json.dump(pesos, f)

print(f"Pesos guardados desde el modelo: {archivo_modelo}")
