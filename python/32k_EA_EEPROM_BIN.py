
# 32k array full of noops

rom = bytearray([0xEA] * 32768)

with open("rom.bin", "wb") as out_file:
  out_file.write(rom)
