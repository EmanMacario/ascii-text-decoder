# ascii-text-decoder
An implementation of an ASCII text decoder written in C
(Note: Only works with prefix free codes)

To compile: 
Go to Windows cmd, then type 
    "gcc -Wall -o decoder.exe dynamic-decode.c"

To use:
  decoder.exe MODEL.txt < ENCODED_TEXT.txt
  
  e.g. decoder.exe future-code.txt < future.txt
  
Made by Emmanuel Macario
