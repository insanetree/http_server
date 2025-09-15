%define parse.assert
%define api.push-pull push
%define api.pure full

%code {
#define yyerror(msg) ((void)0)
}

%token GET
%token POST

%%

request: method;
method: GET
      | POST;

%%
