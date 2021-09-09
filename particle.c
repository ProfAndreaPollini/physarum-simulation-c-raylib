//
// Created by pollini on 07/09/2021.
//

#include "raylib.h"

#include "particle.h"

void init_particle(particle_t* p_particle,int width, int height)
{
    p_particle->position.x = GetRandomValue(0,width-1);
    p_particle->position.y = height/2;GetRandomValue(0,height-1);
    p_particle->heading = Vector2Normalize(Vector2Rotate(Vector2One(), GetRandomValue(0,359)));
    p_particle->deposit_amount =0.1+GetRandomValue(0,1000)/1000.0;
    p_particle->sensor_distance = 160;//10.0+ 20.0 * GetRandomValue(0,1000)/1000.0;
    p_particle->sensor_angle =30;//GetRandomValue(0,200);
    p_particle->speed = GetRandomValue(1,10);

    Vector2 sensor_displacement_c = Vector2Scale(p_particle->heading,p_particle->sensor_distance);
    p_particle->sensor_pos_c = Vector2Add(p_particle->position,sensor_displacement_c);

    Vector2 heading_l = Vector2Rotate(p_particle->heading,-1.0*p_particle->sensor_angle);
    Vector2 sensor_displacement_l = Vector2Scale(heading_l,p_particle->sensor_distance);
    p_particle->sensor_pos_l = Vector2Add(p_particle->position,sensor_displacement_l);

    Vector2 heading_r = Vector2Rotate(p_particle->heading,1.0*p_particle->sensor_angle);
    Vector2 sensor_displacement_r = Vector2Scale(heading_r,p_particle->sensor_distance);
    p_particle->sensor_pos_r = Vector2Add(p_particle->position,sensor_displacement_r);

    particle_check_bounds(p_particle,width-1,height-1);
}

int coord_to_pos(float x, float y,int bytes_per_stride, int bytes_per_pixel ) {
//    printf("(%d,%d) -> {%d}\n",(int)x,(int)y,(int)x*bytes_per_pixel+(int)y*bytes_per_stride);
    return (int)x*bytes_per_pixel+(int)y*bytes_per_stride;
}

float get_sensor_data_from_image(Vector2 pos,  Image* p_img,int bytes_per_stride, int bytes_per_pixel) {
    return GetPixelColor(p_img->data + coord_to_pos(pos.x,pos.y,bytes_per_stride,bytes_per_pixel) ,p_img->format).r;
}

float particle_sense(particle_t* p_particle, Image* p_img,int bytes_per_stride, int bytes_per_pixel) {
//    printf("-- (%.2f,%.2f) --\n",p_particle->position.x,p_particle->position.y);
    int col_l = get_sensor_data_from_image(p_particle->sensor_pos_l,p_img,bytes_per_stride,bytes_per_pixel);
    int col_c = get_sensor_data_from_image(p_particle->sensor_pos_c,p_img,bytes_per_stride,bytes_per_pixel);
    int col_r = get_sensor_data_from_image(p_particle->sensor_pos_r,p_img,bytes_per_stride,bytes_per_pixel);
//    printf("----\n");
    if (col_c >= col_r && col_c >= col_l) {
        return 0;
    } else if (col_c < col_r && col_c < col_l) {
        return GetRandomValue(-30,30);
    } else if(col_r > col_c && col_c > col_l) {
        return 30;
    } else if(col_l > col_c && col_c > col_r) {
        return -30;
    }
return GetRandomValue(-10,10);
}

