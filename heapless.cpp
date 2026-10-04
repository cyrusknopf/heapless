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
handle_attribute(tree* node, tree attr_name, tree attr_args, int flags,
                 bool* no_add_attrs) -> tree
{
    std::println("Found attribute: {}", IDENTIFIER_POINTER(attr_name));

    union tree_node* x = *node;
    if (!x) {
        return NULL_TREE;
    }

    struct tree_function_decl func_decl = x->function_decl;
    struct function* func = func_decl.f;

    enum tree_code code = TREE_CODE(x);

    switch (code) {
      case ERROR_MARK:
      case IDENTIFIER_NODE:
      case TREE_LIST:
      case TREE_VEC:
      case BLOCK:
      case OFFSET_TYPE:
      case ENUMERAL_TYPE:
      case BOOLEAN_TYPE:
      case INTEGER_TYPE:
      case BITINT_TYPE:
      case REAL_TYPE:
      case POINTER_TYPE:
      case REFERENCE_TYPE:
      case NULLPTR_TYPE:
      case FIXED_POINT_TYPE:
      case COMPLEX_TYPE:
      case VECTOR_TYPE:
      case ARRAY_TYPE:
      case RECORD_TYPE:
      case UNION_TYPE:
      case QUAL_UNION_TYPE:
      case VOID_TYPE:
      case FUNCTION_TYPE:
      case METHOD_TYPE:
      case LANG_TYPE:
      case OPAQUE_TYPE:
      case VOID_CST:
      case INTEGER_CST:
      case POLY_INT_CST:
      case REAL_CST:
      case FIXED_CST:
      case COMPLEX_CST:
      case VECTOR_CST:
      case STRING_CST:
      case RAW_DATA_CST:
      case FUNCTION_DECL:
      case LABEL_DECL:
      case FIELD_DECL:
      case VAR_DECL:
      case CONST_DECL:
      case PARM_DECL:
      case TYPE_DECL:
      case RESULT_DECL:
      case DEBUG_EXPR_DECL:
      case DEBUG_BEGIN_STMT:
      case NAMESPACE_DECL:
      case IMPORTED_DECL:
      case NAMELIST_DECL:
      case TRANSLATION_UNIT_DECL:
      case COMPONENT_REF:
      case BIT_FIELD_REF:
      case ARRAY_REF:
      case ARRAY_RANGE_REF:
      case REALPART_EXPR:
      case IMAGPART_EXPR:
      case VIEW_CONVERT_EXPR:
      case INDIRECT_REF:
      case OBJ_TYPE_REF:
      case CONSTRUCTOR:
      case COMPOUND_EXPR:
      case MODIFY_EXPR:
      case INIT_EXPR:
      case TARGET_EXPR:
      case COND_EXPR:
      case VEC_DUPLICATE_EXPR:
      case VEC_SERIES_EXPR:
      case VEC_COND_EXPR:
      case VEC_PERM_EXPR:
      case BIND_EXPR:
      case CALL_EXPR:
      case WITH_CLEANUP_EXPR:
      case CLEANUP_POINT_EXPR:
      case PLACEHOLDER_EXPR:
      case PLUS_EXPR:
      case MINUS_EXPR:
      case MULT_EXPR:
      case POINTER_PLUS_EXPR:
      case POINTER_DIFF_EXPR:
      case MULT_HIGHPART_EXPR:
      case TRUNC_DIV_EXPR:
      case CEIL_DIV_EXPR:
      case FLOOR_DIV_EXPR:
      case ROUND_DIV_EXPR:
      case TRUNC_MOD_EXPR:
      case CEIL_MOD_EXPR:
      case FLOOR_MOD_EXPR:
      case ROUND_MOD_EXPR:
      case RDIV_EXPR:
      case EXACT_DIV_EXPR:
      case FIX_TRUNC_EXPR:
      case FLOAT_EXPR:
      case NEGATE_EXPR:
      case MIN_EXPR:
      case MAX_EXPR:
      case ABS_EXPR:
      case ABSU_EXPR:
      case LSHIFT_EXPR:
      case RSHIFT_EXPR:
      case LROTATE_EXPR:
      case RROTATE_EXPR:
      case BIT_IOR_EXPR:
      case BIT_XOR_EXPR:
      case BIT_AND_EXPR:
      case BIT_NOT_EXPR:
      case TRUTH_ANDIF_EXPR:
      case TRUTH_ORIF_EXPR:
      case TRUTH_AND_EXPR:
      case TRUTH_OR_EXPR:
      case TRUTH_XOR_EXPR:
      case TRUTH_NOT_EXPR:
      case LT_EXPR:
      case LE_EXPR:
      case GT_EXPR:
      case GE_EXPR:
      case LTGT_EXPR:
      case EQ_EXPR:
      case NE_EXPR:
      case UNORDERED_EXPR:
      case ORDERED_EXPR:
      case UNLT_EXPR:
      case UNLE_EXPR:
      case UNGT_EXPR:
      case UNGE_EXPR:
      case UNEQ_EXPR:
      case RANGE_EXPR:
      case PAREN_EXPR:
      case CONVERT_EXPR:
      case ADDR_SPACE_CONVERT_EXPR:
      case FIXED_CONVERT_EXPR:
      case NOP_EXPR:
      case NON_LVALUE_EXPR:
      case COMPOUND_LITERAL_EXPR:
      case SAVE_EXPR:
      case ADDR_EXPR:
      case FDESC_EXPR:
      case BIT_INSERT_EXPR:
      case COMPLEX_EXPR:
      case CONJ_EXPR:
      case PREDECREMENT_EXPR:
      case PREINCREMENT_EXPR:
      case POSTDECREMENT_EXPR:
      case POSTINCREMENT_EXPR:
      case VA_ARG_EXPR:
      case TRY_CATCH_EXPR:
      case TRY_FINALLY_EXPR:
      case EH_ELSE_EXPR:
      case DECL_EXPR:
      case LABEL_EXPR:
      case GOTO_EXPR:
      case RETURN_EXPR:
      case EXIT_EXPR:
      case LOOP_EXPR:
      case SWITCH_EXPR:
      case CASE_LABEL_EXPR:
      case ASM_EXPR:
      case SSA_NAME:
      case CATCH_EXPR:
      case EH_FILTER_EXPR:
      case SCEV_KNOWN:
      case SCEV_NOT_KNOWN:
      case POLYNOMIAL_CHREC:
      case STATEMENT_LIST:
      case ASSERT_EXPR:
      case TREE_BINFO:
      case WITH_SIZE_EXPR:
      case REALIGN_LOAD_EXPR:
      case TARGET_MEM_REF:
      case MEM_REF:
      case OACC_PARALLEL:
      case OACC_KERNELS:
      case OACC_SERIAL:
      case OACC_DATA:
      case OACC_HOST_DATA:
      case OMP_PARALLEL:
      case OMP_TASK:
      case OMP_FOR:
      case OMP_SIMD:
      case OMP_DISTRIBUTE:
      case OMP_TASKLOOP:
      case OMP_LOOP:
      case OMP_TILE:
      case OMP_UNROLL:
      case OACC_LOOP:
      case OMP_TEAMS:
      case OMP_TARGET_DATA:
      case OMP_TARGET:
      case OMP_SECTIONS:
      case OMP_ORDERED:
      case OMP_CRITICAL:
      case OMP_SINGLE:
      case OMP_SCOPE:
      case OMP_TASKGROUP:
      case OMP_MASKED:
      case OMP_SCAN:
      case OMP_DISPATCH:
      case OMP_INTEROP:
      case OMP_SECTION:
      case OMP_STRUCTURED_BLOCK:
      case OMP_MASTER:
      case OACC_CACHE:
      case OACC_DECLARE:
      case OACC_ENTER_DATA:
      case OACC_EXIT_DATA:
      case OACC_UPDATE:
      case OMP_TARGET_UPDATE:
      case OMP_TARGET_ENTER_DATA:
      case OMP_TARGET_EXIT_DATA:
      case OMP_METADIRECTIVE:
      case OMP_ATOMIC:
      case OMP_ATOMIC_READ:
      case OMP_ATOMIC_CAPTURE_OLD:
      case OMP_ATOMIC_CAPTURE_NEW:
      case OMP_CLAUSE:
      case OMP_ARRAY_SECTION:
      case OMP_NEXT_VARIANT:
      case OMP_TARGET_DEVICE_MATCHES:
      case TRANSACTION_EXPR:
      case DOT_PROD_EXPR:
      case WIDEN_SUM_EXPR:
      case SAD_EXPR:
      case WIDEN_MULT_EXPR:
      case WIDEN_MULT_PLUS_EXPR:
      case WIDEN_MULT_MINUS_EXPR:
      case WIDEN_LSHIFT_EXPR:
      case VEC_WIDEN_MULT_HI_EXPR:
      case VEC_WIDEN_MULT_LO_EXPR:
      case VEC_WIDEN_MULT_EVEN_EXPR:
      case VEC_WIDEN_MULT_ODD_EXPR:
      case VEC_UNPACK_HI_EXPR:
      case VEC_UNPACK_LO_EXPR:
      case VEC_UNPACK_FLOAT_HI_EXPR:
      case VEC_UNPACK_FLOAT_LO_EXPR:
      case VEC_UNPACK_FIX_TRUNC_HI_EXPR:
      case VEC_UNPACK_FIX_TRUNC_LO_EXPR:
      case VEC_PACK_TRUNC_EXPR:
      case VEC_PACK_SAT_EXPR:
      case VEC_PACK_FIX_TRUNC_EXPR:
      case VEC_PACK_FLOAT_EXPR:
      case VEC_WIDEN_LSHIFT_HI_EXPR:
      case VEC_WIDEN_LSHIFT_LO_EXPR:
      case PREDICT_EXPR:
      case OPTIMIZATION_NODE:
      case TARGET_OPTION_NODE:
      case ANNOTATE_EXPR:
      case LAST_AND_UNUSED_TREE_CODE:
      case C_MAYBE_CONST_EXPR:
      case EXCESS_PRECISION_EXPR:
      case USERDEF_LITERAL:
      case SIZEOF_EXPR:
      case PAREN_SIZEOF_EXPR:
      case FOR_STMT:
      case WHILE_STMT:
      case DO_STMT:
      case BREAK_STMT:
      case CONTINUE_STMT:
      case SWITCH_STMT:
      case CONCEPT_DECL:
      case UNCONSTRAINED_ARRAY_TYPE:
      case UNCONSTRAINED_ARRAY_REF:
      case LOAD_EXPR:
      case NULL_EXPR:
      case PLUS_NOMOD_EXPR:
      case MINUS_NOMOD_EXPR:
      case POWER_EXPR:
      case ATTR_ADDR_EXPR:
      case STMT_STMT:
      case LOOP_STMT:
      case EXIT_STMT:
      case C_TOKEN_VEC:
      case OFFSET_REF:
      case PTRMEM_CST:
      case NEW_EXPR:
      case VEC_NEW_EXPR:
      case DELETE_EXPR:
      case VEC_DELETE_EXPR:
      case SCOPE_REF:
      case MEMBER_REF:
      case TYPE_EXPR:
      case AGGR_INIT_EXPR:
      case VEC_INIT_EXPR:
      case THROW_EXPR:
      case EMPTY_CLASS_EXPR:
      case BASELINK:
      case TEMPLATE_DECL:
      case TEMPLATE_PARM_INDEX:
      case TEMPLATE_TEMPLATE_PARM:
      case TEMPLATE_TYPE_PARM:
      case TYPENAME_TYPE:
      case TYPEOF_TYPE:
      case BOUND_TEMPLATE_TEMPLATE_PARM:
      case UNBOUND_CLASS_TEMPLATE:
      case USING_DECL:
      case USING_STMT:
      case DEFERRED_PARSE:
      case DEFERRED_NOEXCEPT:
      case TEMPLATE_ID_EXPR:
      case OVERLOAD:
      case BINDING_VECTOR:
      case PSEUDO_DTOR_EXPR:
      case MODOP_EXPR:
      case CAST_EXPR:
      case REINTERPRET_CAST_EXPR:
      case CONST_CAST_EXPR:
      case STATIC_CAST_EXPR:
      case DYNAMIC_CAST_EXPR:
      case IMPLICIT_CONV_EXPR:
      case DOTSTAR_EXPR:
      case TYPEID_EXPR:
      case NOEXCEPT_EXPR:
      case SPACESHIP_EXPR:
      case CTOR_INITIALIZER:
      case TRY_BLOCK:
      case EH_SPEC_BLOCK:
      case HANDLER:
      case MUST_NOT_THROW_EXPR:
      case CLEANUP_STMT:
      case IF_STMT:
      case RANGE_FOR_STMT:
      case EXPR_STMT:
      case TAG_DEFN:
      case OFFSETOF_EXPR:
      case ADDRESSOF_EXPR:
      case ARROW_EXPR:
      case ALIGNOF_EXPR:
      case AT_ENCODE_EXPR:
      case STMT_EXPR:
      case UNARY_PLUS_EXPR:
      case STATIC_ASSERT:
      case TYPE_ARGUMENT_PACK:
      case NONTYPE_ARGUMENT_PACK:
      case TYPE_PACK_EXPANSION:
      case EXPR_PACK_EXPANSION:
      case PACK_INDEX_TYPE:
      case PACK_INDEX_EXPR:
      case ARGUMENT_PACK_SELECT:
      case UNARY_LEFT_FOLD_EXPR:
      case UNARY_RIGHT_FOLD_EXPR:
      case BINARY_LEFT_FOLD_EXPR:
      case BINARY_RIGHT_FOLD_EXPR:
      case BIT_CAST_EXPR:
      case TRAIT_EXPR:
      case TRAIT_TYPE:
      case LAMBDA_EXPR:
      case DECLTYPE_TYPE:
      case BASES:
      case DEPENDENT_OPERATOR_TYPE:
      case TEMPLATE_INFO:
      case OMP_DEPOBJ:
      case CONSTRAINT_INFO:
      case REQUIRES_EXPR:
      case SIMPLE_REQ:
      case TYPE_REQ:
      case COMPOUND_REQ:
      case NESTED_REQ:
      case ATOMIC_CONSTR:
      case CONJ_CONSTR:
      case DISJ_CONSTR:
      case CO_AWAIT_EXPR:
      case CO_YIELD_EXPR:
      case CO_RETURN_EXPR:
      case ASSERTION_STMT:
      case PRECONDITION_STMT:
      case POSTCONDITION_STMT:
      case TU_LOCAL_ENTITY:
      case UNSIGNED_RSHIFT_EXPR:
      case FLOAT_MOD_EXPR:
      case FUNCFRAME_INFO:
      case SET_TYPE:
      case CLASS_INTERFACE_TYPE:
      case CLASS_IMPLEMENTATION_TYPE:
      case CATEGORY_INTERFACE_TYPE:
      case CATEGORY_IMPLEMENTATION_TYPE:
      case PROTOCOL_INTERFACE_TYPE:
      case KEYWORD_DECL:
      case INSTANCE_METHOD_DECL:
      case CLASS_METHOD_DECL:
      case PROPERTY_DECL:
      case MESSAGE_SEND_EXPR:
      case CLASS_REFERENCE_EXPR:
      case PROPERTY_REF:
      case MAX_TREE_CODES:
        break;
    }

    if (!func) {
        std::println("null func");
        return NULL_TREE;
    }

    basic_block bb;
    FOR_ALL_BB_FN(bb, func) {
      gimple_stmt_iterator gsi{};
      for (gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
          gimple* stmt = gsi_stmt(gsi);

          if (!is_gimple_call(stmt)) {
              std::println("Found non-call statement");
              continue;
          }
          std::println("Found call statement");
      }
    }

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
