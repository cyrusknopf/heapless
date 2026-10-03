#include "gcc-plugin.h"

int plugin_is_GPL_compatible;
using hl_plugin_info = struct plugin_info;
using hl_plugin_gcc_version = struct plugin_gcc_version;

static hl_plugin_info heapless_info =
{
  .version = "0.0",
  .help = "help not yet implemented"
};

static hl_plugin_gcc_version heapless_gcc_ver =
{
    .basever = "15.3",
};

auto
main() -> int
{
}
