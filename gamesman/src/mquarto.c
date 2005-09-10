// $id$
// $log$

/*
 * The above lines will include the name and log of the last person
 * to commit this file to CVS
 */

/************************************************************************
<<<<<<< mquarto.c
**
** NAME:        mquarto.c
**
** DESCRIPTION: Quarto
**
** AUTHORS:      Yanpei CHEN  <ychen@berkeley.edu>
**               Amy HSUEH    <amyhsueh@berkeley.edu>
**               Mario TANEV  <mtanev@berkeley.edu>
**
** DATE:        Began Jan 2005; 
**
** UPDATE HIST: RECORD CHANGES YOU HAVE MADE SO THAT TEAMMATES KNOW
**
** 30 Jan 2005 Yanpei: the data structure framework added, PrintPosition() coded
** 01 Feb 2005 Yanpei: PrintPosition() wrong, must be corrected later
** 08 Feb 2005 Amy:    corrected my name, changed kTieIsPossible to TRUE.
** 11 Feb 2005 Yanpei: added hashQuarto(), hashQuartoHelper(), setFactorialTable(),
**                     permutation(), combination(), setOffsetTable();
**                     killed incorrect comments and code for PrintPosition(). 
** 14 Feb 2005 Yanpei: one line fix to hashQuarto()
**                     changed static declarations of factorialTable and offsetTable
** 27 Feb 2005 Yanpei: more changes to hash() etc to enable new non-redundant
**                     implementation. unhash() in the works. need printPosition()
**                     before code can be tested independent of core. 
** 27 Feb 2005 Yanpei: more changes to hash() and unhash(), both yet to be ready.
** 28 Feb 2005 Amy:    added gGameSpecificTclInit, as suggested in email
** 05 Mar 2005 Mario:  fixed some ungodly compilation errors and warnings to get this to compile, diff for details
** 06 Mar 2005 Mario:  corrected incorrect behavior of combination() and permutation()
**                     added non-memoizing factorial for debugging purposes (#define DEBUG to enable), currently enabled
**                     corrected defines as ^ is not a power operator in C, but XOR
**                     changed QTBOard field sizes to short as it should be sufficient
**                     added function pointers hash and unhash, to be set to the default implementations
**                     coded inefficient bitpacking hasher and testing function so we can proceed with further coding
**                     added print_board and boards_equal to print and compare board contents (for internal use)
**                     coded PrintPosition and auxilliary functions
**                     buggy implementation of ValidTextInput
**                     made PrintPosition work correctly with >2 dimension boards, try setting GAMEDIMENSION to 3 or 4
** 07 Mar 2005 Mario:  added DoMove, but it doesn't work yet (maybe I am missing a step here, what is ValidMove? )
**                     feel free to modify
** 08 Mar 2005 Mario:  added CreateMove(), GetMovePiece(), GetMoveSlot() abstractions as I realized I
**                     after I realized I had repetitions of code
**                     seemingly complete () coded, whoever is to code the other MOVE related functions
**                     should look at the implementation of PrintMove
**                     started using define constants for things like index of hand in slot array (HAND)
**                     and first and last board indices in slot array (FIRSTSLOT, LASTSLOT)
**                     GenerateMoves() and DoMove() fully coded.
**                     YOU CAN NOW MOVE AROUND BOARD. Try with ws:H then ws:0 and so on.
**                     There's a bug in that players are switched by gamesman, even though a player may get 2 moves
**                     One for placing the piece from the hand to the board, and the other into oponent's hand
**                     Not sure of how to avoid it, 
**                     but I dislike stacking two moves into one as parsing would get even uglier.
**                     HOT & SPICY :There is a scary bug somewhere, a
**                     apparently with a dangling reference to an overwritten stack
**                     It might be in my code or the core code. If anyone dares to solve it,
**                     please look at GetAndPrintPlayersMove and remove the noted comment and run. YIKES!
** 08 Mar 2005 Yanpei: added Primitive(), introduced global constant EMPTYSLOT 
**                     to replace NUMPIECES to encode empty slots in QTBOARD->slots[];
** 08 Mar 2005 Yanpei: EMPTYSLOT changed to 0 to accomodate for present hash()/unhash();
**                     QTBOARD invariants temporarily violated to make things work;
**                     Primitive() changed to reflect this; must change back later. 
** 09 Mar 2005 Mario:  Updated code to use EMPTYSLOT, updated EMPTYSLOT to be NUMPIECES instead of 0. Seems to work.
** 09 Mar 2005 Amy:    added move format in getandPrintPlayersMove(), printComputerMove() coded.
** 10 Mar 2005 Mario:  corrected problem after switch to EMPTYSLOT, made game kPartizan
**                     For some reason the second move is always a win, maybe Primitive needs to be updated
**                     introducing some indirection for manipulating the board, still deciding what
** 11 Mar 2005 Yanpei: EMPTYSLOT issue seems to be over. Primitive() strange behavior caused by toggling
**                     of board->usersTurn upon each move. To deal with this we must have place piece and
**                     select next piece combined into one move as in [square]:[nextPiece]. 
**                     Else we need to have [nextPiece]:H moves not toggle board->usersTurn. 
**                     See new debugging printf's. 
**                     Kind of got the full version of hash figured out. If no bugs, hopefully working tomorrow.
** 14 Mar 2005 Yanpei: HOORAY!!!!! The full blown version of hash and unhash works for GAMEDIMENSION = 2, 3!!!!
**                     Takes a split second to test all 317 positions for GAMEDIMENSION = 2 .... ok. 
**                     Takes 15 min approx to test all 8419329 positions for GAMEDIMENSION = 3 .... scary!
**                     Needs 64 bit machine to test for GAMEDIMENSION = 4. 
**                     Try to understand the full blown hash/unhash if you are looking for something to do. 
**                     I barely understand it myself ... yes it is that ugly and complicated. 
**                     Also added some function points to accomodate multiple implementations.
** 22 Mar 2005 Mario:  Switched to marioInitialize to showcase (seemingly) hashing/unhashing error
**                     Yanpei, please review: compile and run, and watch error output.
**                     Reverted to yanpeiInitialize
** 26 Mar 2005 Yanpei: Some structural changes to allow for variable GAMEDIMENSION. Effect on
**                     existing code should be minimal. Use MallocBoard() and FreeBoard() now
**                     for memory management with boards. Use yanpeiInitializaGame(). 
** 26 Mar 2005 Yanpei: getCannonical() coded and tested. Very straight forward in fact. 
**                     Combined Mario's and Prof Garcia's ideas.
** 26 Mar 2005 Yanpei: Some data of interest: 
**                     GAMEDIMENSION = 2: 317 positions, 17 cannonicals
**                     GAMEDIMENSION = 3: 8419329 positions, 
** 27 Mar 2005 Yanpei: Tried counting total cannonical positions for GAMEDIMENSION = 3
**                     not enough memory. 
** 29 Mar 2005 Yanpei: One line fix to logical error in Primitive().
** 03 Apr 2005 Mario:  Modified DoMove, GenerateMoves to perform 2-moves
**                     Switched to non-memoizing factorial as memoizing produces arithmetic exceptions
**                     I still however have hashing errors
**                     Example: 
**                     [| 3 | X  X  X  X ] pieces = 1; squares = 0; turn = 1
**                     hashes to
**                     [| X | 0  2  3  1 ] pieces = 4; squares = 4; turn = 0
**                     Input handling is not modified yet, but in theory it should work as is
**                     I did update : to . to remind us of the change
** 06 Apr 2005 Mario:  removed packhash(), packunhash(), marioInitializeGame()
**                     When game is started with w, p it is initialized a second time
**                     Thus changed the spot where the is set variables are set - yanpeiInitialize now
**                     Changed print layout
**                     Changed GAMEDIMENSION to 3, game is "playable" now
** 07 Apr 2005 Mario:  Removed PrintBoard(), TestHash()
**                     Made POSITION 64-bits so it works in 4 dimensions (in gamesman.h )
**                     Updated printf format strings for POSITION to POSITION_FORMAT
**                     Changed Primitive() to declare tie if all pieces are on board and no win on board
**                     Previous behavior was if board was full, which doesn't work well in 3 dimensions
**                     Stopped using fflush() for inputs (game-specific menu) as its behavior is undefined
**                     Game is now solvable in 2 and 3D and doesn't have enough mem for 4D solving
** 21 Apr 2005 Yanpei: Should not peek under the hood and set gSymmetries. One line fix in 
**                     yanpeiInitializeGame()
** 25 Apr 2005 Mario:  Added GPS support. Works only if symmetries are on. 
**                     I don't know why, but in 3 dimensions if symmetries are OFF, I get errors even in non-GPS.
** 08 Sep 2005 Yanpei: What is going on? Quarto has error now!!!! Mario what did you change with GPS?
**                     kGameName changed to "Quarto" at Dan's request.
** 09 Sep 2005 Amy:    Changed the way moves are printed/inputted. Also support for 1-dim quarto added. yay! Format for moves is now
                       [slot to place piece],[new piece to select]
**************************************************************************/


/*************************************************************************
 **
 ** Everything below here must be in every game file
 **
 **************************************************************************/

