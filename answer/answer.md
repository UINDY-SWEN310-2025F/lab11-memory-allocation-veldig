to-do1-1
The “si” column means swap-in. It shows how many kilobytes per second are being moved from disk back into RAM.
The “so” column means swap-out. It shows how many kilobytes per second are being moved from RAM to disk when the system is under memory pressure.
The “bi” column means block-in. It shows how many blocks per second are being read from a block device (disk reads).
The “bo” column means block-out. It shows how many blocks per second are being written to a block device (disk writes).
In your output, si and so were always zero, which means there was no swapping and the system had enough RAM. The bi and bo values were small, meaning only light background disk activity.

to-do2-1 (first-fit)
Free partitions (addresses in KB):
600100–600300 (200 KB), 601200–601800 (600 KB), 602000–602500 (500 KB), 602800–603100 (300 KB), 603400–603650 (250 KB).
First-fit result (processes arrive P1, P2, P3, P4):
P1: starting address 601200, ending address 601800
P2: starting address 602000, ending address 602500
P3: not allocated (no remaining partition big enough)
P4: not allocated (no remaining partition big enough)

to-do2-2 (best-fit)
Start from the original free partitions again.
Best-fit result:
P1: starting address 602000, ending address 602500
P2: starting address 603400, ending address 603650
P3: starting address 601200, ending address 601800
P4: not allocated (no remaining partition big enough)

to-do3-1
The machine has 16 TB = 2^44 bytes. With 4 KB pages = 2^12 bytes, the number of frames is 2^44 / 2^12 = 2^32 frames.

to-do3-2
A 4 KB page is 2^12 bytes, so the displacement (offset) needs 12 bits.

to-do3-3
Page number uses 36 bits and offset uses 12 bits, giving a 48-bit virtual address. The maximum virtual memory is 2^48 bytes = 256 TB.

