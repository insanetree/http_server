%{
#include "http_parser.hpp"
%}

%option reentrant
%option noyywrap
%option nounput
%option noinput
%option nodefault

%%

"GET"   { return 0Xbabaceca; }
[ \t]+  ;
"\r\n"  return '\n';

%%