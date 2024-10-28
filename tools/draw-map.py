import arcade
import struct

from process import *
from pygw import *

COLORS = [
    (0xe6, 0x19, 0x4b),
    (0x3c, 0xb4, 0x4b),
    (0xff, 0xe1, 0x19),
    (0x43, 0x63, 0xd8),
    (0xf5, 0x82, 0x31),
    (0x91, 0x1e, 0xb4),
    (0x46, 0xf0, 0xf0),
    (0xf0, 0x32, 0xe6),
    (0xbc, 0xf6, 0x0c),
    (0xfa, 0xbe, 0xbe),
    (0x00, 0x80, 0x80),
    (0xe6, 0xbe, 0xff),
    (0x9a, 0x63, 0x24),
    (0xff, 0xfa, 0xc8),
    (0x80, 0x00, 0x00),
    (0xaa, 0xff, 0xc3),
    (0x80, 0x80, 0x00),
    (0xff, 0xd8, 0xb1),
    (0x00, 0x00, 0x75),
    (0x80, 0x80, 0x80),
    (0xff, 0xff, 0xff),
    (0x00, 0x00, 0x00),
]

if __name__ == '__main__':
    proc, = GetProcesses('Gw.exe')
    game = pygw(proc)
    pathing_maps = game.get_pathing_maps()

    SIZE = 0x54
    bytes = pathing_maps.len * SIZE
    data, = proc.read(pathing_maps.buf, f'{bytes}s')

    min_x = float('inf')
    min_y = float('inf')
    max_x = float('-inf')
    max_y = float('-inf')

    planes = []
    for idx in range(pathing_maps.len):
        zplane, = struct.unpack_from('<I', data, (idx * SIZE) + 0)
        trap_len, trap_ptr = struct.unpack_from('<II', data, (idx * SIZE) + 0x14)
        traps_bytes, = proc.read(trap_ptr, f'{trap_len * 48}s')
        traps = []
        for jdx in range(trap_len):
            xtl, xtr, yt, xbl, xbr, yb = struct.unpack_from('<ffffff', traps_bytes, jdx * 48 + 0x18)
            traps.append((xtl, xtr, yt, xbl, xbr, yb))
            min_x = min(min_x, xtl, xbl)
            max_x = max(max_x, xtr, xbr)
            min_y = min(min_y, yb)
            max_y = max(max_y, yt)
        planes.append((zplane, traps))

    # print(f'min_x = {min_x}, max_x = {max_x}, min_y = {min_y}, max_y = {max_y}')

    WIDTH = 1280
    HEIGHT = 920

    offset_x = -min_x
    offset_y = -min_y
    ratio_w  = WIDTH / (max_x - min_x)
    ratio_h  = HEIGHT / (max_y - min_y)

    arcade.open_window(WIDTH, HEIGHT, "Walkable mesh")
    arcade.set_background_color(arcade.color.WHITE)
    arcade.start_render()

    for idx, (plane, traps) in enumerate(planes):
        for xtl, xtr, yt, xbl, xbr, yb in traps:
            xtl = (xtl + offset_x) * ratio_w
            xtr = (xtr + offset_x) * ratio_w
            xbl = (xbl + offset_x) * ratio_w
            xbr = (xbr + offset_x) * ratio_w
            yt  = (yt + offset_y) * ratio_h
            yb  = (yb + offset_y) * ratio_h

            # print(f'xtl = {xtl}, xtr = {xtr}, yt = {yt}, xbl = {xbl}, xbr = {xbr}, yb = {yb}')
            color = COLORS[min(idx, len(COLORS) - 1)]
            arcade.draw_triangle_filled(xbl, yb, xtl, yt, xtr, yt, color)
            arcade.draw_triangle_filled(xbr, yb, xtr, yt, xbl, yb, color)

    # Finish the render.
    arcade.finish_render()

    # Keep the window up until someone closes it.
    arcade.run()
