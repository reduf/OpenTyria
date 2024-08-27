import sys
import argparse
import struct

from process import *
from datetime import datetime
from consts import *
from msgs import *
from hexdump import hexdump

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
        print(f'RecvPacket ({ctx.Esi:X}): {header}, 0x{header:X}, {name}')

        if name == 'GAME_SMSG_INSTANCE_MANIFEST_PHASE':
            phase = proc.read(packet + 4, 'I')
            print(f'phase = {phase}')
        if name == 'GAME_SMSG_INSTANCE_MANIFEST_DONE':
            phase, map_id, unk = proc.read(packet + 4, 'III')
            print(f'phase = {phase}, map_id = {map_id}, unk = {unk}')

        # if name == 'GAME_SMSG_INVENTORY_CREATE_BAG':
        #     stream_id, bag_type, bag_model_id, bag_id, slot_count, assoc_item_id = proc.read(packet + 4, 'IIIIII')
        #     bag_type = bag_types[bag_type]
        #     bag_model_id = bag_model_ids[bag_model_id]
        #     print(f'stream_id = {stream_id}, bag_type = {bag_type}, bag_model_id = {bag_model_id}, bag_id = {bag_id}, slot_count = {slot_count}, assoc_item_id = {assoc_item_id}')
        # if name == 'GAME_SMSG_PLAYER_HERO_NAME_AND_INFO':
        #     name, *rem = proc.read(packet + 4, '64s6I')
        #     # name = struct.unpack('<32H', name)
        #     # name = ''.join(chr(cp) for cp in name)
        #     print(name, *rem)

        # if name == 'GAME_SMSG_ITEM_GENERAL_INFO':
        #     item_id, file_id, type, unk0, dye_color, materials, unk1, flags, value, model, quantity, name, n_modifier, modifier = proc.read(packet + 4, 'IIIIIIIIIII128sI256s')
        #     n_name = len(name) // 2
        #     name = struct.unpack(f'{n_name}H', name)
        #     name = '[' + ', '.join(f'0x{val:X}' for val in name) + ']'
        #     modifier = struct.unpack('<64I', modifier)[:n_modifier]
        #     modifier = '[' + ', '.join(f'0x{val:X}' for val in modifier) + ']'
        #     print(f'item_id = {item_id}, file_id = 0x{file_id:X}, type = {type}, unk0 = {unk0}, dye_color = {dye_color}, materials = {materials}, unk1 = {unk1}, flags = 0x{flags:X}, value = {value}, model = {model}, quantity = {quantity}, name = {name}, modifier = {modifier}')
        # if name == 'GAME_SMSG_ITEM_UPDATE_CUSTOMIZED_NAME':
        #     item_id, name = proc.read(packet + 4, 'I64s')
        #     n_name = len(name) // 2
        #     name = struct.unpack(f'{n_name}H', name)
        #     name = '[' + ', '.join(f'0x{val:X}' for val in name) + ']'
        #     print(f'item_id = {item_id}, name = {name}')

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
