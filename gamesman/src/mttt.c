/************************************************************************
**
** NAME:        mttt.c
**
** DESCRIPTION: Tic-Tac-Toe
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 1995. All rights reserved.
**
** DATE:        08/28/91
**
** UPDATE HIST:
**
**  8-30-91 1.0a1 : Fixed the bug in reading the input - now 'q' doesn't barf.
**  9-06-91 1.0a2 : Added the two extra arguments to PrintPosition
**                  Recoded the way to do "visited" - bitmask
**  9-06-91 1.0a3 : Added Symmetry code - whew was that a lot of code!
**  9-06-91 1.0a4 : Added ability to have random linked list in gNextMove.
**  9-06-91 1.0a5 : Removed redundant code - replaced w/GetRawValueFromDatabase
**  9-17-91 1.0a7 : Added graphics code.
**  5-12-92 1.0a8 : Added Static Evaluation - it's far from perfect, but 
**                  it works!
** 05-15-95 1.0   : Final release code for M.S.
** 97-05-12 1.1   : Removed gNextMove and any storage of computer's move
**
** Decided to check out how much space was wasted with the array:
**
** A Dartboard 9-slot hash has 6046 positions (all symmetries included)
**
** Without checking for symmetries
**
** Undecided = 14205 out of 19683
** Lose      =  1574 out of 19683
** Win       =  2836 out of 19683
** Tie       =  1068 out of 19683
** Unknown   =     0 out of 19683
** TOTAL     =  5478 out of 19683
**
** With SLIM = Symmetry-limiting Initial Move
** (only 1st move do we limit moves to 1,2,5)
**
** Lose      =  1274 out of 4163
** Win       =  2083 out of 4163
** Tie       =   806 out of 4163
** Unknown   =     0 out of 4163
** TOTAL     =  4163 out of 19683 allocated
**
** With SLIMFAST = Symmetry-LImiting Move Fast!
** (EVERY move we limit if there are symmetries)
** 
** Lose      =  1084 out of 3481
** Win       =  1725 out of 3481
** Tie       =   672 out of 3481
** Unknown   =     0 out of 3481 (Sanity-check...should always be 0)
** TOTAL     =  3481 out of 19683 allocated
**
**     Time Loss : ??
** Space Savings : 1.573
**
** While checking for symmetries and storing a canonical elt from them.
**
** Evaluating the value of Tic-Tac-Toe...done in 5.343184 seconds!
** Undecided = 18917 out of 19682
** Lose      =   224 out of 19682
** Win       =   390 out of 19682
** Tie       =   151 out of 19682
** Unk       =     0 out of 19682
** TOTAL     =   765 out of 19682
**
**     Time Loss : 3.723
** Space Savings : 7.160 (why did I earlier write 6.279?)
**
**************************************************************************/

/*************************************************************************
**
** Everything below here must be in every game file
**
**************************************************************************/

#include <stdio.h>
#include "gamesman.h"

POSITION gNumberOfPositions  = 19683;  /* 3^9 */
POSITION kBadPosition	     = -1;

POSITION gInitialPosition    =  0;
POSITION gMinimalPosition    =  0;

STRING   kAuthorName         = "Dan Garcia";
STRING   kGameName           = "Tic-Tac-Toe";
BOOLEAN  kPartizan           = TRUE;
BOOLEAN  kDebugMenu          = TRUE;
BOOLEAN  kGameSpecificMenu   = FALSE;
BOOLEAN  kTieIsPossible      = TRUE;
BOOLEAN  kLoopy               = FALSE;
BOOLEAN  kDebugDetermineValue = FALSE;
void*	 gGameSpecificTclInit = NULL;

STRING   kHelpGraphicInterface =
"The LEFT button puts an X or O (depending on whether you went first\n\
or second) on the spot the cursor was on when you clicked. The MIDDLE\n\
button does nothing, and the RIGHT button is the same as UNDO, in that\n\
it reverts back to your your most recent position.";

