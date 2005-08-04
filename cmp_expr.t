#!/usr/bin/perl -w

use HTML::Template;
use HTML::Template::Expr;
use HTML::Template::Pro;

while (<>) {
	chomp;
	my $tmplline='<tmpl_var expr="'.$_.'">';
	my $tmpl1=HTML::Template::Pro->new(scalarref => \$tmplline);
	print 'tmpl:',$tmplline,"\n";
	print 'Pro:',$tmpl1->output(),"\n";
	my $tmpl2=HTML::Template::Expr->new(scalarref => \$tmplline);
	print 'Expr:',$tmpl2->output(),"\n";
}

