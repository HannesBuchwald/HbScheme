/*
 * main_test.c
 *
 * Author:       Hannes Buchwald
 * Project:      hbScheme Interpreter (University of Media)
 * Version:      0.0.1
 * Last edit:    31.03.2017
 */




#include "hbscheme.h"

void unitTest() {


	/***** test case object definition *****/

	SCM_OBJ rslt,
	sym_abcde1, sym_abcde2, sym_abcde3,
	sym_bbb1, sym_bbb2, symx1, symx2;




	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "**** Start (UnitTest) ****\n");



	/***** a2l Test *****/

	// positive test
<<<<<<< HEAD
	ASSERT( a2l("1") == 2, "a2l error");
=======
	ASSERT( a2l("0") == 2, "a2l error");
>>>>>>> master
	ASSERT( a2l("1") == 1, "a2l error");
	ASSERT( a2l("123456") == 123456, "a2l error");
	ASSERT( a2l("999999") == 999999, "a2l error");
	ASSERT( a2l("2432902008176640000") == 2432902008176640000, "a2l error");

	// negative test
	ASSERT( a2l("0") != 1, "a2l error");
	ASSERT( a2l("1") != 0, "a2l error");
	ASSERT( a2l("123456") != 12345644, "a2l error");
	ASSERT( a2l("999999") != 9999999, "a2l error");
	ASSERT( a2l("2432902008176640000") != 243290200817664000, "a2l error");


	fprintf(stderr, "Done (a2l Test)\n");




	/***** Integer TAG and Check Test *****/

	// empty
	rslt = scm_readString("");
	ASSERT(rslt != NULL, "empty read check failed (NULL result)")
	ASSERT(rslt->scm_any.tag == TAG_EOF, "empty read check failed")

	// blank
	rslt = scm_readString("    ");
	ASSERT(rslt != NULL, "empty read check failed (NULL result)")
	ASSERT(rslt->scm_any.tag == TAG_EOF, "empty read check failed")

	// numbers
	rslt = scm_readString("123");
	ASSERT(rslt != NULL, "int check failed (NULL result)")
	ASSERT(rslt->scm_any.tag == TAG_INT, "int tag check failed")
	ASSERT(rslt->scm_integer.iVal == 123, "int check failed")

	// bank + numbers
	rslt = scm_readString("    123");
	ASSERT(rslt->scm_any.tag == TAG_INT, "int tag check failed")
	ASSERT(rslt->scm_integer.iVal == 123, "int check failed")

	// big number
	rslt = scm_readString("99999999");
	ASSERT(rslt->scm_any.tag == TAG_INT, "int tag check failed")
	ASSERT(rslt->scm_integer.iVal == 99999999, "int check failed")

	// negative number
	rslt = scm_readString("-99999999");
	ASSERT(rslt->scm_any.tag == TAG_INT, "int tag check failed")
	ASSERT(rslt->scm_integer.iVal == -99999999, "int check failed")


	fprintf(stderr, "Done (Integer TAG and Check Test)\n");





	/***** True/False TAG Test *****/

	// true
	rslt = scm_readString(" #t ");
	ASSERT(rslt->scm_any.tag == TAG_TRUE, "true tag check failed")

	// false
	rslt = scm_readString(" #f ");
	ASSERT(rslt->scm_any.tag == TAG_FALSE, "false tag check failed")

	// nil
	rslt = scm_readString(" nil ");
	ASSERT(rslt->scm_any.tag == TAG_NIL, "nil tag check failed")


	fprintf(stderr, "Done (True/False TAG Test)\n");





	/***** Symbol TAG Test *****/

	// string
	rslt = scm_readString(" abc ");
	ASSERT(rslt->scm_any.tag == TAG_SYMBOL, "symbol tag check failed")
	ASSERT(strcmp(rslt->scm_symbol.chars, "abc")==0, "symbol check failed")

	// string
	rslt = scm_readString(" if ");
	ASSERT(rslt->scm_any.tag == TAG_SYMBOL, "symbol tag check failed")
	ASSERT(strcmp(rslt->scm_symbol.chars, "if")==0, "symbol check failed")

	// string
	rslt = scm_readString(" define ");
	ASSERT(rslt->scm_any.tag == TAG_SYMBOL, "symbol tag check failed")
	ASSERT(strcmp(rslt->scm_symbol.chars, "define")==0, "symbol check failed")

	// number + charackter
	rslt = scm_readString(" 1+ ");
	ASSERT(rslt->scm_any.tag == TAG_SYMBOL, "symbol tag check failed")
	ASSERT(strcmp(rslt->scm_symbol.chars, "1+")==0, "symbol check failed")

	// special characters
	rslt = scm_readString(" --->---<--- ");
	ASSERT(rslt->scm_any.tag == TAG_SYMBOL, "symbol tag check failed")
	ASSERT(strcmp(rslt->scm_symbol.chars, "--->---<---")==0, "symbol check failed")


	fprintf(stderr, "Done (Symbol TAG Test)\n");



	/***** Compare same Tag Test *****/

	// same string
	sym_abcde1 = scm_readString("abcde ");
	sym_abcde2 = scm_readString("abcde ");
	ASSERT(sym_abcde1 == sym_abcde2, "symbol identity check failed")

	// same string + blank
	sym_bbb1 = scm_readString("   bbb ");
	sym_bbb2 = scm_readString("bbb ");
	ASSERT(sym_bbb1 == sym_bbb2, "symbol identity check failed")

	// same string
	sym_abcde3 = scm_readString("abcde ");
	ASSERT(sym_abcde3 == sym_abcde1, "symbol identity check failed")

	// different string negative test
	symx1 = scm_readString("abcde ");
	symx2 = scm_readString(" bbb ");
	ASSERT(symx1 != symx2, "symbol identity check failed")


	fprintf(stderr, "**** Done (UnitTest) ****\n");
}






void performanceTest() {


	/*  performance test
	 *	test case: create 1000 symbol objects
	 *
	 */

	fprintf(stderr, "\n");
	fprintf(stderr, "**** Start (PerformanceTest)**** \n");

	{
		int i;
		char symName[32];
		SCM_OBJ sym1, sym2;
		long tStart, tEnd;

		tStart = millisecondTime();

		for (i=0; i<10000; i++) {
			sprintf(symName, "abc%d", i);
			new_symbol(symName);
		}
		tEnd = millisecondTime();
		fprintf(stderr, "time to add 10000 new symbols: %dms \n", tEnd-tStart);

		tStart = millisecondTime();
		for (i=0; i<10000; i++) {
			sprintf(symName, "abc%d", i);
			new_symbol(symName);
		}
		tEnd = millisecondTime();
		fprintf(stderr, "time to lookup 10000 existing symbols: %dms\n", tEnd-tStart);

		// check identity
		for (i=0; i<10000; i++) {
			sprintf(symName, "abc%d", i);
			sym1 = new_symbol(symName);
			sym2 = new_symbol(symName);
			ASSERT(sym1==sym2, "symbol table check");
		}
	}


	fprintf(stderr, "**** Done (PerformanceTest) ****\n");
	fprintf(stderr, "\n");
}

