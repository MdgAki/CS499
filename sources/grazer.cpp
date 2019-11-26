/*
Name: grazer.cpp
Purpose: Grazer's class that defines their EU consumption and reproduction information.
Last edit: 11-18-2019
Last editor: BP
*/
#include "grazer.h"

grazer::grazer(point init_loc, int init_e, int e_input, int e_output, int e_reprod_min, double m_spd, double maintain_spd) :
    energy_input(e_input),
    mammal(init_loc, init_e, e_output, e_reprod_min, m_spd, maintain_spd)
{
    this->retained_movement_time    = false;
    this->retained_eat_time         = false;
    this->retained_gain_energy_time = false;
}

grazer::~grazer()
{

}

std::string grazer::get_type() 
{
    return "grazer";
}

/*
Name: print_self()
Purpose: everytime it is called, it returns to environment_object this symbol.
Trace: Traces to Epic 2, acceptance criteria 2
Parameters: N/A
Returns: string with at symbol
*/
int grazer::print_self()
{
    std::cout << "@";
    return 0;
}

/*Name: eat()
Purpose: gain energy per minute. Amount gained is determined by energy input
Trace: Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
BP 11/18/19
*/
void grazer::eat()
{
    /*
    if(!retained_eat_time)
            {
                //figure out the rate & stuff
                //message.get_future_time(0,10);
            }
            if(!retained_gain_energy_time)
            {
                message.get_future_time(0,1);
                gain_energy_time = message.get_time_info();
                retained_gain_energy_time = true;
            }*/
     if(current_time == eat_time)
     {
        reset_eat_time();
        //move: this is based on eating all plants in 5du not the grazers sight
     }
    
    if(current_time == gain_energy_time)
    {
        this->energy += energy_input;
        reset_gain_energy_time();
        //call for deletion of leaf
    }
}

grazer::direction grazer::invert_dir()
{
    switch(dir)
    {
        case up:
            return down;
            break;
        case down:
            return up;
            break;
        case left:
            return right;
            break;
        case right:
            return left;
            break;
        case up_left:
            return down_right;
            break;
        case up_right:
            return down_left;
            break;
        case down_left:
            return up_right;
            break;
        case down_right:
            return up_left;
            break;
    }
    return up;
}

void grazer::act()
{
    map<point, string> things_in_sight = sight(150);
    //return;   
    point danger(-1, -1);
    point food(-1, -1);

    point left(-1, -1);
    point right(-1, -1);
    point middle(-1, -1);

    for (auto const& x : things_in_sight)
    {
        /*direction obj_dir = find_direction(x.first);
        if(x.second == "boulder")
        {
            if(location.x_loc == x.first.x_loc)
            {
                if(obj_dir == left)
                {
                    //ignore x,y's past boulder's x,y
                }
            }
        }*/
        if(x.second == "predator" && x.first.distance(x.first, location) <= 25)
        {
            danger = x.first;
            break;
        }
        if(x.second == "plant" && food.x_loc == -1)
        {
            food = x.first;
        }
    }

    sim_message& message = sim_message::get_instance();
    message.get_current_time();
    current_time = message.get_time_info();

    if(danger.x_loc != -1)
    {
        retained_eat_time = false;
        retained_gain_energy_time = false;
        if(!retained_danger_time)
        {
            retained_danger_time = true;
            //need to look at this amount of time
            message.get_future_time(0, 7, 0);
            danger_time = message.get_time_info();
        }
        dir = invert_dir();
    }
    else if(retained_danger_time)
    {
        if(danger_time == current_time)
        {
            //stop & look behind
            dir = invert_dir();
            retained_movement_time = false;
            retained_danger_time = false;
            curr_speed = init_speed;
            return;
        }
    }
    else if(food.x_loc != -1)
    {
        if(food.distance(food, location) <= 5)
        {
            retained_movement_time = false;
            curr_speed = init_speed;
            eat();
            return;
        }
        else
        {
            dir = find_direction(food);
        }
    }
    
    if(!retained_movement_time)
    {
        message.get_future_time(0, maintain_speed, 0);
        movement_time = message.get_time_info();
        retained_movement_time = true;
    }
    else
    {
        if(current_time == movement_time)
        {
            current_speed *= .75;
        }
    }

    //move this between running from preds & before eating
    if(ready_to_reproduce())
    {
        sim_message& message = sim_message::get_instance();
        if(message.request_reproduce(location, this))
        {
            energy /= 2;
        }
    }
    //loop for move rate
    if(move() && energy < 25)
    {
        move_count++;
        if(move_count >= 10)
        {
            //essage.die(this);
        }
    }
    //make sure this is in loop ^
    if(energy <= 0)
    {
        //message.die(this);
    }
}


