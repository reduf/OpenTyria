import sys
import argparse
import struct

from process import *
from datetime import datetime
from consts import *
from msgs import *
from hexdump import hexdump

def game_str_from_bytes(bytes):
    length = len(bytes) // 2
    codepoints = struct.unpack(f'<{length}H', bytes)
    try:
        idx = codepoints.index(0)
        return codepoints[:idx]
    except:
        return codepoints

def main(args):
    if (2 ** 32) < sys.maxsize:
        print('Use a 32 bits version of Python')
        sys.exit(1)

    proc, = GetProcesses(args.proc)
    scanner = ProcessScanner(proc)
    smsg_addr = scanner.find(b'\x50\x8B\x41\x08\xFF\xD0\x83\xC4\x08', 4)
    cmsg_addr = scanner.find(b'\xC7\x47\x54\x01\x00\x00\x00\x1B\xC9\x81\xE1\x00\x80', -0xC5)

    running = True
    def signal_handler(sig, frame):
        global running
        running = False

    @Hook.stdcall(LPVOID, DWORD, LPVOID)
    def on_send_packet(ctx, size, packet):
        header, = proc.read(packet, 'I')
        if header in game_cmsg_names:
            name = game_cmsg_names[header]
        else:
            name = "unknown"
        # now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        print(f'SendPacket: {header}, 0x{header:X}, {name}')

    @Hook.rawcall
    def on_recv_packet(ctx):
        packet, = proc.read(ctx.Esp, 'I')
        header, = proc.read(packet, 'I')
        if header in game_smsg_names:
            name = game_smsg_names[header]
        else:
            name = "unknown"

        if name in ('GAME_SMSG_AGENT_MOVEMENT_TICK', 'GAME_SMSG_AGENT_UPDATE_DIRECTION', 'GAME_SMSG_AGENT_MOVE_TO_POINT', 'GAME_SMSG_AGENT_UPDATE_SPEED', 'GAME_SMSG_AGENT_UPDATE_ROTATION', 'GAME_SMSG_AGENT_ATTR_UPDATE_INT'):
            return

        # now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # print(f'RecvPacket ({ctx.Esi:X}): {header}, 0x{header:X}, {name}')

        if name == 'GAME_SMSG_TITLE_TRACK_INFO':
            title_id, group_id, current_points, current_rank, points_needed_to_reach_current_rank, next_title_rank_id, points_needed_to_reach_next_rank, max_rank, max_rank_id, point_name, description = proc.read(packet + 4, 'IIIIIIIII16s16s')
            title_id = titles[title_id]
            point_name = game_str_from_bytes(point_name)
            point_name = ', '.join(f'0x{cp:04X}' for cp in point_name)
            description = game_str_from_bytes(description)
            description = ', '.join(f'0x{cp:04X}' for cp in description)
            print(f'title_id = {title_id}, group_id = {group_id}, current_points = {current_points}, current_rank = {current_rank}, points_needed_to_reach_current_rank = {points_needed_to_reach_current_rank}, next_title_rank_id = {next_title_rank_id}, points_needed_to_reach_next_rank = {points_needed_to_reach_next_rank}, max_rank = {max_rank}, max_rank_id = {max_rank_id}, point_name = {point_name}, description = {description}')

        if name == 'GAME_SMSG_TITLE_RANK_DATA':
            rank_id, unk0, rank = proc.read(packet + 4, 'III')
            print(f'rank_id = {rank_id}, unk0 = {unk0}, rank = {rank}')
            # name

    with ProcessDebugger(proc) as dbg:
        dbg.add_hook(smsg_addr, on_recv_packet)
        dbg.add_hook(cmsg_addr, on_send_packet)
        print(f'Start debugging process {proc.name}, {proc.id}')
        while running:
            dbg.poll(32)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Trace server and client messages', add_help=True)
    parser.add_argument("--proc", type=str, default='Gw.exe',
        help="Process name of the target Guild Wars instance.")
    args = parser.parse_args()
    main(args)
