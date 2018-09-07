// Copyright 2016 Open Source Robotics Foundation, Inc.
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

#include "./default_state_machine.h"  // NOLINT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lifecycle_msgs/msg/state.h"
#include "lifecycle_msgs/msg/transition.h"

#include "rcl/error_handling.h"
#include "rcl/rcl.h"

#include "rcl_lifecycle/transition_map.h"

#ifdef __cplusplus
extern "C"
{
#endif

rcl_ret_t
_register_primary_states(
  rcl_lifecycle_transition_map_t * transition_map, const rcutils_allocator_t * allocator)
{
  rcl_ret_t ret = RCL_RET_ERROR;

  // default values for when registering states
  // all states are registered with no transitions attached
  // the valid transitions per state are filled once transitions are registered
  rcl_lifecycle_transition_key_t * valid_transition_keys = NULL;
  rcl_lifecycle_transition_t * valid_transitions = NULL;
  unsigned int valid_transition_size = 0;

  // register unknown state
  {
    rcl_lifecycle_state_t rcl_state_unknown = {
      "unknown", lifecycle_msgs__msg__State__PRIMARY_STATE_UNKNOWN,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_unknown, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register unconfigured state
  {
    rcl_lifecycle_state_t rcl_state_unconfigured = {
      "unconfigured", lifecycle_msgs__msg__State__PRIMARY_STATE_UNCONFIGURED,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_unconfigured, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register inactive state
  {
    rcl_lifecycle_state_t rcl_state_inactive = {
      "inactive", lifecycle_msgs__msg__State__PRIMARY_STATE_INACTIVE,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_inactive, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register active state
  {
    rcl_lifecycle_state_t rcl_state_active = {
      "active", lifecycle_msgs__msg__State__PRIMARY_STATE_ACTIVE,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_active, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register finalized state
  {
    rcl_lifecycle_state_t rcl_state_finalized = {
      "finalized", lifecycle_msgs__msg__State__PRIMARY_STATE_FINALIZED,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_finalized, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  return ret;
}

rcl_ret_t
_register_transition_states(
  rcl_lifecycle_transition_map_t * transition_map, const rcutils_allocator_t * allocator)
{
  rcl_ret_t ret = RCL_RET_ERROR;

  // default values for when registering states
  // all states are registered with no transitions attached
  // the valid transitions per state are filled once transitions are registered
  rcl_lifecycle_transition_key_t * valid_transition_keys = NULL;
  rcl_lifecycle_transition_t * valid_transitions = NULL;
  unsigned int valid_transition_size = 0;

  // register configuring state
  {
    rcl_lifecycle_state_t rcl_state_configuring = {
      "configuring", lifecycle_msgs__msg__State__TRANSITION_STATE_CONFIGURING,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_configuring, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register cleaningup state
  {
    rcl_lifecycle_state_t rcl_state_cleaningup = {
      "cleaningup", lifecycle_msgs__msg__State__TRANSITION_STATE_CLEANINGUP,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_cleaningup, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register shutting down state
  {
    rcl_lifecycle_state_t rcl_state_shuttingdown = {
      "shuttingdown", lifecycle_msgs__msg__State__TRANSITION_STATE_SHUTTINGDOWN,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_shuttingdown, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register activating state
  {
    rcl_lifecycle_state_t rcl_state_activating = {
      "activating", lifecycle_msgs__msg__State__TRANSITION_STATE_ACTIVATING,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_activating, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register deactivating state
  {
    rcl_lifecycle_state_t rcl_state_deactivating = {
      "deactivating", lifecycle_msgs__msg__State__TRANSITION_STATE_DEACTIVATING,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_deactivating, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register errorprocessing state
  {
    rcl_lifecycle_state_t rcl_state_errorprocessing = {
      "errorprocessing", lifecycle_msgs__msg__State__TRANSITION_STATE_ERRORPROCESSING,
      valid_transition_keys, valid_transitions, valid_transition_size
    };
    ret = rcl_lifecycle_register_state(
      transition_map, rcl_state_errorprocessing, allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  return ret;
}

rcl_ret_t
_register_transitions(
  rcl_lifecycle_transition_map_t * transition_map, const rcutils_allocator_t * allocator)
{
  rcl_ret_t ret = RCL_RET_ERROR;

  // retrieve primary and transition states from the transition map
  // note: These states are pointing to the states inside the map
  // the states created before (see _register_primary_states) were copied into the map
  // and thus have to be retrieved from the map again to not invalidate pointers.

  rcl_lifecycle_state_t * unconfigured_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__PRIMARY_STATE_UNCONFIGURED);
  rcl_lifecycle_state_t * inactive_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__PRIMARY_STATE_INACTIVE);
  rcl_lifecycle_state_t * active_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__PRIMARY_STATE_ACTIVE);
  rcl_lifecycle_state_t * finalized_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__PRIMARY_STATE_FINALIZED);

  rcl_lifecycle_state_t * configuring_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__TRANSITION_STATE_CONFIGURING);
  rcl_lifecycle_state_t * activating_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__TRANSITION_STATE_ACTIVATING);
  rcl_lifecycle_state_t * deactivating_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__TRANSITION_STATE_DEACTIVATING);
  rcl_lifecycle_state_t * cleaningup_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__TRANSITION_STATE_CLEANINGUP);
  rcl_lifecycle_state_t * shuttingdown_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__TRANSITION_STATE_SHUTTINGDOWN);
  rcl_lifecycle_state_t * errorprocessing_state = rcl_lifecycle_get_state(
    transition_map, lifecycle_msgs__msg__State__TRANSITION_STATE_ERRORPROCESSING);

  // register transition from unconfigured to configuring
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CONFIGURE,
      "configure"
    };
    rcl_lifecycle_transition_t rcl_transition_configure = {
      "configure_to_configuring",
      lifecycle_msgs__msg__Transition__TRANSITION_CONFIGURE,
      unconfigured_state, configuring_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_configure,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from configuring to inactive
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_SUCCESS,
      "on_configure_success"
    };
    rcl_lifecycle_transition_t rcl_transition_on_configure_success = {
      "configuring_to_inactive",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_CONFIGURE_SUCCESS,
      configuring_state, inactive_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_configure_success,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from configuring to unconfigured
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_FAILURE,
      "on_configure_failure"
    };
    rcl_lifecycle_transition_t rcl_transition_on_configure_failure = {
      "configuring_to_unconfigured",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_CONFIGURE_FAILURE,
      configuring_state, unconfigured_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_configure_failure,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from configuring to errorprocessing
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_ERROR,
      "on_configure_error"
    };
    rcl_lifecycle_transition_t rcl_transition_on_configure_error = {
      "configuring_to_errorprocessing",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_CONFIGURE_ERROR,
      configuring_state, errorprocessing_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_configure_error,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from inactive to cleaningup
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CLEANUP,
      "cleanup"
    };
    rcl_lifecycle_transition_t rcl_transition_cleanup = {
      "inactive_to_cleaningup",
      lifecycle_msgs__msg__Transition__TRANSITION_CLEANUP,
      inactive_state, cleaningup_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_cleanup,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from cleaningup to unconfigured
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_SUCCESS,
      "on_cleanup_success"
    };
    rcl_lifecycle_transition_t rcl_transition_on_cleanup_success = {
      "cleaningup_to_unconfigured",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_CLEANUP_SUCCESS,
      cleaningup_state, unconfigured_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_cleanup_success,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from cleaningup to inactive
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_FAILURE,
      "on_cleanup_failure"
    };
    rcl_lifecycle_transition_t rcl_transition_on_cleanup_failure = {
      "cleaningup_to_inactive",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_CLEANUP_FAILURE,
      cleaningup_state, inactive_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_cleanup_failure,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from cleaniningup to errorprocessing
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_ERROR,
      "on_cleanup_error"
    };
    rcl_lifecycle_transition_t rcl_transition_on_cleanup_error = {
      "cleaningup_to_errorprocessing",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_CLEANUP_ERROR,
      cleaningup_state, errorprocessing_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_cleanup_error,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from inactive to activating
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_ACTIVATE,
      "activate"
    };
    rcl_lifecycle_transition_t rcl_transition_activate = {
      "inactive_to_activating",
      lifecycle_msgs__msg__Transition__TRANSITION_ACTIVATE,
      inactive_state, activating_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_activate,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from activating to active
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_SUCCESS,
      "on_activate_sucess"
    };
    rcl_lifecycle_transition_t rcl_transition_on_activate_success = {
      "activating_to_active",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_ACTIVATE_SUCCESS,
      activating_state, active_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_activate_success,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from activating to inactive
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_FAILURE,
      "on_activate_failure"
    };
    rcl_lifecycle_transition_t rcl_transition_on_activate_failure = {
      "activating_to_inactive",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_ACTIVATE_FAILURE,
      activating_state, inactive_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_activate_failure,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from activating to errorprocessing
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_ERROR,
      "on_activate_error"
    };
    rcl_lifecycle_transition_t rcl_transition_on_activate_error = {
      "activating_to_errorprocessing",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_ACTIVATE_ERROR,
      activating_state, errorprocessing_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_activate_error,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from active to deactivating
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_DEACTIVATE,
      "deactivate"
    };
    rcl_lifecycle_transition_t rcl_transition_deactivate = {
      "active_to_deactivating",
      lifecycle_msgs__msg__Transition__TRANSITION_DEACTIVATE,
      active_state, deactivating_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_deactivate,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from deactivating to inactive
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_SUCCESS,
      "on_deactivate_success"
    };
    rcl_lifecycle_transition_t rcl_transition_on_deactivate_success = {
      "deactivating_to_inactive",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_DEACTIVATE_SUCCESS,
      deactivating_state, inactive_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_deactivate_success,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from deactivating to active
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_FAILURE,
      "on_deactivate_failure"
    };
    rcl_lifecycle_transition_t rcl_transition_on_deactivate_failure = {
      "deactivating_to_active",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_DEACTIVATE_FAILURE,
      deactivating_state, active_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_deactivate_failure,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from deactivating to errorprocessing
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_ERROR,
      "on_deactivate_error"
    };
    rcl_lifecycle_transition_t rcl_transition_on_deactivate_error = {
      "deactivating_to_errorprocessing",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_DEACTIVATE_ERROR,
      deactivating_state, errorprocessing_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_deactivate_error,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from unconfigured to shuttingdown
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_SHUTDOWN,
      "shutdown"
    };
    rcl_lifecycle_transition_t rcl_transition_unconfigured_shutdown = {
      "unconfigured_to_shuttingdown",
      lifecycle_msgs__msg__Transition__TRANSITION_UNCONFIGURED_SHUTDOWN,
      unconfigured_state, shuttingdown_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_unconfigured_shutdown,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from inactive to shuttingdown
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_SHUTDOWN,
      "shutdown"
    };
    rcl_lifecycle_transition_t rcl_transition_inactive_shutdown = {
      "inactive_to_shuttingdown",
      lifecycle_msgs__msg__Transition__TRANSITION_INACTIVE_SHUTDOWN,
      inactive_state, shuttingdown_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_inactive_shutdown,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from active to shuttingdown
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_SHUTDOWN,
      "shutdown"
    };
    rcl_lifecycle_transition_t rcl_transition_active_shutdown = {
      "active_to_shuttingdown",
      lifecycle_msgs__msg__Transition__TRANSITION_ACTIVE_SHUTDOWN,
      active_state, shuttingdown_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_active_shutdown,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from shutting down to finalized
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_SUCCESS,
      "on_shutdown_success"
    };
    rcl_lifecycle_transition_t rcl_transition_on_shutdown_success = {
      "shuttingdown_to_finalized",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_SHUTDOWN_SUCCESS,
      shuttingdown_state, finalized_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_shutdown_success,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from shutting down to finalized
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_FAILURE,
      "on_shutdown_failure"
    };
    rcl_lifecycle_transition_t rcl_transition_on_shutdown_failure = {
      "shuttingdown_to_finalized",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_SHUTDOWN_FAILURE,
      shuttingdown_state, finalized_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_shutdown_failure,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from shutting down to errorprocessing
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_ERROR,
      "on_shutdown_error"
    };
    rcl_lifecycle_transition_t rcl_transition_on_shutdown_error = {
      "shuttingdown_to_errorprocessing",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_SHUTDOWN_ERROR,
      shuttingdown_state, errorprocessing_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_shutdown_error,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from errorprocessing to uncofigured
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_SUCCESS,
      "on_error_success"
    };
    rcl_lifecycle_transition_t rcl_transition_on_error_success = {
      "errorprocessing_to_unconfigured",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_ERROR_SUCCESS,
      errorprocessing_state, unconfigured_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_error_success,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from errorprocessing to finalized
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_FAILURE,
      "on_error_failure"
    };
    rcl_lifecycle_transition_t rcl_transition_on_error_failure = {
      "errorprocessing_to_finalized_on_failure",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_ERROR_FAILURE,
      errorprocessing_state, finalized_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_error_failure,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  // register transition from errorprocessing to finalized
  {
    rcl_lifecycle_transition_key_t key = {
      lifecycle_msgs__msg__Transition__TRANSITION_CALLBACK_ERROR,
      "on_error_error"
    };
    rcl_lifecycle_transition_t rcl_transition_on_error_error = {
      "errorprocessing_to_finalized_on_error",
      lifecycle_msgs__msg__Transition__TRANSITION_ON_ERROR_ERROR,
      errorprocessing_state, finalized_state
    };
    ret = rcl_lifecycle_register_transition(
      transition_map,
      rcl_transition_on_error_error,
      key,
      allocator);
    if (ret != RCL_RET_OK) {
      return ret;
    }
  }

  return ret;
}

// default implementation as despicted on
// design.ros2.org
rcl_ret_t
rcl_lifecycle_init_default_state_machine(
  rcl_lifecycle_state_machine_t * state_machine, const rcutils_allocator_t * allocator)
{
  rcl_ret_t ret = RCL_RET_ERROR;

  // ***************************
  // register all primary states
  // ***************************
  ret = _register_primary_states(&state_machine->transition_map, allocator);
  if (ret != RCL_RET_OK) {
    goto fail;
  }

  // ******************************
  // register all transition states
  // ******************************
  ret = _register_transition_states(&state_machine->transition_map, allocator);
  if (ret != RCL_RET_OK) {
    goto fail;
  }

  // ************************
  // register all transitions
  // ************************
  ret = _register_transitions(&state_machine->transition_map, allocator);
  if (ret != RCL_RET_OK) {
    goto fail;
  }

  // *************************************
  // set the initial state to unconfigured
  // *************************************
  state_machine->current_state = rcl_lifecycle_get_state(
    &state_machine->transition_map, lifecycle_msgs__msg__State__PRIMARY_STATE_UNCONFIGURED);

  return ret;

fail:
  if (rcl_lifecycle_transition_map_fini(&state_machine->transition_map, allocator) != RCL_RET_OK) {
    RCL_SET_ERROR_MSG("could not free lifecycle transition map. Leaking memory!\n",
      rcl_get_default_allocator());
  }
  return RCL_RET_ERROR;
}

#ifdef __cplusplus
}
#endif  // extern "C"