STRING   kHelpTextInterface    =
"On your turn, use the LEGEND to determine which number to choose (between\n\
1 and 9, with 1 at the upper left and 9 at the lower right) to correspond\n\
to the empty board position you desire and hit return. If at any point\n\
you have made a mistake, you can type u and hit return and the system will\n\
revert back to your most recent position.";

STRING   kHelpOnYourTurn =
"You place one of your pieces on one of the empty board positions.";

STRING   kHelpStandardObjective =
"To get three of your markers (either X or O) in a row, either\n\
horizontally, vertically, or diagonally. 3-in-a-row WINS.";

STRING   kHelpReverseObjective =
"To force your opponent into getting three of his markers (either X or\n\
O) in a row, either horizontally, vertically, or diagonally. 3-in-a-row\n\
LOSES.";

STRING   kHelpTieOccursWhen = /* Should follow 'A Tie occurs when... */
"the board fills up without either player getting three-in-a-row.";

STRING   kHelpExample =
"         ( 1 2 3 )           : - - -\n\
LEGEND:  ( 4 5 6 )  TOTAL:   : - - - \n\
         ( 7 8 9 )           : - - - \n\n\
Computer's move              :  3    \n\n\
         ( 1 2 3 )           : - - X \n\
LEGEND:  ( 4 5 6 )  TOTAL:   : - - - \n\
         ( 7 8 9 )           : - - - \n\n\
     Dan's move [(u)ndo/1-9] : { 2 } \n\n\
         ( 1 2 3 )           : - O X \n\
LEGEND:  ( 4 5 6 )  TOTAL:   : - - - \n\
         ( 7 8 9 )           : - - - \n\n\
Computer's move              :  6    \n\n\
         ( 1 2 3 )           : - O X \n\
LEGEND:  ( 4 5 6 )  TOTAL:   : - - X \n\
         ( 7 8 9 )           : - - - \n\n\
     Dan's move [(u)ndo/1-9] : { 9 } \n\n\
         ( 1 2 3 )           : - O X \n\
LEGEND:  ( 4 5 6 )  TOTAL:   : - - X \n\
         ( 7 8 9 )           : - - O \n\n\
Computer's move              :  5    \n\n\
         ( 1 2 3 )           : - O X \n\
LEGEND:  ( 4 5 6 )  TOTAL:   : - X X \n\
         ( 7 8 9 )           : - - O \n\n\
     Dan's move [(u)ndo/1-9] : { 7 } \n\n\
         ( 1 2 3 )           : - O X \n\
LEGEND:  ( 4 5 6 )  TOTAL:   : - X X \n\
         ( 7 8 9 )           : O - O \n\n\
Computer's move              :  4    \n\n\
         ( 1 2 3 )           : - O X \n\
LEGEND:  ( 4 5 6 )  TOTAL:   : X X X \n\
         ( 7 8 9 )           : O - O \n\n\
Computer wins. Nice try, Dan.";

/*************************************************************************
**
** Everything above here must be in every game file
**
**************************************************************************/

/*************************************************************************
**
** Every variable declared here is only used in this file (game-specific)
**
**************************************************************************/

#define BOARDSIZE     9           /* 3x3 board */

typedef enum possibleBoardPieces {
  Blank, o, x
} BlankOX;

char *gBlankOXString[] = { "-", "O", "X" };

/* Powers of 3 - this is the way I encode the position, as an integer */
int g3Array[] = { 1, 3, 9, 27, 81, 243, 729, 2187, 6561 };

/* Global position solver variables.*/
struct {
  BlankOX board[BOARDSIZE];
  BlankOX nextPiece;
  int piecesPlaced;
} gGPS;

/** Function Prototypes **/
POSITION GPS_DoMove(MOVE move);
MOVELIST *GPS_GenerateMoves();
VALUE GPS_Primitive();
void GPS_UndoMove(MOVE move);

