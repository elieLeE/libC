#ifndef _SHARED_SWAP_H_
#define _SHARED_SWAP_H_

#include <string.h>

#define SWAP(_type, _a, _b)                                                   \
    do {                                                                      \
        _type _c = *_b;                                                       \
        *_b = *_a;                                                            \
        *_a = _c;                                                             \
    } while(0)

#define MAX(_a, _b)                                                           \
     _a > _b ? _a : _b;

#define MIN(_a, _b)                                                           \
    _a < _b ? _a : _b;

#define PERCENTAGE(_a, _b)                                                    \
    ((double)_a) / _b * 100;

#endif
