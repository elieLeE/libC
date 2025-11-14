#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

#define __attr_unused__  __attribute__((unused))
#define __attr_nonnull__  __attribute__((nonnull))

#ifndef typeof
#define typeof __typeof__
#endif

#endif