#include <stdio.h>
#include "gamesman.h"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define	DEBUG 1

/***************** ********************************************************
 **
 ** Game-specific constants
 **
 **************************************************************************/

STRING   kGameName            = "Quarto"; /* The name of your game */
STRING   kAuthorName          = "Yanpei CHEN, Amy HSUEH, Mario TANEV"; /* Your name(s) */
STRING   kDBName              = ""; /* The name to store the database under */

BOOLEAN  kPartizan            = TRUE ; /* A partizan game is a game where each player has different moves from the same board (chess - different pieces) */
BOOLEAN  kGameSpecificMenu    = TRUE ; /* TRUE if there is a game specific menu. FALSE if there is not one. */
BOOLEAN  kTieIsPossible       = TRUE ; /* TRUE if a tie is possible. FALSE if it is impossible.*/
BOOLEAN  kLoopy               = FALSE ; /* TRUE if the game tree will have cycles (a rearranger style game). FALSE if it does not.*/

BOOLEAN  kDebugMenu           = TRUE ; /* TRUE only when debugging. FALSE when on release. */
BOOLEAN  kDebugDetermineValue = TRUE ; /* TRUE only when debugging. FALSE when on release. */

/*added by amy */
BOOLEAN amyDebug                 = TRUE; /* TRUE when debugging, prints debug stuff */
/*added by amy */ 

POSITION gNumberOfPositions   =  0; /* The number of total possible positions | If you are using our hash, this is given by the hash_init() function*/
POSITION gInitialPosition     =  0; /* The initial hashed position for your starting board */
POSITION kBadPosition         = -1; /* A position that will never be used */

/* 
 * Help strings that are pretty self-explanatory
 * Strings than span more than one line should have backslashes (\) at the end of the line.
 */

STRING kHelpGraphicInterface =
"Not written yet";

STRING   kHelpTextInterface    =
""; 

STRING   kHelpOnYourTurn =
"";

STRING   kHelpStandardObjective =
"";

STRING   kHelpReverseObjective =
"";

STRING   kHelpTieOccursWhen =
"A tie occurs when ...";

STRING   kHelpExample =
"";


/*************************************************************************
 **
 ** #defines and structs
 **
 **************************************************************************/

/* Basic error codes */
#define ERROR	1
#define SUCCESS	0

#define newline() printf( "\n" )

/* Squares x */
#define square(x) ((x)*(x))

/* Creates sequence n least significant 1 bits, preceeded by 0 bits */
#define maskseq(n) ~(~0<<(n))

int GAMEDIMENSION = 3;

int BOARDSIZE;
int NUMPIECES;

int EMPTYSLOT;

int FIRSTSLOT;
int LASTSLOT;

int FACTORIALMAX;

int HAND = 0;


typedef struct board_item {

    short *slots;             // to record the 0 to NUMPIECES-1 pieces contained in each slot
    // slots[1-16] = board squares, slots[0] = next piece
    // 0 to NUMPIECES-1 encode the pieces, EMPTYSLOT encodes an empty slot
    short squaresOccupied;    // number of squares occupied
    short piecesInPlay;       // number of pieces in play
    BOOLEAN usersTurn;        // whose turn it is

} QTBOARD;

typedef QTBOARD* QTBPtr;

/* Letter codes for the different piece states */
char states[][2]={{'w', 'B'}, {'s', 'T'}, {'h', 'S'}, {'r', 'E'}};

/* ASCII Hex */
char hex_ascii[] = { 'H', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'y', 'Z' };


/*************************************************************************
 **
 ** Global Variables
 **
 *************************************************************************/

BOOLEAN factorialTableSet;
POSITION *factorialTable;
BOOLEAN offsetTableSet;
POSITION *offsetTable;

/*************************************************************************
 **
 ** Function Prototypes
 **
 *************************************************************************/

/* External */
extern GENERIC_PTR	SafeMalloc ();
extern void		SafeFree ();
void* 			gGameSpecificTclInit = NULL;  /* newly added to tempalte */

/* External */
QTBPtr                  MallocBoard();
void                    FreeBoard(QTBPtr b);
void			setOffsetTable();
POSITION		combination(int n, int r);
POSITION		permutation(int n, int r);

/* Implementations of hash/unhash */
POSITION                hashUnsymQuarto(QTBPtr b);
QTBPtr                  unhashUnsymQuarto(POSITION p);
/* Implementations of InitializeGame */
void                    yanpeiInitializeGame();
/* Implementations of PrintPosition */
void                    marioPrintPos(POSITION position, STRING playersName, BOOLEAN usersTurn );
void                    yanpeiPrintSlots(POSITION position, STRING playersName, BOOLEAN usersTurn );
/* Implementations of factorial */
POSITION                factorialMem(int n);
POSITION                factorialNoMem(int n);
/* Implementations of getCannonical */
POSITION                yanpeiGetCannonical(POSITION p);

/* Since we may switch implementations, here are function pointers to be set in choosing implementation */
POSITION                (*hash)( QTBPtr ) = &hashUnsymQuarto;
QTBPtr                  (*unhash)( POSITION ) = &unhashUnsymQuarto;
void                    (*initGame)( ) = &yanpeiInitializeGame;
void                    (*printPos)(POSITION position, STRING playersName, BOOLEAN usersTurn ) = &marioPrintPos;
//void                    (*printPos)(POSITION position, STRING playersName, BOOLEAN usersTurn ) = &yanpeiPrintSlots;
POSITION                (*factorial)(int n) = &factorialMem;
POSITION                (*getCannonical)(POSITION p) = &yanpeiGetCannonical;

/* support functions */
POSITION                hashUnsymQuartoHelper(QTBPtr b, int baseSlot);
void                    unhashUnsymQuartoHelper(POSITION p, int baseSlot, QTBPtr toReturn);

BOOLEAN                 searchPrimitive(short *);

POSITION                rotateBoard90(POSITION p);
POSITION                reflectBoard(POSITION p);
POSITION                maskBoard(POSITION p, short mask);

void                    yanpeiTestOffset();
void                    yanpeiTestHash();
void                    yanpeiTestCannonicalSupport();
void                    yanpeiTestCannonical();

void                    FreePosList(POSITIONLIST *l);
BOOLEAN                 SearchPosList(POSITIONLIST *l, POSITION p);

BOOLEAN                 boards_equal ( QTBPtr, QTBPtr );
void                    print_board( QTBPtr );
QTBPtr                  TestHash( QTBPtr, int );

MOVE			CreateMove( MOVE slot, MOVE piece );
MOVE			GetMovePiece( MOVE move );
MOVE			GetMoveSlot( MOVE move );
unsigned short	        GetHandPiece( QTBPtr );
void			SetHandPiece( QTBPtr, unsigned short );
unsigned short	        GetBoardPiece( QTBPtr, unsigned short );

void PrintCell( void *cell, char (*CellContent)( short, void * ) );
void PrintBoard( void *cells, size_t content_size, char *heading, char (*CellContent)( short, void * ) );
char LegendCoordinate( short pad, void *p_coordinate );
char PieceTrait( short trait, void *p_piece );
char BlankCell( short ignored, void *p_ignored );
char BorderCell( short ignored, void* p_ignored );
void PrintHorizontalBorder( char fill, char border, char *startmark, char *endmark );
void PrintRange( void *cells, size_t content_size, int offset, int size, char (*CellContent)( short, void * ), char border, char *endmark );
char readchar( );
void UndoMove(MOVE move);


QTBPtr		GPSBoard;

/*************************************************************************
 **
 ** Global Database Declaration
 **
 **************************************************************************/

extern VALUE     *gDatabase;

/************************************************************************
 **
 ** NAME:        InitializeGame
 **
 ** DESCRIPTION: Prepares the game for execution.
 **              Initializes required variables.
 **              Sets up gDatabase (if necessary).
 ** 
 ************************************************************************/

void InitializeGame ()
{

    initGame();
  
}

// Yanpei's implementation, to be used with the full blown hash/unhash
// Rips off some code from Mario's implementation
void yanpeiInitializeGame() {

    QTBPtr board;
    short slot;

	
    /* initializing globals */
    factorialTableSet = FALSE; // Mario added to work around double calling of InitializeGame
    offsetTableSet = FALSE; // Mario added to work around double calling of InitializeGame
    BOARDSIZE = square(GAMEDIMENSION);
    NUMPIECES = (1 << GAMEDIMENSION);
    EMPTYSLOT = NUMPIECES;
    FIRSTSLOT = 1;
    LASTSLOT = BOARDSIZE;
    if (BOARDSIZE<NUMPIECES)
	FACTORIALMAX = (NUMPIECES+1);
    else
	FACTORIALMAX = (BOARDSIZE+1);
    //factorialTable = (POSITION *) SafeMalloc(FACTORIALMAX*(sizeof(POSITION)));
    factorialTable = (POSITION *) malloc( FACTORIALMAX * (sizeof( POSITION ) ) );
    offsetTable = (POSITION *) SafeMalloc((NUMPIECES+2)*(sizeof(POSITION)));
	
    if(!offsetTableSet) setOffsetTable();
    board = MallocBoard();

    /* Initialize all fields to 0 */
    board->squaresOccupied = 0;
    board->piecesInPlay = 0;
    board->usersTurn = FALSE;
	
    /* Initialize all slots to EMPTYSLOT */
    for(slot=0; slot<BOARDSIZE+1; slot++) {
	board->slots[slot] = EMPTYSLOT;  
    }

    /* calls to test functions */
    //yanpeiTestOffset();
    //yanpeiTestHash();
    //yanpeiTestCannonicalSupport();
    //yanpeiTestCannonical();

    /* Set initial position to empty board */
    gCanonicalPosition = getCannonical;
    gInitialPosition = hash(board);
    gNumberOfPositions = offsetTable[NUMPIECES+1];
	
    GPSBoard = board;
    gUndoMove = &UndoMove;
   
    printf("\n"); 
    printf("gInitialPosition = " POSITION_FORMAT "\n",gInitialPosition);
    printf("gNumberOfPositions = " POSITION_FORMAT "\n",gNumberOfPositions);

}


