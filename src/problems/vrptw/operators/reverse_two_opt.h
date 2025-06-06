#ifndef VRPTW_REVERSE_TWO_OPT_H
#define VRPTW_REVERSE_TWO_OPT_H

/*

This file is part of VROOM.

Copyright (c) 2015-2025, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "problems/cvrp/operators/reverse_two_opt.h"

namespace vroom::vrptw {

class ReverseTwoOpt : public cvrp::ReverseTwoOpt {
private:
  TWRoute& _tw_s_route;
  TWRoute& _tw_t_route;

public:
  ReverseTwoOpt(const Input& input,
                const utils::SolutionState& sol_state,
                TWRoute& tw_s_route,
                Index s_vehicle,
                Index s_rank,
                TWRoute& tw_t_route,
                Index t_vehicle,
                Index t_rank);

  bool is_valid() override;

  void apply() override;
};

} // namespace vroom::vrptw

#endif
