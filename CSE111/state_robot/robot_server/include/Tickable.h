#pragma once
#include <cstdint>
#include <string>  

#include "Message.h"
#include "RobotState.h"

// Code was simple enough that I didn't bother making a .c file
class Tickable{
    uint64_t last_tick_time = 0;

    public:
        virtual void tick(const small_world::SM_Event &event){
            last_tick_time = std::stoul(event.event_time());
        }
        virtual uint64_t get_time(){
            return last_tick_time;
        }
};

class StateMachine : public Tickable{
    std::shared_ptr<RobotState> current_state;

    public:
        virtual void tick(const small_world::SM_Event &e){
            Tickable::tick(e);
            if(current_state != nullptr){
                current_state = current_state->tick(e);
            }
        }

        virtual void set_current_state(std::shared_ptr<RobotState> s){
            current_state = s;
        }
};