# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl HTML-Template-Pro.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test;
BEGIN { plan tests => 1+2*16 };
#use HTML::Template;
use HTML::Template::Pro;
ok(1); # If we made it this far, we're ok.

#########################

my $tmpl;
my $output;

my $DEBUG=$ENV{HTMLTEMPLATEPRODEBUG};
$DEBUG||=0;

my @varset1=(VAR1=>VAR1,VAR2=>VAR2,VAR3=>VAR3,VAR10=>VAR10);
my @varset2=(STUFF1 => '\<>"; %FA'."hidden:\r\nend");
my @refset1=(
HASHREF0=>[],
HASHREF2=>[{},{}],
HASHREF1=>[
{LOOPVAR1=>'LOOP1-VAR1',LOOPVAR2=>'LOOP1-VAR2',LOOPVAR3=>'LOOP1-VAR3',LOOPVAR10=>'LOOP1-VAR10'},
{LOOPVAR1=>'LOOP2-VAR1',LOOPVAR2=>'LOOP2-VAR2',LOOPVAR3=>'LOOP2-VAR3',LOOPVAR10=>'LOOP2-VAR10'},
{LOOPVAR1=>'LOOP3-VAR1',LOOPVAR2=>'LOOP3-VAR2',LOOPVAR3=>'LOOP3-VAR3',LOOPVAR10=>'LOOP3-VAR10'},
{LOOPVAR1=>'LOOP4-VAR1',LOOPVAR2=>'LOOP4-VAR2',LOOPVAR3=>'LOOP4-VAR3',LOOPVAR10=>'LOOP4-VAR10'},
]);

if ($ENV{HTMLTEMPLATEPROBROKEN}) {
    # manual test
    test_tmpl('test_broken', @varset1, @refset1);
}

test_tmpl('test_var1', @varset1);
test_tmpl('test_var2', @varset1);
test_tmpl('test_var3', @varset1, @varset2);
test_tmpl('test_if1',  @varset1);
test_tmpl('test_if2',  @varset1);
test_tmpl('test_if3',  @refset1);
test_tmpl('test_if4',  @varset1);
test_tmpl('test_include1', @varset1);
test_tmpl('test_include2', @varset1);
test_tmpl('test_include3', @varset1);
test_tmpl('test_loop1', @varset1, @refset1);
test_tmpl('test_loop2', @varset1, @refset1);
test_tmpl('test_loop3', @varset1, @refset1);
test_tmpl('test_loop4', @varset1, @refset1);
test_tmpl('test_loop5', @varset1, @refset1);

close (STDERR);
open (STDERR, '>/dev/null');
test_tmpl('test_broken1', @varset1, @refset1);
# not a test -- to see warnings on broken tmpl
# test_tmpl('test_broken', @varset1, @refset1);


# -------------------------

sub test_tmpl {
    my $testname=shift;
    my $tmpl;
    my $output;
    print "\n--------------- Test: $testname ---------------------\n";
    chdir 'templates-Pro';
    my $file=$testname;
#    $tmpl=HTML::Template->new(filename=>$file.'.tmpl', die_on_bad_params=>0, strict=>0);
    $tmpl=HTML::Template::Pro->new(filename=>$file.'.tmpl', loop_context_vars=>1, case_sensitive=>0,debug=>$DEBUG);
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
