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

        print(f'RecvPacket ({ctx.Esi:X}): {header}, 0x{header:X}, {name}')
        if name == 'GAME_SMSG_UPDATE_AGENT_INT_PROPERTY':
            prop_id, agent_id, value = proc.read(packet + 4, 'III')
            print(f'>> prop_id = {prop_id}, agent_id = {agent_id}, value = {value}')

        if name == 'GAME_SMSG_UPDATE_AGENT_FLOAT_PROPERTY':
            prop_id, agent_id, value = proc.read(packet + 4, 'IIf')
            print(f'>> prop_id = {prop_id}, agent_id = {agent_id}, value = {value}')

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
