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

        if name == 'GAME_SMSG_AGENT_SPAWNED':
            data = proc.read(packet + 4, 'IIIIffIffIffIIIIIIIffffIIffI')
            agent_id = data[0]
            model_id = data[1]
            agent_type = data[2]
            h000B = data[3]
            pos_x = data[4]
            pos_y = data[5]
            plane = data[6]
            direction_x = data[7]
            direction_y = data[8]
            h001E = data[9]
            speed_base = data[10]
            h0023 = data[11]
            h0027 = data[12]
            model_type = data[13]
            h002F = data[14]
            h0033 = data[15]
            h0037 = data[16]
            h003B = data[17]
            h003F = data[18]
            h0043_x = data[19]
            h0043_y = data[20]
            h004B_x = data[21]
            h004B_y = data[22]
            h0053 = data[23]
            h0055 = data[24]
            h0059_x = data[25]
            h0059_y = data[26]
            h0061 = data[27]
            print(f'>> agent_id = {agent_id}, model_id = {model_id}, agent_type = {agent_type}, h000B = {h000B}, pos_x = {pos_x}, pos_y = {pos_y}, plane = {plane}, direction_x = {direction_x}, direction_y = {direction_y}, h001E = {h001E}, speed_base = {speed_base}, h0023 = {h0023}, h0027 = {h0027}, model_type = {model_type}, h002F = {h002F}, h0033 = {h0033}, h0037 = {h0037}, h003B = {h003B}, h003F = {h003F}, h0043_x = {h0043_x}, h0043_y = {h0043_y}, h004B_x = {h004B_x}, h004B_y = {h004B_y}, h0053 = {h0053}, h0055 = {h0055}, h0059_x = {h0059_x}, h0059_y = {h0059_y}, h0061 = {h0061}')

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
