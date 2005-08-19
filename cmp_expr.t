#!/usr/bin/perl -w

use HTML::Template;
use HTML::Template::Expr;
use HTML::Template::Pro;

while (<>) {
	chomp;
	my $tmplline='<tmpl_var expr="'.$_.'">';
	print 'tmpl:',$tmplline,"\n";
	eval {
	    print 'Pro:',"\n";
	    my $tmpl1=HTML::Template::Pro->new(scalarref => \$tmplline);
	    print 'Output:',$tmpl1->output(),"\n";
	};
	print 'Pro: failed with:',"\n",$@,"\n" if $@;
	eval {
	    print 'Expr:',"\n";
	    my $tmpl2=HTML::Template::Expr->new(scalarref => \$tmplline);
	    print 'Output:',$tmpl2->output(),"\n";
	};
	print 'Expr: failed with:',"\n",$@,"\n" if $@;
}

