; vasm6502_oldstyle.exe -Fbin -dotdir -wdc02 rom.s
; xxd -a -o 32768 a.out

  .org $8000
    nop
  .org $9000
    ;Setup stack
    ldx #$ff
    txs
NOPS
    nop
    nop

  .org $fff7
    jmp NOPS ; Go back to 9000, skipping IO region and read NOPs again.  
  .org $fffA
  .word $FFFF   ; NMIB
  .word $0090   ; Jump on boot
  .word $FFFF   ; IRQB/BRK