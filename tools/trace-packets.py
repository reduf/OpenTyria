import sys
from process import *
from datetime import datetime
from consts import *
from msgs import *

def main(args):
    if (2 ** 32) < sys.maxsize:
        print('Use a 32 bits version of Python')
        sys.exit(1)

    # proc = Process(7208)
    proc, = GetProcesses('Gw.exe')
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
        # if header == 94:
        #     campaign_type, profession = proc.read(packet + 4, 'II')
        #     if profession in professions:
        #         profession = professions[profession]
        #     if campaign_type in campaign_types:
        #         campaign_type = campaign_types[campaign_type]
        #     print(f'>> profession = {profession}, campaign_type = {campaign_type}')
        # if header == 130:
        #     param1, param2 = proc.read(packet + 4, 'II')
        #     print(f'>> param1 = {param1}, param2 = {param2}')

    @Hook.rawcall
    def on_recv_packet(ctx):
        packet, = proc.read(ctx.Esp, 'I')
        header, = proc.read(packet, 'I')
        if header in game_smsg_names:
            name = game_smsg_names[header]
        else:
            name = "unknown"

        # now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        print(f'RecvPacket ({ctx.Esi:X}): {header}, 0x{header:X}, {name}')

        # if header == 394:
        #     value, = proc.read(packet + 4, 'I')
        #     print(f'value = {value}')
        # if header == 26:
        #     _, val, count, *values = proc.read(packet, 'III64I')
        #     temp = ', '.join(f'{val:X}' for val in values[:count])
        #     print(f'  {val}, {count}, {temp}')
        # if header == 0x000F:
        #     _, val1, val2, val3 = proc.read(packet, 'IIII')
        #     print(f'  {val1}, {val2}, {val3},')
        # if name in ('GAME_SMSG_PLAYER_ATTR_MAX_KURZICK', 'GAME_SMSG_PLAYER_ATTR_MAX_LUXON', 'GAME_SMSG_PLAYER_ATTR_MAX_BALTHAZAR', 'GAME_SMSG_PLAYER_ATTR_MAX_IMPERIAL'):
        #     value, = proc.read(packet + 4, 'I')
        #     print(f'{name} -> {value}')
        # if name == 'GAME_SMSG_PING_REPLY':
        #     val = proc.read(packet + 4, 'I')
        #     print(f'>> {val}')
        # if name == 'GAME_SMSG_AGENT_ATTR_UPDATE_INT':
        #     attr_id, agent_id, value = proc.read(packet + 4, 'III')
        #     print(f'>>> (int) attr_id: {attr_id}, agent_id: {agent_id}, value: {value}')
        # if name == 'GAME_SMSG_ITEM_SET_PROFESSION':
        #     a, b = proc.read(packet + 4, 'II')
        #     print(f'a = {a}, b = {b}')
        # elif name == 'GAME_SMSG_AGENT_ATTR_UPDATE_FLOAT':
        #     attr_id, agent_id, value = proc.read(packet + 4, 'III')
        #     print(f'>>> (float) attr_id: {attr_id}, agent_id: {agent_id}, value: {value}')
        # if name == 'GAME_SMSG_INVENTORY_CREATE_BAG':
        #     stream_id, bag_type, bag_model_id, bag_id, slot_count, assoc_item_id = proc.read(packet + 4, 'IIIIII')
        #     print(f'>> stream_id = {stream_id}, bag_type = {bag_type}, bag_model_id = {bag_model_id}, bag_id = {bag_id}, slot_count = {slot_count}, assoc_item_id = {assoc_item_id}')
        # if name == 'GAME_SMSG_ITEM_MOVED_TO_LOCATION':
        #     stream_id, item_id, bag_id, slot = proc.read(packet + 4, 'IIII')
        #     print(f'>> stream_id = {stream_id}, item_id = {item_id}, bag_id = {bag_id}, slot = {slot}')
        # if name == 'GAME_SMSG_ITEM_CHANGE_LOCATION':
        #     unk1, item_id, bag_id, slot = proc.read(packet + 4, 'IIII')
        #     print(f'>> unk1 = {unk1}, item_id = {item_id}, bag_id = {bag_id}, slot = {slot}')
        # if name == 'GAME_SMSG_ITEM_REMOVE':
        #     a, b = proc.read(packet + 4, 'II')
        #     print(f'>> {a}, {b}')
        # if name == 'GAME_SMSG_ITEM_STREAM_CREATE':
        #     stream_id = proc.read(packet + 4, 'II')
        #     print(f'stream_id = {stream_id}')
        # if name == 'GAME_SMSG_ITEM_GENERAL_INFO':
        #     item_id, file_id, item_type, unk0, dye_color, materials, unk1, flags, value, model, quantity, name, n_modifier, *modifier = proc.read(packet + 4, 'IIIIIIIIIII128sI64I')
        #     name = name[:name.index(b'\0\0')]
        #     name = tuple(name[i] + (name[i+1] << 8) for i in range(0, len(name), 2))
        #     name_str = ', '.join(f'0x{val:X}' for val in name)
        #     name = f'{{{len(name)}, {{{name_str}}}}}'
        #     modifier = modifier[:n_modifier]
        #     modifier_str = ', '.join(f'0x{val:X}' for val in modifier)
        #     modifier = f'{{{len(modifier)}, {{{modifier_str}}}}}'
        #     item_type = item_types[item_type]
        #     dye_color = dye_colors[dye_color]
        #     if dye_color in ('DyeColor_None', 'DyeColor_Gray'):
        #         print(f'{{\n    .file_id = 0x{file_id:X},\n    .item_type = {item_type},\n    .unk0 = {unk0},\n    .dye_color = {dye_color},\n    .materials = {materials},\n    .unk1 = {unk1},\n    .flags = 0x{flags:X},\n    .value = {value},\n    .model = {model},\n    .quantity = {quantity},\n    .name = {name},\n    .modifiers = {modifier},\n}},')

    with ProcessDebugger(proc) as dbg:
        dbg.add_hook(smsg_addr, on_recv_packet)
        dbg.add_hook(cmsg_addr, on_send_packet)
        print(f'Start debugging process {proc.name}, {proc.id}')
        while running:
            dbg.poll(32)


if __name__ == '__main__':
    main(sys.argv[1::])
