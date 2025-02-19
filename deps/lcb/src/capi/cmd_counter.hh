/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2016-2021 Couchbase, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef LIBCOUCHBASE_CAPI_COUNTER_HH
#define LIBCOUCHBASE_CAPI_COUNTER_HH

#include <cstddef>
#include <cstdint>
#include <chrono>

#include "key_value_error_context.hh"
#include "collection_qualifier.hh"

/**
 * @private
 */
struct lcb_CMDCOUNTER_ {
  public:
    lcb_STATUS expiry(std::uint32_t expiry)
    {
        if (!initialize_if_does_not_exist_) {
            return LCB_ERR_OPTIONS_CONFLICT;
        }
        expiry_ = expiry;
        return LCB_SUCCESS;
    }

    lcb_STATUS key(std::string key)
    {
        key_ = std::move(key);
        return LCB_SUCCESS;
    }

    lcb_STATUS collection(lcb::collection_qualifier collection)
    {
        collection_ = std::move(collection);
        return LCB_SUCCESS;
    }

    lcb_STATUS parent_span(lcbtrace_SPAN *parent_span)
    {
        parent_span_ = parent_span;
        return LCB_SUCCESS;
    }

    lcb_STATUS timeout_in_milliseconds(std::uint32_t timeout)
    {
        timeout_ = std::chrono::milliseconds(timeout);
        return LCB_SUCCESS;
    }

    lcb_STATUS timeout_in_microseconds(std::uint32_t timeout)
    {
        timeout_ = std::chrono::microseconds(timeout);
        return LCB_SUCCESS;
    }

    lcb_STATUS start_time_in_nanoseconds(std::uint64_t val)
    {
        start_time_ = std::chrono::nanoseconds(val);
        return LCB_SUCCESS;
    }

    std::uint64_t start_time_or_default_in_nanoseconds(std::uint64_t default_val)
    {
        if (start_time_ == std::chrono::nanoseconds::zero()) {
            return default_val;
        }
        return start_time_.count();
    }

    lcb_STATUS delta(std::int64_t delta)
    {
        delta_ = delta;
        return LCB_SUCCESS;
    }

    lcb_STATUS initialize_with(std::uint64_t initial_value)
    {
        initialize_if_does_not_exist_ = true;
        initial_value_ = initial_value;
        return LCB_SUCCESS;
    }

    lcb_STATUS durability_level(lcb_DURABILITY_LEVEL level)
    {
        durability_level_ = level;
        return LCB_SUCCESS;
    }

    const lcb::collection_qualifier &collection() const
    {
        return collection_;
    }

    lcb::collection_qualifier &collection()
    {
        return collection_;
    }

    bool has_durability_requirements() const
    {
        return durability_level_ != LCB_DURABILITYLEVEL_NONE;
    }

    lcb_DURABILITY_LEVEL durability_level() const
    {
        return durability_level_;
    }

    const std::string &key() const
    {
        return key_;
    }

    std::uint64_t timeout_or_default_in_nanoseconds(std::uint64_t default_timeout) const
    {
        if (timeout_ > std::chrono::microseconds::zero()) {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(timeout_).count();
        }
        return default_timeout;
    }

    std::uint32_t timeout_in_microseconds() const
    {
        return static_cast<std::uint32_t>(timeout_.count());
    }

    std::uint64_t initial_value() const
    {
        return initial_value_;
    }

    bool initialize_if_does_not_exist() const
    {
        return initialize_if_does_not_exist_;
    }

    std::uint32_t expiry() const
    {
        return expiry_;
    }

    std::int64_t delta() const
    {
        return delta_;
    }

    lcbtrace_SPAN *parent_span() const
    {
        return parent_span_;
    }

    void cookie(void *cookie)
    {
        cookie_ = cookie;
    }

    void *cookie()
    {
        return cookie_;
    }

  private:
    lcb::collection_qualifier collection_{};
    std::uint32_t expiry_{0};
    std::uint64_t initial_value_{0};
    std::int64_t delta_{0};
    std::chrono::microseconds timeout_{0};
    std::chrono::nanoseconds start_time_{0};
    lcbtrace_SPAN *parent_span_{nullptr};
    void *cookie_{nullptr};
    std::string key_{};
    bool initialize_if_does_not_exist_{false};
    lcb_DURABILITY_LEVEL durability_level_{LCB_DURABILITYLEVEL_NONE};
};

/**
 * @private
 */
struct lcb_RESPCOUNTER_ {
    lcb_KEY_VALUE_ERROR_CONTEXT ctx{};
    lcb_MUTATION_TOKEN mt{};
    /**
     Application-defined pointer passed as the `cookie` parameter when
     scheduling the command.
     */
    void *cookie;
    /** Response specific flags. see ::lcb_RESPFLAGS */
    std::uint16_t rflags;
    /** Contains the _current_ value after the operation was performed */
    std::uint64_t value;
};
#endif // LIBCOUCHBASE_CAPI_COUNTER_HH
