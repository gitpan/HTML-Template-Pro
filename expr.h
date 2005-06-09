/* -*- c -*- 
 * File: expr.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jun  6 14:54:45 2005
 *
 * $Id: expr.h,v 1.1 2005/06/09 08:46:42 igor Exp $
 */

void expr_init(void);
void expr_free(void);
double parse_expr(PSTRING line, struct tmplpro_param* param);
