#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MEASUREMENT_NOISE 3.0   
#define PROCESS_NOISE 0.01      

typedef struct 
{
    float Q;    
    float P;        
} KalmanFilter;


void KalmanFilter_Init(KalmanFilter* kf, float initial_estimate, float initial_variance) 
{
    kf->Q = initial_estimate;
    kf->P = initial_variance;
}


void KalmanFilter_Update(KalmanFilter* kf, float z_meas) 
{
    float x_pred = kf->Q;   
    float P_pred = kf->P + PROCESS_NOISE; 

    float K = P_pred / (P_pred + MEASUREMENT_NOISE);   
    kf->Q = x_pred + K * (z_meas - x_pred);        
    kf->P = (1 - K) * P_pred;                           
}

int main() {
    FILE* input_file = fopen("C:\\Users\\HS\\Desktop\\Kalman_Filter\\Position_Data(StateEstimate.x_y).txt", "r");
    FILE* output_file = fopen("C:\\Users\\HS\\Desktop\\Kalman_Filter\\Filter_Position_Data(StateEstimate.x_y).txt", "w");

    if (input_file == NULL || output_file == NULL) 
    {
        printf("Cannot open the file.\n");
        return 1;
    }

    KalmanFilter kf;
    KalmanFilter_Init(&kf, 2.0826842785, 1.0);

    float measurement;
    while (fscanf(input_file, "%f", &measurement) == 1) 
    {

        KalmanFilter_Update(&kf, measurement);

        fprintf(output_file, "%f\n", kf.Q);
    }

    fclose(input_file);
    fclose(output_file);

    printf("Filtering has been successfully completed.\n");

    return 0;
}
