AM_CFLAGS = $(all_includes)

SUBDIRS = app

EXTRA_DIST = README.org doc autogen.sh

# Create documentation
README.md : README.org
	emacs README.org --batch -f org-md-export-to-markdown

dist_doc_DATA = README.md
