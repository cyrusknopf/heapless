#pragma once

#include <gcc-plugin.h>
#include <tree-core.h> // needed as cannot fwd decl enum tree_code

namespace hpl {
using plugin_info = struct plugin_info;
using plugin_gcc_version = struct plugin_gcc_version;
using plugin_name_args = struct plugin_name_args;   
using attr_spec = struct attribute_spec;
using exclusions = attr_spec::exclusions;
using scoped_attr_specs = struct scoped_attribute_specs;
using register_pass_info = struct register_pass_info;
using tree_code = enum tree_code;
using handler_func = tree (*)(tree* node, tree name, tree args, int flags,
                              bool* no_add_attrs);
}
