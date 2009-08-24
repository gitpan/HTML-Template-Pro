/* 
 * File: callback_stubs.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Tue Jul 14 22:47:11 2009
 */

/* writer_functype stub */
/* endnext points on next character to end of interval as in c++ */
static void BACKCALL stub_write_chars_to_stdout (ABSTRACT_WRITER* none,const char* begin, const char* endnext) {
  if (endnext==begin) return;
  /* register const char* i;
  for (i=begin; i<endnext; i++) putchar(*i); */
  if (0==fwrite(begin, sizeof(char), endnext-begin, stdout)) {
    tmpl_log(NULL,TMPL_LOG_ERROR,"find_file_func stub: can't fwrite to stdout\n");
  }
}

#ifdef WIN32
# define IS_FILE_SEP(X) ((X=='/') || (X=='\\'))
#else
# define IS_FILE_SEP(X) (X=='/')
#endif

static int _ff_exists(const char* path) {
  FILE *file_p;
  file_p = fopen(path, "r");
  if (file_p) {
      fclose(file_p);
      //tmpl_log(NULL,TMPL_LOG_ERROR,"_ff_exists: found [%s]\n",path);
      return 1;
    }
  //tmpl_log(NULL,TMPL_LOG_ERROR,"_ff_exists: not found [%s]\n",path);
  return 0;
}

/* lame dirname implementation */
static PSTRING _ff_dirname(const char* path) {
  PSTRING retval={(char*)path,(char*)path};
  char c=0;
  if (path!=NULL) retval.endnext += strlen(path);
  else return retval;
  while (retval.endnext > retval.begin && (c=*(--retval.endnext)) && ! IS_FILE_SEP(c));
  return retval;
}
/* just don't check it */
#define _ff_is_absolute(X) 1

/* remember about \\?\ and \\?\UNC\ prefixes on WIN platform.
 * see "File Names, Paths, and Namespaces"
 * http://msdn.microsoft.com/en-us/library/aa365247(VS.85).aspx
 */
#define _ff_canonical_path(X) (X)

static PSTRING _ff_add_pstr_to_buffer(PSTRING buf, PSTRING pstr) {
  PSTRING ret = buf;
  const char* s;
  //tmpl_log(NULL,TMPL_LOG_ERROR,"_ff_add_pstr_to_buffer: called as [%p,%p]+[%p,%p]\n",buf.begin,buf.endnext, pstr.begin,pstr.endnext);
  for (s=pstr.begin;s<pstr.endnext;s++) {*(ret.endnext++)=*s;}
  //tmpl_log(NULL,TMPL_LOG_ERROR,"_ff_add_pstr_to_buffer: ret = [%p,%p]\n",ret.begin,ret.endnext);
  return ret;
}
static PSTRING _ff_add_str_to_buffer(PSTRING buf, const char* str) {
  PSTRING ret = buf;
  const char* s=str;
  //tmpl_log(NULL,TMPL_LOG_ERROR,"_ff_add_str_to_buffer: called as [%p,%p]+[%s]\n",buf.begin,buf.endnext, str);
  while ('\0'!=*s) {*(ret.endnext++)=*s++;}
  //tmpl_log(NULL,TMPL_LOG_ERROR,"_ff_add_str_to_buffer: ret = [%p,%p]\n",ret.begin,ret.endnext);
  return ret;
}
static PSTRING _ff_add_sep_to_buffer(PSTRING buf) {
  PSTRING ret = buf;
  if (ret.endnext>ret.begin && IS_FILE_SEP(*(ret.endnext-1))) return ret;
  *(ret.endnext++)='/';
  return ret;
}
static PSTRING _ff_add_0_to_buffer(PSTRING buf) {
  PSTRING ret = buf;
  *(ret.endnext++)='\0';
  return ret;
}
static const char* _find_file (struct tmplpro_param* param, const char* filename, PSTRING extra_dir) {
  // TODO: finish it
  char* HTML_TEMPLATE_ROOT = getenv("HTML_TEMPLATE_ROOT");
  size_t HTML_TEMPLATE_ROOT_length=0;
  size_t buffsize=0;
  char** pathlist=param->path;
  PSTRING pbuf_begin, filepath;

  /* first check for a full path */
  if (_ff_is_absolute(filename) && _ff_exists(filename)) return _ff_canonical_path(filename);

  if (HTML_TEMPLATE_ROOT!=NULL) HTML_TEMPLATE_ROOT_length=strlen(HTML_TEMPLATE_ROOT);
  if (pathlist!=NULL) {
    while (NULL!=*pathlist) {
      size_t pathentrylen=strlen(*pathlist);
      if (buffsize<pathentrylen) buffsize=pathentrylen;
      pathlist++;
    }
  }
  /* bufsize is max possible length path of path considered
   * min is max_len(foreach pathlist)+HTML_TEMPLATE_ROOT_length+strlen(filename)+len(extra_dir)+1) 
   * but we malloc an extra space to avoid  frequent reallocing
   */
  buffsize+=HTML_TEMPLATE_ROOT_length+strlen(filename)+(extra_dir.endnext-extra_dir.begin)+4+1; /* 4 - for slashes */
  if (0==pbuffer_size(&param->builtin_findfile_buffer)) {
    pbuffer_init_as(&param->builtin_findfile_buffer, 3*buffsize);
  } else {
    pbuffer_resize(&param->builtin_findfile_buffer, buffsize);
  }
  pbuf_begin.begin=pbuffer_string(&param->builtin_findfile_buffer);
  pbuf_begin.endnext=pbuf_begin.begin;

  /* try the extra_path if one was specified */
  if (extra_dir.begin!=NULL) {
    filepath=_ff_add_pstr_to_buffer(pbuf_begin,extra_dir);
    filepath=_ff_add_sep_to_buffer(filepath);
    filepath=_ff_add_str_to_buffer(filepath,filename);
    filepath=_ff_add_0_to_buffer(filepath);
    if (_ff_exists(filepath.begin)) return _ff_canonical_path(filepath.begin);
  }

  /* try pre-prending HTML_Template_Root */
  if (HTML_TEMPLATE_ROOT!=NULL) {
    filepath=_ff_add_str_to_buffer(pbuf_begin,HTML_TEMPLATE_ROOT);
    filepath=_ff_add_sep_to_buffer(filepath);
    filepath=_ff_add_str_to_buffer(filepath,filename);
    filepath=_ff_add_0_to_buffer(filepath);
    if (_ff_exists(filepath.begin)) return _ff_canonical_path(filepath.begin);
  }

  /* try "path" option list.. */
  pathlist=param->path;
  if (pathlist!=NULL) {
    while (NULL!=*pathlist) {
      //tmpl_log(NULL,TMPL_LOG_ERROR,"try 'path' option list..: looking in [%s]\n",*pathlist);
      filepath=_ff_add_str_to_buffer(pbuf_begin,*pathlist);
      filepath=_ff_add_sep_to_buffer(filepath);
      filepath=_ff_add_str_to_buffer(filepath,filename);
      filepath=_ff_add_0_to_buffer(filepath);
      if (_ff_exists(filepath.begin)) return _ff_canonical_path(filepath.begin);
      pathlist++;
    }
  }

  /* try even a relative path from the current directory...*/
  if (_ff_exists(filename)) return _ff_canonical_path(filename);

  /* try "path" option list with HTML_TEMPLATE_ROOT prepended... */
  if (HTML_TEMPLATE_ROOT!=NULL) {
    pathlist=param->path;
    if (pathlist!=NULL) {
      while (NULL!=*pathlist) {
	filepath=_ff_add_str_to_buffer(pbuf_begin,HTML_TEMPLATE_ROOT);
	filepath=_ff_add_sep_to_buffer(filepath);
	filepath=_ff_add_str_to_buffer(filepath,*pathlist);
	filepath=_ff_add_sep_to_buffer(filepath);
	filepath=_ff_add_str_to_buffer(filepath,filename);
	filepath=_ff_add_0_to_buffer(filepath);
	if (_ff_exists(filepath.begin)) return _ff_canonical_path(filepath.begin);
	pathlist++;
      }
    }
  }

  return NULL;
} 

