#pragma once

#include <gcc-plugin.h>

#include <tree-core.h>
#include <tree-pass.h>

#include "hpl_types.h"

template <typename AttrT> auto
consteval attr_instance() -> hpl::attr_spec
{
    return {
      .name = AttrT::attr_name,
      .min_length = AttrT::min_length,
      .max_length = AttrT::max_length,
      .decl_required = AttrT::decl_required,
      .type_required = AttrT::type_required,
      .function_type_required =
          AttrT::function_type_required,
      .affects_type_identity = AttrT::affects_type_identity,
      .handler = AttrT::handler,
      .exclude = AttrT::exclusions
    };
}

template <typename AttrT> auto
consteval pass_data_instance() -> pass_data
{
    return {
      .type = AttrT::type,
      .name = AttrT::pass_name,
      .optinfo_flags = AttrT::opt_flags,
      .tv_id = AttrT::tv_id,
      .properties_required = AttrT::props_required,
      .properties_provided = AttrT::props_provided,
      .properties_destroyed = AttrT::props_destroyed,
      .todo_flags_start = AttrT::todo_flags_start,
      .todo_flags_finish = AttrT::todo_flags_finish
    };
}

