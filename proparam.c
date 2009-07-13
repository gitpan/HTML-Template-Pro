/* generated; do not edit */
#include "procore.h"

int get_option_global_vars(struct tmplpro_param* param) {
    return param->global_vars;
}

void set_option_global_vars(struct tmplpro_param* param, int val) {
    param->global_vars=val;
}

int get_option_max_includes(struct tmplpro_param* param) {
    return param->max_includes;
}

void set_option_max_includes(struct tmplpro_param* param, int val) {
    param->max_includes=val;
}

int get_option_debug(struct tmplpro_param* param) {
    return param->debug;
}

void set_option_debug(struct tmplpro_param* param, int val) {
    param->debug=val;
}

int get_option_no_includes(struct tmplpro_param* param) {
    return (int) param->no_includes;
}

void set_option_no_includes(struct tmplpro_param* param, int val) {
    param->no_includes=(flag)val;
}

int get_option_case_sensitive(struct tmplpro_param* param) {
    return (int) param->case_sensitive;
}

void set_option_case_sensitive(struct tmplpro_param* param, int val) {
    param->case_sensitive=(flag)val;
}

int get_option_loop_context_vars(struct tmplpro_param* param) {
    return (int) param->loop_context_vars;
}

void set_option_loop_context_vars(struct tmplpro_param* param, int val) {
    param->loop_context_vars=(flag)val;
}

int get_option_strict(struct tmplpro_param* param) {
    return (int) param->strict;
}

void set_option_strict(struct tmplpro_param* param, int val) {
    param->strict=(flag)val;
}

int get_option_filters(struct tmplpro_param* param) {
    return (int) param->filters;
}

void set_option_filters(struct tmplpro_param* param, int val) {
    param->filters=(flag)val;
}

int get_option_default_escape(struct tmplpro_param* param) {
    return param->default_escape;
}

void set_option_default_escape(struct tmplpro_param* param, int val) {
    param->default_escape=val;
}

const char* get_option_filename(struct tmplpro_param* param) {
    return param->filename;
}

void set_option_filename(struct tmplpro_param* param, const char* val) {
    param->filename=val;
}

PSTRING get_option_scalarref(struct tmplpro_param* param) {
    return param->scalarref;
}

void set_option_scalarref(struct tmplpro_param* param, PSTRING val) {
    param->scalarref=val;
}

int get_option_path_like_variable_scope(struct tmplpro_param* param) {
    return (int) param->path_like_variable_scope;
}

void set_option_path_like_variable_scope(struct tmplpro_param* param, int val) {
    param->path_like_variable_scope=(flag)val;
}

int get_option_search_path_on_include(struct tmplpro_param* param) {
    return (int) param->search_path_on_include;
}

void set_option_search_path_on_include(struct tmplpro_param* param, int val) {
    param->search_path_on_include=(flag)val;
}

char** get_option_path(struct tmplpro_param* param) {
    return param->path;
}

void set_option_path(struct tmplpro_param* param, char** val) {
    param->path=val;
}

int get_option_die_on_bad_params(struct tmplpro_param* param) {
    return (int) param->die_on_bad_params;
}

void set_option_die_on_bad_params(struct tmplpro_param* param, int val) {
    param->die_on_bad_params=(flag)val;
}

writer_functype get_option_WriterFuncPtr(struct tmplpro_param* param) {
    return param->WriterFuncPtr;
}

void set_option_WriterFuncPtr(struct tmplpro_param* param, writer_functype val) {
    param->WriterFuncPtr=val;
}

get_ABSTRACT_VALUE_functype get_option_GetAbstractValFuncPtr(struct tmplpro_param* param) {
    return param->GetAbstractValFuncPtr;
}

void set_option_GetAbstractValFuncPtr(struct tmplpro_param* param, get_ABSTRACT_VALUE_functype val) {
    param->GetAbstractValFuncPtr=val;
}

ABSTRACT_VALUE2PSTRING_functype get_option_AbstractVal2pstringFuncPtr(struct tmplpro_param* param) {
    return param->AbstractVal2pstringFuncPtr;
}

void set_option_AbstractVal2pstringFuncPtr(struct tmplpro_param* param, ABSTRACT_VALUE2PSTRING_functype val) {
    param->AbstractVal2pstringFuncPtr=val;
}

ABSTRACT_VALUE2ABSTRACT_ARRAY_functype get_option_AbstractVal2abstractArrayFuncPtr(struct tmplpro_param* param) {
    return param->AbstractVal2abstractArrayFuncPtr;
}

void set_option_AbstractVal2abstractArrayFuncPtr(struct tmplpro_param* param, ABSTRACT_VALUE2ABSTRACT_ARRAY_functype val) {
    param->AbstractVal2abstractArrayFuncPtr=val;
}

