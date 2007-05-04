%define ku_ver 1.6.0

Name:		ku2
Version:	%{ku_ver}
Release:	0
Group:		Applications
Summary:	Kane Utilities 2
License:	LGPL
URL: http://developer.berlios.de/projects/ku2/
Source: ku2-%{ku_ver}.tar.bz2

%description
test
description!

%prep
%setup

%build
scons

%install
scons install
