use 5.005;
use ExtUtils::MakeMaker;
$Verbose = 1;

$O_FILES=qw/procore.o pbuffer.o proscope.o expr.o pstring.o tmpllog.o/;
#  loadfile.o calc.o exprtool.o pstack.o

my $INC='-I.';
my $DEFINE='';
my $LIBS='-lm';

my $PCRE=1;
my $DEBUG=0;
my $MMAP=1;
#$PCRE=0 if $^O =~ /win/i;
$PCRE=has_pcre();


my $i;
for ($i=0; $i<@ARGV; $i++)  {
    if ($ARGV[$i]=~/PCRE=(.+)/) {
	$PCRE=$1;
    } elsif ($ARGV[$i]=~/DEBUG=(.+)/) {
	$DEBUG=$1;
    } elsif ($ARGV[$i]=~/MMAP=(.+)/) {
	$MMAP=$1;
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
expr.c: calc.inc exprtool.inc
procore\$(OBJ_EXT): loadfile.inc pstack.inc

rpm: dist
	rpmbuild -ta --clean --target i586-alt-linux HTML-Template-Pro-$(VERSION).tar.gz

index.html: README Makefile
	echo '<html><body><p><a href="http://sourceforge.net"><img src="http://sourceforge.net/sflogo.php?group_id=138507&amp;type=5" width="210" height="62" border="0" alt="SourceForge.net Logo" /></a><br> <a href="http://sourceforge.net/projects/html-tmpl-pro/">SourceForge Summary page of the project</a><br><a href="http://www.imath.kiev.ua/~vlasenko/">Home page of the author</a><br><a href="http://prdownloads.sourceforge.net/html-tmpl-pro/HTML-Template-Pro-$(VERSION).tar.gz?download">Download</a><p><pre>' > $@
	cat $< >> $@
	echo '</pre></body></html>' >> $@


MAKE_FRAG
}

# those tests are copied from Time-HiRes-01.20

sub TMPDIR {
    my $TMPDIR =
	(grep(defined $_ && -d $_ && -w _,
	      ((defined $ENV{'TMPDIR'} ? $ENV{'TMPDIR'} : undef),
	       qw(/var/tmp /usr/tmp /tmp))))[0]
		   unless defined $TMPDIR;
    $TMPDIR || die "Cannot find writable temporary directory.\n";
}

sub try_compile_and_link {
    my ($c, $cccmd, $ccflags) = @_;

    my ($ok) = 0;
    my ($tmp) = ($^O eq 'VMS') ? "tmp$$" : TMPDIR . '/' . "tmp$$";
    local(*TMPC);

    my $obj_ext = $Config{obj_ext} || ".o";
    unlink("$tmp.c", "$tmp$obj_ext");

    if (open(TMPC, ">$tmp.c")) {
	print TMPC $c;
	close(TMPC);
	my $COREincdir = $Config{'archlibexp'} . '/' . 'CORE';
	$ccflags .= ' '. $Config{'ccflags'} . ' ' . "-I$COREincdir";
	my $cc=$Config{'cc'};
	$cc||='cc';
	if ($^O eq 'VMS') {
            my $perl_core = $Config{'installarchlib'};
            $perl_core =~ s/\]$/.CORE]/;
            $cccmd = "$cc /include=(perl_root:[000000],$perl_core) $tmp.c"; 
        }
	$cccmd = "$cc -o $tmp $ccflags $tmp.c @$LIBS"
	 unless (defined $cccmd);
    print "trying $cccmd\n";
	system($cccmd);
	if ($^O eq 'VMS') {
	    $ok = -s "$tmp$obj_ext" && -x _;
	    unlink("$tmp.c", "$tmp$obj_ext");
        }
        else
        {
	    $ok = -s $tmp && -x _;
	    unlink("$tmp.c", $tmp);
        }
    }
    
    $ok;
}

sub has_pcre {
    print "looking for pcre...\n";
    return 1 if try_compile_and_link('
#include <pcre.h>
    static int foo()
    {
	return 0;
    }
    int main (int argc, char** argv)
    {
	foo();
    }
',undef, '-I/usr/include/pcre -lpcre');
    return 0;
}

sub has_x {
    my ($x) = @_; 

    return 1 if
    try_compile_and_link(<<EOM);
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#ifdef I_UNISTD
#   include <unistd.h>
#endif

#ifdef I_SYS_TYPES
#   include <sys/types.h>
#endif

#ifdef I_SYS_TIME
#   include <sys/time.h>
#endif

int main _((int argc, char** argv, char** env))
{
	$x;
}
EOM
    return 0;
}

