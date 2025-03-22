import struct
import random


elems = [random.randint(-100000, 10000) for x in range(100)]

binints = struct.pack('100i', *elems)

out =  open("cerca_file.bin", "wb")
out.write(binints)
