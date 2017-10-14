/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file exhaustive.h
 */
#ifndef ECGEN_EXHAUSTIVE_H
#define ECGEN_EXHAUSTIVE_H

#include "misc/types.h"

/**
 * @brief
 */
typedef struct {
	gen_f *generators;
	check_t **validators;
	arg_t **argss;
	unroll_f *unrolls;
} exhaustive_t;

/**
 * @brief
 * @param setup
 */
void exhaustive_clear(exhaustive_t *setup);

/**
 *
 */
void exhaustive_uinit(unroll_f *unrolls);

/**
 *
 * @param curve
 * @param setup
 * @param start_offset
 * @param end_offset
 * @param retry
 * @return
 */
int exhaustive_gen_retry(curve_t *curve, const exhaustive_t *setup,
						 offset_e start_offset, offset_e end_offset, int retry);

/**
 *
 * @param curve
 * @param setup
 * @param start_offset
 * @param end_offset
 * @return
 */
int exhaustive_gen(curve_t *curve, const exhaustive_t *setup,
				   offset_e start_offset, offset_e end_offset);

/**
 *
 * @return
 */
int exhaustive_do();

#endif  // ECGEN_EXHAUSTIVE_H
