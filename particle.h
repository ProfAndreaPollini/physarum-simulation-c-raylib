//
// Created by pollini on 07/09/2021.
//

#ifndef C_PHYSARUM_PARTICLE_H
#define C_PHYSARUM_PARTICLE_H

#include "raymath.h"

typedef struct particle {
    Vector2 position;
    Vector2 old_position;
    Vector2 heading;
    float sensor_distance;
    double sensor_angle;
    float speed;
    float deposit_amount;

    // sensors
    Vector2 sensor_pos_l;
    Vector2 sensor_pos_c;
    Vector2 sensor_pos_r;

} particle_t;

void init_particle(particle_t* p_particle,int width, int height);
float particle_sense(particle_t* p_particle, Image* p_img,int bytes_per_stride, int bytes_per_pixel);
void particle_update(particle_t* p_particle, Image* p_img,int bytes_perstride, int bytes_per_pixel);
void particle_deposit(particle_t* p_particle,RenderTexture2D* texture_trails);
void particle_check_bounds(particle_t*p_particle,int width, int height);
void particle_show(particle_t* p_particle);

#endif //C_PHYSARUM_PARTICLE_H
