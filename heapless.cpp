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
#include "gimple-iterator.h"

#include "dumpfile.h"

#include "stringpool.h"
#include "attribs.h"
#include "diagnostic-core.h"

#include "heapless/noalloc.h"
#include "heapless/hpl_utils.h"
#include "heapless/hpl_types.h"

int plugin_is_GPL_compatible;

static constexpr std::string_view ver_err = "Plugin is for version {}.{}";

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

static std::vector<hpl::attr_spec> all_attrs {
    attr_instance<hpl::noalloc::attr_traits>()
};

static const array_slice<const hpl::attr_spec> attrs{ all_attrs.data(), all_attrs.size() };

static hpl::scoped_attr_specs specs{ .ns = "heapless", .attributes = attrs };

static auto
register_attributes(void* event_data, void* user_data) -> void
{
    register_scoped_attributes(specs);
}

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
        auto* pass = new hpl::noalloc::pass_driver(gcc_global_context);
        // TODO: check when is best to run this pass
        hpl::register_pass_info pass_info {
          .pass = pass,
          .reference_pass_name = "cfg",
          .ref_pass_instance_number = 1,
          .pos_op = PASS_POS_INSERT_AFTER
        };
        constexpr std::nullptr_t callback = nullptr;

        register_callback(name_chars, PLUGIN_PASS_MANAGER_SETUP, callback,
                          &pass_info);
    }

    return 0;
}
