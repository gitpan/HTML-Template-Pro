use 5.005;
use ExtUtils::MakeMaker;
$Verbose = 1;

$O_FILES=qw/procore.o pstack.o pbuffer.o proscope.o calc.o expr.o pstring.o exprtool.o/;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    NAME              => 'HTML::Template::Pro',
    VERSION_FROM      => 'lib/HTML/Template/Pro.pm', # finds $VERSION
    PREREQ_PM         => {'File::Spec' => 0}, # e.g., Module::Name => 1.1
    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
      (ABSTRACT_FROM  => 'lib/HTML/Template/Pro.pm', # retrieve abstract from module
       AUTHOR         => 'I. Yu. Vlasenko <viy@altlinux.org>') : ()),
    LIBS              => ['-lm'],
    DEFINE            => '', # e.g., '-DHAVE_SOMETHING'
    INC               => '-I.', # e.g., '-I. -I/usr/include/other'
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

