import torch
import torch.nn as nn
import torch.optim as optim
import random
import numpy as np
from collections import deque

# ========== Red Neuronal DQN ========== 
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

# ========== Replay Buffer ========== 
class ReplayBuffer:
    def __init__(self, capacity=100_000):
        self.buffer = deque(maxlen=capacity)

    def push(self, state, action, reward, next_state, done):
        self.buffer.append((state, action, reward, next_state, done))

    def sample(self, batch_size):
        batch = random.sample(self.buffer, batch_size)
        state, action, reward, next_state, done = map(np.array, zip(*batch))
        return (
            torch.FloatTensor(state),
            torch.LongTensor(action),
            torch.FloatTensor(reward),
            torch.FloatTensor(next_state),
            torch.FloatTensor(done)
        )

    def __len__(self):
        return len(self.buffer)

# ========== Agente DQN ========== 
class AgenteDQN:
    def __init__(self, input_dim, output_dim, device):
        self.device = device
        self.model = DQN(input_dim, output_dim).to(self.device)
        self.target_model = DQN(input_dim, output_dim).to(self.device)
        self.target_model.load_state_dict(self.model.state_dict())
        self.target_model.eval()

        self.optimizer = optim.Adam(self.model.parameters(), lr=1e-4)
        self.replay_buffer = ReplayBuffer()
        self.batch_size = 64
        self.gamma = 0.99

        self.epsilon = 1.0
        self.epsilon_decay = 0.995
        self.epsilon_min = 0.05
        self.update_target_steps = 1000
        self.steps_done = 0

    def select_action(self, state):
        # Asegurarse que 'state' es numpy
        state_tensor = torch.FloatTensor(state).unsqueeze(0).to(self.device)

        # Muestreo aleatorio
        sample = np.random.rand()

        # Selección de acción
        if sample < self.epsilon:
            action = random.randint(0, 31)
            return action
        else:
            with torch.no_grad():
                q_values = self.model(state_tensor)
                action = q_values.argmax().item()
                return action

    def train_step(self):
        if len(self.replay_buffer) < self.batch_size:
            return

        state, action, reward, next_state, done = self.replay_buffer.sample(self.batch_size)

        state = state.to(self.device)
        next_state = next_state.to(self.device)
        action = action.to(self.device)
        reward = reward.to(self.device)
        done = done.to(self.device)

        q_values = self.model(state)
        current_q = q_values.gather(1, action.unsqueeze(1)).squeeze(1)

        with torch.no_grad():
            next_q = self.target_model(next_state).max(1)[0]
            expected_q = reward + (1 - done) * self.gamma * next_q

        loss = nn.MSELoss()(current_q, expected_q)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        self.steps_done += 1

        if self.steps_done % self.update_target_steps == 0:
            self.target_model.load_state_dict(self.model.state_dict())

        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

    def save(self, path):
        torch.save(self.model.state_dict(), path)

    def load(self, path):
        self.model.load_state_dict(torch.load(path, map_location=self.device))
        self.target_model.load_state_dict(self.model.state_dict())
