#include <cstddef>
#include <print>
#include <string_view>

#include "gcc-plugin.h"
#include "plugin-version.h"
#include "plugin.h"

int plugin_is_GPL_compatible;

static constexpr std::string_view ver_err = "Plugin is for version {}.{}";

namespace hpl {
using plugin_info = struct plugin_info;
using plugin_gcc_version = struct plugin_gcc_version;
using plugin_name_args = struct plugin_name_args;   
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

auto
plugin_init(hpl::plugin_name_args* plugin_info,
            hpl::plugin_gcc_version* version) -> int
{
    if (!plugin_default_version_check(version, &gcc_version)) {
      std::println(ver_err, GCCPLUGIN_VERSION_MINOR, GCCPLUGIN_VERSION_MINOR);
      return -1;
    }

    const std::string_view plugin_name = plugin_info->base_name;
    std::println("Using plugin: {}", plugin_name);

    {
      constexpr plugin_event event = PLUGIN_INFO;
      constexpr std::nullptr_t callback = nullptr;
      register_callback(plugin_name.data(), event, callback, &heapless_info);
    }
    return 0;
}
