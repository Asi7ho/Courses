/**
 * @file global.h
 * @brief Global definitions.
 *
 * Global definitions.
 */

#ifndef _GLOBAL_H
#define _GLOBAL_H



/**
 * @mainpage MIPS Assembler
 *
 * @section sec0 Command line invocation
 *
 * Usage: <br/>
 * <br/>
 * ./as-mips source.asm
 *
 *
 * @section sec3 What works
 *
 * - the documentation! <br/>
 * - the lexical analysis <br/>
 * - the grammatical analysis <br/>
 * - addressing modes management <br/>
 * - loading of instructions for a dictionary file <br/>
 * - labels management <br/>
 * - code generation as a binary file and as a text file <br/>
 *
 * @section sec4 What is left for future happy hacking
 *
 * - management of all directives <br/>
 * - generation of elf relocatable files <br/>
 * - extensive testing.
 *
 */


/*!
  \brief INTERNALS: Maximum length of static strings.
 */
#define STRLEN          256

/*!
  \brief INTERNALS: Value for boolean FALSE.
 */
#define FALSE            0

/*!
  \brief INTERNALS: Value for boolean TRUE.
 */
#define  TRUE            1

/*!
  \brief INTERNALS: Return value for boolean SUCCESS.
 */
#define SUCCESS          0

/*!
  \brief INTERNALS: Return value for boolean FAILURE.
 */
#define FAILURE          1


#endif /* _GLOBAL_H */

