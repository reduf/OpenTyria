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
        print(f"Couldn't find '.text' section in executable '{input_path}'")
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

    print('[+] Searching for mutex name')
    for section in gw_pe.sections:
        if b'.rdata' in section.Name:
            rdata_section = section
    if not rdata_section:
        print(f"Couldn't find the '.rdata' section in executable '{input_path}'")
        sys.exit(1)

    rdata_section_data = rdata_section.get_data()
    mutex_name_file_pos = rdata_section_data.find(b'AN-Mutex-Window')
    if mutex_name_file_pos < 0:
        print("Couldn't find the mutex name in .rdata")
        sys.exit(1)
    mutex_name_file_pos += rdata_section.PointerToRawData
    print(f'[+] mutex_name_file_pos is 0x{mutex_name_file_pos:X}')

    gw_pe.close()

    print('[+] Patching executable...')
    with open(input_path, 'rb') as f:
        data = f.read()

    data_mut = list(data)
    data_mut[key_file_pos:key_file_pos+4]      = prim_root.to_bytes(4, byteorder='little')
    data_mut[key_file_pos+4:key_file_pos+68]   = prime_mod.to_bytes(64, byteorder='little')
    data_mut[key_file_pos+68:key_file_pos+132] = public_key.to_bytes(64, byteorder='little')

    data_mut[jmp_file_pos:jmp_file_pos+1]      = b'\x31'
    data_mut[mutex_name_file_pos:mutex_name_file_pos+len(b'AN-Futex')] = b'AN-Futex'

    data = bytes(data_mut)
    with open(out_path, 'wb') as f:
        f.write(data)

    print("[+] Patched executable written here '%s'" % out_path)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Patch Gw executable with custom DHL keys', add_help=True)
    parser.add_argument("--proc", type=str, default='Gw.exe',
        help="Process name of the target Guild Wars instance.")
    parser.add_argument('keys_file')
    parser.add_argument('--output', metavar='output', type=str, help='The output file', required=False)

    args = parser.parse_args()
    main(args)