POSITION BlankOXToPosition(BlankOX *theBlankOX);
void PositionToBlankOX(POSITION thePos,BlankOX *theBlankOX);

BOOLEAN ThreeInARow(BlankOX[], int, int, int);
POSITION GetCanonicalPosition(POSITION position);

/**************************************************/
/**************** SYMMETRY FUN BEGIN **************/
/**************************************************/

BOOLEAN kSupportsSymmetries = TRUE; /* Whether we support symmetries */

#define NUMSYMMETRIES 8   /*  4 rotations, 4 flipped rotations */

int gSymmetryMatrix[NUMSYMMETRIES][BOARDSIZE];

/* Proofs of correctness for the below arrays:
**
** FLIP						ROTATE
**
** 0 1 2	2 1 0		0 1 2		6 3 0		8 7 6		2 5 8
** 3 4 5  ->  	5 4 3		3 4 5	->	7 4 1  ->	5 4 3	->	1 4 7
** 6 7 8	8 7 6		6 7 8		8 5 2		2 1 0		2 1 0
*/

/* This is the array used for flipping along the N-S axis */
int gFlipNewPosition[] = { 2, 1, 0, 5, 4, 3, 8, 7, 6 };

/* This is the array used for rotating 90 degrees clockwise */
int gRotate90CWNewPosition[] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };

/**************************************************/
/**************** SYMMETRY FUN END ****************/
/**************************************************/

/************************************************************************
**
** NAME:        InitializeDatabases
**
** DESCRIPTION: Initialize the gDatabase, a global variable.
** 
************************************************************************/

void InitializeGame()
{
  /**************************************************/
  /**************** SYMMETRY FUN BEGIN **************/
  /**************************************************/

  gCanonicalPosition = GetCanonicalPosition;

  int i, j, temp; /* temp is used for debugging */

  if(kSupportsSymmetries) { /* Initialize gSymmetryMatrix[][] */
    for(i = 0 ; i < BOARDSIZE ; i++) {
      temp = i;
      for(j = 0 ; j < NUMSYMMETRIES ; j++) {
	if(j == NUMSYMMETRIES/2)
	  temp = gFlipNewPosition[i];
	if(j < NUMSYMMETRIES/2)
	  temp = gSymmetryMatrix[j][i] = gRotate90CWNewPosition[temp];
	else
	  temp = gSymmetryMatrix[j][i] = gRotate90CWNewPosition[temp];
      }
    }
  }

  /**************************************************/
  /**************** SYMMETRY FUN END ****************/
  /**************************************************/

  PositionToBlankOX(gInitialPosition, gGPS.board);
  gGPS.nextPiece = x;
  gGPS.piecesPlaced = 0;
  gGPSDoMove = GPS_DoMove;
  gGPSGenerateMoves = GPS_GenerateMoves;
  gGPSPrimitive = GPS_Primitive;
  gGPSUndoMove = GPS_UndoMove;
}

void FreeGame()
{}

/************************************************************************
**
** NAME:        DebugMenu
**
** DESCRIPTION: Menu used to debub internal problems. Does nothing if
**              kDebugMenu == FALSE
** 
************************************************************************/

void DebugMenu()
{
  int tttppm();
  char GetMyChar();

  do {
    printf("\n\t----- Module DEBUGGER for %s -----\n\n", kGameName);
    
    printf("\tc)\tWrite PPM to s(C)reen\n");
    printf("\ti)\tWrite PPM to f(I)le\n");
    printf("\ts)\tWrite Postscript to (S)creen\n");
    printf("\tf)\tWrite Postscript to (F)ile\n");
    printf("\n\n\tb)\t(B)ack = Return to previous activity.\n");
    printf("\n\nSelect an option: ");
    
    switch(GetMyChar()) {
    case 'Q': case 'q':
      ExitStageRight();
    case 'H': case 'h':
      HelpMenus();
      break;
    case 'C': case 'c': /* Write PPM to s(C)reen */
      tttppm(0,0);
      break;
    case 'I': case 'i': /* Write PPM to f(I)le */
      tttppm(0,1);
      break;
    case 'S': case 's': /* Write Postscript to (S)creen */
      tttppm(1,0);
      break;
    case 'F': case 'f': /* Write Postscript to (F)ile */
      tttppm(1,1);
      break;
    case 'B': case 'b':
      return;
    default:
      BadMenuChoice();
      HitAnyKeyToContinue();
      break;
    }
  } while(TRUE);
  
}

