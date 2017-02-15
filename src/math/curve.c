/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "curve.h"
#include "exhaustive/seed.h"
#include "field.h"
#include "point.h"

curve_t *curve_new(void) {
	curve_t *curve = pari_malloc(sizeof(curve_t));
	if (!curve) {
		perror("Couldn't malloc.");
		exit(1);
	}
	memset(curve, 0, sizeof(curve_t));
	return curve;
}

curve_t *curve_copy(curve_t *src, curve_t *dest) {
	if (src->seed) dest->seed = seed_copy(src->seed, dest->seed);
	if (src->field) dest->field = gcopy(src->field);
	if (src->a) dest->a = gcopy(src->a);
	if (src->b) dest->b = gcopy(src->b);
	if (src->curve) dest->curve = gcopy(src->curve);
	if (src->order) dest->order = gcopy(src->order);
	if (src->points) {
		dest->points = points_new(src->npoints);
		dest->points = points_copy(src->points, dest->points, src->npoints);
		dest->npoints = src->npoints;
	}
	return dest;
}

void curve_free(curve_t **curve) {
	if (*curve) {
		seed_free(&(*curve)->seed);
		if ((*curve)->points) {
			for (size_t i = 0; i < (*curve)->npoints; ++i) {
				point_free(&(*curve)->points[i]);
			}
			points_free(&(*curve)->points);
		}
		pari_free(*curve);
		*curve = NULL;
	}
}

int curve_init(curve_t *curve, config_t *config, ...) {
	pari_sp ltop = avma;
	GEN v = gen_0;
	switch (typ(curve->field)) {
		case t_INT:
			v = gtovec0(gen_0, 2);
			gel(v, 1) = curve->a;
			gel(v, 2) = curve->b;
			break;
		case t_FFELT:
			v = gtovec0(gen_0, 5);
			gel(v, 1) = gen_1;
			gel(v, 4) = curve->a;
			gel(v, 5) = curve->b;
			break;
		default:
			pari_err_TYPE("curve_init", curve->field);
	}

	curve->curve = gerepilecopy(ltop, ellinit(v, curve->field, -1));
	return 1;
}

int curve_nonzero(curve_t *curve, config_t *config, ...) {
	pari_sp ltop = avma;
	curve_init(curve, config);
	if (gequal0(ell_get_disc(curve->curve))) {
		avma = ltop;
		return -3;
	} else {
		return 1;
	}
}

int curve_seed_fp(curve_t *curve, config_t *config, ...) {
	//TODO implement
	return INT_MIN;
}

int curve_seed_f2m(curve_t *curve, config_t *config, ...) {
	//TODO implement
	return INT_MIN;
}

int curve_seed(curve_t *curve, config_t *config, ...) {
	switch (typ(curve->field)) {
		case t_INT:
			return curve_seed_fp(curve, config);
		case t_FFELT:
			return curve_seed_f2m(curve, config);
		default:
			pari_err_TYPE("curve_seed", curve->field);
			return INT_MIN; /* NOT REACHABLE */
	}
}

GEN curve_params(curve_t *curve) {
	pari_sp ltop = avma;

	GEN result = field_params(curve->field);
	if (curve->a) result = gconcat(result, field_elementi(curve->a));
	if (curve->b) result = gconcat(result, field_elementi(curve->b));
	if (curve->order) result = gconcat(result, gtovec(curve->order));
	if (curve->points) {
		for (size_t i = 0; i < curve->npoints; ++i) {
			GEN point =
			    gconcat(field_elementi(gel(curve->points[i]->point, 1)),
			            field_elementi(gel(curve->points[i]->point, 2)));
			result = gconcat(result, point);
			result = gconcat(result, curve->points[i]->order);
		}
	}

	return gerepilecopy(ltop, result);
}