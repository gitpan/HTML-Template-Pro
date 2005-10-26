#################### WARNING! ######################
# this spec file is for ALT Linux distro only.     #
# other distro may have problems with rpm macro!!! #
####################################################

%define module HTML-Template-Pro

Name: perl-%module
Version: 0.55
Release: alt1

Packager: Igor Yu. Vlasenko <viy@altlinux.org>

Summary: Perl+C/XS module to produce HTML from HTML Template files.
Group: Development/Perl
License: GPL or Artistic
#Source: http://www.cpan.org/modules/by-module/HTML/%module-%version.tar.gz
Source: %module-%version.tar.gz
Url: http://sourceforge.net/projects/html-tmpl-pro/

# Automatically added by buildreq on Wed Nov 06 2002
BuildRequires: perl-devel pcre libpcre-devel

%description
HTML::Template::Pro is a fast lightweight C/Perl+XS reimplementation
of HTML::Template and HTML::Template::Expr. It is not intended 
to be a complete replacement, but to be a fast implementation 
of HTML::Template if you don't need quering, the extended facility
of HTML::Template. 
Designed for heavy upload, resource limitations, abcence of mod_perl.

HTML::Template module attempts make using HTML templates simple and natural. 
It extends standard HTML with a few new HTML-esque tags - <TMPL_VAR>,
<TMPL_LOOP>, <TMPL_INCLUDE>, <TMPL_IF> and <TMPL_ELSE>.  The file
written with HTML and these new tags is called a template.  It is
usually saved separate from your script - possibly even created by
someone else!  Using this module you fill in the values for the
variables, loops and branches declared in the template.  This allows
you to seperate design - the HTML - from the data, which you generate
in the Perl script.

%prep
%setup -q -n %module-%version

%build
%perl_vendor_build

%install
%perl_vendor_install

%files
%doc README Changes README.ru FAQ
%perl_vendor_privlib/*
%perl_vendor_man3dir/*

%changelog
* Mon Oct 24 2005 Igor Vlasenko <viy@altlinux.ru> 0.55-alt1
- new version

* Mon Oct 17 2005 Igor Vlasenko <viy@altlinux.ru> 0.54-alt1
- new version

* Thu Oct 06 2005 Igor Vlasenko <viy@altlinux.ru> 0.53-alt1
- new version

* Fri Sep 30 2005 Igor Vlasenko <viy@altlinux.ru> 0.52-alt1
- new version

* Thu Sep 15 2005 Igor Vlasenko <viy@altlinux.ru> 0.51-alt1
- new version

* Thu Sep 01 2005 Igor Vlasenko <viy@altlinux.ru> 0.49-alt1
- new version

* Wed Aug 31 2005 Igor Vlasenko <viy@altlinux.ru> 0.48-alt1
- new version

* Wed Aug 31 2005 Igor Vlasenko <viy@altlinux.ru> 0.47-alt1
- new version

* Sat Aug 20 2005 Igor Vlasenko <viy@altlinux.ru> 0.46-alt1
- new version

* Fri Aug 19 2005 Igor Vlasenko <viy@altlinux.ru> 0.45-alt1
- new version

* Fri Aug 12 2005 Igor Vlasenko <viy@altlinux.ru> 0.44-alt1
- new version

* Thu Aug 04 2005 Igor Vlasenko <viy@altlinux.ru> 0.43-alt1
- new version

* Thu Aug 04 2005 Igor Vlasenko <viy@altlinux.org> 0.42-alt1
- new version

* Tue Jul 26 2005 Igor Vlasenko <viy@altlinux.org> 0.41-alt1
- new version

* Fri Jul 01 2005 Igor Vlasenko <viy@altlinux.org> 0.40-alt1
- basic support for string comparing (HTML::Template::Expr)

* Thu Jun 22 2005 Igor Vlasenko <viy@altlinux.ru> 0.39-alt1
- new version

* Thu Jun 09 2005 Igor Vlasenko <viy@altlinux.org> 0.38-alt1
- new version, basic support for HTML::Template::Expr

* Thu Jun 02 2005 Igor Vlasenko <viy@altlinux.ru> 0.37-alt1
- new version

* Mon May 23 2005 Igor Vlasenko <viy@altlinux.ru> 0.36-alt1
- new version

* Thu May 19 2005 Igor Vlasenko <viy@altlinux.ru> 0.35-alt1
- First build for Sisyphus.

* Sun May 15 2005 Igor Vlasenko <viy@altlinux.ru> 0.34-alt1.1
- First build for Daedalus.
