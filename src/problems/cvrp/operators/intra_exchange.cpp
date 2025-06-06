/*

This file is part of VROOM.

Copyright (c) 2015-2025, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "problems/cvrp/operators/intra_exchange.h"

namespace vroom::cvrp {

IntraExchange::IntraExchange(const Input& input,
                             const utils::SolutionState& sol_state,
                             RawRoute& s_raw_route,
                             Index s_vehicle,
                             Index s_rank,
                             Index t_rank)
  : Operator(OperatorName::IntraExchange,
             input,
             sol_state,
             s_raw_route,
             s_vehicle,
             s_rank,
             s_raw_route,
             s_vehicle,
             t_rank),
    _moved_jobs(t_rank - s_rank + 1),
    _first_rank(s_rank),
    _last_rank(t_rank + 1),
    _delivery(source.delivery_in_range(_first_rank, _last_rank)) {
  // Assume s_rank < t_rank for symmetry reasons. Set aside cases
  // where t_rank = s_rank + 1, as the move is also an intra_relocate.
  assert(0 < t_rank);
  assert(s_rank < t_rank - 1);
  assert(s_route.size() >= 3);
  assert(t_rank < s_route.size());

  std::copy(s_route.begin() + _first_rank,
            s_route.begin() + _last_rank,
            _moved_jobs.begin());
  std::swap(_moved_jobs[0], _moved_jobs.back());
}

void IntraExchange::compute_gain() {
  const auto& v = _input.vehicles[s_vehicle];

  // Consider the cost of replacing job at rank s_rank with target
  // job. Part of that cost (for adjacent edges) is stored in
  // _sol_state.edge_evals_around_node.
  const Index s_index = _input.jobs[s_route[s_rank]].index();
  const Index t_index = _input.jobs[t_route[t_rank]].index();

  // Determine costs added with target job.
  Eval new_previous_cost;

  if (s_rank == 0) {
    if (v.has_start()) {
      auto p_index = v.start.value().index();
      new_previous_cost = v.eval(p_index, t_index);
    }
  } else {
    auto p_index = _input.jobs[s_route[s_rank - 1]].index();
    new_previous_cost = v.eval(p_index, t_index);
  }

  auto n_index = _input.jobs[s_route[s_rank + 1]].index();
  Eval new_next_cost = v.eval(t_index, n_index);

  const Eval s_gain = _sol_state.edge_evals_around_node[s_vehicle][s_rank] -
                      new_previous_cost - new_next_cost;

  // Consider the cost of replacing job at rank t_rank with source
  // job. Part of that cost (for adjacent edges) is stored in
  // _sol_state.edge_evals_around_node.

  // Determine costs added with source job.
  new_next_cost = Eval();

  auto p_index = _input.jobs[t_route[t_rank - 1]].index();
  new_previous_cost = v.eval(p_index, s_index);

  if (t_rank == t_route.size() - 1) {
    if (v.has_end()) {
      n_index = v.end.value().index();
      new_next_cost = v.eval(s_index, n_index);
    }
  } else {
    n_index = _input.jobs[t_route[t_rank + 1]].index();
    new_next_cost = v.eval(s_index, n_index);
  }

  const Eval t_gain = _sol_state.edge_evals_around_node[s_vehicle][t_rank] -
                      new_previous_cost - new_next_cost;

  stored_gain = s_gain + t_gain;
  gain_computed = true;
}

bool IntraExchange::is_valid() {
  return is_valid_for_range_bounds() &&
         source.is_valid_addition_for_capacity_inclusion(_input,
                                                         _delivery,
                                                         _moved_jobs.begin(),
                                                         _moved_jobs.end(),
                                                         _first_rank,
                                                         _last_rank);
}

void IntraExchange::apply() {
  std::swap(s_route[s_rank], t_route[t_rank]);

  source.update_amounts(_input);
}

std::vector<Index> IntraExchange::addition_candidates() const {
  return {};
}

std::vector<Index> IntraExchange::update_candidates() const {
  return {s_vehicle};
}

} // namespace vroom::cvrp
