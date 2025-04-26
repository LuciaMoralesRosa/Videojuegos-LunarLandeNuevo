import torch
import numpy as np
import os
import matplotlib.pyplot as plt
from agente_dqn import AgenteDQN
import gymnasium as gym
import entrenador_ia  # Importa tu entorno personalizado

# ========== Configuración ==========
ENV_NAME = "LunarIA-v0"
NUM_EPISODIOS = 10_000
DISPOSITIVO = torch.device("cuda" if torch.cuda.is_available() else "cpu")
GUARDAR_MODELO_CADA = 500
RUTA_MODELOS = "modelos"

os.makedirs(RUTA_MODELOS, exist_ok=True)

# ========== Función para cargar modelo si existe ==========
def cargar_modelo_si_existe(agente):
    modelos_disponibles = [f for f in os.listdir(RUTA_MODELOS) if f.endswith(".pth")]
    if modelos_disponibles:
        ultimo_modelo = sorted(modelos_disponibles)[-1]
        print(f"🔄 Cargando modelo desde {ultimo_modelo}...")
        ruta_modelo = os.path.join(RUTA_MODELOS, ultimo_modelo)
        agente.load(ruta_modelo)
        print("✅ Modelo cargado correctamente.")
        return True
    else:
        print("🚫 No se encontró ningún modelo previo. Comenzando desde cero.")
        return False

# ========== Inicializar entorno ==========
def conectar_al_servidor():
    try:
        env = gym.make(ENV_NAME)
        return env
    except Exception as e:
        print(f"❗ Error al inicializar el entorno: {e}")
        return None

env = conectar_al_servidor()

if env is None:
    print("No se pudo conectar al servidor del juego.")
    exit(1)

# Inicializar el agente después de haber conectado al entorno
input_dim = 300  # El tamaño del estado debe ser de 300 como has definido en el entorno
output_dim = 2 ** env.action_space.n  # 2^5 = 32 combinaciones posibles

agente = AgenteDQN(input_dim, output_dim, DISPOSITIVO)

# Cargar el modelo si existe
cargar_modelo_si_existe(agente)

print(f"🎯 Tamaño del estado: {input_dim} floats")
print(f"🎮 Tamaño del espacio de acción: {output_dim} posibles acciones")

# Lista para almacenar las recompensas totales de cada episodio
recompensas_episodios = []

# ========== Entrenamiento ==========
plt.figure(figsize=(10, 5))  # Crear la figura una vez antes de comenzar el entrenamiento
for episodio in range(1, NUM_EPISODIOS + 1):
    print(f"\n🏁 Episodio {episodio} iniciado...")

    try:
        # Solo llamar a reset() al inicio de cada episodio
        estado, _ = env.reset()

    except Exception as e:
        print(f"❗ Error durante reset: {e}")
        print("Intentando reconectar al servidor...")
        env = conectar_al_servidor()  # Reintentar la conexión
        if env is None:
            print("Fallo al reconectar. Finalizando entrenamiento.")
            break

    estado = np.array(estado)
    recompensa_total = 0
    terminado = False

    if estado is None or estado.shape[0] != 300:
        print(f"⚠️ Error: Estado inválido: {estado}")
        break

    while not terminado:
        try:
            # Seleccionar acción con el agente
            accion_idx = agente.select_action(estado)

            if accion_idx is None:
                print("⚠️ El índice de acción es None.")
                break

            # Convertir el índice a una lista binaria (5 bits)
            accion = [(accion_idx >> i) & 1 for i in range(5)]

            if len(accion) != 5:
                print("⚠️ La acción generada no tiene 5 bits.")
                break

            # Enviar acción al entorno y recibir el siguiente estado
            siguiente_estado, recompensa, terminado, truncado, _ = env.step(accion)

            siguiente_estado = np.array(siguiente_estado)

            if siguiente_estado is None or siguiente_estado.shape[0] != 300:
                print("⚠️ El siguiente estado es inválido. Rompiendo bucle.")
                break

            # Guardar en el buffer y realizar paso de entrenamiento
            agente.replay_buffer.push(estado, accion_idx, recompensa, siguiente_estado, float(terminado))
            agente.train_step()

            estado = siguiente_estado
            recompensa_total += recompensa
            agente.steps_done += 1

        except Exception as e:
            print(f"❗ Excepción atrapada durante el step: {e}")
            break

    print(f"🏁 Episodio {episodio} | 🎯 Recompensa Total: {recompensa_total:.2f} | 🎲 Epsilon: {agente.epsilon:.3f}")
    
    # Guardar la recompensa total para el gráfico
    recompensas_episodios.append(recompensa_total)

    # Mostrar gráfica de las últimas 1000 recompensas
    if len(recompensas_episodios) > 1000:
        recientes_recompensas = recompensas_episodios[-1000:]
    else:
        recientes_recompensas = recompensas_episodios

    plt.clf()  # Limpiar la figura
    plt.plot(recientes_recompensas)
    plt.title(f'Recompensas de los últimos {len(recientes_recompensas)} episodios')
    plt.xlabel('Episodio')
    plt.ylabel('Recompensa Total')
    plt.pause(0.1)  # Actualiza la gráfica sin bloquear el código

    # Guardar el modelo cada ciertos episodios
    if episodio % GUARDAR_MODELO_CADA == 0:
        ruta_guardado = os.path.join(RUTA_MODELOS, f"modelo_ep{episodio}.pth")
        agente.save(ruta_guardado)
        print(f"✅ Modelo guardado en {ruta_guardado}")

# Cerrar el entorno al finalizar
env.close()
print("🎉 Entrenamiento finalizado.")
