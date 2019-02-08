#include <stdio.h>

int myproc1()
{
   int test = 0;
   const char * text = "aa";

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
   char codepoint = '0';
   while(codepoint == '\\' /*test*/ || codepoint == '"'/*asas*/ || codepoint < 0x20)
      break;
   // sdasdasda sasd /* qsdasd */ !
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

   // asdasdasd /* asdasdasd */ asdasdsad

   // Comment 2 /*
   int x = 0;
   // Comment 3 */
   int y = 3;
   //Comment 4

       // Start comment /*
    const char * str = "/* Test */\n";
    // End comment */

    char x = '\"';
    char y = '/* This is not allowed!'; /* Test 2
        Test 3 */ // New test???
    char z = '\\\"'; // Test1


    int z = x / y;

    printf("\\\\" /* Weird comment here */ "xxx\n"); // Test;

    printf("%d", 13 / 123);
    return 1;
}