void particle_update(particle_t* p_particle, Image* p_img,int bytes_per_stride, int bytes_per_pixel)
{
    float rnd = particle_sense(p_particle,p_img,bytes_per_stride,bytes_per_pixel);

    Vector2 sensor_displacement_c = Vector2Scale(p_particle->heading,p_particle->sensor_distance);
    p_particle->sensor_pos_c = Vector2Add(p_particle->position,sensor_displacement_c);

//    int col_c = GetPixelColor(p_img->data + coord_to_pos(sensor_pos_c.x,sensor_pos_c.y,bytes_per_stride,bytes_per_pixel) ,p_img->format).r;
//
    Vector2 heading_l = Vector2Rotate(p_particle->heading,-1.0*p_particle->sensor_angle);
    Vector2 sensor_displacement_l = Vector2Scale(heading_l,p_particle->sensor_distance);
    p_particle->sensor_pos_l = Vector2Add(p_particle->position,sensor_displacement_l);
//    int col_l = GetPixelColor(p_img->data + coord_to_pos(sensor_pos_l.x,sensor_pos_l.y,bytes_per_stride,bytes_per_pixel) ,p_img->format).r;
//
    Vector2 heading_r = Vector2Rotate(p_particle->heading,1.0*p_particle->sensor_angle);
    Vector2 sensor_displacement_r = Vector2Scale(heading_r,p_particle->sensor_distance);
    p_particle->sensor_pos_r = Vector2Add(p_particle->position,sensor_displacement_r);

    p_particle->heading = Vector2Rotate(p_particle->heading,rnd);
    p_particle->old_position = p_particle->position;
    p_particle->position = Vector2Add(p_particle->position, Vector2Scale(p_particle->heading,p_particle->speed));

}

void check_bounds_v(Vector2* p_v,int width, int height) {
    if(p_v->x >= width){
        p_v->x=0;

    }
    if(p_v->y >= height){
        p_v->y=0;

    }
    if(p_v->x <0){
        p_v->x=width;

    }
    if(p_v->y <0 ){
        p_v->y=height;
    }
}

void particle_check_bounds(particle_t*p_particle,int width, int height) {
    if(p_particle->position.x >= width){
        p_particle->position.x=0;
        p_particle->old_position.x=0;
    }
    if(p_particle->position.y >= height){
        p_particle->position.y=0;
        p_particle->old_position.y=0;
    }
    if(p_particle->position.x <0){
        p_particle->position.x=width;
        p_particle->old_position.x=width;
    }
    if(p_particle->position.y <0 ){
        p_particle->position.y=height;
        p_particle->old_position.y=height;
    }
    check_bounds_v(&p_particle->sensor_pos_l,width,height);
    check_bounds_v(&p_particle->sensor_pos_c,width,height);
    check_bounds_v(&p_particle->sensor_pos_r,width,height);
}

void particle_deposit(particle_t* p_particle,RenderTexture2D* texture_trails)
{

    DrawLineV(p_particle->old_position,p_particle->position,WHITE);
//    DrawCircleV(p_particle->position, Vector2Length(Vector2Subtract(p_particle->position,p_particle->old_position)),WHITE);
//DrawPixelV(p_particle->position,WHITE);
}


void particle_show(particle_t* p_particle) {
//    DrawCircleV(p_particle->position,10,DARKGREEN);
//    Vector2 sensor_displacement = Vector2Scale(p_particle->heading,p_particle->sensor_distance);
//    Vector2 sensor_pos = Vector2Add(p_particle->position,sensor_displacement);
//    DrawLineV(p_particle->position, sensor_pos,DARKPURPLE);
//
//    Vector2 heading_l = Vector2Rotate(p_particle->heading,-1.0*p_particle->sensor_angle);
//    Vector2 sensor_displacement_l = Vector2Scale(heading_l,p_particle->sensor_distance);
//    Vector2 sensor_pos_l = Vector2Add(p_particle->position,sensor_displacement_l);
//    DrawCircleV(sensor_pos_l,10,RED);
//
//    Vector2 heading_r = Vector2Rotate(p_particle->heading,1.0*p_particle->sensor_angle);
//    Vector2 sensor_displacement_r = Vector2Scale(heading_r,p_particle->sensor_distance);
//    Vector2 sensor_pos_r = Vector2Add(p_particle->position,sensor_displacement_r);
//    DrawCircleV(sensor_pos_r,10,BLUE);
//
//    DrawCircleV(sensor_pos,10,DARKPURPLE);
}