/************************************************************************
**
** NAME:        GameSpecificMenu
**
** DESCRIPTION: Menu used to change game-specific parmeters, such as
**              the side of the board in an nxn Nim board, etc. Does
**              nothing if kGameSpecificMenu == FALSE
** 
************************************************************************/

void GameSpecificMenu() { }

/************************************************************************
**
** NAME:        SetTclCGameSpecificOptions
**
** DESCRIPTION: Set the C game-specific options (called from Tcl)
**              Ignore if you don't care about Tcl for now.
** 
************************************************************************/

void SetTclCGameSpecificOptions(theOptions)
int theOptions[];
{
  /* No need to have anything here, we have no extra options */
}

/************************************************************************
**
** NAME:        DoMove
**
** DESCRIPTION: Apply the move to the position.
** 
** INPUTS:      POSITION thePosition : The old position
**              MOVE     theMove     : The move to apply.
**
** OUTPUTS:     (POSITION) : The position that results after the move.
**
** CALLS:       PositionToBlankOX(POSITION,*BlankOX)
**              BlankOX WhosTurn(*BlankOX)
**
************************************************************************/

POSITION DoMove(thePosition, theMove)
     POSITION thePosition;
     MOVE theMove;
{
  BlankOX theBlankOX[BOARDSIZE], WhoseTurn();

  PositionToBlankOX(thePosition,theBlankOX);

  return(thePosition + (g3Array[theMove] * (int)WhoseTurn(theBlankOX)));
}

POSITION GPS_DoMove(MOVE move)
{
  gGPS.board[move] = gGPS.nextPiece;
  gGPS.nextPiece = gGPS.nextPiece == x ? o : x;
  ++gGPS.piecesPlaced;

  return BlankOXToPosition(gGPS.board);
}

void GPS_UndoMove(MOVE move)
{
  gGPS.board[move] = Blank;
  gGPS.nextPiece = gGPS.nextPiece == x ? o : x;
  --gGPS.piecesPlaced;
}

/************************************************************************
**
** NAME:        GetInitialPosition
**
** DESCRIPTION: Ask the user for an initial position for testing. Store
**              it in the space pointed to by initialPosition;
** 
** OUTPUTS:     POSITION initialPosition : The position to fill.
**
************************************************************************/

POSITION GetInitialPosition()
{
  BlankOX theBlankOX[BOARDSIZE], whosTurn;
  signed char c;
  int i, goodInputs = 0;


  printf("\n\n\t----- Get Initial Position -----\n");
  printf("\n\tPlease input the position to begin with.\n");
  printf("\tNote that it should be in the following format:\n\n");
  printf("O - -\nO - -            <----- EXAMPLE \n- X X\n\n");

  i = 0;
  getchar();
  while(i < BOARDSIZE && (c = getchar()) != EOF) {
    if(c == 'x' || c == 'X')
      theBlankOX[i++] = x;
    else if(c == 'o' || c == 'O' || c == '0')
      theBlankOX[i++] = o;
    else if(c == '-')
      theBlankOX[i++] = Blank;
    else
      ;   /* do nothing */
  }

  /*
  getchar();
  printf("\nNow, whose turn is it? [O/X] : ");
  scanf("%c",&c);
  if(c == 'x' || c == 'X')
    whosTurn = x;
  else
    whosTurn = o;
    */

  return(BlankOXToPosition(theBlankOX));
}

