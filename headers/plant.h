
/*
Name: plant.h

Purpose: Header file for plants.
Last edit: 11-19-2019
Last editor: MG
*/

#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <iostream>
#include <vector>
#include "environment_object.h"
#include <stdlib.h>
#include "clock.h"
#include "sim_message.h"
#include "seed.h"
#include "leaf.h"

class plant : public environment_object
{
    private:
        double growth_rate;
        int max_size;
        int max_seed_cast_dist;
        int max_seed_num;
        int initial_plant_size;
        double seed_viability;
        int current_size;
        vector<leaf> list_of_leaves;
    public:
        plant(point, double, int, int, int, double, int);
        ~plant();
        std::string get_type();
        int print_self();
        void grow();
        void set_seed_pod_values();
        void radially_disperse_seed();
        void act();
};

#endif
