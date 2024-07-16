import os
import sys
import pefile
import struct
import argparse

def reverse_endian(data):
    return data[::-1]

def parse_key_file(file_path):
    lines = open(file_path, 'r').read().splitlines()
    for line in lines:
        key, val = tuple(map(lambda val: val.strip(), line.split('=')))
        if key == 'prime':
            prime_mod = int(val)
        elif key == 'server_public':
            public_key = int(val)
        elif key == 'generator':
            prim_root = int(val)
    return prim_root, prime_mod, public_key

def main(args):
    input_path = args.input
    if args.output:
        out_path = args.output
    else:
        name, ext = os.path.splitext(input_path)
        out_path = name + '.custom' + ext

    print('[+] Input:', input_path)
    print('[+] Output:', out_path)
    print('[+] Key\'s file:', args.keys_file)

    prim_root, prime_mod, public_key = parse_key_file(args.keys_file)

    print('[+] Searching for the keys in the executable...')
    gw_pe = pefile.PE(input_path)
    for section in gw_pe.sections:
        if b'.text' in section.Name:
            text_section = section
            break

    if not text_section:
        print("Couldn't find '.text' section in executable '%s'" % input_path)
        sys.exit(1)

    text_sec_data = text_section.get_data()
    found = text_sec_data.find(b'\x8B\x45\x08\xC7\x00\x88\x00\x00\x00\xB8')
    if found < 0:
        print("Couldn't find the accessor of the Diffie-Hellman keys in text section")
        sys.exit(1)

    keys_addr,  = struct.unpack_from('<L', text_sec_data, found + 0xA)
    keys_rva    = keys_addr - gw_pe.OPTIONAL_HEADER.ImageBase
    print('[+] Found DHL keys at address:', hex(keys_addr))
    print('[+] DHL keys are at RVA:', hex(keys_rva))

    # the first 4 bytes are unknown, but always 1
    key_file_pos = gw_pe.get_offset_from_rva(keys_rva) + 4

    print('[+] Searching for the GwLoginClient skip...')
    found = text_sec_data.find(b'\xEB\x05\xBF\x01\x00\x00\x00\x53\x56\xE8')
    if found < 0:
        print("Couldn't find the jmp to patch to avoid overriding GwLoginClient.dll in text section");
        sys.exit(1)

    jmp_rva = found + text_section.VirtualAddress + 0x17
    print('[+] jmp_rva is:', hex(jmp_rva))
    jmp_file_pos = gw_pe.get_offset_from_rva(jmp_rva)

    gw_pe.close()

    print('[+] Patching executable...')
    with open(input_path, 'rb') as f:
        data = f.read()

    data_list = list(data)
    data_list[key_file_pos:key_file_pos+4]      = prim_root.to_bytes(4, byteorder='little')
    data_list[key_file_pos+4:key_file_pos+68]   = prime_mod.to_bytes(64, byteorder='little')
    data_list[key_file_pos+68:key_file_pos+132] = public_key.to_bytes(64, byteorder='little')

    data_list[jmp_file_pos:jmp_file_pos+1]      = b'\x31'

    data = bytes(data_list)
    with open(out_path, 'wb') as f:
        f.write(data)

    print("[+] Patched executable written here '%s'" % out_path)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Patch Gw executable with custom DHL keys', add_help=True)
    parser.add_argument('input')
    parser.add_argument('keys_file')
    parser.add_argument('--output', metavar='output', type=str, help='The output file', required=False)

    args = parser.parse_args()
    main(args)
