import argparse
import collections
import csv
import os
import re
import struct

import capstone
import pefile

KEY = 24
SOURCE_ROOTS = ('Client', 'WebService', 'util', '3rdparty/RakNet30', '3rdparty/lua-5.1.1')
MARKER = re.compile(r'^(\s*)// (FUNCTION|TEMPLATE|SYNTHETIC|STUB|GLOBAL|VTABLE|LIBRARY): WEBSERVICE 0x([0-9a-fA-F]+)(.*)$')


class Image:
    def __init__(self, path):
        self.pe = pefile.PE(path, fast_load=True)
        self.pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_BASERELOC']])
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        text = [s for s in self.pe.sections if s.Name.rstrip(b'\0') == b'.text'][0]
        self.text_start = self.base + text.VirtualAddress
        self.text = text.get_data()[:text.Misc_VirtualSize]
        self.text_end = self.text_start + len(self.text)
        self.relocs = set()
        for block in getattr(self.pe, 'DIRECTORY_ENTRY_BASERELOC', []):
            for entry in block.entries:
                if entry.type == 3:
                    self.relocs.add(self.base + entry.rva)
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)

    def code(self, addr, size):
        off = addr - self.text_start
        if off < 0 or off + size > len(self.text):
            return None
        return self.text[off:off + size]

    def masked(self, addr, size):
        code = self.code(addr, size)
        if code is None:
            return None
        mask = bytearray(size)
        for i in range(size):
            if addr + i in self.relocs:
                for k in range(min(4, size - i)):
                    mask[i + k] = 1
        for a, length, mnemonic, _ in self.md.disasm_lite(bytes(code), addr):
            rel = a - addr
            if mnemonic in ('call', 'jmp') and length == 5 and code[rel] in (0xE8, 0xE9):
                for k in range(1, 5):
                    mask[rel + k] = 1
        return bytes(0 if mask[i] else code[i] for i in range(size)) + bytes(mask)

    def references(self, addr, size):
        code = self.code(addr, size)
        out = []
        for i in range(size - 3):
            if addr + i in self.relocs:
                out.append((i, struct.unpack_from('<I', code, i)[0]))
        for a, length, mnemonic, _ in self.md.disasm_lite(bytes(code), addr):
            rel = a - addr
            if mnemonic in ('call', 'jmp') and length == 5 and code[rel] in (0xE8, 0xE9):
                out.append((rel + 1, a + 5 + struct.unpack_from('<i', code, rel + 1)[0]))
        return sorted(out)

    def starts(self):
        found = set()
        for i in range(len(self.text) - 5):
            if self.text[i] == 0xE8:
                target = self.text_start + i + 5 + struct.unpack_from('<i', self.text, i + 1)[0]
                if self.text_start <= target < self.text_end:
                    found.add(target)
        for r in self.relocs:
            try:
                value = self.pe.get_dword_at_rva(r - self.base)
            except pefile.PEFormatError:
                continue
            if self.text_start <= value < self.text_end:
                found.add(value)
        return found


def match_bodies(ws, cl, sizes):
    index = collections.defaultdict(list)
    signatures = {}
    for addr, size in sizes.items():
        if size < KEY:
            continue
        sig = ws.masked(addr, size)
        if sig is None:
            continue
        signatures[addr] = sig
        index[ws.masked(addr, KEY)].append(addr)
    found = collections.defaultdict(list)
    for start in cl.starts():
        key = cl.masked(start, KEY)
        for addr in index.get(key, []):
            if cl.masked(start, sizes[addr]) == signatures[addr]:
                found[addr].append(start)
    return {addr: starts[0] for addr, starts in found.items() if len(starts) == 1}


