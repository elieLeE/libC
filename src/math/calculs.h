#ifndef _SHARED_SUM_H_
#define _SHARED_SUM_H_

__attribute__((const)) unsigned long sum_1_to_n(unsigned long n);
__attribute__((const)) unsigned long sum_carre_1_to_n(unsigned long n);

__attribute__((const))
unsigned long get_triangle_nber_from_index(unsigned long idx);
__attribute__((const))
unsigned long get_next_triangle_nber(unsigned long previous_n,
                                     unsigned long idx);

__attribute__((const)) long get_next_closest_power_of_two(long n);

#endif
