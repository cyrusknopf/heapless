#include <cstddef>
#include <print>
#include <string_view>
#include <vector>

#include "gcc-plugin.h"

#include "plugin-version.h"
#include "plugin.h"
#include "tree-core.h"
#include "tree.h"
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

    return 0;
}
