// Copyright 2020 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rmw_zenoh_cpp/identifier.hpp"
#include "rcutils/logging_macros.h"
#include "rmw/error_handling.h"
#include "rmw/get_topic_names_and_types.h"
#include "rmw/rmw.h"
#include "rmw/names_and_types.h"
#include "rmw/topic_endpoint_info_array.h"
#include "rmw/impl/cpp/macros.hpp"

#include "rmw_zenoh_cpp/zenoh-net-interface.h"
#include "rmw_zenoh_cpp/rmw_context_impl.hpp"

extern "C"
{
rmw_ret_t
rmw_get_topic_names_and_types(
  const rmw_node_t * node,
  rcutils_allocator_t * allocator,
  bool no_demangle,
  rmw_names_and_types_t * topic_names_and_types)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(node, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    node,
    node->implementation_identifier,
    eclipse_zenoh_identifier,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);
  RCUTILS_CHECK_ALLOCATOR_WITH_MSG(
    allocator, "allocator argument is invalid", return RMW_RET_INVALID_ARGUMENT);
  if (RMW_RET_OK != rmw_names_and_types_check_zero(topic_names_and_types)) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  char *uri = "/zenoh/ros/node/*/topics";
  zn_reply_data_array_t replies = zn_query_collect(node->context->impl->session, zn_rname(uri), "", zn_query_target_default(), zn_query_consolidation_default());

  for(unsigned int i = 0; i < replies.len; ++i) {
      printf(">> [Reply handler] received (%.*s, %.*s)\n",
            (int)replies.val[i].data.key.len, replies.val[i].data.key.val,
            (int)replies.val[i].data.value.len, replies.val[i].data.value.val);
  }
  zn_reply_data_array_free(replies);
  (void)no_demangle;
  (void)topic_names_and_types;
  RCUTILS_LOG_INFO_NAMED("rmw_zenoh_cpp", "rmw_get_topic_names_and_types");
  return RMW_RET_ERROR;
}
}  // extern "C"
