#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

typedef struct vector vector;

struct vector {
    double x;
    double y;
    double z;
};

static inline vector vec_add(vector a, vector b) {
    return (vector) { a.x + b.x, a.y + b.y, a.z + b.z };
}

static inline vector vec_sub(vector a, vector b) {
    return (vector) { a.x - b.x, a.y - b.y, a.z - b.z };
}

static inline vector vec_smul(vector v, double s) {
    return (vector) { v.x * s, v.y * s, v.z * s };
}

static inline vector vec_sdiv(vector v, double s) {
    return (vector) { v.x / s, v.y / s, v.z / s };
}

static inline double vec_len(vector v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline vector vec_normalize(vector v) {
    double len = vec_len(v);
    return (vector) { v.x / len, v.y / len, v.z / len };
}

static inline vector vec_with_len(vector dir, double len) {
    double oldlen = vec_len(dir);
    return (vector) { len * dir.x / oldlen, len * dir.y / oldlen, len * dir.z / oldlen };
}

#endif
