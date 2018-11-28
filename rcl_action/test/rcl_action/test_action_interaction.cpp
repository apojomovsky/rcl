// Copyright 2018 Open Source Robotics Foundation, Inc.
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
#include <gtest/gtest.h>

#include "rcl_action/action_client.h"
#include "rcl_action/action_server.h"
#include "rcl_action/wait.h"

#include "rcl/error_handling.h"
#include "rcl/rcl.h"

#include "rosidl_generator_c/primitives_sequence_functions.h"

#include "test_msgs/action/fibonacci.h"

#ifdef RMW_IMPLEMENTATION
# define CLASSNAME_(NAME, SUFFIX) NAME ## __ ## SUFFIX
# define CLASSNAME(NAME, SUFFIX) CLASSNAME_(NAME, SUFFIX)
#else
# define CLASSNAME(NAME, SUFFIX) NAME
#endif

class CLASSNAME (TestActionClientServerInteraction, RMW_IMPLEMENTATION) : public ::testing::Test
{
protected:
  void SetUp() override
  {
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rcl_ret_t ret = rcl_init(0, nullptr, allocator);
    ASSERT_EQ(RCL_RET_OK, ret) << rcl_get_error_string().str;
    this->node = rcl_get_zero_initialized_node();
    rcl_node_options_t node_options = rcl_node_get_default_options();
    ret = rcl_node_init(&this->node, "test_action_communication_node", "", &node_options);
    ASSERT_EQ(RCL_RET_OK, ret) << rcl_get_error_string().str;
    ret = rcl_clock_init(RCL_STEADY_TIME, &this->clock, &allocator);
    const rosidl_action_type_support_t * ts = ROSIDL_GET_ACTION_TYPE_SUPPORT(
      test_msgs, Fibonacci);
    const char * action_name = "test_action_commmunication_name";
    const rcl_action_server_options_t server_options = rcl_action_server_get_default_options();
    this->action_server = rcl_action_get_zero_initialized_server();
    ret = rcl_action_server_init(
      &this->action_server, &this->node, &this->clock, ts, action_name, &server_options);
    ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    const rcl_action_client_options_t client_options = rcl_action_client_get_default_options();
    this->action_client = rcl_action_get_zero_initialized_client();
    ret = rcl_action_client_init(
      &this->action_client, &this->node, ts, action_name, &client_options);
    ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;

    size_t num_subscriptions_server;
    size_t num_guard_conditions_server;
    size_t num_timers_server;
    size_t num_clients_server;
    size_t num_services_server;
    size_t num_subscriptions_client;
    size_t num_guard_conditions_client;
    size_t num_timers_client;
    size_t num_clients_client;
    size_t num_services_client;

    this->wait_set = rcl_get_zero_initialized_wait_set();
    ret = rcl_action_server_wait_set_get_num_entities(
      &this->action_server,
      &num_subscriptions_server,
      &num_guard_conditions_server,
      &num_timers_server,
      &num_clients_server,
      &num_services_server);
    ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    ret = rcl_action_client_wait_set_get_num_entities(
      &this->action_client,
      &num_subscriptions_client,
      &num_guard_conditions_client,
      &num_timers_client,
      &num_clients_client,
      &num_services_client);
    ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    ret = rcl_wait_set_init(
      &this->wait_set,
      num_subscriptions_server + num_subscriptions_client,
      num_guard_conditions_server + num_guard_conditions_client,
      num_timers_server + num_timers_client,
      num_clients_server + num_clients_client,
      num_services_server + num_services_client,
      rcl_get_default_allocator());
    ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  }

  void TearDown() override
  {
    // Finalize
    rcl_ret_t ret = rcl_action_server_fini(&this->action_server, &this->node);
    EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    ret = rcl_clock_fini(&this->clock);
    EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    ret = rcl_action_client_fini(&this->action_client, &this->node);
    EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    ret = rcl_node_fini(&this->node);
    EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    ret = rcl_shutdown();
    EXPECT_EQ(RCL_RET_OK, ret) << rcl_get_error_string().str;
    ret = rcl_wait_set_fini(&this->wait_set);
    EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
    rcl_reset_error();
  }

