/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file random.h
 */
#ifndef ECGEN_RANDOM_H
#define ECGEN_RANDOM_H

#include <pari/pari.h>
#include <stdbool.h>

/**
 * @brief Init the PARI-GP random generator.
 *
 * Initializes the PARI-GP random generator, tries to do so from
 * cryptographically strong sources(/dev/urandom) at first but falls back on
 * clock_gettime and time(NULL).
 *
 * @return whether the initialization was successful
 */
bool random_init(void);

/**
 * @brief Generate random <code>bits</code> sized prime.
 * @param bits the size of the prime to generate
 * @return a random prime in range [2^(bits - 1), 2^bits]
 */
GEN random_prime(unsigned long bits);

/**
 * @brief Generate random <code>bits</code> sized integer.
 * @param bits the size of the integer to generate
 * @return a random integer in range [2^(bits - 1), 2^bits]
 */
GEN random_int(unsigned long bits);

#endif  // ECGEN_RANDOM_H