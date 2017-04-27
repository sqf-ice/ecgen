/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
/**
 * @file output.h
 */
#ifndef ECGEN_OUTPUT_H
#define ECGEN_OUTPUT_H

#include <pari/pari.h>
#include <stdbool.h>
#include "math/types.h"

/**
 * @brief Output curve to a pari_malloc'ed string in CSV format.
 * @param curve
 * @param config
 * @return
 */
char *output_scsv(curve_t *curve, const config_t *cfg);

/**
 * @brief Output CSV separator(newline) to a pari_malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_scsv_separator(const config_t *cfg);

/**
 * @brief Output CSV output header to a pari_malloc'ed string in CSV format.
 * @param cfg
 * @return
 */
char *output_scsv_begin(const config_t *cfg);

/**
 * @brief Output CSV output footer to a pari_malloc'ed string in CSV format.
 * @param cfg
 * @return
 */
char *output_scsv_end(const config_t *cfg);

/**
 * @brief Output curve to a pari_malloc'ed string in JSON format.
 * @param curve
 * @param config
 * @return
 */
char *output_sjson(curve_t *curve, const config_t *cfg);

/**
 * @brief Output JSON separator(a ",\n") to a pari_malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_sjson_separator(const config_t *cfg);

/**
 * @brief Output JSON output header(a "[") to a pari_malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_sjson_begin(const config_t *cfg);

/**
 * @brief Output JSON output footer(a "]") to a pari_malloc'ed string in CSV
 * format.
 * @param cfg
 * @return
 */
char *output_sjson_end(const config_t *cfg);

/**
 * @brief Output curve to a pari_malloc'ed string in configured format.
 * @param curve
 * @param config
 * @return
 */
char *(*output_s)(curve_t *curve, const config_t *cfg);

/**
 * @brief Output curve to a FILE *out in configured format.
 * @param out
 * @param curve
 * @param config
 */
void output_f(FILE *out, curve_t *curve, const config_t *cfg);

/**
 * @brief Output curve to configured output in configured format.
 * @param curve
 * @param config
 */
void output_o(curve_t *curve, const config_t *cfg);

/**
 * @brief Output separator to a pari_malloc'ed string in configured format.
 * @param cfg
 * @return
 */
char *(*output_s_separator)(const config_t *cfg);

/**
 * @brief Output separator to a FILE *out in configured format.
 * @param out
 * @param cfg
 */
void output_f_separator(FILE *out, const config_t *cfg);

/**
 * @brief Output separator to configured output in configured format.
 * @param cfg
 */
void output_o_separator(const config_t *cfg);

/**
 * @brief Output header to a pari_malloc'ed string in configured format.
 * @param cfg
 * @return
 */
char *(*output_s_begin)(const config_t *cfg);

/**
 * @brief Output header to a FILE *out in configured format.
 * @param out
 * @param cfg
 */
void output_f_begin(FILE *out, const config_t *cfg);

/**
 * @brief Output header to configured output in configured format.
 * @param cfg
 */
void output_o_begin(const config_t *cfg);

/**
 * @brief Output footer to a pari_malloc'ed string in configured format.
 * @param cfg
 * @return
 */
char *(*output_s_end)(const config_t *cfg);

/**
 * @brief Output footer to a FILE *out in configured format.
 * @param out
 * @param cfg
 */
void output_f_end(FILE *out, const config_t *cfg);

/**
 * @brief Output header to configured output in configured format.
 * @param cfg
 */
void output_o_end(const config_t *cfg);

/**
 * @brief Configured output FILE*.
 */
extern FILE *out;

/**
 * @brief Configured verbose output FILE*.
 */
extern FILE *verbose;

/**
 * @brief Initialize output based on cfg.
 * @param cfg
 */
void output_init(const config_t *cfg);

/**
 * @brief Deinitialize output.
 */
void output_quit(void);

#ifdef DEBUG
#define debug(...) fprintf(out, __VA_ARGS__)
#else
#define debug(...)
#endif

#endif  // ECGEN_OUTPUT_H