/************************************************************************
 **
 ** NAME:        GenerateMoves
 **
 ** DESCRIPTION: Creates a linked list of every move that can be reached
 **              from this position. Returns a pointer to the head of the
 **              linked list.
 ** 
 ** INPUTS:      POSITION position : Current position for move
 **                                  generation.
 **
 ** OUTPUTS:     (MOVELIST *)      : A pointer to the first item of
 **                                  the linked list of generated moves
 **
 ** CALLS:       MOVELIST *CreateMovelistNode();
 **
 ************************************************************************/
int FlagAvailablePieces( QTBPtr board, BOOLEAN pieces[] ) {

    int slot;
    int available = NUMPIECES;

    /* Initialize array of pieces */
    memset( pieces, TRUE, sizeof( *pieces ) * NUMPIECES );
		
    /* For each slot on board */
    for( slot = 0; slot < BOARDSIZE + 1; slot++ ) {
			
	/* If slot is not empty */
	if( board->slots[slot] != EMPTYSLOT ) {
				
	    /* Mark piece unavailable */
	    pieces[board->slots[slot]] = FALSE;
	    available--;
				
	}
			
    }
	
    return available;
		
}

MOVELIST *GenerateMoves (POSITION position)
{
    
    QTBPtr board;
    MOVELIST *moves	= NULL;
    MOVE slot;
    MOVE piece;

    /* Use GPS board if GPS solving, otherwise use unhashed position */
    board = gUseGPS ? GPSBoard : unhash( position );

    /* If there's no piece in hand, the only valid moves are the ones placing a piece into the hand */
    /* These are special moves, as their board position indicates HAND, and are only valid for a first move */
    if( GetHandPiece( board ) == EMPTYSLOT ) {
		
	/* For every piece possible (since board is initial, every piece is allowed into the game */
	for( piece = 0; piece < NUMPIECES; piece++ ) {
			
	    /* Generate a move that places the piece into the hand. The slot = HAND is indicative of initial move */
	    moves	= CreateMovelistNode( CreateMove( HAND, piece ), moves );
			
	}
		
    } else {
		
	BOOLEAN available_pieces[NUMPIECES];
	MOVE piece;
	int available;
				
	available = FlagAvailablePieces( board, available_pieces );
		
	/* For each slot on board */
	for( slot = FIRSTSLOT; slot <= LASTSLOT; slot++ ) {
			
	    /* If slot is empty */
	    if ( board->slots[slot] == EMPTYSLOT ) {
				
		if( available == 0 ) {
					
		    moves = CreateMovelistNode( CreateMove( slot, board->slots[HAND] ), moves );
					
		} else {
				
		    /* For each piece */
		    for( piece = 0; piece < NUMPIECES; piece++ ) {
			
			/* If piece is available */
			if( available_pieces[piece] != FALSE ) {
		
			    /* Add move which moves item from hand into slot, and piece into hand */
			    moves	= CreateMovelistNode( CreateMove( slot, piece ), moves );
				
			}
					
		    }
					
		}
				
	    }
			
	}
		
    }

    /* Return list of valid moves */
    return moves;
	
}

/************************************************************************
 **
 ** NAME:        DoMove
 **
 ** DESCRIPTION: Applies the move to the position.
 ** 
 ** INPUTS:      POSITION position : The old position
 **              MOVE     move     : The move to apply to the position
 **
 ** OUTPUTS:     (POSITION)        : The position that results from move
 **
 ** CALLS:       Some Board Hash Function
 **              Some Board Unhash Function
 **
 *************************************************************************/

POSITION DoMove (POSITION position, MOVE move)
{
	
    QTBPtr board;
    int piece, slot;
	 
    /* Use GPS board if GPS solving, otherwise use unhashed position */
    board = gUseGPS ? GPSBoard : unhash( position );
    
    /* Determine slot on board piece from hand is to go into */
    slot = GetMoveSlot( move );
	
    /* Determine which piece is to go into hand */
    piece = GetMovePiece( move );
	
    /* Increment number of pieces */
    board->piecesInPlay += ( piece == GetHandPiece( board ) ) ? 0 : 1;
	
    /* Place hand piece into indicated slot */
    board->slots[slot] = GetHandPiece( board );
	
    /* Place indicated piece into hand */
    SetHandPiece( board, ( piece == board->slots[HAND] ) ? EMPTYSLOT : piece );
	
    /* If indicated slot is not hand, also increment number of squares */
    board->squaresOccupied += ( slot == HAND ) ? 0 : 1;
   
    /* Return hashed board */
    return hash( board );
	
}

void UndoMove( MOVE move )
{
  
    int piece, slot;
    QTBPtr board = GPSBoard;
 
    /* Determine slot on board piece from hand is to go into */
    slot = GetMoveSlot( move );
  
    /* Determine which piece is to go into hand */
    piece = GetMovePiece( move );
  
    /* Decrement number of pieces */
    board->piecesInPlay -= ( GetHandPiece( board ) == EMPTYSLOT ) ? 0 : 1;
  
    /* Move piece from board into hand */
    SetHandPiece( board, board->slots[slot] );
 
    /* Empty indicated slot */
    board->slots[slot] = EMPTYSLOT;
  
    /* If indicated slot is not hand, also decrement number of squares */
    board->squaresOccupied -= ( slot == HAND ) ? 0 : 1;
  
}

/************************************************************************
 **
 ** NAME:        Primitive
 **
 ** DESCRIPTION: Returns the value of a position if it fulfills certain
 **              'primitive' constraints.
 **
 **              Example: Tic-tac-toe - Last piece already placed
 **
 **              Case                                  Return Value
 **              *********************************************************
 **              Current player sees three in a row    lose
 **              Entire board filled                   tie
 **              All other cases                       undecided
 ** 
 ** INPUTS:      POSITION position : The position to inspect.
 **
 ** OUTPUTS:     (VALUE)           : one of
 **                                  (win, lose, tie, undecided)
 **
 ** CALLS:       None              
 **
 ************************************************************************/

VALUE Primitive (POSITION position)
{
    QTBPtr b;
    short *rowColDiag = (short *) SafeMalloc(GAMEDIMENSION*sizeof(short));
    BOOLEAN primitiveFound = FALSE;
    BOOLEAN emptyFound = FALSE;
    int i,j;
    VALUE toReturn;

    /* Use GPS board if GPS solving, otherwise use unhashed position */
    b = gUseGPS ? GPSBoard : unhash( position );
		
    // print debugging stuff
    /*
      printf("**** Primitive()\n");
      printf("**** The slots are: ");
      for (i=0; i<BOARDSIZE+1; i++) {
      printf("%d,",b->slots[i]);
      }
      printf("\n**** usersTurn is %s\n", (b->usersTurn) ? "TRUE" : "FALSE");
      printf("**** Primitive()\n");      
    */

    // checking the ranks/files in one direction
    i=0;
    while (!primitiveFound && i<GAMEDIMENSION) {
	j=0;
	while (!emptyFound && j<GAMEDIMENSION) {
	    if (b->slots[i*GAMEDIMENSION+j+1] == EMPTYSLOT) {
		emptyFound = TRUE;
	    }
	    rowColDiag[j] = b->slots[i*GAMEDIMENSION+j+1];
	    j++;
	}
	if (!emptyFound) primitiveFound = searchPrimitive(rowColDiag);
	i++;
	emptyFound = FALSE;
    }

    // checking the ranks/files in the other direction
    i=0;
    while (!primitiveFound && i<GAMEDIMENSION) {
	j=0;
	while (!emptyFound && j<GAMEDIMENSION) {
	    if (b->slots[j*GAMEDIMENSION+i+1] == EMPTYSLOT) {
		emptyFound = TRUE;
	    }
	    rowColDiag[j] = b->slots[j*GAMEDIMENSION+i+1];
	    j++;
	}
	if (!emptyFound) primitiveFound = searchPrimitive(rowColDiag);
	i++;
	emptyFound = FALSE;
    }

    // checking one of the diagonals
    i=0;
    while (!primitiveFound && !emptyFound && i<GAMEDIMENSION) {
	if (b->slots[i*GAMEDIMENSION+i+1] == EMPTYSLOT) {
	    emptyFound = TRUE;
	}
	rowColDiag[i] = b->slots[i*GAMEDIMENSION+i+1];
	i++;
    }
    if (!emptyFound) primitiveFound = searchPrimitive(rowColDiag);
    emptyFound = FALSE;

    // checking the other diagonal
    i=0;
    while (!primitiveFound && !emptyFound && i<GAMEDIMENSION) {
	if (b->slots[(i+1)*GAMEDIMENSION-i] == EMPTYSLOT) {
	    emptyFound = TRUE;
	}
	rowColDiag[i] = b->slots[(i+1)*GAMEDIMENSION-i];
	i++;
    }
    if (!emptyFound) primitiveFound = searchPrimitive(rowColDiag);
    emptyFound = FALSE;

    // returning stuff
    if (primitiveFound) {
	toReturn = lose;
    } else {
	// Mario: comparing to NUMPIECES instead of BOARDSIZE
	toReturn = (b->squaresOccupied<NUMPIECES) ? undecided : tie;
    }

    SafeFree(rowColDiag);
    if( !gUseGPS) FreeBoard(b);

    return toReturn;

}