def propagate(ws, cl, sizes, functions):
    data_votes = collections.defaultdict(collections.Counter)
    call_votes = collections.defaultdict(collections.Counter)
    done = set()
    while True:
        for addr in [a for a in functions if a not in done]:
            done.add(addr)
            a = ws.references(addr, sizes[addr])
            b = cl.references(functions[addr], sizes[addr])
            if [x[0] for x in a] != [y[0] for y in b]:
                continue
            for (_, target), (_, client) in zip(a, b):
                votes = call_votes if ws.text_start <= target < ws.text_end else data_votes
                votes[target][client] += 1
        claimed = set(functions.values())
        added = 0
        for target, votes in call_votes.items():
            if target in functions or target not in sizes or len(votes) != 1:
                continue
            client = next(iter(votes))
            if client not in claimed:
                functions[target] = client
                claimed.add(client)
                added += 1
        if not added:
            break
    data = {target: next(iter(votes)) for target, votes in data_votes.items() if len(votes) == 1}
    return functions, data


def write_tables(reccmp, mapping):
    per_client = collections.Counter(mapping.values())
    for name in sorted(os.listdir(reccmp)):
        if not name.startswith('webservice-') or not name.endswith('.csv'):
            continue
        with open(os.path.join(reccmp, name), newline='', encoding='latin-1') as f:
            reader = csv.DictReader(f)
            fields = reader.fieldnames
            rows = list(reader)
        out = []
        for row in rows:
            try:
                client = mapping.get(int(row['address'], 16))
            except (KeyError, ValueError):
                continue
            if client is None or per_client[client] > 1:
                continue
            row = dict(row)
            row['address'] = '%08x' % client
            out.append(row)
        target = os.path.join(reccmp, 'roblox-' + name[len('webservice-'):])
        if not out:
            if os.path.exists(target):
                os.remove(target)
            continue
        with open(target, 'w', newline='', encoding='latin-1') as f:
            writer = csv.DictWriter(f, fieldnames=fields, lineterminator='\n')
            writer.writeheader()
            writer.writerows(sorted(out, key=lambda r: int(r['address'], 16)))


def write_markers(repo, mapping):
    per_client = collections.Counter(mapping.values())
    for root in SOURCE_ROOTS:
        for dirpath, _, names in os.walk(os.path.join(repo, root)):
            for name in names:
                if not name.endswith(('.h', '.cpp', '.c', '.hpp', '.inl')):
                    continue
                path = os.path.join(dirpath, name)
                raw = open(path, newline='', encoding='latin-1').read()
                if 'WEBSERVICE 0x' not in raw:
                    continue
                crlf = '\r\n' in raw
                lines = raw.replace('\r\n', '\n').split('\n')
                out = []
                for i, line in enumerate(lines):
                    out.append(line)
                    m = MARKER.match(line)
                    if not m or (i + 1 < len(lines) and ': ROBLOX 0x' in lines[i + 1]):
                        continue
                    client = mapping.get(int(m.group(3), 16))
                    if client is None:
                        continue
                    suffix = m.group(4)
                    if per_client[client] > 1 and 'FOLDED' not in suffix and m.group(2) not in ('GLOBAL', 'VTABLE'):
                        suffix += ' FOLDED'
                    out.append('%s// %s: ROBLOX 0x%08x%s' % (m.group(1), m.group(2), client, suffix))
                if len(out) != len(lines):
                    text = '\n'.join(out)
                    open(path, 'w', newline='', encoding='latin-1').write(text.replace('\n', '\r\n') if crlf else text)


def main():
    parser = argparse.ArgumentParser(description='Carry WEBSERVICE names over to the ROBLOX target')
    parser.add_argument('webservice', help='original WebService.dll')
    parser.add_argument('roblox', help='original Roblox.exe')
    args = parser.parse_args()

    repo = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    reccmp = os.path.join(repo, 'reccmp')
    sizes = {}
    for row in csv.DictReader(open(os.path.join(reccmp, 'webservice-function-size.csv'))):
        if row['type'] == 'function' and row['size']:
            sizes[int(row['address'], 16)] = int(row['size'])

    ws = Image(args.webservice)
    cl = Image(args.roblox)
    functions, data = propagate(ws, cl, sizes, match_bodies(ws, cl, sizes))
    mapping = dict(data)
    mapping.update(functions)
    write_tables(reccmp, mapping)
    write_markers(repo, mapping)
    print('functions %d, data %d' % (len(functions), len(data)))


if __name__ == '__main__':
    main()