/************************************************************************
**
** NAME:        PrintComputersMove
**
** DESCRIPTION: Nicely format the computers move.
** 
** INPUTS:      MOVE   *computersMove : The computer's move. 
**              STRING  computersName : The computer's name. 
**
************************************************************************/

void PrintComputersMove(computersMove,computersName)
     MOVE computersMove;
     STRING computersName;
{
  printf("%8s's move              : %2d\n", computersName, computersMove+1);
}

/************************************************************************
**
** NAME:        Primitive
**
** DESCRIPTION: Return the value of a position if it fulfills certain
**              'primitive' constraints. Some examples of this is having
**              three-in-a-row with TicTacToe. TicTacToe has two
**              primitives it can immediately check for, when the board
**              is filled but nobody has one = primitive tie. Three in
**              a row is a primitive lose, because the player who faces
**              this board has just lost. I.e. the player before him
**              created the board and won. Otherwise undecided.
** 
** INPUTS:      POSITION position : The position to inspect.
**
** OUTPUTS:     (VALUE) an enum which is oneof: (win,lose,tie,undecided)
**
** CALLS:       BOOLEAN ThreeInARow()
**              BOOLEAN AllFilledIn()
**              PositionToBlankOX()
**
************************************************************************/

VALUE Primitive(position) 
     POSITION position;
{
  BOOLEAN ThreeInARow(), AllFilledIn();
  BlankOX theBlankOX[BOARDSIZE];

  PositionToBlankOX(position,theBlankOX);

  if( ThreeInARow(theBlankOX,0,1,2) || 
      ThreeInARow(theBlankOX,3,4,5) || 
      ThreeInARow(theBlankOX,6,7,8) || 
      ThreeInARow(theBlankOX,0,3,6) || 
      ThreeInARow(theBlankOX,1,4,7) || 
      ThreeInARow(theBlankOX,2,5,8) || 
      ThreeInARow(theBlankOX,0,4,8) || 
      ThreeInARow(theBlankOX,2,4,6) )
    return(gStandardGame ? lose : win);
  else if(AllFilledIn(theBlankOX))
    return(tie);
  else
    return(undecided);
}

VALUE GPS_Primitive()
{
  if (ThreeInARow(gGPS.board, 0, 1, 2) ||
      ThreeInARow(gGPS.board, 3, 4, 5) ||
      ThreeInARow(gGPS.board, 6, 7, 8) ||
      ThreeInARow(gGPS.board, 0, 3, 6) ||
      ThreeInARow(gGPS.board, 1, 4, 7) ||
      ThreeInARow(gGPS.board, 2, 5, 8) ||
      ThreeInARow(gGPS.board, 0, 4, 8) ||
      ThreeInARow(gGPS.board, 2, 4, 6))
    return gStandardGame ? lose : win;
  else if (gGPS.piecesPlaced == BOARDSIZE)
    return tie;
  else
    return undecided;
}

/************************************************************************
**
** NAME:        PrintPosition
**
** DESCRIPTION: Print the position in a pretty format, including the
**              prediction of the game's outcome.
** 
** INPUTS:      POSITION position   : The position to pretty print.
**              STRING   playerName : The name of the player.
**              BOOLEAN  usersTurn  : TRUE <==> it's a user's turn.
**
** CALLS:       PositionToBlankOX()
**              GetValueOfPosition()
**              GetPrediction()
**
************************************************************************/

void PrintPosition(position,playerName,usersTurn)
     POSITION position;
     STRING playerName;
     BOOLEAN  usersTurn;
{
  int i;
  VALUE GetValueOfPosition();
  BlankOX theBlankOx[BOARDSIZE];

  PositionToBlankOX(position,theBlankOx);

  printf("\n         ( 1 2 3 )           : %s %s %s\n",
	 gBlankOXString[(int)theBlankOx[0]],
	 gBlankOXString[(int)theBlankOx[1]],
	 gBlankOXString[(int)theBlankOx[2]] );
  printf("LEGEND:  ( 4 5 6 )  TOTAL:   : %s %s %s\n",
	 gBlankOXString[(int)theBlankOx[3]],
	 gBlankOXString[(int)theBlankOx[4]],
	 gBlankOXString[(int)theBlankOx[5]] );
  printf("         ( 7 8 9 )           : %s %s %s %s\n\n",
	 gBlankOXString[(int)theBlankOx[6]],
	 gBlankOXString[(int)theBlankOx[7]],
	 gBlankOXString[(int)theBlankOx[8]],
	 GetPrediction(position,playerName,usersTurn));
}