/*
Name: check_energy()
Purpose: Add function that ensures after the grazer's energy unit drops below the defined level, 
therefore the grazer cannot move more than 10 Distance Unit.
Trace: Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
Returns: N/A
*/
void grazer::check_energy()
{
    if(this->energy < 25)
    {
        move_count++;
    }
    else
    {
        move_count = 0;
    }

    if(move_count > 10)
    {   
        sim_message& die_message = sim_message::get_instance();
        die_message.die(this);   
    }
}

/*Name: reset_eat_timer()
Purpose: reset eat_timer member variable to zero
Parameters: N/A
Traces to Epic 3, Acceptance Criteria 2
BP 11/18/19
*/
void grazer::reset_eat_time()
{
    eat_time = {0,0,0};
    retained_eat_time = false;
}

/*Name: reset_movement_time()
Purpose: resets time_container movement_time min, secs, & hours = 0 
Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
BP 11/18/19
*/
void grazer::reset_movement_time()
{
    movement_time = {0,0,0};
    retained_movement_time = false;
}

/*Name: reset_gain_energy_time()
Purpose: resets time_container gain_energy_time min, secs, & hours = 0 
Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
BP 11/18/19
*/
void grazer::reset_gain_energy_time()
{
    gain_energy_time = {0,0,0};
    retained_gain_energy_time = false;
}

/*Name: reset_movement_time()
Purpose: resets time_container movement_time min, secs, & hours = 0 
Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
BP 11/18/19
*/
void grazer::start_movement_time()
{
    sim_message& message = sim_message::get_instance();
    message.get_future_time(0, maintain_speed);
    movement_time = message.get_time_info();
    retained_movement_time = true;
}

/*Name: start_eat_time()
Purpose: Sets eat_time to a future time_container 10 minutes from current_time.
void grazer::reset_gain_energy_time()
{
    gain_energy_time = {0,0,0};
    retained_gain_energy_time = false;
}

/*Name: start_movement_time()
Purpose: Sets movement_time to a future time_container. The future time created uses
            the data file defined maintain_speed
Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
BP 11/18/19
*/
void grazer::start_eat_time()
{
    sim_message& message = sim_message::get_instance();
    message.get_future_time(0,this->maintain_speed);
    message.process_message();
    movement_time = message.get_time_info();
    retained_movement_time = true;
}

/*Name: start_energy_time()
Purpose: Sets energy_time to a future time_container 1 minute from current_time.
Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
BP 11/18/19
*/
void grazer::start_gain_energy_time()
{
    sim_message& message = sim_message::get_instance();
    message.get_future_time(0,1);
    message.process_message();
    gain_energy_time = message.get_time_info();
    retained_gain_energy_time = true;
}

/*
Name: sight_on_predator()
Purpose: Add aspect of Grazer's class that the grazer can see predators within 25 DU.
Trace: Traces to Epic 3, Acceptance Criteria 2
Parameters: N/A
Returns: N/A
*/

void grazer::sight_cone()
{   
    vector<point> points_to_check;
    points_to_check.reserve(22500);
    string predator_check;
    point look_point;
    look_point = location;
    look_point.origin_x_loc = location.x_loc;
    look_point.origin_y_loc = location.y_loc;
    looking_direction = 1;

    int test_count = 0; 
    for(int i = 0; i < plant_sight_dist; i++)
    {
        for(int j = -i; j <= i; j++)
        {
            test_count++;
            //right
            if(this->looking_direction == 2)
            {
                look_point.x_loc += i;
                look_point.y_loc += j;
                //if(look_message.look_at_cell(pr))
                //{
                //    predator_check = look_message.get_simulation_response();
                //    look_check(look_point, i);
                //}
            }

            //left
            if(this->looking_direction == 4)
            {
                look_point.x_loc -= i;
                look_point.y_loc += j;
                //look_check(look_point, i);
            }

            //up
            if(this->looking_direction == 1)
            {
                look_point.x_loc += j;
                look_point.y_loc += i;
                //look_check(look_point, i);
            }

            //down
            if (this->looking_direction == 3)
            {
                look_point.x_loc -= i;
                look_point.y_loc -= j;
                //look_check(look_point, i);
            }
            points_to_check.push_back(look_point);
        }
    }
    sim_message& look_message = sim_message::get_instance();
    look_message.look_at_cell(location, points_to_check);
}

void grazer::reset_speed()
{
    
}