// helper function called by Primitive()
// precondition: all elements of rowColDiag != EMPTYSLOT
// returns true iff rowColDiag is a primitive
BOOLEAN searchPrimitive(short *rowColDiag) {

    short inverterMask = NUMPIECES-1;
    short noninvertedResult = rowColDiag[0];
    short invertedResult = inverterMask ^ rowColDiag[0];
    short i;

    // print debugging stuff
    /*
      printf("**** searchPrimitive() examines ");
      for (i=0; i<GAMEDIMENSION; i++) {
      printf("%d,",rowColDiag[i]);
      }
      printf("\n");
    */

    // detects primitives through cumulative bitwise &
    for (i=0; i<GAMEDIMENSION; i++) {
	noninvertedResult &= rowColDiag[i];
	invertedResult &= inverterMask ^ rowColDiag[i];
    }

    return (noninvertedResult>0 || invertedResult>0);

}

/************************************************************************
 **
 ** NAME:        PrintPosition
 **
 ** DESCRIPTION: Prints the position in a pretty format, including the
 **              prediction of the game's outcome.
 ** 
 ** INPUTS:      POSITION position    : The position to pretty print.
 **              STRING   playersName : The name of the player.
 **              BOOLEAN  usersTurn   : TRUE <==> it's a user's turn.
 **
 ** CALLS:       Unhash()
 **              GetPrediction()      : Returns the prediction of the game
 **
 ************************************************************************/

	
void PrintPosition ( POSITION position, STRING playersName, BOOLEAN usersTurn )
{
    newline();
    printPos(position,playersName,usersTurn);
}

void marioPrintPos(POSITION position, STRING playersName, BOOLEAN usersTurn )
{

    QTBPtr board;
    int j, rsize;
    BOOLEAN pieces[NUMPIECES], comma;
	
    /* Use GPS board if GPS solving, otherwise use unhashed position */
    board = gUseGPS ? GPSBoard : unhash( position );
	
    for( rsize = GAMEDIMENSION, j = FIRSTSLOT; j <= LASTSLOT; j+=rsize ) {
	
	if( j == FIRSTSLOT ) {
	  //printf("debugging mode:");
	  // newline();				
	    printf("        +");
	    PrintCell( NULL, BorderCell );
	    PrintRange( hex_ascii, sizeof( *hex_ascii ), j, rsize, BorderCell, '+', "" );
	    printf("                +");
	    PrintCell( NULL, BorderCell );
	    PrintRange( hex_ascii, sizeof( *hex_ascii ), j, rsize, BorderCell, '+', "\n" );
	    printf("LEGEND: |");
	    PrintCell( hex_ascii + HAND * sizeof( char ), LegendCoordinate );
		
	} else {
		
	    printf( "         " );
	    PrintCell( NULL, BlankCell );
			
	}
	PrintRange( hex_ascii, sizeof( *hex_ascii ), j, rsize, LegendCoordinate, '|', "" );
	if( j == FIRSTSLOT ) {
	    printf("         BOARD: |");
	    PrintCell( board->slots + HAND * sizeof( *board->slots ), PieceTrait );
		
	} else {
			
	    printf( "                 " );
	    PrintCell( NULL, BlankCell );
			
	}
	PrintRange( board->slots, sizeof( *board->slots ), j, rsize, PieceTrait, '|', "\n" );
		
	if( j == FIRSTSLOT ) {
			
	    printf("        +");
	    PrintCell( NULL, BorderCell );
	    PrintRange( hex_ascii, sizeof( *hex_ascii ), j, rsize, BorderCell, '+', "" );
	    printf("                +");
	    PrintCell( NULL, BorderCell );
	    PrintRange( hex_ascii, sizeof( *hex_ascii ), j, rsize, BorderCell, '+', "\n" );
			
	} else {
			
	    printf("         ");
	    PrintCell( NULL, BlankCell );
	    PrintRange( hex_ascii, sizeof( *hex_ascii ), j, rsize, BorderCell, '+', "" );
	    printf("                 ");
	    PrintCell( NULL, BlankCell );
	    PrintRange( hex_ascii, sizeof( *hex_ascii ), j, rsize, BorderCell, '+', "\n" );
			
	}
		
    }
	
    FlagAvailablePieces( board, pieces );
    printf("\nAvailable pieces: ");
	
    for( comma = FALSE, j = 0; j < NUMPIECES; j++ ) {

	if( pieces[j] ) {

	    if ( comma ) printf( ", " ); else comma = TRUE;
	    PrintCell( &j, PieceTrait );

	}

    }
    newline();

}

// very crude printSlots for testing hash/unhash
void yanpeiPrintSlots(POSITION position, STRING playersName, BOOLEAN usersTurn ) {

    QTBPtr b = unhash(position);
    short i;

    printf("slots: ");
    for (i=0; i<BOARDSIZE+1; i++) {
	if (b->slots[i] != EMPTYSLOT) {
	    printf("%3d",b->slots[i]);
	} else {
	    printf("  -");
	}
    }
    newline();
    //printf("squaresOccupied %d\n",b->squaresOccupied);
    //printf("piecesInPlay    %d\n",b->piecesInPlay);


}

/************************************************************************
 **
 ** NAME:        PrintComputersMove
 **
 ** DESCRIPTION: Nicely formats the computers move.
 ** 
 ** INPUTS:      MOVE    computersMove : The computer's move. 
 **              STRING  computersName : The computer's name. 
 **
 ************************************************************************/

void PrintComputersMove (MOVE computersMove, STRING computersName)
{
    printf( "%s's move was: ", computersName);
    PrintMove(computersMove);
    newline();
}


/************************************************************************
 **
 ** NAME:        PrintMove
 **
 ** DESCRIPTION: Prints the move in a nice format.
 ** 
 ** INPUTS:      MOVE move         : The move to print. 
 **
 ************************************************************************/

void PrintMove( MOVE move )
{
	
    unsigned short slot, piece, trait;
	
    /* Determine piece information */
    piece	= GetMovePiece( move );
	
    /* Determine slot information */
    slot	= GetMoveSlot( move );
	
    /* Print character describing position on board/hand */
    printf( "%c,", hex_ascii[slot] );

    /* For each piece trait */
    for( trait = 0; trait < GAMEDIMENSION; trait++ ) {
		
	/* Print the corresponding character describing its state */
	printf( "%c", states[trait][(piece >> trait) & 1] );
				
    }
	

	
}


/************************************************************************
 **
 ** NAME:        GetAndPrintPlayersMove
 **
 ** DESCRIPTION: Finds out if the player wishes to undo, abort, or use
 **              some other gamesman option. The gamesman core does
 **              most of the work here. 
 **
 ** INPUTS:      POSITION position    : Current position
 **              MOVE     *move       : The move to fill with user's move. 
 **              STRING   playersName : Current Player's Name
 **
 ** OUTPUTS:     USERINPUT          : One of
 **                                   (Undo, Abort, Continue)
 **
 ** CALLS:       USERINPUT HandleDefaultTextInput(POSITION, MOVE*, STRING)
 **                                 : Gamesman Core Input Handling
 **
 ************************************************************************/

USERINPUT GetAndPrintPlayersMove (POSITION position, MOVE *move, STRING playersName)
{
    USERINPUT input;
    USERINPUT HandleDefaultTextInput();
	
    for (;;) {
			
	int i;
	printf("\n%s's move [(undo)/", playersName );

	//printf("[%c,%c-%c]], ", hex_ascii[HAND], hex_ascii[FIRSTSLOT], hex_ascii[LASTSLOT] );
	printf("[%c-%c],[", hex_ascii[FIRSTSLOT], hex_ascii[LASTSLOT] );
	for( i = 0; i < GAMEDIMENSION; i++ ) {
				
	    printf( "(%c,%c)", states[i][0], states[i][1] );
				
	}
	printf("]]: ");
	input = HandleDefaultTextInput(position, move, playersName);
	
	if (input != Continue) {
			
	    return input;
			
	}
		
    }
	
    /* NOTREACHED */
    return Continue;
}