/************************************************************************
**
** NAME:        GenerateMoves
**
** DESCRIPTION: Create a linked list of every move that can be reached
**              from this position. Return a pointer to the head of the
**              linked list.
** 
** INPUTS:      POSITION position : The position to branch off of.
**
** OUTPUTS:     (MOVELIST *), a pointer that points to the first item  
**              in the linked list of moves that can be generated.
**
** CALLS:       MOVELIST *CreateMovelistNode(MOVE,MOVELIST *)
**
************************************************************************/

MOVELIST *GenerateMoves(position)
     POSITION position;
{
  MOVELIST *CreateMovelistNode(), *head = NULL;
  BlankOX theBlankOX[BOARDSIZE];
  int i;

  PositionToBlankOX(position,theBlankOX);
  for(i = 0 ; i < BOARDSIZE ; i++) {
    if(theBlankOX[i] == Blank)
      head = CreateMovelistNode(i,head);
  }
  return(head);
}

MOVELIST *GPS_GenerateMoves()
{
  int index;
  MOVELIST *moves = NULL;

  for (index = 0; index < BOARDSIZE; ++index)
    if (gGPS.board[index] == Blank)
      moves = CreateMovelistNode(index, moves);

  return moves;
}

/**************************************************/
/**************** SYMMETRY FUN BEGIN **************/
/**************************************************/

/************************************************************************
**
** NAME:        GetCanonicalPosition
**
** DESCRIPTION: Go through all of the positions that are symmetrically
**              equivalent and return the SMALLEST, which will be used
**              as the canonical element for the equivalence set.
** 
** INPUTS:      POSITION position : The position return the canonical elt. of.
**
** OUTPUTS:     POSITION          : The canonical element of the set.
**
************************************************************************/

POSITION GetCanonicalPosition(position)
     POSITION position;
{
  POSITION newPosition, theCanonicalPosition, DoSymmetry();
  int i;
  
  theCanonicalPosition = position;
  
  for(i = 0 ; i < NUMSYMMETRIES ; i++) {
    
    newPosition = DoSymmetry(position, i);    /* get new */
    if(newPosition < theCanonicalPosition)    /* THIS is the one */
      theCanonicalPosition = newPosition;     /* set it to the ans */
  }

  return(theCanonicalPosition);
}

/************************************************************************
**
** NAME:        DoSymmetry
**
** DESCRIPTION: Perform the symmetry operation specified by the input
**              on the position specified by the input and return the
**              new position, even if it's the same as the input.
** 
** INPUTS:      POSITION position : The position to branch the symmetry from.
**              int      symmetry : The number of the symmetry operation.
**
** OUTPUTS:     POSITION, The position after the symmetry operation.
**
************************************************************************/

POSITION DoSymmetry(position, symmetry)
     POSITION position;
     int symmetry;
{  
  int i;
  BlankOX theBlankOx[BOARDSIZE], symmBlankOx[BOARDSIZE];
  POSITION BlankOXToPosition();
  
  PositionToBlankOX(position,theBlankOx);
  PositionToBlankOX(position,symmBlankOx); /* Make copy */
  
  /* Copy from the symmetry matrix */
  
  for(i = 0 ; i < BOARDSIZE ; i++)
    symmBlankOx[i] = theBlankOx[gSymmetryMatrix[symmetry][i]];
  
  return(BlankOXToPosition(symmBlankOx));
} 

/**************************************************/
/**************** SYMMETRY FUN END ****************/
/**************************************************/

