use 5.005;
use ExtUtils::MakeMaker;
$Verbose = 1;

$O_FILES=qw/procore.o pstack.o pbuffer.o proscope.o calc.o expr.o pstring.o exprtool.o 
   loadfile.o tmpllog.o/;

my $INC='-I.';
my $DEFINE='';
my $LIBS='-lm';

my $PCRE=1;
my $DEBUG=0;
$PCRE=0 if $^O =~ /win/i;

my $i;
for ($i=0; $i<@ARGV; $i++)  {
    if ($ARGV[$i]=~/PCRE=(.+)/) {
	$PCRE=$1;
    } elsif ($ARGV[$i]=~/DEBUG=(.+)/) {
	$DEBUG=$1;
    } else {
	next;
    }
    splice @ARGV, $i, 1;
}

$DEFINE.=' -DDEBUG' if ($DEBUG);

if ($PCRE) {
    $INC.=' -I/usr/include/pcre';
    $DEFINE.=' -DHAVE_PCRE';
    $LIBS.=' -lpcre';
    print "
=====================================================================
build with libpcre (Stanislav Yadykin's regexp extension) is enabled.
=====================================================================
if you have not installed libpcre, you still can build the module 
without libpcre (with this extension disabled) using 
perl Makefile.PL PCRE=0
";
} else {
    print "
=================================================
build with libpcre is disabled.
=================================================
Stanislav Yadykin's regexp extension is disabled.
";
}

WriteMakefile(
    NAME              => 'HTML::Template::Pro',
    VERSION_FROM      => 'lib/HTML/Template/Pro.pm', # finds $VERSION
    PREREQ_PM         => {'File::Spec' => 0}, # e.g., Module::Name => 1.1
    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
      (ABSTRACT_FROM  => 'lib/HTML/Template/Pro.pm', # retrieve abstract from module
       AUTHOR         => 'I. Yu. Vlasenko <viy@altlinux.org>') : ()),
    LIBS              => [$LIBS],
    DEFINE            => $DEFINE, # e.g., '-DHAVE_SOMETHING'
    INC               => $INC, 
    OBJECT            => '$(O_FILES)', # link all the C files too
#   PERL_MALLOC_OK    => 1 # read first
);

sub MY::postamble {
    return <<'MAKE_FRAG';
rpm: dist
	rpmbuild -ta --clean --target i586-alt-linux HTML-Template-Pro-$(VERSION).tar.gz

index.html: README Makefile
	echo '<html><body><p><a href="http://sourceforge.net"><img src="http://sourceforge.net/sflogo.php?group_id=138507&amp;type=5" width="210" height="62" border="0" alt="SourceForge.net Logo" /></a><br> <a href="http://sourceforge.net/projects/html-tmpl-pro/">SourceForge Summary page of the project</a><br><a href="http://www.imath.kiev.ua/~vlasenko/">Home page of the author</a><br><a href="http://prdownloads.sourceforge.net/html-tmpl-pro/HTML-Template-Pro-${VERSION}.tar.gz?download">Download</a><p><pre>' > $@
	cat $< >> $@
	echo '</pre></body></html>' >> $@


MAKE_FRAG
}