/************************************************************************
 **
 ** NAME:        ValidTextInput
 **
 ** DESCRIPTION: Rudimentary check to check if input is in the move form
 **              you are expecting. Does not check if it is a valid move.
 **              Only checks if it fits the move form.
 **
 **              Reserved Input Characters - DO NOT USE THESE ONE CHARACTER
 **                                          COMMANDS IN YOUR GAME
 **              ?, s, u, r, h, a, c, q
 **                                          However, something like a3
 **                                          is okay.
 ** 
 **              Example: Tic-tac-toe Move Format : Integer from 1 to 9
 **                       Only integers between 1 to 9 are accepted
 **                       regardless of board position.
 **                       Moves will be checked by the core.
 **
 ** INPUTS:      STRING input : The string input the user typed.
 **
 ** OUTPUTS:     BOOLEAN      : TRUE if the input is a valid text input.
 **
 ************************************************************************/

BOOLEAN ValidTextInput( STRING input )
{
	
    BOOLEAN valid = FALSE;
	
    if ( ( strlen( input ) == ( 2 + GAMEDIMENSION ) ) && input[1] == ',' ) {
		
	int i;
	int valid_traits[GAMEDIMENSION];
		
	memset( valid_traits, 0, GAMEDIMENSION * sizeof( int ) );		

	// Checking if position indicated is valid
	for( i = 0; i < BOARDSIZE+1; i++ ) {
	  if (amyDebug) {
	    printf ("the current slot is : %c, the input position is: %c\n", hex_ascii[i], input[0]);
	    printf("are they equal? %d\n", input[0] == hex_ascii[i] );
	  }
	    if( input[0] == hex_ascii[i] ) {
	      if (amyDebug) printf("%c is, a valid position\n", input[0]);		
		valid = TRUE;
		break;
				
	    }
			
	}
		
	if ( valid ) {
	  //check traits		   
	    int trait;
			
	    for( trait = 2; trait < GAMEDIMENSION+2; trait++ ) {
		for( i = 0; i < GAMEDIMENSION; i++ ) {
		  if (amyDebug)  printf("the input trait: %c, the states comparing: %c, %c\n", input[trait], states[i][0], states[i][1]);
		    if ( input[trait] == states[i][0] || input[trait] == states[i][1] ) {
			BOOLEAN repeat = FALSE;
			int j;
						
			for( j = 2; j < trait; j++ ) {
			    if ( valid_traits[j] == i + 1 ) {
				repeat = TRUE;
			    }
			}
			if ( !repeat ) {
			    valid_traits[trait] = i + 1;
			    break;
			}
		    }
		}
				
		if ( valid_traits[trait] == 0 ) {
					
		    valid = FALSE;
		    break;
					
		}
			
	    }
	}
		
    } 
	
    printf("Move does%s adhere to valid syntax\n", valid ? "" : " not" );
    return valid;
}


/************************************************************************
 **
 ** NAME:        ConvertTextInputToMove
 **
 ** DESCRIPTION: Converts the string input your internal move representation.
 **              Gamesman already checked the move with ValidTextInput
 **              and ValidMove.
 ** 
 ** INPUTS:      STRING input : The VALID string input from the user.
 **
 ** OUTPUTS:     MOVE         : Move converted from user input.
 **
 ************************************************************************/

MOVE ConvertTextInputToMove (STRING input)
{
	
    MOVE piece = 0, slot = 0;
    int i, j, k;
	
    /* Lower GAMEDIMENSION + 1 bits for position */
    for( slot = 0; ( slot < BOARDSIZE + 1 ) && hex_ascii[slot] != input[0]; slot++ );
	
    /* Adjacent GAMEDIMENSION bits for piece */
    for( i = 2; i < GAMEDIMENSION+2; i++ ) {
		
	BOOLEAN ready = FALSE;
		
	for( j = 0; j < GAMEDIMENSION; j++ ) {
			
	    for( k = 0; k < 2; k++ ) {
				
		if( states[j][k] == input[i] ) {
				
		    piece += k << j;
		    ready = TRUE;
		    break;
					
		}
				
	    }
			
	    if ( ready ) {
				
		break;
				
	    }
			
	}
		
    }

    return CreateMove( slot, piece );

}


/************************************************************************
 **
 ** NAME:        GameSpecificMenu
 **
 ** DESCRIPTION: Prints, receives, and sets game-specific parameters.
 **
 **              Examples
 **              Board Size, Board Type
 **
 **              If kGameSpecificMenu == FALSE
 **                   Gamesman will not enable GameSpecificMenu
 **                   Gamesman will not call this function
 ** 
 **              Resets gNumberOfPositions if necessary
 **
 ************************************************************************/

void GameSpecificMenu ()
{
    char choice;
    BOOLEAN validInput = FALSE;

    while (!validInput) {
	printf("\t---- mquarto Option Menu ---- \n\n");
	printf("\ti)\tChange Game D(i)mension: Currently %d-Dimensional\n",GAMEDIMENSION);
    
	printf("Select an option: ");
	//fflush(stdin); // This should only work on windows, flushing input is an ambiguous concept
	//flush();
	choice = readchar();
	choice = toupper(choice);
	switch(choice) {
	case 'I':
	    printf("Please enter the new GAMEDIMENSION (must be less than 4): ");
	    scanf("%d",&GAMEDIMENSION);
	    InitializeGame( );
	    validInput = TRUE;
	    break;
	default:
	    printf("Not a valid option.\n");
	    break;
	}
    }
 
}


/************************************************************************
 **
 ** NAME:        SetTclCGameSpecificOptions
 **
 ** DESCRIPTION: Set the C game-specific options (called from Tcl)
 **              Ignore if you don't care about Tcl for now.
 ** 
 ************************************************************************/

void SetTclCGameSpecificOptions (int options[])
{
    
}
  
  
/************************************************************************
 **
 ** NAME:        GetInitialPosition
 **
 ** DESCRIPTION: Called when the user wishes to change the initial
 **              position. Asks the user for an initial position.
 **              Sets new user defined gInitialPosition and resets
 **              gNumberOfPositions if necessary
 ** 
 ** OUTPUTS:     POSITION : New Initial Position
 **
 ************************************************************************/

POSITION GetInitialPosition ()
{
    return 0;
}


/************************************************************************
 **
 ** NAME:        NumberOfOptions
 **
 ** DESCRIPTION: Calculates and returns the number of variants
 **              your game supports.
 **
 ** OUTPUTS:     int : Number of Game Variants
 **
 ************************************************************************/

int NumberOfOptions ()
{
    return 0;
}


/************************************************************************
 **
 ** NAME:        getOption
 **
 ** DESCRIPTION: A hash function that returns a number corresponding
 **              to the current variant of the game.
 **              Each set of variants needs to have a different number.
 **
 ** OUTPUTS:     int : the number representation of the options.
 **
 ************************************************************************/

int getOption ()
{
    return 0;
}


/************************************************************************
 **
 ** NAME:        setOption
 **
 ** DESCRIPTION: The corresponding unhash function for game variants.
 **              Unhashes option and sets the necessary variants.
 **
 ** INPUT:       int option : the number representation of the options.
 **
 ************************************************************************/

void setOption (int option)
{
    
}


/************************************************************************
 **
 ** NAME:        DebugMenu
 **
 ** DESCRIPTION: Game Specific Debug Menu (Gamesman comes with a default
 **              debug menu). Menu used to debug internal problems.
 **
 **              If kDebugMenu == FALSE
 **                   Gamesman will not display a debug menu option
 **                   Gamesman will not call this function
 ** 
 ************************************************************************/

void DebugMenu ()
{
    
}


/************************************************************************
 **
 ** Everything specific to this module goes below these lines.
 **
 ** Things you want down here:
 ** Move Hasher
 ** Move Unhasher
 ** Any other function you deem necessary to help the ones above.
 ** 
 ************************************************************************/

// printing an unsigned int as a GAMEDIMENSION digits binary characters
STRING binStr(unsigned int x) {

    int i;
    STRING toReturn = (STRING) SafeMalloc(GAMEDIMENSION*sizeof(char));

    for (i=0; i<GAMEDIMENSION; i++) {
	toReturn[GAMEDIMENSION-1-i] = (x % 2) + '0';
	x = x/2;
    }

    return toReturn;
   
}

QTBPtr MallocBoard() {
    QTBPtr toReturn = (QTBPtr) SafeMalloc(sizeof(QTBOARD));
    toReturn->slots = (short *) SafeMalloc((BOARDSIZE+1)*sizeof(short));
    return toReturn;
}

void FreeBoard(QTBPtr b) {
    SafeFree(b->slots);
    SafeFree(b);
}

/* Mario: returns true if board contents match exactly */
BOOLEAN boards_equal ( QTBPtr b1, QTBPtr b2 ) {

    BOOLEAN equal = TRUE;
    int slot;

    equal &= ( b1->piecesInPlay == b2->piecesInPlay );
    equal &= ( b1->squaresOccupied == b2->squaresOccupied );
    equal &= ( b1->usersTurn == b2->usersTurn );

    for( slot = 0; slot < BOARDSIZE + 1; slot++ ) {

	equal &= ( b1->slots[slot] == b2->slots[slot] );

    }

    return equal;

}

