import os
import subprocess

def main():
    res = subprocess.run(['openssl', 'prime', '-generate', '-bits', '512'], capture_output=True)
    if res.returncode != 0:
        print(f'Failed to generate the prime function, stderr:\n{res.stderr}\nstdout:\n{res.stdout}')

    prime = int(res.stdout.decode('ascii'))
    generator = 4
    server_private = int.from_bytes(os.urandom(512 // 8), byteorder='little')
    server_public  = pow(generator, server_private, prime)

    print(f"""\
generator      = {generator}
prime          = {prime}
server_private = {server_private}
server_public  = {server_public}
""")

if __name__ == '__main__':
    main()
