#pragma once

#include <cassert>

#include <gcc-plugin.h>

#include <tree-core.h>
#include <tree-pass.h>
#include <tree.h>

#include <gimple.h>
#include <gimple-iterator.h>

#include <cgraph.h>

#include <context.h>
#include <diagnostic-core.h>

#include <stringpool.h>

#include <attribs.h>
#include <dumpfile.h>

#include "hpl_types.h"
#include "hpl_utils.h"

namespace gcc {
  using gimple_opt_pass = gimple_opt_pass;
  using pass_data = pass_data;
}

static auto
handle_attribute(tree* node, tree attr_name, tree attr_args, int flags,
                 bool* no_add_attrs) -> tree
{
  hpl::tree_code code = TREE_CODE(*node);

  if (code == FUNCTION_DECL) {
    return NULL_TREE;
  }

  warning(OPT_Wattributes, "%qE applied to non-function", attr_name);
  *no_add_attrs = true;

  return NULL_TREE;
}

namespace hpl::noalloc {

struct attr_traits : static_trait_mixin {
  constexpr static const char* const attr_name = "noalloc";
  constexpr static int min_length = 0;
  constexpr static int max_length = 0;
  constexpr static bool decl_required = true;
  constexpr static bool type_required = false;
  constexpr static bool function_type_required = false;
  constexpr static bool affects_type_identity = false;
  constexpr static attr_spec::exclusions* exclusions = nullptr;
  constexpr static handler_func handler = handle_attribute;
};

struct pass_traits  : static_trait_mixin {
  constexpr static auto type = GIMPLE_PASS;
  constexpr static const char* pass_name = "noalloc_pass";
  constexpr static auto opt_flags = OPTGROUP_NONE;
  constexpr static auto tv_id = TV_NONE;
  constexpr static auto props_required = PROP_gimple_any;
  constexpr static unsigned int props_provided = 0;
  constexpr static unsigned int props_destroyed = 0;
  constexpr static unsigned int todo_flags_start = 0;
  constexpr static unsigned int todo_flags_finish = 0;
};

static auto
check_func(function* fun) -> bool
{
  basic_block bb = nullptr;
  // FOR_ALL_BB_FUN
  for (bb = ((fun)->cfg->x_entry_block_ptr); bb; bb = bb->next_bb)
  {
    gimple_stmt_iterator gsit{};
    for (gsit = gsi_start_bb(bb); !gsi_end_p(gsit); gsi_next(&gsit)) {
      gimple* stmt = gsi_stmt(gsit);
      bool call = is_gimple_call(stmt);
      bool builtin = gimple_call_builtin_p(stmt);

      if (not call) continue;

      tree fndecl = gimple_call_fndecl(stmt);

      bool is_new = DECL_IS_OPERATOR_NEW_P(fndecl);
      if (is_new) {
        location_t loc = gimple_location(stmt);
        error_at(loc, "operator new used in %s function",
                 hpl::noalloc::attr_traits::attr_name);
        return false;
      }

      if (builtin) {
        const built_in_function fncode = DECL_FUNCTION_CODE(fndecl);
        if (fncode == BUILT_IN_MALLOC) {
          location_t loc = gimple_location(stmt);
          error_at(loc, "malloc() used in %s function",
                   hpl::noalloc::attr_traits::attr_name);
          return false;
        }
      }

      cgraph_node* called = cgraph_node::get(fndecl);
      if (!called) {
        location_t loc = gimple_location(stmt);
        error_at(loc, "unable to prove this function is allocation free");
        return false;
      }

      // user-defined/non-builtin function call
      called->get_untransformed_body();
      function* sub_fun = DECL_STRUCT_FUNCTION(fndecl);
      assert(sub_fun);

      if (!sub_fun->cfg) {
        location_t loc = gimple_location(stmt);
        inform(loc, "internal error occured here");
        internal_error("called function has no cfg");
        continue;
      }

      if (sub_fun == fun) continue; // recursive call

      if (!check_func(sub_fun)) {
        location_t loc = gimple_location(stmt);
        inform(loc, "allocation occurs in function called here");
        return false;
      }
    }
  }
  return true;
}

struct pass_driver final : public gcc::gimple_opt_pass
{
  explicit pass_driver(gcc::context* ctx)
  : gcc::gimple_opt_pass(pass_data_instance<pass_traits>(), ctx)
  { }

  auto
  execute(function* fun) -> unsigned int final
  {
    union tree_node* decl = fun->decl;
    union tree_node* fun_attrs = DECL_ATTRIBUTES(decl);
    bool has_attr =
        lookup_attribute(hpl::noalloc::attr_traits::attr_name, fun_attrs);
    if (!has_attr) {
      return 0;
    }

    check_func(fun);
    return 0;
  }

  auto
  clone() -> decltype(this) final
  {
    return this;
  }
};

} // namespace hpl
