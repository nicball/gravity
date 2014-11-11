#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

struct vector {
    double x;
    double y;
    double z;
};

static inline struct vector* vec_add(struct vector* a, const struct vector* b) {
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
    return a;
}

static inline struct vector* vec_sub(struct vector* a, const struct vector* b) {
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

static inline double vec_dot(const struct vector* a, const struct vector* b) {
    return a->x*b->x + a->y*b->y + a->z*b->z;
}

static inline double vec_len(const struct vector* v) {
    return sqrt(vec_dot(v, v));
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

static inline struct vector* vec_rotate_x(struct vector* v, double roll) {
    double r = sqrt(v->y*v->y + v->z*v->z);
    if (r == 0) return v;
    double old = asin(v->y / r);
    v->y = sin(old-roll) * r;
    v->z = cos(old-roll) * r;
    return v;
}

static inline struct vector* vec_rotate_y(struct vector* v, double pitch) {
    double r = sqrt(v->z*v->z + v->x*v->x);
    if (r == 0) return v;
    double old = asin(v->z / r);
    v->z = sin(old-pitch) * r;
    v->x = cos(old-pitch) * r;
    return v;
}

static inline struct vector* vec_rotate_z(struct vector* v, double yaw) {
    double r = sqrt(v->y*v->y + v->x*v->x);
    if (r == 0) return v;
    double old = asin(v->y / r);
    v->y = sin(old-yaw) * r;
    v->x = cos(old-yaw) * r;
    return v;
}

#endif
