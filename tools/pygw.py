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

    def get_game_ctx(self):
        tmp, = self.proc.read(self.ctx)
        return self.proc.read(tmp + 24)[0]

    def get_map_ctx(self):
        return self.proc.read(self.get_game_ctx() + 0x14)[0]

    def get_pathing_maps(self):
        map_ctx = self.get_map_ctx()
        tmp, = self.proc.read(map_ctx + 0x74)
        tmp, = self.proc.read(tmp)
        return Array(*self.proc.read(tmp + 0x18, 'IIII'))
