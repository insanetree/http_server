%define parse.assert
%define api.push-pull push
%define api.pure full
%define api.token.prefix {TOK_}
%define api.symbol.prefix {SYM_}

%code {
#define yyerror(request, msg) ((void)0)
}

%code requires {
#include "http_request.hpp"
}

%parse-param{http_request* request}

%union {
      const char* str;
}

%token CRLF
%token SPACE
%token HTAB
%token<str> STRING

%token GET
%token HEAD
%token POST
%token PUT
%token DELETE
%token CONNECT
%token OPTIONS
%token TRACE
%token PATCH

%token SLASH
%token HTTPversion

%%

request: method SPACE request-target SPACE HTTPversion CRLF CRLF;

method: GET {request->set_method(http_request::method_e::GET);}
      | HEAD {request->set_method(http_request::method_e::HEAD);}
      | POST {request->set_method(http_request::method_e::POST);}
      | PUT {request->set_method(http_request::method_e::PUT);}
      | DELETE {request->set_method(http_request::method_e::DELETE);}
      | CONNECT {request->set_method(http_request::method_e::CONNECT);}
      | OPTIONS {request->set_method(http_request::method_e::OPTIONS);}
      | PATCH {request->set_method(http_request::method_e::PATCH);}
      ;

request-target: origin-form
              ;

origin-form: SLASH segment-list
           | SLASH
           ;
segment-list: segment
            | SLASH segment
            ;
segment: STRING {request->add_path_segment(std::string($1)); delete $1;}
       ;
%%