/* Mario: prints board (for internal use)*/
void print_board( QTBPtr board ) {

    int slot;
	
    fprintf( stderr, "[" );
    for ( slot = 0; slot < BOARDSIZE + 1; slot++ ) {
		
	if ( slot == HAND ) {
			
	    fprintf( stderr, "|" );
			
	}
		
	if( board->slots[slot] == EMPTYSLOT ) {
			
	    fprintf( stderr, " X " );
			
	} else {
			
	    fprintf( stderr, " %d ", board->slots[slot] );
			
	}
		
	if ( slot == HAND ) {
			
	    fprintf( stderr, "|" );
			
	}

    }
    fprintf( stderr, "] pieces = %d; squares = %d; turn = %d ", board->piecesInPlay, board->squaresOccupied, board->usersTurn );

}

// hashing an internally represented QTBOARD into a POSITION
// fully general, works for any GAMEDIMENSION
POSITION hashUnsymQuarto(QTBPtr b) {

    POSITION toReturn;
    QTBPtr helperBoard = MallocBoard();
    POSITION squaresOccupiedOffset, firstSlotOffset;
    short i;

    if (!offsetTableSet) setOffsetTable();

    if (b->squaresOccupied==0 && b->piecesInPlay==0) {
	// no piece in hand, no pieces on board, trivial case 
	toReturn = 0;
    } else if (b->squaresOccupied==0 && b->piecesInPlay==1) {
	// only piece in hand, no pieces on board, also straight forward
	toReturn = b->slots[0] + offsetTable[b->squaresOccupied];
    } else {
	// pieces on board, a bit more complicated
	// make deep structural copy of b
	for (i=0; i<BOARDSIZE+1; i++) {
	    helperBoard->slots[i] = b->slots[i];
	}
	helperBoard->squaresOccupied = b->squaresOccupied;
	helperBoard->piecesInPlay = b->piecesInPlay;
	helperBoard->usersTurn = b->usersTurn;
	// finding some offsets
	squaresOccupiedOffset = offsetTable[b->squaresOccupied];
	firstSlotOffset =  permutation(NUMPIECES-1,b->squaresOccupied) 
	    *combination(BOARDSIZE,  b->squaresOccupied);
	// prepare helperBoard for recursive call
	for (i=1; i<BOARDSIZE+1; i++) {
	    if (helperBoard->slots[i] >= b->slots[0] &&
		helperBoard->slots[i] != EMPTYSLOT) {
		helperBoard->slots[i]--;
	    }
	}
	// throw the rest of the problem to a helper function
	if (b->squaresOccupied < NUMPIECES) {
	    // pieces in hand and on board
	    toReturn =   squaresOccupiedOffset
		+ b->slots[0]*firstSlotOffset
		+ hashUnsymQuartoHelper(helperBoard, 1);
	} else {
	    // no piece in hand, all pieces on board
	    toReturn =   squaresOccupiedOffset
		+ hashUnsymQuartoHelper(helperBoard, 1);
	}

    } 


    /* 
       if (b->usersTurn) {
       toReturn = toReturn + offsetTable[NUMPIECES];
       }
    */

    // giving space back to the malloc Godfather
    FreeBoard(helperBoard);
    return toReturn;

}

POSITION hashUnsymQuartoHelper(QTBPtr b, int baseSlot) {

    short slotsSubset = BOARDSIZE - baseSlot + 1; // # of slots starting from baseSlot
    short slotsOccupiedSubset = 0; // # of occupied slots starting from baseSlot
    short firstSlot = 0; // first occupied slot
    short firstPiece = EMPTYSLOT; // piece in firstSlot
    short piecesBeforeBase = 0; // # of pieces before baseSlot
    POSITION firstPieceOffset = 0;
    POSITION firstSlotOffset = 0;
    short i;
    POSITION toReturn;

    // traverse b->slots to set up local vars
    for (i=0; i<BOARDSIZE+1; i++) {
	if (i<baseSlot && b->slots[i]!=EMPTYSLOT) {
	    piecesBeforeBase++;
	}
	if (i>=baseSlot && b->slots[i]!=EMPTYSLOT) {
	    if (firstSlot == 0) firstSlot = i;
	    if (firstPiece == EMPTYSLOT) firstPiece = b->slots[i];
	}
    }
    slotsOccupiedSubset = b->piecesInPlay - piecesBeforeBase;

    if (slotsOccupiedSubset == 0) {
	// error: recursive call should never have no pieces starting from baseSlot
	toReturn = 0;
	printf("\nError: hashUnsymQuartoHelper() check recursive call\n");
    } else if (slotsOccupiedSubset == 1) {
	// base case
	toReturn = firstPiece*slotsSubset + (firstSlot - baseSlot);
    } else {
	// more complicated
	// calculating a couple of offsets
	for (i=slotsSubset-1; i>=slotsOccupiedSubset-1; i--) {
	    if (slotsSubset-i-1 == firstSlot-baseSlot) {
		firstSlotOffset = firstPieceOffset;
	    }
	    firstPieceOffset +=  
		permutation(NUMPIECES-piecesBeforeBase-1,slotsOccupiedSubset-1)
		*combination(i,slotsOccupiedSubset-1);
	}
	// preparing b for recursive call
	for (i=firstSlot+1; i<BOARDSIZE+1; i++) {
	    if (b->slots[i] > b->slots[firstSlot] &&
		b->slots[i] != EMPTYSLOT) {
		b->slots[i]--;
	    }
	}
	// throw the rest of the problem onto a recursive call
	toReturn =  firstPiece*firstPieceOffset
	    +firstSlotOffset
	    +hashUnsymQuartoHelper(b,firstSlot+1);
    }

    return toReturn;
}

// unhashing a POSITION into an internally represented QTBOARD
QTBPtr unhashUnsymQuarto(POSITION p) {

    QTBPtr toReturn = MallocBoard();
    short i;
    POSITION firstSlotOffset, squaresOccupiedOffset;
    POSITION pHelper;

    // set all empty square to EMPTYSLOT at first, to be filled up w/ pieces
    for (i=0; i<BOARDSIZE+1; i++) {
	toReturn->slots[i] = EMPTYSLOT;
    }

    if (p == 0) {
	// no piece in hand and no piece on board, trivial case
	toReturn->squaresOccupied = 0;
	toReturn->piecesInPlay = 0;
    } else if (p < offsetTable[NUMPIECES]) {
	// piece in hand and possibly on board, more complicated
	for (i=1; i<NUMPIECES+1; i++) {
	    if (p>=offsetTable[i-1] && p<offsetTable[i]) {
		// figure out these values
		toReturn->squaresOccupied = i-1;
		toReturn->piecesInPlay = i;
		// calculating a couple of offsets
		squaresOccupiedOffset = offsetTable[toReturn->squaresOccupied];
		firstSlotOffset =  permutation(NUMPIECES-1,toReturn->squaresOccupied) 
		    *combination(BOARDSIZE,  toReturn->squaresOccupied);
		// the first piece is easy enough
		toReturn->slots[0] = (p - squaresOccupiedOffset) / firstSlotOffset;
		// prepare for recursion
		pHelper = p - squaresOccupiedOffset 
	            - toReturn->slots[0]*firstSlotOffset;
		// throw rest of problem to helper function
		unhashUnsymQuartoHelper(pHelper, 1, toReturn);
	    } 
	} 
    } else {
	// no piece in hand, all pieces on board
	toReturn->squaresOccupied = NUMPIECES;
	toReturn->piecesInPlay = NUMPIECES;
	squaresOccupiedOffset = offsetTable[toReturn->squaresOccupied];
	pHelper = p - squaresOccupiedOffset;
	unhashUnsymQuartoHelper(pHelper, 1, toReturn);
    } 

    return toReturn;

}

