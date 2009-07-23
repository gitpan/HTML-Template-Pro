/* generated; do not edit */
#include "pabstract.h"
#include "pparam.h"

int tmplpro_get_option_global_vars(struct tmplpro_param* param) {
    return param->global_vars;
}

void tmplpro_set_option_global_vars(struct tmplpro_param* param, int val) {
    param->global_vars=val;
}

int tmplpro_get_option_max_includes(struct tmplpro_param* param) {
    return param->max_includes;
}

void tmplpro_set_option_max_includes(struct tmplpro_param* param, int val) {
    param->max_includes=val;
}

int tmplpro_get_option_debug(struct tmplpro_param* param) {
    return param->debug;
}

void tmplpro_set_option_debug(struct tmplpro_param* param, int val) {
    param->debug=val;
}

int tmplpro_get_option_no_includes(struct tmplpro_param* param) {
    return (int) param->no_includes;
}

void tmplpro_set_option_no_includes(struct tmplpro_param* param, int val) {
    param->no_includes=(flag)val;
}

int tmplpro_get_option_case_sensitive(struct tmplpro_param* param) {
    return (int) param->case_sensitive;
}

void tmplpro_set_option_case_sensitive(struct tmplpro_param* param, int val) {
    param->case_sensitive=(flag)val;
}

int tmplpro_get_option_loop_context_vars(struct tmplpro_param* param) {
    return (int) param->loop_context_vars;
}

void tmplpro_set_option_loop_context_vars(struct tmplpro_param* param, int val) {
    param->loop_context_vars=(flag)val;
}

int tmplpro_get_option_strict(struct tmplpro_param* param) {
    return (int) param->strict;
}

void tmplpro_set_option_strict(struct tmplpro_param* param, int val) {
    param->strict=(flag)val;
}

int tmplpro_get_option_filters(struct tmplpro_param* param) {
    return (int) param->filters;
}

void tmplpro_set_option_filters(struct tmplpro_param* param, int val) {
    param->filters=(flag)val;
}

int tmplpro_get_option_default_escape(struct tmplpro_param* param) {
    return param->default_escape;
}

void tmplpro_set_option_default_escape(struct tmplpro_param* param, int val) {
    param->default_escape=val;
}

const char* tmplpro_get_option_filename(struct tmplpro_param* param) {
    return param->filename;
}

void tmplpro_set_option_filename(struct tmplpro_param* param, const char* val) {
    param->filename=val;
}

PSTRING tmplpro_get_option_scalarref(struct tmplpro_param* param) {
    return param->scalarref;
}

void tmplpro_set_option_scalarref(struct tmplpro_param* param, PSTRING val) {
    param->scalarref=val;
}

int tmplpro_get_option_path_like_variable_scope(struct tmplpro_param* param) {
    return (int) param->path_like_variable_scope;
}

void tmplpro_set_option_path_like_variable_scope(struct tmplpro_param* param, int val) {
    param->path_like_variable_scope=(flag)val;
}

int tmplpro_get_option_search_path_on_include(struct tmplpro_param* param) {
    return (int) param->search_path_on_include;
}

void tmplpro_set_option_search_path_on_include(struct tmplpro_param* param, int val) {
    param->search_path_on_include=(flag)val;
}

char** tmplpro_get_option_path(struct tmplpro_param* param) {
    return param->path;
}

void tmplpro_set_option_path(struct tmplpro_param* param, char** val) {
    param->path=val;
}

int tmplpro_get_option_die_on_bad_params(struct tmplpro_param* param) {
    return (int) param->die_on_bad_params;
}

void tmplpro_set_option_die_on_bad_params(struct tmplpro_param* param, int val) {
    param->die_on_bad_params=(flag)val;
}

writer_functype tmplpro_get_option_WriterFuncPtr(struct tmplpro_param* param) {
    return param->WriterFuncPtr;
}

void tmplpro_set_option_WriterFuncPtr(struct tmplpro_param* param, writer_functype val) {
    param->WriterFuncPtr=val;
}

get_ABSTRACT_VALUE_functype tmplpro_get_option_GetAbstractValFuncPtr(struct tmplpro_param* param) {
    return param->GetAbstractValFuncPtr;
}

