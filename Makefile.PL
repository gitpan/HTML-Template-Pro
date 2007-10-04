use 5.005;
use ExtUtils::MakeMaker;
use Config;
$Verbose = 1;

$O_FILES=qw/procore.o pbuffer.o proscope.o expr.o pstring.o tmpllog.o/;
#  loadfile.o calc.o exprtool.o pstack.o

my $INC='-I.';
my $DEFINE='';
my $LIBS='-lm';

my %DEF={
    PCRE=>1,
    DEBUG=>0,
    MMAP=>1,
    IMITATE=>0,
};
#$DEF{PCRE}=0 if $^O =~ /MSWin32/;
$DEF{PCRE}=has_pcre();
if ($^O eq 'os2' or $^O eq 'dos') {
    $DEF{MMAP}=0;
} elsif ($^O eq 'MSWin32') {
    # win 32 mmap is disabled by default
#    $DEF{MMAP}=1;
    $DEF{MMAP}=0;
} else {
    $DEF{MMAP}=has_mmap();
}

my $i;
for ($i=0; $i<@ARGV; $i++)  {
    if ($ARGV[$i]=~/^(PCRE|PEDANTIC|DEBUG|MMAP|IMITATE)=(.+)/) {
	$DEF{$1}=$2;
    } else {
	next;
    }
    splice @ARGV, $i, 1;
    $i--;
}

#$DEF{MMAP}=0 if $^O =~ /MSWin32/ and $DEF{'IMITATE'};
$DEFINE.=' -DDEBUG' if ($DEF{DEBUG});
$DEFINE.=' -DMMAP' if ($DEF{MMAP});
$DEFINE.=' -DCOMPAT_ON_BROKEN_QUOTE -DCOMPAT_ALLOW_NAME_IN_CLOSING_TAG' if ($DEF{'IMITATE'});
$DEFINE.=' -pedantic -DPEDANTIC' if ($DEF{'PEDANTIC'});

if ($DEF{PCRE}) {
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

if ($DEF{IMITATE}) {
    print "
=====================================================================
Compatibility mode for syntactically incorrect templates is enabled.
Note, that this mode could be a bit slower than default but it will be
useful if you rely on HTML::Template behaviour in undefined cases.
=====================================================================
";
} else {
    print "
=====================================================================
Compatibility mode for syntactically incorrect templates is disabled.
It is the fast default.
=====================================================================
";
}

### --------------------------------------
### testing whether sources are consistent
### --------------------------------------

unless ((stat('test_crlf.out'))[7] == 47) {
    my @tests_to_skip=('HTML-Template-Pro.t', 'HTML-Template.t', 'realloc.t');
    print <<ENDTEXT;
#=================================
#
#          WARNING!!!
#   test data are corrupted!
#   may be CRLF conversion?
#   skipping most of tests!!!!
#   skipped: 
#   @tests_to_skip
#
#=================================
ENDTEXT
    sleep(2);
    chdir 't';
    # test that fail with corrupted data
    unlink @tests_to_skip;
    chdir '..';
}



WriteMakefile(
    NAME              => 'HTML::Template::Pro',
    VERSION_FROM      => 'lib/HTML/Template/Pro.pm', # finds $VERSION
    PREREQ_PM         => {'File::Spec' => 0,
			  'Test::More' => 0,
			  }, # e.g., Module::Name => 1.1
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
expr.c: calc.inc exprtool.inc exprpstr.inc
procore\$(OBJ_EXT): loadfile.inc pstack.inc

rpm: dist
	rpmbuild -ta --clean --target i586-alt-linux HTML-Template-Pro-$(VERSION).tar.gz

MANIFEST.git::
	git-ls-index > MANIFEST.git

index.html: README Makefile
	echo '<html><body><p><a href="http://sourceforge.net"><img src="http://sourceforge.net/sflogo.php?group_id=138507&amp;type=5" width="210" height="62" border="0" alt="SourceForge.net Logo" /></a><br> <a href="http://sourceforge.net/projects/html-tmpl-pro/">SourceForge Summary page of the project</a><br><a href="http://www.imath.kiev.ua/~vlasenko/">Home page of the author</a><br><a href="http://prdownloads.sourceforge.net/html-tmpl-pro/HTML-Template-Pro-$(VERSION).tar.gz?download">Download</a><p><pre>' > $@
	cat $< >> $@
	echo '</pre></body></html>' >> $@

TARBALL=HTML-Template-Pro-$(VERSION).tar.gz
SF_HOME='viy2@html-tmpl-pro.sf.net:~/www-html-tmpl-pro/htdocs'
sourceforge: tardist index.html
	( echo 'anonymous';	\
	  echo prompt;		\
	  echo hash;		\
	  echo cd incoming;	\
	  echo put $(TARBALL);	\
	  echo quit ) | /usr/bin/ftp upload.sourceforge.net
	ssh-add ~/.ssh/id_dsa.sf
	scp index.html $(SF_HOME)
 

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
    if (try_compile_and_link('
#include <pcre.h>
    static int foo()
    {
	return 0;
    }
    int main (int argc, char** argv)
    {
	foo();
    }
',undef, '-I/usr/include/pcre -lpcre')){
	print "pcre found.\n";
	return 1;
    }
    print "pcre not found.\n";
    return 0;
}

sub has_mmap {
    print "looking for mmap...\n";
    if (try_compile_and_link('
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

static
char*
mmap_load_file (const char* filepath) {
  int fd;
  struct stat st;
  size_t size_in_bytes;
  char* memarea=NULL;
  fd = open(filepath, O_RDONLY);
  if (fd == -1) return memarea; /* {NULL,NULL} */
  fstat(fd, &st);
  size_in_bytes = st.st_size;
  /* mmap size_in_bytes+1 to avoid crash with empty file */
  memarea = (char *) mmap(0, size_in_bytes+1,  PROT_READ,  MAP_SHARED, fd, 0);
  close(fd);
  return memarea;
}
    int main (int argc, char** argv)
    {
	mmap_load_file("file");
    }
',undef, '')){
	print "mmap found.\n";
	return 1;
    }
    print "mmap not found.\n";
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

