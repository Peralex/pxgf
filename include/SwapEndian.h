#ifndef _SWAP_ENDIAN
#define _SWAP_ENDIAN

/******************************************************************************
  FUNCTION: SwapEndian
  PURPOSE: Swap the byte order of a structure
  EXAMPLE: float F=123.456;; SWAP_FLOAT(F);
******************************************************************************/

#define SWAP_SHORT(Var) SwapEndian((void *)&Var, sizeof(short))
#define SWAP_USHORT(Var) SwapEndian((void *)&Var, sizeof(short))
#define SWAP_INT(Var) SwapEndian((void *)&Var, sizeof(int))
#define SWAP_UINT(Var) SwapEndian((void *)&Var, sizeof(int))
#define SWAP_LONG(Var) SwapEndian((void *)&Var, sizeof(long))
#define SWAP_INT64(Var) SwapEndian((void *)&Var, sizeof(int64_t))
#define SWAP_ULONG(Var) SwapEndian((void *)&Var, sizeof(long))
#define SWAP_FLOAT(Var) SwapEndian((void *)&Var, sizeof(float))
#define SWAP_DOUBLE(Var) SwapEndian((void *)&Var, sizeof(double))

extern void SwapEndian(void *Addr, const int Nb);

#endif
