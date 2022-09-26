; vasm -Fbin -dotdir -wdc02 rom.s

  .org $8000

  .org $9000
    ;Setup stack
    ldx #$ff
    txs
loop:
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    jmp loop
  .org $fffC
  .word $0090
  .word $0000   ; Fill to 32k