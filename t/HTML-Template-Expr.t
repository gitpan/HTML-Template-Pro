# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl HTML-Template-Pro.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test;
BEGIN { plan tests => 1+2*4 };
#use HTML::Template;
use HTML::Template::Pro;
ok(1); # If we made it this far, we're ok.

#########################

my $tmpl;
my $output;

my $DEBUG=$ENV{HTMLTEMPLATEPRODEBUG};
$DEBUG||=0;

HTML::Template::Pro->register_function('registered_func'=>sub { return shift(); });
HTML::Template::Pro->register_function('hello_string'=>sub { return 'hello!'; });

my @exprset1=(ONE=>1,TWO=>2,THREE=>3,ZERO=>0);
my @brunoext=('foo.bar'=>'<test passed>');

test_tmpl('test_expr1', @exprset1);
test_tmpl('test_expr2', @exprset1);
test_tmpl('test_expr3', @exprset1);
test_tmpl('test_expr4', @brunoext);


# -------------------------

sub test_tmpl {
    my $testname=shift;
    my $tmpl;
    my $output;
    print "\n--------------- Test: $testname ---------------------\n";
    chdir 'templates-Pro';
    my $file=$testname;
#    $tmpl=HTML::Template->new(filename=>$file.'.tmpl', die_on_bad_params=>0, strict=>0);
    $tmpl=HTML::Template::Pro->new(filename=>$file.'.tmpl', loop_context_vars=>1, case_sensitive=>0,debug=>$DEBUG, functions=>{'hello' => sub { return "hi, $_[0]!" }});
    open (OUTFILE, ">$file.raw");
    $tmpl->param(@_);
    $tmpl->output(print_to => *OUTFILE);
    close (OUTFILE);
    ok(`cat $file.raw` eq `cat $file.out`) && unlink "$file.raw";
    $output=$tmpl->output();
    ok (defined $output and $output eq `cat $file.out`);
    chdir '..';
}

### Local Variables: 
### mode: perl
### End: 
