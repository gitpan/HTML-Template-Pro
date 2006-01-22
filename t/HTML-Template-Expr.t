# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl HTML-Template-Pro.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test;
BEGIN { plan tests => 1+2*5 };
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
HTML::Template::Pro->register_function('arglist'=>sub { return '['.join('][',@_).']'; });

my @exprset1=(ONE=>1,TWO=>2,THREE=>3,ZERO=>0,MINUSTEN=>-10, FILE=>'test_if1.tmpl');
my @brunoext=('foo.bar'=>'<test passed>');

test_tmpl('test_expr1', @exprset1);
test_tmpl('test_expr2', @exprset1);
test_tmpl('test_expr3', @exprset1);
test_tmpl('test_expr4', @brunoext);
test_tmpl('test_expr5', @exprset1);


# -------------------------

sub test_tmpl {
    my $testname=shift;
    my $tmpl;
    print "\n--------------- Test: $testname ---------------------\n";
    chdir 'templates-Pro';
    my $file=$testname;
    $tmpl=HTML::Template::Pro->new(filename=>$file.'.tmpl', loop_context_vars=>1, case_sensitive=>0,debug=>$DEBUG, functions=>{'hello' => sub { return "hi, $_[0]!" }});
    $tmpl->param(@_);
    &dryrun($tmpl,$file);
    chdir '..';
}

sub dryrun {
    my $tmpl=shift;
    my $file=shift;
    open (OUTFILE, ">$file.raw") || die "can't open $file.raw: $!";
    binmode (OUTFILE);
    $tmpl->output(print_to => *OUTFILE);
    close (OUTFILE) || die "can't close $file.raw: $!";
    my $files_equal=&catfile("$file.raw") eq &catfile("$file.out");
    if ($files_equal) {
	ok($files_equal) && unlink "$file.raw";
    } else {
	if (-x '/usr/bin/diff') {
	    print STDERR `diff -u $file.out $file.raw`;
	} else {
	    print STDERR "# >>> ---$file.raw---\n$output\n>>> ---end $file.raw---\n";
	}
    }
    my $output=$tmpl->output();
    ok (defined $output and $output eq &catfile("$file.out"));
}

sub catfile {
    my $file=shift;
    open (INFILE, $file) || die "can't open $file: $!";
    binmode (INFILE);
    local $/;
    my $catfile=<INFILE>;
    close (INFILE) || die "can't close $file: $!";
    return $catfile;
}


### Local Variables: 
### mode: perl
### End: 
