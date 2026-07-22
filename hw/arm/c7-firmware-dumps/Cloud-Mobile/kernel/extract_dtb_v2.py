import sys
import struct

def carve_dtb(filename):
    magic = b'\xd0\x0d\xfe\xed'

    with open(filename, 'rb') as f:
        data = f.read()

    # Find all occurrences of the magic bytes
    offsets = []
    idx = data.find(magic)
    while idx != -1:
        offsets.append(idx)
        idx = data.find(magic, idx + 4)

    if not offsets:
        print(f"[-] No DTB found in {filename}")
        return

    print(f"[+] Found DTB magic at offsets: {[hex(o) for o in offsets]}")

    # The actual DTB is almost always the last occurrence (appended to the end)
    real_offset = offsets[-1]
    print(f"[+] Selecting payload at offset: {real_offset} (0x{real_offset:X})")

    # A valid DTB has its total size stored as a 32-bit big-endian integer
    # exactly 4 bytes after the magic number.
    if len(data) >= real_offset + 8:
        # Unpack the 4-byte size field
        dtb_size = struct.unpack('>I', data[real_offset+4 : real_offset+8])[0]
        print(f"[+] DTB Header reports total size: {dtb_size} bytes")

        # Sanity check: is the size reasonable? (Usually between 10KB and 2MB)
        if 1000 < dtb_size < 5000000:
            out_name = f"{filename}_real.dtb"
            with open(out_name, 'wb') as out:
                out.write(data[real_offset : real_offset + dtb_size])
            print(f"[+] Successfully extracted exact DTB to: {out_name}")
        else:
            print("[-] Size seems invalid. This might not be a real DTB.")
    else:
        print("[-] File cut off, cannot read DTB header.")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 extract_dtb_v2.py <file_to_scan>")
    else:
        carve_dtb(sys.argv[1])
