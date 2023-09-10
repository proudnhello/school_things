#pragma once
#include <cstdint>
#include <string>  

#include "Message.h"

class RobotState{
    uint64_t init_time = 0;
    uint64_t current_time = 0;
    uint64_t time_to_wait = 30000;

    // The strings to be printed when the robot starts that state, is currently in that state, and when the state ends
    std::string start = "";
    std::string current = "";
    std::string end = "";

    std::map<std::string, std::shared_ptr<RobotState>> next_states;

    uint64_t get_elapsed(){
        return current_time-init_time;
    }

    void set_next_state(const std::string &state_name, std::shared_ptr<RobotState> state){
        next_states[state_name] = state;
    }

    std::shared_ptr<RobotState> get_next_state(const std::string &next_name){
        std::map<std::string, std::shared_ptr<RobotState>>::iterator it = next_states.find(next_name);
        if (it == next_states.end()) return nullptr;
        return it->second;
    }

    virtual std::shared_ptr<RobotState> decide_action(uint64_t elapsed);

    public:
        virtual std::shared_ptr<RobotState> tick(const small_world::SM_Event &e){
            if(init_time == 0){
                std::shared_ptr<RobotState> temp = decide_action(get_elapsed());
                temp->init_time = std::stoul(e.event_time());
                temp->current_time = std::stoul(e.event_time());
                return temp;
            }
            current_time = std::stoul(e.event_time());
            return decide_action(get_elapsed());
        }
        void set_time_to_wait(uint64_t x){time_to_wait = x;}
        void set_start(std::string n){start = n;}
        void set_current(std::string n){current = n;}
        void set_end(std::string v){end = v;};
        void add_next_state(std::string n, std::shared_ptr<RobotState> s){
            next_states[n] = s;
        }
};