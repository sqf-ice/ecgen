/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_OUTPUT_H
#define ECGEN_OUTPUT_H

#include <stdbool.h>
#include <parson/parson.h>
#include <pari/pari.h>

/**
 *
 * @param delim
 * @param format
 * @param vector
 * @return
 */
char *output_scsv(const char *format, char delim, GEN vector);

/**
 *
 * @param out
 * @param delim
 * @param format
 * @param vector
 */
void output_csv(FILE *out, const char *format, char delim, GEN vector);

/**
 *
 * @param vector
 * @return
 */
char *output_sjson(GEN vector);

/**
 *
 * @param out
 * @param vector
 */
void output_json(FILE *out, GEN vector);

FILE *output_open(const char *output, bool append);

void output_close(FILE *out);

#endif  // ECGEN_OUTPUT_H
