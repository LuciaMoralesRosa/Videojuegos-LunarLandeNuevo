import socket
import time
import numpy as np
import gymnasium as gym
from gymnasium import spaces
from gymnasium.envs.registration import register

class LunarEnv(gym.Env):
    def __init__(self, host='127.0.0.1', port=8080):
        time.sleep(3)
        super(LunarEnv, self).__init__()
        self.host = host
        self.port = port
        self.socket = None

        self.observation_space = spaces.Box(low=-np.inf, high=np.inf, shape=(300,), dtype=np.float32)
        self.action_space = spaces.MultiBinary(5)
        self.buffer_size = 300 * 4  # 300 floats, 4 bytes cada uno

        self._connect()

    def _connect(self):
        for intento in range(10):
            try:
                self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.socket.connect((self.host, self.port))
                time.sleep(1)
                return
            except ConnectionRefusedError:
                time.sleep(0.5)
        raise ConnectionError("No se pudo conectar al servidor.")

    def _recv_exact(self, size):
        start = time.time()
        data = b''
        while len(data) < size:
            packet = self.socket.recv(size - len(data))
            if not packet:
                raise ConnectionError("Conexión cerrada inesperadamente mientras se recibía.")
            data += packet
        end = time.time()
        return data

    def reset(self, seed=None, options=None):
        try:
            start_reset = time.time()
            raw = self._recv_exact(self.buffer_size)
            obs = np.frombuffer(raw, dtype=np.float32)
            end_reset = time.time()
        except Exception as e:
            self.close()
            raise e
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
                obs = np.zeros(300, dtype=np.float32)
                terminated = True
                truncated = False
            finally:
                self.socket.settimeout(None)

            return obs, reward, terminated, truncated, {}

        except (socket.error, ConnectionError) as e:
            self.close()
            raise e

    def close(self):
        if self.socket:
            self.socket.close()
            self.socket = None

# Registrar entorno personalizado en Gym
register(
    id="LunarIA-v0",
    entry_point="entrenador_ia:LunarEnv",
)
