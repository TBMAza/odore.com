#ifndef C64UTILS_H
#define C64UTILS_H

#define ADDRINRANGE(addr, lower, upper) ( (addr) >= (lower) && (addr) <= (upper) )
#define FLAGREAD(flag, bitmap) ( (bitmap)>>(flag) & 1 )
#define FLAGON(flag, bitmap) ( (bitmap) |= 1<<(flag) )
#define FLAGOFF(flag, bitmap) ( (bitmap) &= ~(1<<(flag)) )
#define FLAGTOG(flag, bitmap) ( (bitmap) ^= 1<<(flag) )
#define BYTESTOWORD(hi, lo) ( (uint16_t)lo + (((uint16_t)hi)<<8) )

#endif
