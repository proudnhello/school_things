#include "RobotState.h"

std::shared_ptr<RobotState> RobotState::decide_action(uint64_t elapsed){
    if(init_time == 0){
        std::cout << start << std::endl;
        return std::make_shared<RobotState>(*this);
    }

    if(elapsed < time_to_wait){
        std::cout << current << std::endl;
        return std::make_shared<RobotState>(*this);
    }

    std::shared_ptr<RobotState> next = get_next_state("done");
    if(next == nullptr){
        std::cout << "No finished state to reach!" << std::endl;
        return nullptr;
    }
    std::cout << end << std::endl;
    // Reset the timer for the next thing to happen
    next->init_time = 0;
    return next;
}
