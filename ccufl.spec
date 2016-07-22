%define __debug_install_post   \
		   %{_rpmconfigdir}/find-debuginfo.sh %{?_find_debuginfo_opts} "%{_builddir}/%{?buildsubdir}"\
%{nil}
%define _rpmdir  ./
Summary:        ccufl library
Name:           ccufl
Version:        1.0
Release:        0
License:        LGPL
Group:          System Environment/Base
URL:            http://www.github.com/prownd
Source:         %{name}-%{version}.tar.gz
Provides:       ccufl

#BuildArch:      noarch

%description
ccufl common c library.

%prep
install -d -p %{_rpmdir}

%setup -q -n ccufl

%build
make

%install
install -p -d $RPM_BUILD_ROOT/usr/local/lib
install -p -d $RPM_BUILD_ROOT/usr/local/include/ccufl
install -p lib/libccufl.so $RPM_BUILD_ROOT/usr/local/lib
install -p lib/libccufl.a $RPM_BUILD_ROOT/usr/local/lib
install -p src/*.h $RPM_BUILD_ROOT/usr/local/include/ccufl/

%files
%defattr(-,root,root,-)
/usr/local/lib/libccufl.a
/usr/local/lib/libccufl.so
/usr/local/include/ccufl/*.h



%changelog
* Fri Jul 22 2016 jinpeng han <545751287@qq.com> - 1-1
- gen ccufl rpm pkg
