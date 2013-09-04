txtnorm
=======

Small program to normalize formatting of a text file. This program is
written in C and produces a small single executable file which performs
some basic manipulation on stdin and streams the new text to standard out.

Usage: txtnorm < infile > outfile

* Converts line endings to LF
* Single quotes (‘’��) become '
* Double quotes (“”��) become "
* en dash (–) becomes -
* em dash (—) becomes -
* hyphen (‐) becomes -
* minus sign (−) becomes -
* Converts hard wrapped paragraphs to long lines