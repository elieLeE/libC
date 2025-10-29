#ifndef _SHARED_SWAP_H_
#define _SHARED_SWAP_H_

#include <string.h>
#include <stdio.h>

#include "assert.h"

#include "logger/logger.h"

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

/* {{{ RETHROW */

#define RETHROW(_exp)                                                         \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res < 0) {                                                       \
            return _res;                                                      \
        }                                                                     \
        _res;                                                                 \
    })


#define RETHROW_P(_exp)                                                       \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res == NULL) {                                                   \
            return NULL;                                                      \
        }                                                                     \
        _res;                                                                 \
    })

#define RETHROW_PN(_exp)                                                      \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res == NULL) {                                                   \
            return -1;                                                        \
        }                                                                     \
        _res;                                                                 \
    })

#define RETHROW_NP(_exp)                                                      \
    ({                                                                        \
        __auto_type _res = _exp;                                              \
        if (_res < 0) {                                                       \
            return NULL;                                                      \
        }                                                                     \
        _res;                                                                 \
    })

/* }}} */
/* {{{ Assert */

#define ASSERT(_cond, fmt, ...)                                               \
    do {                                                                      \
        if (!(_cond)) {                                                       \
            logger_assert_failed(fmt, ##__VA_ARGS__);                         \
            assert(_cond);                                                    \
        }                                                                     \
    } while (0)

#define ASSERT_EQUAL(_v_obtained, _v_expected)                                \
    do {                                                                      \
        ASSERT((_v_obtained == _v_expected),                                  \
               "obtained: %d, expected: %d", _v_obtained, _v_expected);       \
    } while (0)

#define ASSERT_STR_EQUAL(_str1, _str2)                                        \
    do {                                                                      \
        ASSERT((strcmp(_str1, _str2) == 0), "expected: %s, obtained: %s",     \
               _str1, _str2);                                                 \
    } while(0);

/* }}} */
/* {{{ For test */

#define CALL_TEST_FUNC(_func)                                                 \
    do {                                                                      \
        logger_test_start(#_func);                                            \
        _func();                                                              \
        logger_test_ok(#_func);                                               \
    } while (0)

#define BEGIN_TEST_MODULE(_module_name)                                       \
    {                                                                         \
        const char *__module_name = _module_name;                             \
        do {                                                                  \
            logger_test_begin_module(__module_name);                          \
        } while(0)

#define END_TEST_MODULE()                                                     \
        do {                                                                  \
            logger_test_end_module(__module_name);                            \
        } while(0);                                                           \
    }

/* }}} */

#endif