void tmplpro_set_option_GetAbstractValFuncPtr(struct tmplpro_param* param, get_ABSTRACT_VALUE_functype val) {
    param->GetAbstractValFuncPtr=val;
}

ABSTRACT_VALUE2PSTRING_functype tmplpro_get_option_AbstractVal2pstringFuncPtr(struct tmplpro_param* param) {
    return param->AbstractVal2pstringFuncPtr;
}

void tmplpro_set_option_AbstractVal2pstringFuncPtr(struct tmplpro_param* param, ABSTRACT_VALUE2PSTRING_functype val) {
    param->AbstractVal2pstringFuncPtr=val;
}

ABSTRACT_VALUE2ABSTRACT_ARRAY_functype tmplpro_get_option_AbstractVal2abstractArrayFuncPtr(struct tmplpro_param* param) {
    return param->AbstractVal2abstractArrayFuncPtr;
}

void tmplpro_set_option_AbstractVal2abstractArrayFuncPtr(struct tmplpro_param* param, ABSTRACT_VALUE2ABSTRACT_ARRAY_functype val) {
    param->AbstractVal2abstractArrayFuncPtr=val;
}

get_ABSTRACT_ARRAY_length_functype tmplpro_get_option_GetAbstractArrayLengthFuncPtr(struct tmplpro_param* param) {
    return param->GetAbstractArrayLengthFuncPtr;
}

void tmplpro_set_option_GetAbstractArrayLengthFuncPtr(struct tmplpro_param* param, get_ABSTRACT_ARRAY_length_functype val) {
    param->GetAbstractArrayLengthFuncPtr=val;
}

get_ABSTRACT_MAP_functype tmplpro_get_option_GetAbstractMapFuncPtr(struct tmplpro_param* param) {
    return param->GetAbstractMapFuncPtr;
}

void tmplpro_set_option_GetAbstractMapFuncPtr(struct tmplpro_param* param, get_ABSTRACT_MAP_functype val) {
    param->GetAbstractMapFuncPtr=val;
}

is_ABSTRACT_VALUE_true_functype tmplpro_get_option_IsAbstractValTrueFuncPtr(struct tmplpro_param* param) {
    return param->IsAbstractValTrueFuncPtr;
}

void tmplpro_set_option_IsAbstractValTrueFuncPtr(struct tmplpro_param* param, is_ABSTRACT_VALUE_true_functype val) {
    param->IsAbstractValTrueFuncPtr=val;
}

find_file_functype tmplpro_get_option_FindFileFuncPtr(struct tmplpro_param* param) {
    return param->FindFileFuncPtr;
}

void tmplpro_set_option_FindFileFuncPtr(struct tmplpro_param* param, find_file_functype val) {
    param->FindFileFuncPtr=val;
}

load_file_functype tmplpro_get_option_LoadFileFuncPtr(struct tmplpro_param* param) {
    return param->LoadFileFuncPtr;
}

void tmplpro_set_option_LoadFileFuncPtr(struct tmplpro_param* param, load_file_functype val) {
    param->LoadFileFuncPtr=val;
}

unload_file_functype tmplpro_get_option_UnloadFileFuncPtr(struct tmplpro_param* param) {
    return param->UnloadFileFuncPtr;
}

void tmplpro_set_option_UnloadFileFuncPtr(struct tmplpro_param* param, unload_file_functype val) {
    param->UnloadFileFuncPtr=val;
}

select_loop_scope_functype tmplpro_get_option_SelectLoopScopeFuncPtr(struct tmplpro_param* param) {
    return param->SelectLoopScopeFuncPtr;
}

void tmplpro_set_option_SelectLoopScopeFuncPtr(struct tmplpro_param* param, select_loop_scope_functype val) {
    param->SelectLoopScopeFuncPtr=val;
}

end_loop_functype tmplpro_get_option_EndLoopFuncPtr(struct tmplpro_param* param) {
    return param->EndLoopFuncPtr;
}

void tmplpro_set_option_EndLoopFuncPtr(struct tmplpro_param* param, end_loop_functype val) {
    param->EndLoopFuncPtr=val;
}

