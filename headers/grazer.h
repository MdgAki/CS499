/*
Name: grazer.h
Purpose: Header file for grazers.
Last edit: 11-7-2019
Last editor: BP
*/

#ifndef GRAZER_H
#define GRAZER_H

#include <string>
#include "mammal.h"

class grazer : public mammal
{
    private:
        int energy_input;
        double move_count;
        double init_speed;
        double curr_speed;
        time_container current_time;
        time_container eat_timer;
        time_container gain_energy_timer;
        time_container movement_timer;
        time_container speed_timer;
        int maintain_time;
        bool danger;
        bool food_available;
        const int eat_reach = 5;
        const int plant_sight_dist = 150;
        const int pred_sight_dist = 25;


    public:
        grazer(point, int, int, int, int, double, double);
        ~grazer();
        std::string get_type();
        int print_self();
        void act();
        void eat();
        void reset_eat_timer();
        void store_speed();
        void reset_speed();
        void reset_movement_timer();
        void set_maintain_time(int);
        void limit_movement();
        void check_energy();
        void sight_cone();
        void look_check(point, int);
        
};
#endif