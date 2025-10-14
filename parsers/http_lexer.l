%{
#include "http_parser.hpp"
%}

%option reentrant
%option bison-bridge
%option noyywrap
%option nounput
%option noinput
%option nodefault

CRLF "\r\n"
SPACE " "
HTAB "\t"
SLASH "/"
PCHAR [A-Za-z0-9]
%x startLine
%x fieldLine

%%

<INITIAL>"GET"     { BEGIN(startLine); return TOK_GET; }
<INITIAL>"HEAD"    { BEGIN(startLine); return TOK_HEAD; }
<INITIAL>"POST"    { BEGIN(startLine); return TOK_POST; }
<INITIAL>"PUT"     { BEGIN(startLine); return TOK_PUT; }
<INITIAL>"DELETE"  { BEGIN(startLine); return TOK_DELETE; }
<INITIAL>"CONNECT" { BEGIN(startLine); return TOK_CONNECT; }
<INITIAL>"OPTIONS" { BEGIN(startLine); return TOK_OPTIONS; }
<INITIAL>"TRACE"   { BEGIN(startLine); return TOK_TRACE; }
<INITIAL>"PATCH"   { BEGIN(startLine); return TOK_PATCH; }

<startLine>{SPACE} { return TOK_SPACE; }
<startLine>{SLASH} { return TOK_SLASH; }
<startLine>{PCHAR}+ { return TOK_STRING; }
<startLine>"HTTP/1.1" { return TOK_HTTPversion; }

<*>{CRLF} { return TOK_CRLF; }

%%