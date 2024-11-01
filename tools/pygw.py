from dataclasses import dataclass
from process import *

@dataclass
class Array:
    buf: int
    cap: int
    len: int
    param: int

class pygw(object):
    def __init__(self, proc):
        self.proc = proc
        self.scanner = ProcessScanner(proc)

        tmp = self.scanner.find(b'\x50\x6A\x0F\x6A\x00\xFF\x35', +7)
        self.ctx, = proc.read(tmp)

        tmp = self.scanner.find(b'\xFF\x50\x10\x47\x83\xC6\x04\x3B\xFB\x75\xE1', +0xD)
        self.agent_ptr, = proc.read(tmp)

        tmp = self.scanner.find(b'\x56\x89\x47\x08\x6A\x01\xD9\x1B\xE8', -0x37)
        self.camera_ptr, = proc.read(tmp)

    def get_game_ctx(self):
        tmp, = self.proc.read(self.ctx)
        return self.proc.read(tmp + 24)[0]

    def get_map_ctx(self):
        return self.proc.read(self.get_game_ctx() + 0x14)[0]

    def get_char_ctx(self):
        return self.proc.read(self.get_game_ctx() + 0x44)[0]

    def get_world_ctx(self):
        return self.proc.read(self.get_game_ctx() + 0x2C)[0]

    def get_player_array(self):
        tmp = self.get_world_ctx()
        return Array(*self.proc.read(tmp + 0x80C, 'IIII'))

    def get_agent_array(self):
        return Array(*self.proc.read(self.agent_ptr, 'IIII'))

    def get_player_id(self):
        tmp = self.get_char_ctx()
        return self.proc.read(tmp + 0x2A4)[0]

    def get_player_by_id(self, player_id):
        players = self.get_player_array()
        if players.len <= player_id:
            raise RuntimeError(f'Invalid player id {player_id}')
        return players.buf + (player_id * 0x4C)

    def get_agent_id_by_player_id(self):
        player = self.get_player_by_id(self.get_player_id())
        return self.proc.read(player)[0]

    def get_agent_by_id(self, agent_id):
        agents = self.get_agent_array()
        if agents.len <= agent_id:
            raise RuntimeError(f'Invalid agent id {agent_id}')
        return self.proc.read(agents.buf + (agent_id * 4))[0]

    def get_agent_pos(self, agent_id):
        agent = self.get_agent_by_id(agent_id)
        return self.proc.read(agent + 0x74, 'ffI')

    def get_pathing_maps(self):
        tmp = self.get_map_ctx()
        tmp, = self.proc.read(tmp + 0x74)
        tmp, = self.proc.read(tmp)
        return Array(*self.proc.read(tmp + 0x18, 'IIII'))

    def get_camera_pos(self):
        return self.proc.read(self.camera_ptr + 0x78, 'ff')
