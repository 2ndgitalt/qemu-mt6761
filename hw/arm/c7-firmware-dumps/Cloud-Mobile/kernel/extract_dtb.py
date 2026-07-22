import sys

def carve_dtb(filename):
    # The standard Device Tree magic bytes: D0 0D FE ED
    magic = b'\xd0\x0d\xfe\xed'

    with open(filename, 'rb') as f:
        data = f.read()

    offset = data.find(magic)

    if offset == -1:
        print(f"[-] No DTB found in {filename}")
        return

    print(f"[+] Found DTB magic at offset: {offset} (0x{offset:X})")

    # Save everything from the magic bytes onward as our new DTB
    out_name = f"{filename}_extracted.dtb"
    with open(out_name, 'wb') as out:
        out.write(data[offset:])

    print(f"[+] Successfully extracted DTB to: {out_name}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 extract_dtb.py <file_to_scan>")
    else:
        carve_dtb(sys.argv[1])
