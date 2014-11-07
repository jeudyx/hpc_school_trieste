#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define G  6.67E-11
#define SUN_MASS 1.9891E30
#define EARTH_MASS 5.972E24
#define N_ITER 1000000


double norm(double vector[3]){
        return sqrt((vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2]));
}

double* gravitational_acceleration(double mass_j, double r1[3], double r2[3], double result[3]){
    double c1;
    double r_diff[3];
    double norm_diff;

    r_diff[0] = r1[0] - r2[0];
    r_diff[1] = r1[1] - r2[1];
    r_diff[2] = r1[2] - r2[2];
    norm_diff = norm(r_diff);

    result[0] = -1.0 * G * mass_j * r_diff[0] / (norm_diff * norm_diff * norm_diff);
    result[1] = -1.0 * G * mass_j * r_diff[1] / (norm_diff * norm_diff * norm_diff);
    result[2] = -1.0 * G * mass_j * r_diff[2] / (norm_diff * norm_diff * norm_diff);
    return result;
}

int main(int argc, char* argv[]){
    int i;

    double r1[3] = {0., 0., 0.};
    double r2[3] = {1., 1., 1.};
    double result[3] = {0., 0., 0.};
    double sum[3] = {0., 0., 0.};


    for(i = 0; i < N_ITER; i++){
        gravitational_acceleration(SUN_MASS, r1, r2, result);
        sum[0] += result[0];
        sum[1] += result[1];
        sum[2] += result[2];
    }

    printf("Resultado: %E - %E - %E\n", sum[0], sum[1], sum[2]);
}

