#include <cstddef>
#include <print>
#include <string_view>
#include <vector>

#include "gcc-plugin.h"

#include "plugin-version.h"
#include "plugin.h"

#include "timevar.h"

#include "tree-core.h"
#include "tree-pass.h"
#include "tree.h"
#include "tree-ssa-alias.h"

#include "context.h"
#include "function.h"
#include "internal-fn.h"

#include "gimple-expr.h"
#include "gimple.h"

#include "dumpfile.h"

#include "stringpool.h"
#include "attribs.h"

int plugin_is_GPL_compatible;

static constexpr std::string_view ver_err = "Plugin is for version {}.{}";

namespace hpl {
using plugin_info = struct plugin_info;
using plugin_gcc_version = struct plugin_gcc_version;
using plugin_name_args = struct plugin_name_args;   
using attr_spec = struct attribute_spec;
using scoped_attr_specs = struct scoped_attribute_specs;
using register_pass_info = struct register_pass_info;
}

namespace gcc {
using pass_data = pass_data;
using gimple_opt_pass = gimple_opt_pass;
}

static hpl::plugin_info heapless_info =
{
  .version = "0.0",
  .help = "help not yet implemented",
};

static hpl::plugin_gcc_version heapless_gcc_ver =
{
    .basever = "15.3",
    .datestamp = "",
    .devphase = "",
    .revision = "",
    .configuration_arguments = ""
};

static auto
handle_attribute(tree* node, tree attr_name, tree attrArguments, int flags,
                 bool* no_add_attrs) -> tree
{
    std::println("Found attribute: {}", IDENTIFIER_POINTER(attr_name));

    return NULL_TREE;
}

static hpl::attr_spec attr1 =
{
  .name = "attr1",
  .min_length = 0,
  .max_length = 0,
  .decl_required = false,
  .type_required = true,
  .function_type_required = true,
  .affects_type_identity = false,
  .handler = handle_attribute
};

static std::vector<hpl::attr_spec> all_attrs{ attr1 };

static const array_slice<const hpl::attr_spec> attrs{ all_attrs.data(), all_attrs.size() };

static hpl::scoped_attr_specs specs{ .ns = "heapless", .attributes = attrs };

static auto
register_attributes(void* event_data, void* user_data) -> void
{
    register_scoped_attributes(specs);
}

static const gcc::pass_data pass1_data {
  .type = GIMPLE_PASS,
  .name = "pass1",
  .optinfo_flags = OPTGROUP_NONE,
  .tv_id = TV_NONE,
  .properties_required = PROP_gimple_any,
  .properties_provided = 0,
  .properties_destroyed = 0,
  .todo_flags_start = 0,
  .todo_flags_finish = 0
};

struct pass1 final : gcc::gimple_opt_pass {
    explicit pass1(gcc::context* ctx)
    : gcc::gimple_opt_pass(pass1_data, ctx) { }

    auto execute(function* fun) -> unsigned int final
    {
      std::println("Executing pass1");
      return 0;
    }

    auto clone() -> pass1* final
    {
      return this;
    }
};

auto
plugin_init(hpl::plugin_name_args* plugin_info,
            hpl::plugin_gcc_version* version) -> int
{
    if (!plugin_default_version_check(version, &gcc_version)) {
      std::println(ver_err, GCCPLUGIN_VERSION_MINOR, GCCPLUGIN_VERSION_MINOR);
      return -1;
    }

    const std::string_view plugin_name = plugin_info->base_name;
    const char* name_chars = plugin_name.data();
    std::println("Using plugin: {}", plugin_name);

    { // register info
      constexpr plugin_event event = PLUGIN_INFO;
      constexpr std::nullptr_t callback = nullptr;
      register_callback(name_chars, event, callback, &heapless_info);
    }

    { // register attributes
      constexpr plugin_event event = PLUGIN_ATTRIBUTES;
      constexpr auto* callback = &register_attributes;
      constexpr std::nullptr_t user_data = nullptr;
      register_callback(name_chars, event, callback, user_data);
    }

    { // register passes
        auto* gcc_global_context = g;
        auto* pass = new pass1(gcc_global_context);
        // TODO: check when is best to run this pass
        hpl::register_pass_info pass_info {
          .pass = pass,
          .reference_pass_name = "cfg",
          .ref_pass_instance_number = 1,
          .pos_op = PASS_POS_INSERT_BEFORE
        };
        constexpr std::nullptr_t callback = nullptr;

        register_callback(name_chars, PLUGIN_PASS_MANAGER_SETUP, callback,
                          &pass_info);
    }

    return 0;
}
