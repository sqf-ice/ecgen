/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "point.h"
#include "order.h"
#include "util/memory.h"

point_t *point_new(void) { return try_calloc(sizeof(point_t)); }

point_t *point_copy(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gcopy(src->point);
	if (src->order) dest->order = gcopy(src->order);
	if (src->cofactor) dest->cofactor = gcopy(src->cofactor);
	return dest;
}

point_t *point_new_copy(const point_t *src) {
	point_t *result = point_new();
	return point_copy(src, result);
}

point_t *point_clone(const point_t *src, point_t *dest) {
	if (src->point) dest->point = gclone(src->point);
	if (src->order) dest->order = gclone(src->order);
	if (src->cofactor) dest->cofactor = gclone(src->cofactor);
	return dest;
}

point_t *point_new_clone(const point_t *src) {
	point_t *result = point_new();
	return point_clone(src, result);
}

void point_free(point_t **point) {
	if (*point) {
		if ((*point)->point && isclone((*point)->point)) {
			gunclone((*point)->point);
		}
		if ((*point)->order && isclone((*point)->order)) {
			gunclone((*point)->order);
		}
		if ((*point)->cofactor && isclone((*point)->cofactor)) {
			gunclone((*point)->cofactor);
		}
		pari_free(*point);
		*point = NULL;
	}
}

point_t **points_new(size_t num) { return try_calloc(num * sizeof(point_t *)); }

point_t **points_copy(point_t **const src, point_t **dest, size_t num) {
	for (size_t i = 0; i < num; ++i) {
		dest[i] = point_new_copy(src[i]);
	}
	return dest;
}

point_t **points_new_copy(point_t **const src, size_t num) {
	point_t **result = points_new(num);
	return points_copy(src, result, num);
}

point_t **points_clone(point_t **const src, point_t **dest, size_t num) {
	for (size_t i = 0; i < num; ++i) {
		dest[i] = point_new_clone(src[i]);
	}
	return dest;
}

point_t **points_new_clone(point_t **const src, size_t num) {
	point_t **result = points_new(num);
	return points_clone(src, result, num);
}

void points_free(point_t ***points) {
	if (*points) {
		pari_free(*points);
		*points = NULL;
	}
}

void points_free_deep(point_t ***points, size_t npoints) {
	if (*points) {
		for (size_t i = 0; i < npoints; ++i) {
			point_free(&(*points)[i]);
		}
		points_free(points);
	}
}

GENERATOR(point_gen_random) {
	point_t *p = point_new();
	p->point = genrand(curve->curve);
	p->order = ellorder(curve->curve, p->point, NULL);

	curve->points = points_new(1);
	curve->points[0] = p;
	curve->npoints = 1;
	return 1;
}

GENERATOR(points_gen_random) {
	if (!args) {
		fprintf(stderr, "No args to an arged function. points_gen_random\n");
		return INT_MIN;
	}

	size_t npoints = *(size_t *)args->args;

	curve->points = points_new(npoints);
	curve->npoints = npoints;
	for (size_t i = 0; i < npoints; ++i) {
		point_t *p = point_new();
		p->point = genrand(curve->curve);
		p->order = ellorder(curve->curve, p->point, NULL);
		curve->points[i] = p;
	}
	return 1;
}

/*
    GEN o = utoi(dprimes[i]);
    GEN mul = ellmul(curve->curve, rand, o);

    if (gequal0(mul)) {
        printf("Success! %lu\n", npoints);
        curve->points[i] = point_new();

        gerepileall(btop, 2, &rand, &o);
        curve->points[i]->point = rand;
        curve->points[i]->order = o;
        npoints++;
        break;
    }
 */

GENERATOR(points_gen_trial) {
	// TODO stack code!!!
	if (!args) {
		fprintf(stderr, "No args to an arged function. points_gen_trial\n");
		return INT_MIN;
	}

	pari_ulong *primes = (pari_ulong *)args->args;
	size_t nprimes = args->nargs;

	curve->points = points_new(nprimes);
	curve->npoints = nprimes;

	size_t npoints = 0;
	while (npoints < nprimes) {
		GEN rand = genrand(curve->curve);
		GEN ord = ellorder(curve->curve, rand, NULL);

		for (long i = 0; i < nprimes; ++i) {
			if (curve->points[i] == NULL && dvdis(ord, primes[i])) {
				pari_sp ftop = avma;

				GEN p = stoi(primes[i]);
				GEN mul = divii(ord, p);
				GEN point = ellmul(curve->curve, rand, mul);

				curve->points[i] = point_new();
				gerepileall(ftop, 2, &point, &p);
				curve->points[i]->point = point;
				curve->points[i]->order = p;
				npoints++;
			}
		}
	}

	return 1;
}

GENERATOR(points_gen_prime) {
	// TODO stack code!!!

	GEN primes = order_factors(curve, cfg);
	long nprimes = glength(primes);

	curve->points = points_new((size_t)nprimes);
	curve->npoints = (size_t)nprimes;

	long npoints = 0;
	while (npoints < nprimes) {
		GEN rand = genrand(curve->curve);
		GEN ord = ellorder(curve->curve, rand, NULL);
		// ord(rand) = ord

		for (long i = 1; i <= nprimes; ++i) {
			if (curve->points[i - 1] == NULL && dvdii(ord, gel(primes, i))) {
				pari_sp ftop = avma;

				// primes[i] divides ord
				// mul = ord/primes[i]
				GEN mul = divii(ord, gel(primes, i));
				GEN point = ellmul(curve->curve, rand, mul);

				curve->points[i - 1] = point_new();
				gerepileall(ftop, 1, &point);
				curve->points[i - 1]->point = point;
				curve->points[i - 1]->order = gcopy(gel(primes, i));
				npoints++;
			}
		}
	}

	return 1;
}

GENERATOR(points_gen_allgroups) {
	// TODO stack code!!!

	GEN primes = order_factors(curve, cfg);

	GEN groups = order_groups(curve, cfg, primes);
	long ngroups = glength(groups);

	curve->points = points_new((size_t)ngroups);
	curve->npoints = (size_t)ngroups;

	long npoints = 0;
	while (npoints < ngroups) {
		GEN rand = genrand(curve->curve);
		GEN ord = ellorder(curve->curve, rand, NULL);

		for (long i = 1; i <= ngroups; ++i) {
			pari_sp ftop = avma;
			GEN num = gel(groups, i);

			if (curve->points[i - 1] == NULL) {
				GEN point = NULL;
				if (equalii(ord, num)) {
					point = gcopy(rand);
				} else if (dvdii(ord, num)) {
					GEN mul = divii(ord, num);
					point = ellmul(curve->curve, rand, mul);
				}

				if (point) {
					curve->points[i - 1] = point_new();
					gerepileall(ftop, 1, &point);
					curve->points[i - 1]->point = point;
					curve->points[i - 1]->order = gcopy(num);
					++npoints;
				}
			}
		}
	}

	return 1;
}

UNROLL(points_unroll) {
	if (curve->points) {
		points_free_deep(&curve->points, curve->npoints);
	}
	return -1;
}