#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

struct vector {
    double x;
    double y;
    double z;
};

static inline struct vector* vec_add(struct vector* a, struct vector* b) {
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
    return a;
}

static inline struct vector* vec_sub(struct vector* a, struct vector* b) {
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
    return a;
}

static inline struct vector* vec_smul(struct vector* v, double s) {
    v->x *= s;
    v->y *= s;
    v->z *= s;
    return v;
}

static inline struct vector* vec_sdiv(struct vector* v, double s) {
    v->x /= s;
    v->y /= s;
    v->z /= s;
    return v;
}

static inline double vec_len(struct vector* v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline struct vector* vec_normalize(struct vector* v) {
    double len = vec_len(v);
    return vec_sdiv(v, len);
}

static inline struct vector* vec_set_len(struct vector* dir, double len) {
    double oldlen = vec_len(dir);
    return vec_sdiv(vec_smul(dir, len), oldlen);
}

static inline struct vector* vec_clear(struct vector* v) {
    v->x = v->y = v->z = 0;
    return v;
}

#endif
