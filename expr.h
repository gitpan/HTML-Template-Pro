/* -*- c -*- 
 * File: expr.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jun  6 14:54:45 2005
 *
 * $Id$
 */

void expr_init(void);
void expr_free(void);
PSTRING parse_expr(PSTRING line, struct tmplpro_param* param);
