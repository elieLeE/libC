#ifndef __MACROS_H_
#define __MACROS_H_

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "assert.h"

#include "logger/logger.h"

#define CONCAT_(_x, _y) _x##_y
#define CONCAT(_x, _y) CONCAT_(_x, _y)

#define MAX(_a, _b)                                                           \
     _a > _b ? _a : _b;

#define MIN(_a, _b)                                                           \
    _a < _b ? _a : _b;

#define PERCENTAGE(_a, _b)                                                    \
    ((double)_a) / _b * 100;

/* {{{ RETHROW */

#define RETHROW(_exp)                                                         \
    ({                                                                        \
        __auto_type __res = (_exp);                                           \
        if (__res < 0) {                                                      \
            return __res;                                                     \
        }                                                                     \
        __res;                                                                \
    })


#define RETHROW_P(_exp)                                                       \
    ({                                                                        \
        __auto_type __res = (_exp);                                           \
        if (__res == NULL) {                                                  \
            return NULL;                                                      \
        }                                                                     \
        __res;                                                                \
    })

#define RETHROW_PN(_exp)                                                      \
    ({                                                                        \
        __auto_type __res = (_exp);                                           \
        if (__res == NULL) {                                                  \
            return -1;                                                        \
        }                                                                     \
        __res;                                                                \
    })

#define RETHROW_NP(_exp)                                                      \
    ({                                                                        \
        __auto_type __res = (_exp);                                           \
        if (__res < 0) {                                                      \
            return NULL;                                                      \
        }                                                                     \
        __res;                                                                \
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

#define ASSERT_TEST_BOOL(_test_bool, _v_left, _v_right, _fmt_data)            \
    do {                                                                      \
        ASSERT((_test_bool),                                                  \
               "left: "_fmt_data ", right: "_fmt_data, _v_left, _v_right);    \
    } while (0)

#define ASSERT_TEST(_v_left, _v_right, _test, _fmt_data)                      \
    ASSERT_TEST_BOOL(((_v_left) _test (_v_right)),                            \
                     _v_left, _v_right, _fmt_data)

#define ASSERT_EQUAL(_v_left, _v_right, _fmt_data)                            \
    ASSERT_TEST(_v_left, _v_right, ==, _fmt_data)

#define ASSERT_EQUAL_INT(_v_left, _v_right)                                   \
    ASSERT_EQUAL((uint64_t)_v_left, (uint64_t)_v_right, "%ld")

#define ASSERT_EQUAL_LONG(_v_left, _v_right)                                  \
    ASSERT_EQUAL(_v_left, _v_right, "%ld")

#define ASSERT_STR_EQUAL(_str1, _str2)                                        \
    do {                                                                      \
        ASSERT_TEST_BOOL((strcmp(_str1, _str2) == 0), _str1, _str2, "%s");    \
    } while(0);

/* }}} */

#define unconst_cast(_type_t, _p)                                             \
    ({ const _type_t *__p = (_p); (_type_t *)__p; })

#endif
