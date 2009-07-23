/* generated; do not edit */
/*
 * public interface to set up libhtmltmplpro options
 */
struct tmplpro_param;

int tmplpro_get_option_global_vars(struct tmplpro_param*);
void tmplpro_set_option_global_vars(struct tmplpro_param*,int);

int tmplpro_get_option_max_includes(struct tmplpro_param*);
void tmplpro_set_option_max_includes(struct tmplpro_param*,int);

int tmplpro_get_option_debug(struct tmplpro_param*);
void tmplpro_set_option_debug(struct tmplpro_param*,int);

int tmplpro_get_option_no_includes(struct tmplpro_param*);
void tmplpro_set_option_no_includes(struct tmplpro_param*,int);

int tmplpro_get_option_case_sensitive(struct tmplpro_param*);
void tmplpro_set_option_case_sensitive(struct tmplpro_param*,int);

int tmplpro_get_option_loop_context_vars(struct tmplpro_param*);
void tmplpro_set_option_loop_context_vars(struct tmplpro_param*,int);

int tmplpro_get_option_strict(struct tmplpro_param*);
void tmplpro_set_option_strict(struct tmplpro_param*,int);

int tmplpro_get_option_filters(struct tmplpro_param*);
void tmplpro_set_option_filters(struct tmplpro_param*,int);

int tmplpro_get_option_default_escape(struct tmplpro_param*);
void tmplpro_set_option_default_escape(struct tmplpro_param*,int);

const char* tmplpro_get_option_filename(struct tmplpro_param*);
void tmplpro_set_option_filename(struct tmplpro_param*,const char*);

PSTRING tmplpro_get_option_scalarref(struct tmplpro_param*);
void tmplpro_set_option_scalarref(struct tmplpro_param*,PSTRING);

int tmplpro_get_option_path_like_variable_scope(struct tmplpro_param*);
void tmplpro_set_option_path_like_variable_scope(struct tmplpro_param*,int);

int tmplpro_get_option_search_path_on_include(struct tmplpro_param*);
void tmplpro_set_option_search_path_on_include(struct tmplpro_param*,int);

char** tmplpro_get_option_path(struct tmplpro_param*);
void tmplpro_set_option_path(struct tmplpro_param*,char**);

int tmplpro_get_option_die_on_bad_params(struct tmplpro_param*);
void tmplpro_set_option_die_on_bad_params(struct tmplpro_param*,int);

writer_functype tmplpro_get_option_WriterFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_WriterFuncPtr(struct tmplpro_param*,writer_functype);

get_ABSTRACT_VALUE_functype tmplpro_get_option_GetAbstractValFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_GetAbstractValFuncPtr(struct tmplpro_param*,get_ABSTRACT_VALUE_functype);

ABSTRACT_VALUE2PSTRING_functype tmplpro_get_option_AbstractVal2pstringFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_AbstractVal2pstringFuncPtr(struct tmplpro_param*,ABSTRACT_VALUE2PSTRING_functype);

ABSTRACT_VALUE2ABSTRACT_ARRAY_functype tmplpro_get_option_AbstractVal2abstractArrayFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_AbstractVal2abstractArrayFuncPtr(struct tmplpro_param*,ABSTRACT_VALUE2ABSTRACT_ARRAY_functype);

get_ABSTRACT_ARRAY_length_functype tmplpro_get_option_GetAbstractArrayLengthFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_GetAbstractArrayLengthFuncPtr(struct tmplpro_param*,get_ABSTRACT_ARRAY_length_functype);

get_ABSTRACT_MAP_functype tmplpro_get_option_GetAbstractMapFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_GetAbstractMapFuncPtr(struct tmplpro_param*,get_ABSTRACT_MAP_functype);

is_ABSTRACT_VALUE_true_functype tmplpro_get_option_IsAbstractValTrueFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_IsAbstractValTrueFuncPtr(struct tmplpro_param*,is_ABSTRACT_VALUE_true_functype);

find_file_functype tmplpro_get_option_FindFileFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_FindFileFuncPtr(struct tmplpro_param*,find_file_functype);

load_file_functype tmplpro_get_option_LoadFileFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_LoadFileFuncPtr(struct tmplpro_param*,load_file_functype);

unload_file_functype tmplpro_get_option_UnloadFileFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_UnloadFileFuncPtr(struct tmplpro_param*,unload_file_functype);

select_loop_scope_functype tmplpro_get_option_SelectLoopScopeFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_SelectLoopScopeFuncPtr(struct tmplpro_param*,select_loop_scope_functype);

end_loop_functype tmplpro_get_option_EndLoopFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_EndLoopFuncPtr(struct tmplpro_param*,end_loop_functype);

ABSTRACT_MAP* tmplpro_get_option_root_param_map(struct tmplpro_param*);
void tmplpro_set_option_root_param_map(struct tmplpro_param*,ABSTRACT_MAP*);

ABSTRACT_WRITER* tmplpro_get_option_ext_writer_state(struct tmplpro_param*);
void tmplpro_set_option_ext_writer_state(struct tmplpro_param*,ABSTRACT_WRITER*);

ABSTRACT_FILTER* tmplpro_get_option_ext_filter_state(struct tmplpro_param*);
void tmplpro_set_option_ext_filter_state(struct tmplpro_param*,ABSTRACT_FILTER*);

ABSTRACT_FINDFILE* tmplpro_get_option_ext_findfile_state(struct tmplpro_param*);
void tmplpro_set_option_ext_findfile_state(struct tmplpro_param*,ABSTRACT_FINDFILE*);

ABSTRACT_CALLER* tmplpro_get_option_ext_calluserfunc_state(struct tmplpro_param*);
void tmplpro_set_option_ext_calluserfunc_state(struct tmplpro_param*,ABSTRACT_CALLER*);

init_expr_arglist_functype tmplpro_get_option_InitExprArglistFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_InitExprArglistFuncPtr(struct tmplpro_param*,init_expr_arglist_functype);

free_expr_arglist_functype tmplpro_get_option_FreeExprArglistFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_FreeExprArglistFuncPtr(struct tmplpro_param*,free_expr_arglist_functype);

push_expr_arglist_functype tmplpro_get_option_PushExprArglistFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_PushExprArglistFuncPtr(struct tmplpro_param*,push_expr_arglist_functype);

call_expr_userfnc_functype tmplpro_get_option_CallExprUserfncFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_CallExprUserfncFuncPtr(struct tmplpro_param*,call_expr_userfnc_functype);

is_expr_userfnc_functype tmplpro_get_option_IsExprUserfncFuncPtr(struct tmplpro_param*);
void tmplpro_set_option_IsExprUserfncFuncPtr(struct tmplpro_param*,is_expr_userfnc_functype);

ABSTRACT_FUNCMAP* tmplpro_get_option_expr_func_map(struct tmplpro_param*);
void tmplpro_set_option_expr_func_map(struct tmplpro_param*,ABSTRACT_FUNCMAP*);

ABSTRACT_ARGLIST* tmplpro_get_option_expr_func_arglist(struct tmplpro_param*);
void tmplpro_set_option_expr_func_arglist(struct tmplpro_param*,ABSTRACT_ARGLIST*);

