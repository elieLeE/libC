#ifndef _SHARED_SWAP_H_
#define _SHARED_SWAP_H_

#include <string.h>
#include <stdio.h>

#include "assert.h"

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

#define RETHROW(_exp)                                                         \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res < 0) {                                                       \
            return _res;                                                      \
        }                                                                     \
    })


#define RETHROW_P(_exp)                                                       \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res == NULL) {                                                   \
            return _res;                                                      \
        }                                                                     \
    })

#define RETHROW_PN(_exp)                                                      \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res == NULL) {                                                   \
            return -1;                                                        \
        }                                                                     \
    })

#define RETHROW_NP(_exp)                                                      \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res < 0) {                                                       \
            return NULL;                                                      \
        }                                                                     \
    })

/* s1_p and s2_p has to be pointers on structures.
 * Return true is s1_p and s2 have the same bytes. */
#define ARE_SAME_STRUCT(_s1_p, _s2_p)                                         \
    ({                                                                        \
         memcmp(_s1_p, _s2_p, sizeof(*_s1_p)) == 0;                           \
    })

#define ASSERT(_cond, fmt, ...)                                               \
    do {                                                                      \
        if (!(_cond)) {                                                       \
            logger_assert_failed(fmt, ##__VA_ARGS__);                         \
            assert(_cond);                                                    \
        }                                                                     \
    } while (0)

#define ASSERT_EQUAL(_v_obtained, _v_expected) \
    do {                                                                      \
        ASSERT((_v_obtained == _v_expected),                                  \
               "obtained: %d, expected: %d", _v_obtained, _v_expected);       \
    } while (0)


#endif
