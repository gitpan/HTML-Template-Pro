/* -*- c -*- 
 * File: expr.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jun  6 14:54:45 2005
 *
 * $Id: expr.h,v 1.2 2005/07/07 07:03:28 igor Exp $
 */

void expr_init(void);
void expr_free(void);
PSTRING parse_expr(PSTRING line, struct tmplpro_param* param);