/************************************************************************
**
** NAME:        GetAndPrintPlayersMove
**
** DESCRIPTION: This finds out if the player wanted an undo or abort or not.
**              If so, return Undo or Abort and don't change theMove.
**              Otherwise get the new theMove and fill the pointer up.
** 
** INPUTS:      POSITION *thePosition : The position the user is at. 
**              MOVE *theMove         : The move to fill with user's move. 
**              STRING playerName     : The name of the player whose turn it is
**
** OUTPUTS:     USERINPUT             : Oneof( Undo, Abort, Continue )
**
** CALLS:       ValidMove(MOVE, POSITION)
**              BOOLEAN PrintPossibleMoves(POSITION) ...Always True!
**
************************************************************************/

USERINPUT GetAndPrintPlayersMove(thePosition, theMove, playerName)
     POSITION thePosition;
     MOVE *theMove;
     STRING playerName;
{
  int xpos, ypos;
  BOOLEAN ValidMove();
  char input = '0';
  BOOLEAN done = FALSE;
  USERINPUT ret, HandleDefaultTextInput();
  
  do {
    printf("%8s's move [(u)ndo/1-9] :  ", playerName);
    
    ret = HandleDefaultTextInput(thePosition, theMove, playerName);
    if(ret != Continue)
      return(ret);
    
  }
  while (TRUE);
  return(Continue); /* this is never reached, but lint is now happy */
}

/************************************************************************
**
** NAME:        ValidTextInput
**
** DESCRIPTION: Return TRUE iff the string input is of the right 'form'.
**              For example, if the user is allowed to select one slot
**              from the numbers 1-9, and the user chooses 0, it's not
**              valid, but anything from 1-9 IS, regardless if the slot
**              is filled or not. Whether the slot is filled is left up
**              to another routine.
** 
** INPUTS:      STRING input : The string input the user typed.
**
** OUTPUTS:     BOOLEAN : TRUE iff the input is a valid text input.
**
************************************************************************/

BOOLEAN ValidTextInput(input)
     STRING input;
{
  return(input[0] <= '9' && input[0] >= '1');
}

/************************************************************************
**
** NAME:        ConvertTextInputToMove
**
** DESCRIPTION: Convert the string input to the internal move representation.
** 
** INPUTS:      STRING input : The string input the user typed.
**
** OUTPUTS:     MOVE : The move corresponding to the user's input.
**
************************************************************************/

MOVE ConvertTextInputToMove(input)
     STRING input;
{
  return((MOVE) input[0] - '1'); /* user input is 1-9, our rep. is 0-8 */
}

/************************************************************************
**
** NAME:        PrintMove
**
** DESCRIPTION: Print the move in a nice format.
** 
** INPUTS:      MOVE *theMove         : The move to print. 
**
************************************************************************/

void PrintMove(theMove)
     MOVE theMove;
{
	/* The plus 1 is because the user thinks it's 1-9, but MOVE is 0-8 */
	printf("%d", theMove + 1); 
}

/************************************************************************
*************************************************************************
** BEGIN   FUZZY STATIC EVALUATION ROUTINES. DON'T WORRY ABOUT UNLESS
**         YOU'RE NOT GOING TO EXHAUSTIVELY SEARCH THIS GAME
*************************************************************************
************************************************************************/

/************************************************************************
*************************************************************************
**         EVERYTHING BELOW THESE LINES IS LOCAL TO THIS FILE
*************************************************************************
************************************************************************/

/************************************************************************
**
** NAME:        PositionToBlankOX
**
** DESCRIPTION: convert an internal position to that of a BlankOX.
** 
** INPUTS:      POSITION thePos     : The position input. 
**              BlankOX *theBlankOx : The converted BlankOX output array. 
**
** CALLS:       BadElse()
**
************************************************************************/

