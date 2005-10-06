use Test::More qw(no_plan);
use_ok('HTML::Template::Pro');
my $t = HTML::Template::Pro->new( filename => 'templates-Pro/test_malloc.tmpl' , debug=>0);
for($x=250;$x<280;$x++) {
    my $txt='x'x$x;
    $t->param('text' => $txt );
    ok($t->output eq ($txt . "\n")) ;
}