  void init_test_uuid0(uint8_t * uuid)
  {
    for (uint8_t i = 0; i < UUID_SIZE; ++i) {
      uuid[i] = i;
    }
  }

  void init_test_uuid1(uint8_t * uuid)
  {
    for (uint8_t i = 0; i < UUID_SIZE; ++i) {
      uuid[i] = 15 - i;
    }
  }

  rcl_action_client_t action_client;
  rcl_action_server_t action_server;
  rcl_node_t node;
  rcl_clock_t clock;

  rcl_wait_set_t wait_set;

  bool is_goal_request_ready;
  bool is_cancel_request_ready;
  bool is_result_request_ready;

  bool is_feedback_ready;
  bool is_status_ready;
  bool is_goal_response_ready;
  bool is_cancel_response_ready;
  bool is_result_response_ready;
};  // class TestActionClientServerInteraction

TEST_F(CLASSNAME(TestActionClientServerInteraction, RMW_IMPLEMENTATION), test_interaction)
{
  test_msgs__action__Fibonacci_Goal_Request outgoing_goal_request;
  test_msgs__action__Fibonacci_Goal_Request incoming_goal_request;
  test_msgs__action__Fibonacci_Goal_Response outgoing_goal_response;
  test_msgs__action__Fibonacci_Goal_Response incoming_goal_response;
  test_msgs__action__Fibonacci_Result_Request outgoing_result_request;
  test_msgs__action__Fibonacci_Result_Request incoming_result_request;
  test_msgs__action__Fibonacci_Result_Response outgoing_result_response;
  test_msgs__action__Fibonacci_Result_Response incoming_result_response;
  test_msgs__action__Fibonacci_Feedback outgoing_feedback;
  test_msgs__action__Fibonacci_Feedback incoming_feedback;
  test_msgs__action__Fibonacci_Goal_Request__init(&outgoing_goal_request);
  test_msgs__action__Fibonacci_Goal_Request__init(&incoming_goal_request);
  test_msgs__action__Fibonacci_Goal_Response__init(&outgoing_goal_response);
  test_msgs__action__Fibonacci_Goal_Response__init(&incoming_goal_response);
  test_msgs__action__Fibonacci_Result_Request__init(&outgoing_result_request);
  test_msgs__action__Fibonacci_Result_Request__init(&incoming_result_request);
  test_msgs__action__Fibonacci_Result_Response__init(&outgoing_result_response);
  test_msgs__action__Fibonacci_Result_Response__init(&incoming_result_response);
  test_msgs__action__Fibonacci_Feedback__init(&outgoing_feedback);
  test_msgs__action__Fibonacci_Feedback__init(&incoming_feedback);

  // Initialize goal request
  init_test_uuid0(outgoing_goal_request.uuid);
  outgoing_goal_request.order = 10;

  // Send goal request with valid arguments
  int64_t sequence_number;
  rcl_ret_t ret = rcl_action_send_goal_request(
    &this->action_client, &outgoing_goal_request, &sequence_number);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_server(&this->wait_set, &this->action_server, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for goal request to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_server_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_server,
    &this->is_goal_request_ready,
    &this->is_cancel_request_ready,
    &this->is_result_request_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_goal_request_ready) << rcl_get_error_string().str;
  EXPECT_FALSE(this->is_cancel_request_ready) << rcl_get_error_string().str;
  EXPECT_FALSE(this->is_result_request_ready) << rcl_get_error_string().str;

  // Take goal request with valid arguments
  rmw_request_id_t request_header;
  ret = rcl_action_take_goal_request(&this->action_server, &request_header, &incoming_goal_request);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the goal request was received correctly
  EXPECT_EQ(outgoing_goal_request.order, incoming_goal_request.order);
  rcl_reset_error();
  EXPECT_TRUE(uuidcmp(outgoing_goal_request.uuid, incoming_goal_request.uuid));
  rcl_reset_error();

  // Initialize goal response
  outgoing_goal_response.accepted = true;
  outgoing_goal_response.stamp.sec = 123;
  outgoing_goal_response.stamp.nanosec = 456789u;

  // Send goal response with valid arguments
  ret = rcl_action_send_goal_response(
    &this->action_server, &request_header, &outgoing_goal_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_client(
    &this->wait_set, &this->action_client, NULL, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for goal response to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_client_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_client,
    &this->is_feedback_ready,
    &this->is_status_ready,
    &this->is_goal_response_ready,
    &this->is_cancel_response_ready,
    &this->is_result_response_ready);

  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_goal_response_ready);
  EXPECT_FALSE(this->is_cancel_response_ready);
  EXPECT_FALSE(this->is_feedback_ready);
  EXPECT_FALSE(this->is_status_ready);
  EXPECT_FALSE(this->is_result_response_ready);

  // Take goal response with valid arguments
  ret = rcl_action_take_goal_response(
    &this->action_client, &request_header, &incoming_goal_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the goal response was received correctly
  EXPECT_EQ(outgoing_goal_response.accepted, incoming_goal_response.accepted);
  rcl_reset_error();
  EXPECT_EQ(outgoing_goal_response.stamp.sec, incoming_goal_response.stamp.sec);
  rcl_reset_error();
  EXPECT_EQ(outgoing_goal_response.stamp.nanosec, incoming_goal_response.stamp.nanosec);
  rcl_reset_error();

  // Initialize result request
  init_test_uuid0(outgoing_result_request.uuid);

  // Send result request with valid arguments
  ret = rcl_action_send_result_request(
    &this->action_client, &outgoing_result_request, &sequence_number);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Initialize feedback
  ASSERT_TRUE(rosidl_generator_c__int32__Sequence__init(
      &outgoing_feedback.sequence, 3));
  outgoing_feedback.sequence.data[0] = 0;
  outgoing_feedback.sequence.data[1] = 1;
  outgoing_feedback.sequence.data[2] = 2;
  init_test_uuid0(outgoing_feedback.uuid);

  // Publish feedback with valid arguments
  ret = rcl_action_publish_feedback(&this->action_server, &outgoing_feedback);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_client(
    &this->wait_set, &this->action_client, NULL, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for feedback to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_client_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_client,
    &this->is_feedback_ready,
    &this->is_status_ready,
    &this->is_goal_response_ready,
    &this->is_cancel_response_ready,
    &this->is_result_response_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_feedback_ready);
  EXPECT_FALSE(this->is_status_ready);
  EXPECT_FALSE(this->is_result_response_ready);
  EXPECT_FALSE(this->is_cancel_response_ready);
  EXPECT_FALSE(this->is_goal_response_ready);

  // Take feedback with valid arguments
  ret = rcl_action_take_feedback(&this->action_client, &incoming_feedback);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that feedback was received correctly
  EXPECT_TRUE(uuidcmp(outgoing_feedback.uuid, incoming_feedback.uuid));
  ASSERT_EQ(outgoing_feedback.sequence.size, incoming_feedback.sequence.size);
  EXPECT_TRUE(!memcmp(
      outgoing_feedback.sequence.data,
      incoming_feedback.sequence.data,
      outgoing_feedback.sequence.size));

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_server(&this->wait_set, &this->action_server, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for result request to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_server_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_server,
    &this->is_goal_request_ready,
    &this->is_cancel_request_ready,
    &this->is_result_request_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_result_request_ready);
  EXPECT_FALSE(this->is_cancel_request_ready);
  EXPECT_FALSE(this->is_goal_request_ready);

  // Take result request with valid arguments
  // rmw_request_id_t request_header;
  ret = rcl_action_take_result_request(
    &this->action_server, &request_header, &incoming_result_request);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the result request was received correctly
  EXPECT_TRUE(uuidcmp(
      outgoing_result_request.uuid,
      incoming_result_request.uuid));

  // Initialize result response
  ASSERT_TRUE(rosidl_generator_c__int32__Sequence__init(
      &outgoing_result_response.sequence, 4));
  outgoing_result_response.sequence.data[0] = 0;
  outgoing_result_response.sequence.data[1] = 1;
  outgoing_result_response.sequence.data[2] = 2;
  outgoing_result_response.sequence.data[3] = 6;
  outgoing_result_response.status =
    action_msgs__msg__GoalStatus__STATUS_SUCCEEDED;

  // Send result response with valid arguments
  ret = rcl_action_send_result_response(
    &this->action_server, &request_header, &outgoing_result_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_client(
    &this->wait_set, &this->action_client, NULL, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for result response to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_client_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_client,
    &this->is_feedback_ready,
    &this->is_status_ready,
    &this->is_goal_response_ready,
    &this->is_cancel_response_ready,
    &this->is_result_response_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_result_response_ready);
  EXPECT_FALSE(this->is_cancel_response_ready);
  EXPECT_FALSE(this->is_feedback_ready);
  EXPECT_FALSE(this->is_status_ready);
  EXPECT_FALSE(this->is_goal_response_ready);

  // Take result response with valid arguments
  ret = rcl_action_take_result_response(
    &this->action_client, &request_header, &incoming_result_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the result response was received correctly
  EXPECT_EQ(outgoing_result_response.status, incoming_result_response.status);
  ASSERT_EQ(outgoing_result_response.sequence.size, incoming_result_response.sequence.size);
  EXPECT_TRUE(!memcmp(
      outgoing_result_response.sequence.data,
      incoming_result_response.sequence.data,
      outgoing_result_response.sequence.size));

  test_msgs__action__Fibonacci_Goal_Request__fini(&outgoing_goal_request);
  test_msgs__action__Fibonacci_Goal_Request__fini(&incoming_goal_request);
  test_msgs__action__Fibonacci_Goal_Response__fini(&incoming_goal_response);
  test_msgs__action__Fibonacci_Goal_Response__fini(&outgoing_goal_response);
  test_msgs__action__Fibonacci_Result_Request__fini(&incoming_result_request);
  test_msgs__action__Fibonacci_Result_Request__fini(&outgoing_result_request);
  test_msgs__action__Fibonacci_Result_Response__fini(&incoming_result_response);
  test_msgs__action__Fibonacci_Result_Response__fini(&outgoing_result_response);
}


TEST_F(
  CLASSNAME(TestActionClientServerInteraction, RMW_IMPLEMENTATION), test_interaction_with_cancel)
{
  test_msgs__action__Fibonacci_Goal_Request outgoing_goal_request;
  test_msgs__action__Fibonacci_Goal_Request incoming_goal_request;
  test_msgs__action__Fibonacci_Goal_Response outgoing_goal_response;
  test_msgs__action__Fibonacci_Goal_Response incoming_goal_response;
  test_msgs__action__Fibonacci_Result_Request outgoing_result_request;
  test_msgs__action__Fibonacci_Result_Request incoming_result_request;
  test_msgs__action__Fibonacci_Result_Response outgoing_result_response;
  test_msgs__action__Fibonacci_Result_Response incoming_result_response;
  test_msgs__action__Fibonacci_Feedback outgoing_feedback;
  test_msgs__action__Fibonacci_Feedback incoming_feedback;
  action_msgs__srv__CancelGoal_Request outgoing_cancel_request;
  action_msgs__srv__CancelGoal_Request incoming_cancel_request;
  action_msgs__srv__CancelGoal_Response outgoing_cancel_response;
  action_msgs__srv__CancelGoal_Response incoming_cancel_response;
  test_msgs__action__Fibonacci_Goal_Request__init(&outgoing_goal_request);
  test_msgs__action__Fibonacci_Goal_Request__init(&incoming_goal_request);
  test_msgs__action__Fibonacci_Goal_Response__init(&outgoing_goal_response);
  test_msgs__action__Fibonacci_Goal_Response__init(&incoming_goal_response);
  test_msgs__action__Fibonacci_Result_Request__init(&outgoing_result_request);
  test_msgs__action__Fibonacci_Result_Request__init(&incoming_result_request);
  test_msgs__action__Fibonacci_Result_Response__init(&outgoing_result_response);
  test_msgs__action__Fibonacci_Result_Response__init(&incoming_result_response);
  test_msgs__action__Fibonacci_Feedback__init(&outgoing_feedback);
  test_msgs__action__Fibonacci_Feedback__init(&incoming_feedback);
  action_msgs__srv__CancelGoal_Request__init(&outgoing_cancel_request);
  action_msgs__srv__CancelGoal_Request__init(&incoming_cancel_request);
  action_msgs__srv__CancelGoal_Response__init(&outgoing_cancel_response);
  action_msgs__srv__CancelGoal_Response__init(&incoming_cancel_response);


  // Initialize goal request
  init_test_uuid0(outgoing_goal_request.uuid);
  outgoing_goal_request.order = 10;

  // Send goal request with valid arguments
  int64_t sequence_number;
  rcl_ret_t ret = rcl_action_send_goal_request(
    &this->action_client, &outgoing_goal_request, &sequence_number);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_server(&this->wait_set, &this->action_server, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for goal request to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_server_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_server,
    &this->is_goal_request_ready,
    &this->is_cancel_request_ready,
    &this->is_result_request_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_goal_request_ready) << rcl_get_error_string().str;
  EXPECT_FALSE(this->is_cancel_request_ready) << rcl_get_error_string().str;
  EXPECT_FALSE(this->is_result_request_ready) << rcl_get_error_string().str;

  // Take goal request with valid arguments
  rmw_request_id_t request_header;
  ret = rcl_action_take_goal_request(&this->action_server, &request_header, &incoming_goal_request);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the goal request was received correctly
  EXPECT_EQ(outgoing_goal_request.order, incoming_goal_request.order);
  rcl_reset_error();
  EXPECT_TRUE(uuidcmp(outgoing_goal_request.uuid, incoming_goal_request.uuid));
  rcl_reset_error();

  // Initialize goal response
  outgoing_goal_response.accepted = true;
  outgoing_goal_response.stamp.sec = 123;
  outgoing_goal_response.stamp.nanosec = 456789u;

  // Send goal response with valid arguments
  ret = rcl_action_send_goal_response(
    &this->action_server, &request_header, &outgoing_goal_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_client(
    &this->wait_set, &this->action_client, NULL, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for goal response to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_client_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_client,
    &this->is_feedback_ready,
    &this->is_status_ready,
    &this->is_goal_response_ready,
    &this->is_cancel_response_ready,
    &this->is_result_response_ready);

  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_goal_response_ready);
  EXPECT_FALSE(this->is_cancel_response_ready);
  EXPECT_FALSE(this->is_feedback_ready);
  EXPECT_FALSE(this->is_status_ready);
  EXPECT_FALSE(this->is_result_response_ready);

  // Take goal response with valid arguments
  ret = rcl_action_take_goal_response(
    &this->action_client, &request_header, &incoming_goal_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the goal response was received correctly
  EXPECT_EQ(outgoing_goal_response.accepted, incoming_goal_response.accepted);
  rcl_reset_error();
  EXPECT_EQ(outgoing_goal_response.stamp.sec, incoming_goal_response.stamp.sec);
  rcl_reset_error();
  EXPECT_EQ(outgoing_goal_response.stamp.nanosec, incoming_goal_response.stamp.nanosec);
  rcl_reset_error();

  // Initialize result request
  init_test_uuid0(outgoing_result_request.uuid);

  // Send result request with valid arguments
  ret = rcl_action_send_result_request(
    &this->action_client, &outgoing_result_request, &sequence_number);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Initialize feedback
  ASSERT_TRUE(rosidl_generator_c__int32__Sequence__init(
      &outgoing_feedback.sequence, 3));
  outgoing_feedback.sequence.data[0] = 0;
  outgoing_feedback.sequence.data[1] = 1;
  outgoing_feedback.sequence.data[2] = 2;
  init_test_uuid0(outgoing_feedback.uuid);

  // Publish feedback with valid arguments
  ret = rcl_action_publish_feedback(&this->action_server, &outgoing_feedback);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_client(
    &this->wait_set, &this->action_client, NULL, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for feedback to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_client_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_client,
    &this->is_feedback_ready,
    &this->is_status_ready,
    &this->is_goal_response_ready,
    &this->is_cancel_response_ready,
    &this->is_result_response_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_feedback_ready);
  EXPECT_FALSE(this->is_status_ready);
  EXPECT_FALSE(this->is_result_response_ready);
  EXPECT_FALSE(this->is_cancel_response_ready);
  EXPECT_FALSE(this->is_goal_response_ready);

  // Take feedback with valid arguments
  ret = rcl_action_take_feedback(&this->action_client, &incoming_feedback);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that feedback was received correctly
  EXPECT_TRUE(uuidcmp(outgoing_feedback.uuid, incoming_feedback.uuid));
  ASSERT_EQ(outgoing_feedback.sequence.size, incoming_feedback.sequence.size);
  EXPECT_TRUE(!memcmp(
      outgoing_feedback.sequence.data,
      incoming_feedback.sequence.data,
      outgoing_feedback.sequence.size));

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_server(&this->wait_set, &this->action_server, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for result request to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_server_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_server,
    &this->is_goal_request_ready,
    &this->is_cancel_request_ready,
    &this->is_result_request_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_result_request_ready);
  EXPECT_FALSE(this->is_cancel_request_ready);
  EXPECT_FALSE(this->is_goal_request_ready);

  // Take result request with valid arguments
  // rmw_request_id_t request_header;
  ret = rcl_action_take_result_request(
    &this->action_server, &request_header, &incoming_result_request);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the result request was received correctly
  EXPECT_TRUE(uuidcmp(
      outgoing_result_request.uuid,
      incoming_result_request.uuid));

  // Initialize cancel request
  init_test_uuid0(outgoing_cancel_request.goal_info.uuid);
  outgoing_cancel_request.goal_info.stamp.sec = 321;
  outgoing_cancel_request.goal_info.stamp.nanosec = 987654u;

  // Send cancel request with valid arguments
  ret = rcl_action_send_cancel_request(
    &this->action_client, &outgoing_cancel_request, &sequence_number);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_server(&this->wait_set, &this->action_server, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_server_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_server,
    &this->is_goal_request_ready,
    &this->is_cancel_request_ready,
    &this->is_result_request_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_cancel_request_ready);
  EXPECT_FALSE(this->is_goal_request_ready);
  EXPECT_FALSE(this->is_result_request_ready);

  // Take cancel request with valid arguments
  ret = rcl_action_take_cancel_request(
    &this->action_server, &request_header, &incoming_cancel_request);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that the cancel request was received correctly
  EXPECT_TRUE(uuidcmp(
      outgoing_cancel_request.goal_info.uuid,
      incoming_cancel_request.goal_info.uuid));
  EXPECT_EQ(
    outgoing_cancel_request.goal_info.stamp.sec,
    incoming_cancel_request.goal_info.stamp.sec);
  EXPECT_EQ(
    outgoing_cancel_request.goal_info.stamp.nanosec,
    incoming_cancel_request.goal_info.stamp.nanosec);

  // Initialize cancel request
  ASSERT_TRUE(action_msgs__msg__GoalInfo__Sequence__init(
      &outgoing_cancel_response.goals_canceling, 2));
  init_test_uuid0(outgoing_cancel_response.goals_canceling.data[0].uuid);
  outgoing_cancel_response.goals_canceling.data[0].stamp.sec = 102;
  outgoing_cancel_response.goals_canceling.data[0].stamp.nanosec = 9468u;
  init_test_uuid1(outgoing_cancel_response.goals_canceling.data[1].uuid);
  outgoing_cancel_response.goals_canceling.data[1].stamp.sec = 867;
  outgoing_cancel_response.goals_canceling.data[1].stamp.nanosec = 6845u;

  // Send cancel response with valid arguments
  // rmw_request_id_t response_header;
  ret = rcl_action_send_cancel_response(
    &this->action_server, &request_header, &outgoing_cancel_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_client(
    &this->wait_set, &this->action_client, NULL, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_client_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_client,
    &this->is_feedback_ready,
    &this->is_status_ready,
    &this->is_goal_response_ready,
    &this->is_cancel_response_ready,
    &this->is_result_response_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_cancel_response_ready);
  EXPECT_FALSE(this->is_feedback_ready);
  EXPECT_FALSE(this->is_status_ready);
  EXPECT_FALSE(this->is_goal_response_ready);
  EXPECT_FALSE(this->is_result_response_ready);

  // Take cancel response with valid arguments
  ret = rcl_action_take_cancel_response(
    &this->action_client, &request_header, &incoming_cancel_response);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Initialize feedback
  ASSERT_TRUE(rosidl_generator_c__int32__Sequence__init(
      &outgoing_feedback.sequence, 3));
  outgoing_feedback.sequence.data[0] = 0;
  outgoing_feedback.sequence.data[1] = 1;
  outgoing_feedback.sequence.data[2] = 2;
  init_test_uuid0(outgoing_feedback.uuid);

  // Publish feedback with valid arguments
  ret = rcl_action_publish_feedback(&this->action_server, &outgoing_feedback);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_client(
    &this->wait_set, &this->action_client, NULL, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for feedback to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_client_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_client,
    &this->is_feedback_ready,
    &this->is_status_ready,
    &this->is_goal_response_ready,
    &this->is_cancel_response_ready,
    &this->is_result_response_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_feedback_ready);
  EXPECT_FALSE(this->is_status_ready);
  EXPECT_FALSE(this->is_result_response_ready);
  EXPECT_FALSE(this->is_cancel_response_ready);
  EXPECT_FALSE(this->is_goal_response_ready);

  // Take feedback with valid arguments
  ret = rcl_action_take_feedback(&this->action_client, &incoming_feedback);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Check that feedback was received correctly
  EXPECT_TRUE(uuidcmp(outgoing_feedback.uuid, incoming_feedback.uuid));
  ASSERT_EQ(outgoing_feedback.sequence.size, incoming_feedback.sequence.size);
  EXPECT_TRUE(!memcmp(
      outgoing_feedback.sequence.data,
      incoming_feedback.sequence.data,
      outgoing_feedback.sequence.size));

  ret = rcl_wait_set_clear(&this->wait_set);

  ret = rcl_action_wait_set_add_action_server(&this->wait_set, &this->action_server, NULL);
  ASSERT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  // Wait for result response to be ready
  ret = rcl_wait(&this->wait_set, 1000000);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  ret = rcl_action_server_wait_set_get_entities_ready(
    &this->wait_set,
    &this->action_server,
    &this->is_goal_request_ready,
    &this->is_cancel_request_ready,
    &this->is_result_request_ready);
  EXPECT_EQ(ret, RCL_RET_OK) << rcl_get_error_string().str;
  rcl_reset_error();

  EXPECT_TRUE(this->is_result_request_ready);
  EXPECT_FALSE(this->is_cancel_request_ready);
  EXPECT_FALSE(this->is_goal_request_ready);


  test_msgs__action__Fibonacci_Goal_Request__fini(&outgoing_goal_request);
  test_msgs__action__Fibonacci_Goal_Request__fini(&incoming_goal_request);
  test_msgs__action__Fibonacci_Goal_Response__fini(&incoming_goal_response);
  test_msgs__action__Fibonacci_Goal_Response__fini(&outgoing_goal_response);
  test_msgs__action__Fibonacci_Result_Request__fini(&incoming_result_request);
  test_msgs__action__Fibonacci_Result_Request__fini(&outgoing_result_request);
  test_msgs__action__Fibonacci_Result_Response__fini(&incoming_result_response);
  test_msgs__action__Fibonacci_Result_Response__fini(&outgoing_result_response);
  action_msgs__srv__CancelGoal_Request__fini(&incoming_cancel_request);
  action_msgs__srv__CancelGoal_Request__fini(&outgoing_cancel_request);
  action_msgs__srv__CancelGoal_Response__fini(&incoming_cancel_response);
  action_msgs__srv__CancelGoal_Response__fini(&outgoing_cancel_response);
}