void unhashUnsymQuartoHelper(POSITION p, int baseSlot, QTBPtr toReturn) {

    short i,j,k;
    BOOLEAN appearedBeforeBase = FALSE;
    short piecesBeforeBase = 0;
    short slotsOccupiedSubset = 0;
    short slotsSubset = BOARDSIZE - baseSlot + 1;
    short firstSlot=-1,firstPiece=0;
    short *piecesSubset = SafeMalloc(NUMPIECES*sizeof(short));
    POSITION firstSlotOffset=0, firstPieceOffset=0;

    // traversing toReturn to setup local vars
    for (i=0; i<BOARDSIZE+1; i++) {
	if ((i<baseSlot) && (toReturn->slots[i] != EMPTYSLOT)) {
	    piecesBeforeBase++;
	}
    }
    slotsOccupiedSubset = toReturn->piecesInPlay - piecesBeforeBase;

    if (slotsOccupiedSubset == 0) {
	// yay we're done!
    } else if (slotsOccupiedSubset == 1) {
	// base case
	firstPiece = p / slotsSubset;
	firstSlot = p - firstPiece*slotsSubset;
	// extracting the offsetted pieces to pieceSubset[]
	k=0;
	for (j=0; j<NUMPIECES; j++) {
	    appearedBeforeBase = FALSE;
	    for (i=0; i<baseSlot; i++) {
		if (j == toReturn->slots[i]) appearedBeforeBase = TRUE;
	    }
	    if (!appearedBeforeBase) {
		piecesSubset[k] = j;
		k++;
	    }
	}
	// assigning the piece just unhashed to its slot
	toReturn->slots[firstSlot+baseSlot] = piecesSubset[firstPiece];
    } else {
	// more complicated
	// calculating an offsets
	for (i=0; i<=slotsSubset-slotsOccupiedSubset; i++) {
	    firstPieceOffset +=  
		permutation(NUMPIECES-piecesBeforeBase-1,slotsOccupiedSubset-1)
		*combination(slotsSubset-i-1,slotsOccupiedSubset-1);
	}
	// finding and setting firstPiece and firstSlot
	firstPiece = p / firstPieceOffset;
	i=0;
	while (i<=slotsSubset-slotsOccupiedSubset && firstSlot<0) {
	    firstSlotOffset +=  
		permutation(NUMPIECES-piecesBeforeBase-1,slotsOccupiedSubset-1)
		*combination(slotsSubset-i-1,slotsOccupiedSubset-1);
	    if (p-firstPiece*firstPieceOffset < firstSlotOffset) {
		firstSlot = i;
	    }
	    i++;
	}
	// finding another offset
	firstSlotOffset -=  permutation(NUMPIECES-piecesBeforeBase-1,slotsOccupiedSubset-1)
	    *combination(slotsSubset-firstSlot-1,slotsOccupiedSubset-1);
	// extracting the offsetted pieces to pieceSubset[]
	k=0;
	for (j=0; j<NUMPIECES; j++) {
	    appearedBeforeBase = FALSE;
	    for (i=0; i<baseSlot; i++) {
		if (j == toReturn->slots[i]) appearedBeforeBase = TRUE;
	    }
	    if (!appearedBeforeBase) {
		piecesSubset[k] = j;
		k++;
	    }
	}
	// assigning the piece just unhashed to its slot
	toReturn->slots[firstSlot+baseSlot] = piecesSubset[firstPiece];
	// preparing for recursion
	p = p - firstPiece*firstPieceOffset - firstSlotOffset;
	// throw the rest of the problem onto a recursive call
	unhashUnsymQuartoHelper(p, firstSlot+baseSlot+1, toReturn);
    }

    // giving space back to the malloc Godfather
    SafeFree(piecesSubset);
}


// set factorialTable
void setFactorialTable() {

    int i;

    if (!factorialTableSet) {
	factorialTable[0] = 1;
	for (i=1; i<FACTORIALMAX; i++) {
	    factorialTable[i] = factorialTable[i-1] * i;
	}
	factorialTableSet = TRUE;
    }
}

POSITION factorialMem(int n) {
    if (!factorialTableSet) setFactorialTable();
    if (n<=0) {
	return 1;
    } else if (FACTORIALMAX<=n) {
	return factorialTable[FACTORIALMAX-1];
    } else {
	return factorialTable[n];
    }
}

/* Non-memoizing factorial */
POSITION factorialNoMem( int n ) {

    POSITION result = 1;

    while ( n > 0 ) {

	result*=n--;

    }

    return result;

}

// returns n P r, the number of ordered arrangements of 
// r items selected from a set of n items
POSITION permutation(int n, int r) {
    /* Mario: updating base cases, should return 1 when r <= 0, 0 when otherwise n <= 0
       Used to return 1 if either n or r are greater than 0, which is virtually always */
    if (r <= 0) {
	return 1;
    } else if (n <= 0) {
	return 0;
    } else {
	return factorial(n) / factorial(n-r);
    }
}

// returns n C r, the number of unordered arrangements of 
// r items selected from a set of n items
POSITION combination(int n, int r) {
    /* Mario: updating base cases, should return 1 when r <= 0, 0 when otherwise n <= 0
       Used to return 1 if either n or r are greater than 0, which is virtually always */
    if (r <= 0) {
	return 1;
    } else if (n <= 0) {
	return 0;
    } else {
	return factorial(n) / factorial(n-r) / factorial(r);
    }
}

// sets the offsetTable
void setOffsetTable() {

    int i;

    if (!offsetTableSet) {
	offsetTable[0] = 1;
	for (i=1; i<NUMPIECES+2; i++) {
	    offsetTable[i] = offsetTable[i-1] + 
		permutation(NUMPIECES,i-1)
		*combination(BOARDSIZE,i-1)
		*(NUMPIECES-i+1);
	}
	offsetTable[NUMPIECES+1] = offsetTable[NUMPIECES] + factorial(NUMPIECES);
	offsetTableSet = TRUE;
    }
}


/* Creates move given slot and piece */
MOVE CreateMove( MOVE slot, MOVE piece ) {
	
    return slot + ( piece << ( GAMEDIMENSION + 1 ) );
	
	
}

/* Returns piece given move */
MOVE GetMovePiece( MOVE move ) {
	
    return move >> ( GAMEDIMENSION + 1 );
	
}

/* Returns slot given move */
MOVE GetMoveSlot( MOVE move ) {
	
    return move & maskseq( GAMEDIMENSION + 1 );
	
}

/* Prints horizontal border for board */
void PrintHorizontalBorder( char fill, char border, char *startmark, char *endmark ) {
	
    int position;
	
    printf( "%s%c", startmark, border );
	
    for( position = 1; position < (GAMEDIMENSION + 1) * GAMEDIMENSION; position++ ) {
		
	if( position % ( GAMEDIMENSION + 1) ) {
			
	    printf( "%c", fill );
			
	} else {
			
	    printf( "%c", border );
			
	}
		
    }
	
    printf( "%c%s", border, endmark );

}

/* Extracts character information about piece trait */
char PieceTrait( short trait, void *p_piece ) {
	
    short piece = *((short *) p_piece);
	
    return ( piece == EMPTYSLOT ) ? ' ' : states[trait][( piece >> trait ) & 1];
	
}

char LegendCoordinate( short pad, void *p_coordinate ) {
	
    char coordinate = *((char *) p_coordinate);
	
    return ( pad < GAMEDIMENSION - 1 ) ? ' ' : coordinate;
	
}

char BorderCell( short ignored, void* p_ignored ) {
	
    return '-';
	
}

char BlankCell( short ignored, void* p_ignored ) {
	
    return ' ';
	
}

/* Prints cell for board */
void PrintCell( void *cell, char (*CellContent)( short, void * ) ) {
	
    short trait;
	
    for( trait = 0; trait < GAMEDIMENSION; trait++ ) {
	
	printf( "%c", CellContent( trait, cell ) );	
		
    }
	
}

void PrintRange( void *cells, size_t content_size, int offset, int size, char (*CellContent)( short, void * ), char border, char *endmark ) {
	
    int column;
	
    printf( "%c", border );
    for( column = 0; column < size; column++ ) {
		
	PrintCell( cells + (offset + column)*content_size, CellContent );
	printf( "%c", border );
		
    }
	
    printf( endmark );
		
	
}

unsigned short GetHandPiece( QTBPtr board ) {
 
    return board->slots[HAND];
     
}

void SetHandPiece( QTBPtr board, unsigned short piece ) {
 
    board->slots[HAND] = piece;
     
}

unsigned short GetBoardPiece( QTBPtr board, unsigned short slot ) {
   
    return board->slots[slot + FIRSTSLOT];
   
}

void yanpeiTestOffset() {

    int i;

    printf("\nTesting offsetTable ... \n");
    for (i=0; i<NUMPIECES+2; i++) {
	printf("offsetTable[%d] = " POSITION_FORMAT "\n",i,offsetTable[i]);
    }
    printf("\n");
 
}

void yanpeiTestHash() {

    POSITION i,h;
    BOOLEAN allPassed = TRUE;
    void (*oldPrintPos)(POSITION position, STRING playersName, BOOLEAN usersTurn ) = printPos;

    printf("\nTesting full blown hash/unhash ... \n");

    printPos = &yanpeiPrintSlots;
  
    i=0;
    while (i<offsetTable[NUMPIECES+1] && allPassed) {
	//printf("%8d\n",i);
	if (i != (h=hash(unhash(i)))) {
	    allPassed = FALSE;
	    printf("hash/unhash error:\n");
	    printf("position: " POSITION_FORMAT ", hashed: " POSITION_FORMAT,i,h);
	    newline();
	}
	PrintPosition(i,"",TRUE);
	i++;
    }
    if (allPassed) printf("\n ... testHash() passed.\n"); 
    else printf("\n ... testHash() failed.\n");

    printPos = oldPrintPos;
}