ABSTRACT_MAP* tmplpro_get_option_root_param_map(struct tmplpro_param* param) {
    return param->root_param_map;
}

void tmplpro_set_option_root_param_map(struct tmplpro_param* param, ABSTRACT_MAP* val) {
    param->root_param_map=val;
}

ABSTRACT_WRITER* tmplpro_get_option_ext_writer_state(struct tmplpro_param* param) {
    return param->ext_writer_state;
}

void tmplpro_set_option_ext_writer_state(struct tmplpro_param* param, ABSTRACT_WRITER* val) {
    param->ext_writer_state=val;
}

ABSTRACT_FILTER* tmplpro_get_option_ext_filter_state(struct tmplpro_param* param) {
    return param->ext_filter_state;
}

void tmplpro_set_option_ext_filter_state(struct tmplpro_param* param, ABSTRACT_FILTER* val) {
    param->ext_filter_state=val;
}

ABSTRACT_FINDFILE* tmplpro_get_option_ext_findfile_state(struct tmplpro_param* param) {
    return param->ext_findfile_state;
}

void tmplpro_set_option_ext_findfile_state(struct tmplpro_param* param, ABSTRACT_FINDFILE* val) {
    param->ext_findfile_state=val;
}

ABSTRACT_CALLER* tmplpro_get_option_ext_calluserfunc_state(struct tmplpro_param* param) {
    return param->ext_calluserfunc_state;
}

void tmplpro_set_option_ext_calluserfunc_state(struct tmplpro_param* param, ABSTRACT_CALLER* val) {
    param->ext_calluserfunc_state=val;
}

init_expr_arglist_functype tmplpro_get_option_InitExprArglistFuncPtr(struct tmplpro_param* param) {
    return param->InitExprArglistFuncPtr;
}

void tmplpro_set_option_InitExprArglistFuncPtr(struct tmplpro_param* param, init_expr_arglist_functype val) {
    param->InitExprArglistFuncPtr=val;
}

free_expr_arglist_functype tmplpro_get_option_FreeExprArglistFuncPtr(struct tmplpro_param* param) {
    return param->FreeExprArglistFuncPtr;
}

void tmplpro_set_option_FreeExprArglistFuncPtr(struct tmplpro_param* param, free_expr_arglist_functype val) {
    param->FreeExprArglistFuncPtr=val;
}

push_expr_arglist_functype tmplpro_get_option_PushExprArglistFuncPtr(struct tmplpro_param* param) {
    return param->PushExprArglistFuncPtr;
}

void tmplpro_set_option_PushExprArglistFuncPtr(struct tmplpro_param* param, push_expr_arglist_functype val) {
    param->PushExprArglistFuncPtr=val;
}

call_expr_userfnc_functype tmplpro_get_option_CallExprUserfncFuncPtr(struct tmplpro_param* param) {
    return param->CallExprUserfncFuncPtr;
}

void tmplpro_set_option_CallExprUserfncFuncPtr(struct tmplpro_param* param, call_expr_userfnc_functype val) {
    param->CallExprUserfncFuncPtr=val;
}

is_expr_userfnc_functype tmplpro_get_option_IsExprUserfncFuncPtr(struct tmplpro_param* param) {
    return param->IsExprUserfncFuncPtr;
}

void tmplpro_set_option_IsExprUserfncFuncPtr(struct tmplpro_param* param, is_expr_userfnc_functype val) {
    param->IsExprUserfncFuncPtr=val;
}

ABSTRACT_FUNCMAP* tmplpro_get_option_expr_func_map(struct tmplpro_param* param) {
    return param->expr_func_map;
}

void tmplpro_set_option_expr_func_map(struct tmplpro_param* param, ABSTRACT_FUNCMAP* val) {
    param->expr_func_map=val;
}

ABSTRACT_ARGLIST* tmplpro_get_option_expr_func_arglist(struct tmplpro_param* param) {
    return param->expr_func_arglist;
}

void tmplpro_set_option_expr_func_arglist(struct tmplpro_param* param, ABSTRACT_ARGLIST* val) {
    param->expr_func_arglist=val;
}
