#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define G  6.67E-11
#define SUN_MASS 1.9891E30
#define EARTH_MASS 5.972E24
#define N_ITER 1000000

double* gravitational_acceleration(double mass1, double mass2, double r_diff[3], double r_diff_scalar_inv, double result[3]){
    double c1 = G * mass1 * mass2 * pow(r_diff_scalar_inv,3);
    result[0] = c1 * r_diff[0];
    result[1] = c1 * r_diff[1];
    result[2] = c1 * r_diff[2];
    return result;
}

double norm(double vector[3]){
        return sqrt((vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2]));
}

int main(int argc, char* argv[]){
    int i;

    double r1[3] = {0., 0., 0.};
    double r2[3] = {1., 1., 0.};
    double diff[3];
    double diff_scalar_inv;
    double result[3] = {0., 0., 0.};
    double sum[3] = {0., 0., 0.};

    diff[0] = r1[0] - r2[0];
    diff[1] = r1[1] - r2[1];
    diff[2] = r1[2] - r2[2];

    diff_scalar_inv = 1. / (norm(diff));

    for(i = 0; i < N_ITER; i++){
        gravitational_acceleration(SUN_MASS, EARTH_MASS, diff, diff_scalar_inv, result);
        sum[0] += result[0];
        sum[1] += result[1];
        sum[2] += result[2];
    }

    printf("Resultado: %E - %E - %E\n", sum[0], sum[1], sum[2]);
}