POSITION yanpeiGetCannonical(POSITION p) {

    POSITION geometricSym[8];
    POSITION toReturn = offsetTable[NUMPIECES+1];
    POSITION temp;
    short i,j;

    geometricSym[0] = p;
    geometricSym[1] = rotateBoard90(geometricSym[0]);
    geometricSym[2] = rotateBoard90(geometricSym[1]);
    geometricSym[3] = rotateBoard90(geometricSym[2]);
    geometricSym[4] = reflectBoard(geometricSym[0]);
    geometricSym[5] = reflectBoard(geometricSym[1]);
    geometricSym[6] = reflectBoard(geometricSym[2]);
    geometricSym[7] = reflectBoard(geometricSym[3]);

    for (i=0; i<NUMPIECES; i++) {
	for (j=0; j<8; j++) {
	    if ((temp=maskBoard(geometricSym[j],i)) < toReturn) toReturn = temp;
	}
    }

    return toReturn;

}

POSITION rotateBoard90(POSITION p) {

    QTBPtr b = unhash(p);
    QTBPtr c = MallocBoard();
    short i,j;
    POSITION toReturn;

    c->squaresOccupied = b->squaresOccupied;
    c->piecesInPlay = b->piecesInPlay;
    c->usersTurn = b->usersTurn;

    for (i=0; i<GAMEDIMENSION; i++) {
	for (j=0; j<GAMEDIMENSION; j++) {
	    c->slots[GAMEDIMENSION*i + j + 1] 
		= b->slots[GAMEDIMENSION*j + (GAMEDIMENSION-i-1) + 1];
	}
    }
    c->slots[0] = b->slots[0];

    toReturn = hash(c);
    FreeBoard(b);
    FreeBoard(c);

    return toReturn;
}

POSITION reflectBoard(POSITION p) {

    QTBPtr b = unhash(p);
    QTBPtr c = MallocBoard();
    short i,j;
    POSITION toReturn;

    c->squaresOccupied = b->squaresOccupied;
    c->piecesInPlay = b->piecesInPlay;
    c->usersTurn = b->usersTurn;

    for (i=0; i<GAMEDIMENSION; i++) {
	for(j=0; j<GAMEDIMENSION; j++) {
	    c->slots[GAMEDIMENSION*i + j + 1] 
		= b->slots[GAMEDIMENSION*i + (GAMEDIMENSION-j-1) + 1];
	}
    }
    c->slots[0] = b->slots[0];

    toReturn = hash(c);
    FreeBoard(b);
    FreeBoard(c);

    return toReturn;
}


POSITION maskBoard(POSITION p, short mask) {

    QTBPtr b = unhash(p);
    short i;
    short pad;
    POSITION toReturn;

    pad = maskseq(GAMEDIMENSION);
    for (i=0; i<BOARDSIZE+1; i++) {
	if (b->slots[i] != EMPTYSLOT) {
	    b->slots[i] ^= mask;
	    b->slots[i] &= pad;
	}
    }

    toReturn = hash(b);
    FreeBoard(b);

    return toReturn;
}

void yanpeiTestRotate();
void yanpeiTestReflect();
void yanpeiTestMask();

void yanpeiTestCannonicalSupport() {
    printf("\nTesting support functions for GetCannonicalPosition()\n");
    yanpeiTestRotate();
    yanpeiTestReflect();
    yanpeiTestMask();
}

void yanpeiTestRotate() {

    short i;
    QTBPtr b = MallocBoard();
    POSITION p,q;
    void (*oldPrintPos)(POSITION position, STRING playersName, BOOLEAN usersTurn ) = printPos;

    printf("\nTesting rotateBoard()\n");

    printPos = &yanpeiPrintSlots;

    b->usersTurn = FALSE;
    b->squaresOccupied = (NUMPIECES<BOARDSIZE) ? NUMPIECES : BOARDSIZE-1;
    b->piecesInPlay = (NUMPIECES<BOARDSIZE) ? NUMPIECES : BOARDSIZE-2;
    for (i=0; i<BOARDSIZE; i++) {
	b->slots[i] = (i<NUMPIECES) ? i : EMPTYSLOT;
    }
    b->slots[BOARDSIZE] = EMPTYSLOT;

    p = hash(b);
    q = p;
    PrintPosition(q,"",TRUE);
    printf("Rotating board ...\n");
    q = rotateBoard90(q);
    PrintPosition(q,"",TRUE);
    printf("Rotating board ...\n");
    q = rotateBoard90(q);
    PrintPosition(q,"",TRUE);
    printf("Rotating board ...\n");
    q = rotateBoard90(q);
    PrintPosition(q,"",TRUE);
    printf("Rotating board ...\n");
    q = rotateBoard90(q);
    PrintPosition(q,"",TRUE);

    if (p == q) printf("\n ... testRotate() passed.\n"); 
    else printf("\n ... testRotate() failed.\n");

    printPos = oldPrintPos;
    FreeBoard(b);

}


void yanpeiTestReflect() {

    short i;
    QTBPtr b = MallocBoard();
    POSITION p,q;
    void (*oldPrintPos)(POSITION position, STRING playersName, BOOLEAN usersTurn ) = printPos;

    printf("\nTesting reflectBoard()\n");

    printPos = &yanpeiPrintSlots;

    b->usersTurn = FALSE;
    b->squaresOccupied = (NUMPIECES<BOARDSIZE) ? NUMPIECES-1 : BOARDSIZE-1;
    b->piecesInPlay = (NUMPIECES<BOARDSIZE) ? NUMPIECES : BOARDSIZE-1;
    for (i=0; i<BOARDSIZE; i++) {
	b->slots[i] = (i<NUMPIECES) ? i : EMPTYSLOT;
    }
    b->slots[BOARDSIZE] = EMPTYSLOT;

    p = hash(b);
    q = p;
    PrintPosition(q,"",TRUE);
    printf("Reflecting board ...\n");
    q = reflectBoard(q);
    PrintPosition(q,"",TRUE);
    printf("Reflecting board ...\n");
    q = reflectBoard(q);
    PrintPosition(q,"",TRUE);

    if (p == q) printf("\n ... testReflect() passed.\n"); 
    else printf("\n ... testReflect() failed.\n");

    printPos = oldPrintPos;
    FreeBoard(b);

}

void yanpeiTestMask() {

    short i;
    short mask = 1;
    QTBPtr b = MallocBoard();
    POSITION p,q;
    void (*oldPrintPos)(POSITION position, STRING playersName, BOOLEAN usersTurn ) = printPos;

    printf("\nTesting maskBoard()\n");

    printPos = &yanpeiPrintSlots;

    b->usersTurn = FALSE;
    b->squaresOccupied = (NUMPIECES<BOARDSIZE) ? NUMPIECES-1 : BOARDSIZE-1;
    b->piecesInPlay = (NUMPIECES<BOARDSIZE) ? NUMPIECES : BOARDSIZE-1;
    for (i=0; i<BOARDSIZE; i++) {
	b->slots[i] = (i<NUMPIECES) ? i : EMPTYSLOT;
    }
    b->slots[BOARDSIZE] = EMPTYSLOT;

    p = hash(b);
    q = p;
    PrintPosition(q,"",TRUE);
    printf("Masking board with %d ...\n",mask);
    q = maskBoard(q,mask);
    PrintPosition(q,"",TRUE);
    printf("Masking board with %d ...\n",mask);
    q = maskBoard(q,mask);
    PrintPosition(q,"",TRUE);

    if (p == q) printf("\n ... testMask() passed.\n"); 
    else printf("\n ... testMask() failed.\n");

    printPos = oldPrintPos;
    FreeBoard(b);

}

void yanpeiTestCannonical() {

    POSITION i,c;
    POSITION cannonicalCount = 0;
    POSITIONLIST *cannonicals = NULL;
    POSITIONLIST *newNode;

    void (*oldPrintPos)(POSITION position, STRING playersName, BOOLEAN usersTurn ) = printPos;

    printf("\nTesting getCannonicalPosition() ... \n");

    printPos = &yanpeiPrintSlots;
  
    for (i=0; i<offsetTable[NUMPIECES+1]; i++) {
	printf("Position = %3llu\t\t",i);
	PrintPosition(i,"",TRUE);
	c = getCannonical(i);
	printf("Cannonical = %3llu\t",c);
	PrintPosition(c,"",TRUE);
	/*
	  if (!SearchPosList(cannonicals,c)) {
	  cannonicalCount++;
	  newNode = SafeMalloc(sizeof(POSITIONLIST));
	  newNode->position = c;
	  if (cannonicals!=NULL) {
	  newNode->next = cannonicals->next;
	  cannonicals->next = newNode;
	  } else {
	  newNode->next = NULL;
	  cannonicals = newNode;
	  }
	  }
	*/
    }

    printf("\n ... " POSITION_FORMAT " cannonical positions in total.\n",cannonicalCount);

    FreePosList(cannonicals);
    printPos = oldPrintPos;
}

BOOLEAN SearchPosList(POSITIONLIST *l, POSITION p) {
    if (l==NULL) {
	return FALSE;
    } else {
	return ((l->position==p) || SearchPosList(l->next,p));
    }
}

void FreePosList(POSITIONLIST *l) {
    if (l!=NULL) {
	if (l->next!=NULL) {
	    FreePosList(l->next);
	}
	SafeFree(l);
    }
}

char readchar( ) {
	
    while( getchar( ) != '\n' );
    return getchar( );
	
}