void PositionToBlankOX(thePos,theBlankOX)
     POSITION thePos;
     BlankOX *theBlankOX;
{
  int i;
  for(i = 8; i >= 0; i--) {
    if(thePos >= ((int)x * g3Array[i])) {
      theBlankOX[i] = x;
      thePos -= (int)x * g3Array[i];
    }
    else if(thePos >= ((int)o * g3Array[i])) {
      theBlankOX[i] = o;
      thePos -= (int)o * g3Array[i];
    }
    else if(thePos >= ((int)Blank * g3Array[i])) {
      theBlankOX[i] = Blank;
      thePos -= (int)Blank * g3Array[i];
    }
    else
      BadElse("PositionToBlankOX");
  }
}

/************************************************************************
**
** NAME:        BlankOXToPosition
**
** DESCRIPTION: convert a BlankOX to that of an internal position.
** 
** INPUTS:      BlankOX *theBlankOx : The converted BlankOX output array.
**
** OUTPUTS:     POSITION: The equivalent position given the BlankOX.
**
************************************************************************/

POSITION BlankOXToPosition(theBlankOX)
     BlankOX *theBlankOX;
{
  int i;
  POSITION position = 0;

  for(i = 0 ; i < BOARDSIZE ; i++)
    position += g3Array[i] * (int)theBlankOX[i]; /* was (int)position... */

    return(position);
}

/************************************************************************
**
** NAME:        ThreeInARow
**
** DESCRIPTION: Return TRUE iff there are three-in-a-row.
** 
** INPUTS:      BlankOX theBlankOX[BOARDSIZE] : The BlankOX array.
**              int a,b,c                     : The 3 positions to check.
**
** OUTPUTS:     (BOOLEAN) TRUE iff there are three-in-a-row.
**
************************************************************************/

BOOLEAN ThreeInARow(theBlankOX,a,b,c)
     BlankOX theBlankOX[];
     int a,b,c;
{
  return(	theBlankOX[a] == theBlankOX[b] && 
		theBlankOX[b] == theBlankOX[c] &&
		theBlankOX[c] != Blank );
}

/************************************************************************
**
** NAME:        AllFilledIn
**
** DESCRIPTION: Return TRUE iff all the blanks are filled in.
** 
** INPUTS:      BlankOX theBlankOX[BOARDSIZE] : The BlankOX array.
**
** OUTPUTS:     (BOOLEAN) TRUE iff all the blanks are filled in.
**
************************************************************************/

BOOLEAN AllFilledIn(theBlankOX)
     BlankOX theBlankOX[];
{
  BOOLEAN answer = TRUE;
  int i;
	
  for(i = 0; i < BOARDSIZE; i++)
    answer &= (theBlankOX[i] == o || theBlankOX[i] == x);

  return(answer);
}

/************************************************************************
**
** NAME:        WhoseTurn
**
** DESCRIPTION: Return whose turn it is - either x or o. Since x always
**              goes first, we know that if the board has an equal number
**              of x's and o's, that it's x's turn. Otherwise it's o's.
** 
** INPUTS:      BlankOX theBlankOX : The input board
**
** OUTPUTS:     (BlankOX) : Either x or o, depending on whose turn it is
**
************************************************************************/

BlankOX WhoseTurn(theBlankOX)
     BlankOX *theBlankOX;
{
  int i, xcount = 0, ocount = 0;

  for(i = 0 ; i < BOARDSIZE ; i++)
    if(theBlankOX[i] == x)
      xcount++;
    else if(theBlankOX[i] == o)
      ocount++;
    else;                 /* don't count blanks */
  
    if(xcount == ocount)
      return(x);            /* in our TicTacToe, x always goes first */
    else
      return(o);
}

STRING kDBName = "ttt" ;
     
int NumberOfOptions()
{    
  return 2 ;
} 
   
int getOption()
{
  if(gStandardGame) 
    return 1 ;
  return 2 ;
} 

void setOption(int option)
{
  if(option == 1)
    gStandardGame = TRUE;
  else
    gStandardGame = FALSE;
}

