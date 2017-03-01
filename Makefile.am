ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/libmisc src/libcomparison src/libquality src/tools src/analysis src/conversion src/preprocessing methods
EXTRA_DIST = execute.sh update_whitelist.sh html

dist-hook:
	rm -rf `find $(distdir)/html/* -type d -name .svn`
