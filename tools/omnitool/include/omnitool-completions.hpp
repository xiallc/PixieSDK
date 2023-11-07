/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file completions.hpp
 * @brief Command completions for the omnitool shell.
 */

#ifndef ONMITOOL_COMPLETIONS_HPP
#define ONMITOOL_COMPLETIONS_HPP

namespace xia {
namespace omnitool {
namespace command {
namespace completions {

/**
 * @brief Function passed to flag_completions to handle completions for flags
 *        arguments.
 *
 * @param[in] std::string String containing the flag to be completed.
 */
using flag_handler = std::function<void(std::string)>;

/**
 * @brief Adds all valid modules to completions. Includes all option.
 *
 * @param[in] context Provides the shell context.
 * @param[in] group The group the completions added are a part of.
 * @param[in] module_arg_idx The index of the modules argument.
 * @param[in,out] completions List of completions to add to.
 */
void modules_completions(
    context& context, const std::string& group, size_t module_arg_idx,
    completion& completions);

/**
 * @brief Adds all valid modules to completions.
 *
 * @param[in] context Provides the shell context.
 * @param[in] group The group the completions added are a part of.
 * @param[in] module_arg_idx The index of the modules argument.
 * @param[in,out] completions List of completions to add to.
 */
void module_completions(
    context& context, const std::string& group, size_t module_arg_idx,
    completion& completions);

/*
 * @brief Adds all valid channels to completions.
 *
 * @param[in] context Provides the shell context.
 * @param[in] group The group the completions added are a part of.
 * @param[in] module_arg_idx The index of the modules argument.
 * @param[in] channel_arg_idx The index of the channels argument.
 * @param[in,out] completions List of completions to add to.
 */
void channels_completions(
    context& context, const std::string& group, size_t module_arg_idx,
    size_t channel_arg_idx, completion& completions);

/*
 * @brief Checks if a channel has been specified.
 *
 * @param[in] context Provides the shell context.
 * @param[in] module_arg_idx The index of the modules argument.
 * @param[in] channel_arg_idx The index of the channels argument.
 * @param[in] completions Completions contains the input arguments.
 *
 * @return True if channel is specified.
 */
 bool valid_channels_check(
    context& context, size_t module_arg_idx,
    size_t channel_arg_idx, completion& completions);

/*
 * @brief Adds completion if a valid option.
 *
 * @param[in] context Provides the shell context.
 * @param[in] completion The completion at be added.
 * @param[in] min_arg_idx The first index the argument can be completed, inclusive.
 * @param[in] max_arg_idx The last index the argument can be completed, inclusive.
 * @param[in,out] completions List of completions to add to.
 */
void argument_completion(
    completion_entry comp, size_t min_arg_idx,
    size_t max_arg_idx, completion& completions);

/*
 * @brief Adds help for an item we can't complete.
 *
 * @param[in] context Provides the shell context.
 * @param[in] entries The entries to add. There must be at least 2 so they never get completed.
 * @param[in] arg_idx The first index the argument can be completed, inclusive.
 * @param[in,out] completions List of completions to add to.
 */
void help_argument_completion(
    completion_entries& entries, size_t arg_idx,
    completion& completions);

/*
 * @brief Handles the flags for a given function
 *
 * @param[in] context Shell context.
 * @param[in] handle_flags Lambda function to handle flag option
 *            completion.
 * @param[in] cmd The name of the command to complete for.
 * @param[in,out] completions List of completions to add to.
 *
 * @return Returns true if no other completions should be added.
 *         False otherwise.
 */
bool flag_completion(
    flag_handler handle_flags, std::string cmd, completion& completions);

/*
 * @brief Finds the offset of the beginning of the positional arguments
 *
 * @param[in] context Shell context.
 * @param[in] cmd The name of the command.
 * @param[in] completions Current completion context.
 *
 * @return Returns the offset of the positional arguments, 0 on error.
 */
size_t get_pos_arg_offset(
    std::string cmd, completion& completions);

/*
 * @brief Adds multiple mutually exclusive completions if a valid option.
 *
 * @param[in] context Provides the shell context.
 * @param[in] entries The group of completions to be added.
 * @param[in] min_arg_idx The first index the argument can be completed, inclusive.
 * @param[in] max_arg_idx The last index the argument can be completed, inclusive.
 * @param[in,out] completions List of completions to add to.
 *
 * @return False if no completions added, True otherwise.
 */
void multiargument_completion(
    completion_entries& entries, size_t min_arg_idx,
    size_t max_arg_idx, completion& completions);

}  // namespace completions
}  // namespace command
}  // namespace omnitool
}  // namespace xia

#endif  // ONMITOOL_COMPLETIONS_HPP
