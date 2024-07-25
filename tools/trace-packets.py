import sys
from process import *

def main(args):
    proc = Process(0x4d8c)
    scanner = ProcessScanner(proc)
    addr = scanner.find(b'\x50\x8B\x41\x08\xFF\xD0\x83\xC4\x08', 4)

    running = True
    def signal_handler(sig, frame):
        global running
        running = False

    @Hook.rawcall
    def on_packet(ctx):
        packet, = proc.read(ctx.Esp, 'I')
        header, = proc.read(packet, 'I')
        print(f'Header {header}, 0x{header:X}')

    with ProcessDebugger(proc) as dbg:
        dbg.add_hook(addr, on_packet)
        print(f'Start debugging process {proc.name}, {proc.id}')
        while running:
            dbg.poll(32)


if __name__ == '__main__':
    main(sys.argv[1::])
