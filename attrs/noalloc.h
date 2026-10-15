#pragma once

namespace hpl
{

struct noalloc {
  constexpr static char* const attr_name = "noalloc\0";
  constexpr static int min_length = 0;
  constexpr static int max_length = 0;
  constexpr static bool decl_required = true;
  constexpr static bool type_required = false;
  constexpr static bool function_type_required = false;
  constexpr static bool affects_type_identity = false;
};

} // namespace hpl
