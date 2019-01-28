int myproc1()
{
   const char* nc = " Don't touch text /* inside */ quoted strings ";
   // This is comment
   printf(" This is // not comment \\/");
   printf(" This is /* not comment */ too");
   switch (test)
   {
      case '/':  text = "\\/"; break;
      case '\r': text = "\\r"; break;
   };
   myproc2("\\");  // This is // // /// /// // asda s da/sd asd/ asd/ asd / x \ basd comment
   /* This is comment too */ myproc3("\\\\\\");
   printf("x / y =\"// \
   10\n"/*comment here! */);
   myproc4();
   if(codepoint == '\\' /*test*/ || codepoint == '"'/*asas*/ || codepoint < 0x20)
      break;

   /* This is
      multiline comment */
   myproc5();
   // This is "cut-line"  \
and prolongation of single-line comment
   myproc6();
   printf(  " This is multiline \
string constant                 \
without /* comment. Don't /*  \
touch me. */ please" );
   /* This is multiline comment
      // this is single-line comment inside multiline */
    myproc7();
   /* This is multiline comment ...
       /* prolongation ....
       end of comment here -> */
    myproc8();

   printf("%d", 13 / 123);
return 1;
}