get_ABSTRACT_ARRAY_length_functype get_option_GetAbstractArrayLengthFuncPtr(struct tmplpro_param* param) {
    return param->GetAbstractArrayLengthFuncPtr;
}

void set_option_GetAbstractArrayLengthFuncPtr(struct tmplpro_param* param, get_ABSTRACT_ARRAY_length_functype val) {
    param->GetAbstractArrayLengthFuncPtr=val;
}

get_ABSTRACT_MAP_functype get_option_GetAbstractMapFuncPtr(struct tmplpro_param* param) {
    return param->GetAbstractMapFuncPtr;
}

void set_option_GetAbstractMapFuncPtr(struct tmplpro_param* param, get_ABSTRACT_MAP_functype val) {
    param->GetAbstractMapFuncPtr=val;
}

is_ABSTRACT_VALUE_true_functype get_option_IsAbstractValTrueFuncPtr(struct tmplpro_param* param) {
    return param->IsAbstractValTrueFuncPtr;
}

void set_option_IsAbstractValTrueFuncPtr(struct tmplpro_param* param, is_ABSTRACT_VALUE_true_functype val) {
    param->IsAbstractValTrueFuncPtr=val;
}

find_file_functype get_option_FindFileFuncPtr(struct tmplpro_param* param) {
    return param->FindFileFuncPtr;
}

void set_option_FindFileFuncPtr(struct tmplpro_param* param, find_file_functype val) {
    param->FindFileFuncPtr=val;
}

load_file_functype get_option_LoadFileFuncPtr(struct tmplpro_param* param) {
    return param->LoadFileFuncPtr;
}

void set_option_LoadFileFuncPtr(struct tmplpro_param* param, load_file_functype val) {
    param->LoadFileFuncPtr=val;
}

unload_file_functype get_option_UnloadFileFuncPtr(struct tmplpro_param* param) {
    return param->UnloadFileFuncPtr;
}

void set_option_UnloadFileFuncPtr(struct tmplpro_param* param, unload_file_functype val) {
    param->UnloadFileFuncPtr=val;
}

init_expr_arglist_functype get_option_InitExprArglistFuncPtr(struct tmplpro_param* param) {
    return param->InitExprArglistFuncPtr;
}

void set_option_InitExprArglistFuncPtr(struct tmplpro_param* param, init_expr_arglist_functype val) {
    param->InitExprArglistFuncPtr=val;
}

free_expr_arglist_functype get_option_FreeExprArglistFuncPtr(struct tmplpro_param* param) {
    return param->FreeExprArglistFuncPtr;
}

void set_option_FreeExprArglistFuncPtr(struct tmplpro_param* param, free_expr_arglist_functype val) {
    param->FreeExprArglistFuncPtr=val;
}

push_expr_arglist_functype get_option_PushExprArglistFuncPtr(struct tmplpro_param* param) {
    return param->PushExprArglistFuncPtr;
}

void set_option_PushExprArglistFuncPtr(struct tmplpro_param* param, push_expr_arglist_functype val) {
    param->PushExprArglistFuncPtr=val;
}

call_expr_userfnc_functype get_option_CallExprUserfncFuncPtr(struct tmplpro_param* param) {
    return param->CallExprUserfncFuncPtr;
}

void set_option_CallExprUserfncFuncPtr(struct tmplpro_param* param, call_expr_userfnc_functype val) {
    param->CallExprUserfncFuncPtr=val;
}

is_expr_userfnc_functype get_option_IsExprUserfncFuncPtr(struct tmplpro_param* param) {
    return param->IsExprUserfncFuncPtr;
}

void set_option_IsExprUserfncFuncPtr(struct tmplpro_param* param, is_expr_userfnc_functype val) {
    param->IsExprUserfncFuncPtr=val;
}

ABSTRACT_FUNCMAP* get_option_expr_func_map(struct tmplpro_param* param) {
    return param->expr_func_map;
}

void set_option_expr_func_map(struct tmplpro_param* param, ABSTRACT_FUNCMAP* val) {
    param->expr_func_map=val;
}

ABSTRACT_ARGLIST* get_option_expr_func_arglist(struct tmplpro_param* param) {
    return param->expr_func_arglist;
}

void set_option_expr_func_arglist(struct tmplpro_param* param, ABSTRACT_ARGLIST* val) {
    param->expr_func_arglist=val;
}

ABSTRACT_MAP* get_option_root_param_map(struct tmplpro_param* param) {
    return param->root_param_map;
}

void set_option_root_param_map(struct tmplpro_param* param, ABSTRACT_MAP* val) {
    param->root_param_map=val;
}
