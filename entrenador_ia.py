import socket
import time
import numpy as np
import gymnasium as gym
from gymnasium import spaces
from gymnasium.envs.registration import register


class LunarEnv(gym.Env):
    def __init__(self, host='127.0.0.1', port=8080):
        time.sleep(3)  # Esperar un segundo por si el servidor aún no está listo
        super(LunarEnv, self).__init__()
        self.host = host
        self.port = port
        self.socket = None

        self.observation_space = spaces.Box(low=-np.inf, high=np.inf, shape=(295,), dtype=np.float32)
        self.action_space = spaces.MultiBinary(5)
        self.buffer_size = 295 * 4  # 295 floats

        self._connect()

    def _connect(self):
        for _ in range(10):
            try:
                self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.socket.connect((self.host, self.port))
                print("Conectado al servidor del juego.")
                return
            except ConnectionRefusedError:
                print("Esperando al servidor del juego...")
                time.sleep(0.5)
        raise ConnectionError("No se pudo conectar al servidor.")

    def _recv_exact(self, size):
        data = b''
        while len(data) < size:
            packet = self.socket.recv(size - len(data))
            if not packet:
                raise ConnectionError("Conexión cerrada inesperadamente.")
            data += packet
        return data

    def reset(self, seed=None, options=None):
        try:
            raw = self._recv_exact(self.buffer_size)
            obs = np.frombuffer(raw, dtype=np.float32)
        except Exception as e:
            print(f"Error al recibir estado inicial: {e}")
            self.close()
            self._connect()
            return self.reset()
        return obs, {}

    def step(self, action):
        try:
            action_bytes = np.array(action, dtype=np.int32).tobytes()
            self.socket.sendall(action_bytes)

            raw = self._recv_exact(4)
            self.socket.settimeout(0.01)
            try:
                remaining = self._recv_exact(self.buffer_size - 4)
                full_raw = raw + remaining
                obs = np.frombuffer(full_raw, dtype=np.float32)
                reward = 0.0
                terminated = False
                truncated = False
            except socket.timeout:
                reward = np.frombuffer(raw, dtype=np.float32)[0]
                obs = np.zeros(295, dtype=np.float32)
                terminated = True
                truncated = False
            finally:
                self.socket.settimeout(None)

            return obs, reward, terminated, truncated, {}

        except Exception as e:
            print(f"Error en step: {e}")
            self.close()
            return np.zeros(295, dtype=np.float32), 0.0, True, False, {}

    def close(self):
        if self.socket:
            self.socket.close()
            self.socket = None
            print("Conexión cerrada.")


# Registrar entorno personalizado
register(
    id="LunarIA-v0",
    entry_point="entrenador_ia:LunarEnv",
)


# ========== PRUEBA CON AGENTE ALEATORIO ==========
if __name__ == "__main__":
    env = gym.make("LunarIA-v0")
    obs, _ = env.reset()
    total_reward = 0

    for step in range(1000):
        action = env.action_space.sample()
        obs, reward, terminated, truncated, _ = env.step(action)
        total_reward += reward
        if terminated or truncated:
            print(f"Fin del episodio. Recompensa total: {total_reward}")
            total_reward = 0
            obs, _ = env.reset()

    env.close()
