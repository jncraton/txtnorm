txtnorm
=======

Small program to normalize formatting of a text file. This program is written in C and produces a small single executable file which performs some basic manipulation on stdin and streams the new text to standard out.

Usage: txtnorm < infile > outfile

* Converts line endings to LF
* Single quotes ('''') become '
* Double quotes ("""") become "
* en dash (-) becomes -
* em dash (-) becomes -
* hyphen (-) becomes -
* minus sign (-) becomes -
* Converts hard wrapped paragraphs to long lines

This programs will also remove paragraph indentation. If a line is preceded by double newlines, then leading spaces will be removed. This  will not imact indentation of lines separated by a single newline. For example, the following should remain untouched:

* List Item
    * Indented list item

    This program
    Should leave
    Indented blocks
    Alone