static const char* BACKCALL stub_find_file_func(ABSTRACT_FINDFILE* param,const char* filename, const char* last_visited_file) {
  const char* filepath;
  PSTRING extra_path ={NULL,NULL};

  if (filename == last_visited_file) tmpl_log(NULL,TMPL_LOG_ERROR,"built-in find_file: internal error: buffer clash for %s\n",filename);

  // look for the included file...
  if (last_visited_file!=NULL && ! ((struct tmplpro_param*) param)->search_path_on_include) {
    extra_path = _ff_dirname(last_visited_file);
  }
  filepath = _find_file((struct tmplpro_param*)param,filename,extra_path);
  if (filepath==NULL) {
    char** path=((struct tmplpro_param*)param)->path;
    tmpl_log(NULL,TMPL_LOG_ERROR,"built-in find_file: can't find file %s", filename);
    if (NULL!=last_visited_file) tmpl_log(NULL,TMPL_LOG_ERROR," (included from %s)", last_visited_file);
    if (NULL!=path) {
      tmpl_log(NULL,TMPL_LOG_ERROR," with path = [");
      while (NULL!=*path) {
	tmpl_log(NULL,TMPL_LOG_ERROR," '%s'",*path);
	path++;
      }
      tmpl_log(NULL,TMPL_LOG_ERROR," ]");
    } else {
      tmpl_log(NULL,TMPL_LOG_ERROR," with empty path list");
    }
    tmpl_log(NULL,TMPL_LOG_ERROR,"\n");
    return NULL;
  } else {
    return filepath;
  }
}


static ABSTRACT_USERFUNC* BACKCALL stub_is_expr_userfnc_func (ABSTRACT_FUNCMAP* af, PSTRING name) {
  tmpl_log(NULL,TMPL_LOG_ERROR,"is_expr_userfnc_func stub: EXPR is not initialized properly. user func dispatcher was not supplied.");
  return NULL;
}

static PSTRING BACKCALL stub_load_file_func (ABSTRACT_FILTER* none, const char* filename) {
  tmpl_log(NULL,TMPL_LOG_ERROR,"load_file_func stub: callback function for filters was not specified.");
  return mmap_load_file(filename);
}

static int BACKCALL stub_unload_file_func (ABSTRACT_FILTER* none, PSTRING memarea) {
  tmpl_log(NULL,TMPL_LOG_ERROR,"unload_file_func stub: callback function for filters was not specified.");
  return mmap_unload_file(memarea);
}

static int BACKCALL stub_get_ABSTRACT_ARRAY_length_func (ABSTRACT_ARRAY* none) {
  return -1; /* treat all arrays as arrays of undefined length */
}


/*
 *  Local Variables:
 *  mode: c
 *  End:
 */
