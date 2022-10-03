; vasm6502_oldstyle.exe -Fbin -dotdir -wdc02 rom.s
; xxd -a -o 32768 a.out

  ; IO sections are masked by the PLD. ROM really starts at $9000
  .org $8000    ; IO0
  .org $8100    ; IO1
  .org $8200    ; IO2
  ; All the way up to $8900 but the PLD only maps 3 right now.
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