
# 32k array full of noops

#rom = bytearray([0xEA] * 32768)
rom = bytearray([0xA9,0xFF] * 16384)
rom[0x7ffc] = 0x00
rom[0x7ffd] = 0x90G
with open("rom.bin", "wb") as out_file:
  out_file.write(rom)
