In general:
  1. You need to turn on the UART in AUX.  Make sure you 
     read-modify-write --- don't kill the SPIm enables.
  2. Immediately disable tx/rx (you don't want to send garbage).
  3. Figure out which registers you can ignore (e.g., IO, p 11).
     Many devices have many registers you can skip.
  4. Find and clear all parts of its state (e.g., FIFO queues) since we
     are not absolutely positive they do not hold garbage.  Disable 
     interrupts.
  5. Configure: 115200 Baud, 8 bits, 1 start bit, 1 stop bit.  No flow
     control.
  6. Enable tx/rx.  It should be working!


Page numbers:

  - IO: p 11, tx, rx FIFO.
  - IER: p 12, turn off interrupts.
  - IIR: p 13, clear FIFOs.
  - LCR: p 14, data size, errata!
  - MCR: p 14
  - LSR: p 15, tx empty or has space, rx empty.
  - MSR: p 15, 
  - SCRATCH: 16
  - CNTL: 16, tx, rx enable.
  - STAT: 18, tx full, rx has data.
  - BAUD: 19
