#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command.h"
#include "stack.h"

#include "regset.h"
#include "logger.h"
#include "usbservice.h"
#include "macro.h"

// Define just one of the _DEFAULTMACRO_ definitions if you don't want to
// support the dip-switch modification and only want to use ONE macro.

//#define _DEFAULTMACRO_WISH // used for wishing on starry nights when Celeste is around.
//#define _DEFAULTMACRO_BUILD // Build 40 items
//#define _DEFAULTMACRO_FRUITBUY400  // Stock up on Fruit to Sell for a Big Profit on a friend's island.
//#define _DEFAULTMACRO_FRUITBUY300
//#define _DEFAULTMACRO_FRUITBUY200
//#define _DEFAULTMACRO_FRUITBUY100
//#define _DEFAULTMACRO_FRUITBUY10
//#define _DEFAULTMACRO_DEMO //Useless demo - move around pocket and swipe net/axe a few times.

#define RST_INIT_JOYSTICK 0
#define RST_INIT_FRUITBUY 1
#define RST_BUY_FRUITBUNDLE 2
#define RST_QUIT_MENU 3
#define RST_MOVEPOCKET 4


#define REPEAT_FOREVER -1
#define NUMFRUITBUYS4ROWS 80     // 40 * 2 = 80
#define NUMFRUITBUYS3ROWS 60   // 30 * 2 = 60



static command macro_initjoystick[] = {
    { NOTHING,  250 },
    { TRIGGERS,   5 },
    { NOTHING,  150 },
    { TRIGGERS,   5 },
    { NOTHING,  150 },
    { A,          5 },
    { NOTHING,  250 },
    { CONFIRM, 5},
    { NOTHING,  25 },
    _RETURN,

} ;
static command macro_quitmenu[] = {
  _REPEAT(4),
    { B,  5 }, // Back Out
    {NOTHING,100},
  _ENDRPT,
  _RETURN
} ;

static command macro_buyfruitbundle[] = {
  { A,  5 }, // Select
  {NOTHING,100},
  {A, 5},
  {NOTHING,100},
  {DOWN, 5},      // MOVE DOWN to 'TAKE 5' Fruit
  {NOTHING,20},
  {A, 5},					// CONFIRM PURCHASE
  {NOTHING,70},
  {A, 5},					// 'EXCELLENT PURCHASE!''
  {NOTHING,60},
  {A, 5},					// ANYTHING ELSE?
  {NOTHING,70},
  _RETURN
} ;


static command macro_initfruitbuy[] = {

  _RST(RST_INIT_JOYSTICK),    // Call init joystick macro

	{ A,  5 }, // Start Open Cabinet
	{NOTHING,200},
	{ A,  5 },
	{NOTHING,50},

  // Move to fruit in cabinet - 9 Down and 3 Across

  // Move 9 Down
  _REPEAT(9),
    { DOWN,  5 },
    {NOTHING,20},
  _ENDRPT,

// Move Right - 3 across
  _REPEAT(3),
    { RIGHT,  5 },
    {NOTHING,20},
  _ENDRPT,
  _RETURN

} ;


// Test Demo - move right,down,left, up (demo to move cursor on pocket-select-item menu)
static command macro_movepocket[] = {
  {RIGHT, 5},
  {NOTHING,20},
  {DOWN, 5},
  {NOTHING,20},
  {LEFT, 5},
  {NOTHING,20},
  {UP, 5},
  {NOTHING,20},
  _RETURN
} ;

static  command macro_buy10fruit[] = {
  _ID(109),
  _RST(RST_INIT_FRUITBUY),
  // Buy 2 lots of 5 fruits (10 Fruit)
  _REPEAT(2),
    _RST(RST_BUY_FRUITBUNDLE),
  _ENDRPT,

  _RST(RST_QUIT_MENU),

  _ENDMACRO


} ;


static  command macro_buyfruit1Rows[] = {
  _ID(108),
  _RST(RST_INIT_FRUITBUY),

  // Buy 20 lots of 5 fruits
  _REPEAT(20),
    _RST(RST_BUY_FRUITBUNDLE),
  _ENDRPT,

  _REPEAT(4),
    { B,  5 }, // Back Out
    {NOTHING,100},
  _ENDRPT,

  _ENDMACRO


} ;

static  command macro_buyfruit2Rows[] = {
  _ID(107),
  _RST(RST_INIT_FRUITBUY),

  // Buy 40 lots of 5 fruits
  _REPEAT(40),
    _RST(RST_BUY_FRUITBUNDLE),
  _ENDRPT,

  _REPEAT(4),
    { B,  5 }, // Back Out
    {NOTHING,100},
  _ENDRPT,

  _ENDMACRO


} ;


static  command macro_buyfruit3Rows[] = {
  _ID(106),
  _RST(RST_INIT_FRUITBUY),

  // Buy 60 lots of 5 fruits
  _REPEAT(NUMFRUITBUYS3ROWS),
    _RST(RST_BUY_FRUITBUNDLE),
  _ENDRPT,

  _RST(RST_QUIT_MENU),

  _ENDMACRO


} ;

static  command macro_buyfruit4Rows[] = {
  _ID(105),
  _RST(RST_INIT_FRUITBUY),  // call: open cabinet and select fruit macro

  // Buy 80 lots of 5 fruits
  _REPEAT(NUMFRUITBUYS4ROWS),
    _RST(RST_BUY_FRUITBUNDLE),
  _ENDRPT,

  _RST(RST_QUIT_MENU),    // call: quit cabinet macro

  _ENDMACRO


} ;


static  command macro_builditems[] = {
        _ID(102),
        _RST(RST_INIT_JOYSTICK),
        _REPEAT(40),
          { A,  5 }, // Select build item
          {NOTHING,20},
          { B,  5 }, // Speed up build animation
          {NOTHING,20},
          {NOTHING,200},
        _ENDRPT,
        _ENDMACRO

} ;

static  command macro_starwish[] = {
        _ID(103),
        _RST(RST_INIT_JOYSTICK),
        _REPEAT(REPEAT_FOREVER),
          { A,  5 }, // Select
          {NOTHING,20},
        _ENDRPT,
        _ENDMACRO

} ;


static  command macro_demo[] = {
        _ID(104),
        _RST(RST_INIT_JOYSTICK),
        _REPEAT(3),

          { X,  5 }, // Select Pocket Menu
          {NOTHING,50},

          // Move cursor around in a square, 4 times
          _REPEAT(4),
            _RST(RST_MOVEPOCKET),
          _ENDRPT,

          { B,  5 }, // Back Out of Pocket Menu
          {NOTHING,20},

          // If your holding an item (such as a net or an axe)
          // swing it 6 times
          _REPEAT(6),
            { A,  5 }, // Select
            {NOTHING,50},
          _ENDRPT,

        _ENDRPT,

        _ENDMACRO

} ;



static command *rstcommands[] = {
  &macro_initjoystick[0],
  &macro_initfruitbuy[0],
  &macro_buyfruitbundle[0],
  &macro_quitmenu[0],
  &macro_movepocket[0]

} ;


// 8 Macros
static command *macros[] = {

    &macro_starwish[0],
    &macro_builditems[0],
    &macro_buyfruit1Rows[0],
    &macro_buyfruit2Rows[0],
    &macro_buyfruit3Rows[0],
    &macro_buyfruit4Rows[0],
    &macro_buy10fruit[0],

    // The Macro below (depending on what _DEFAULTMACRO_XXXX is 'defined') will
    // be the macro which is run if dip-switch hardware is
    // NOT supported

    #ifdef _DEFAULTMACRO_DEMO
      &macro_demo[0],
    #elif defined  _DEFAULTMACRO_WISH
      &macro_starwish[0],
    #elif defined  _DEFAULTMACRO_BUILD
      &macro_builditems[0],
    #elif defined  _DEFAULTMACRO_FRUITBUY400
      &macro_buyfruit4Rows[0],     // This macro will be selected if you don't bother with the dip-switch mods.
    #elif defined  _DEFAULTMACRO_FRUITBUY300
      &macro_buyfruit3Rows[0],
    #elif defined  _DEFAULTMACRO_FRUITBUY200
      &macro_buyfruit2Rows[0],
    #elif defined  _DEFAULTMACRO_FRUITBUY100
      &macro_buyfruit1Rows[0],
    #elif defined  _DEFAULTMACRO_FRUITBUY10
      &macro_buy10fruit[0],
    #else
      #warning "NO DEFAULT MACRO (_DEFAULTMACRO_xxxx) DEFINED!"
    #endif

    &macro_demo[0]              // Note: This MAY NOT be the last macro! Only placed here to remove possible gcc warnings.

} ;


command *getCall(int rstnumber) {
  return rstcommands[rstnumber] ;
}

regset *initButtonInstructions(command *startcmd) {
    regset *rset = createRegSet(startcmd) ;
    return rset ;
}


int completed = 0 ;

int isCompleted() {
    return completed ;
}

void runMacro() {

    if (!completed) {

      if (usbReady()) {
        command *cmd = getCurrentCommand(peek()) ;
        execute(peek()) ;
        if (cmd->mcmd == ENDMACRO) {
          completed = 1 ;
        }
        nextCommand(peek()) ;

      }
      usbHandler() ;

    }
}


void runMacroUntilComplete() {

  while (!completed) {
    runMacro() ;
  }

}


void initMacro(int macronumber) {
  command *cmd = macros[macronumber] ;
  regset *rs = initButtonInstructions(cmd) ;
  push(rs) ;
  completed = 0 ;
}


#ifdef TESTMACRO

#define MACRO_STARWISH     0
#define MACRO_BUILDITEMS   1
#define MACRO_FRUIT1ROWS   2
#define MACRO_FRUIT2ROWS   3
#define MACRO_FRUIT3ROWS   4
#define MACRO_FRUIT4ROWS   5
#define MACRO_FRUIT10      6
#define MACRO_COMPILED     7  // Ultimately defined by _DEFAULTMACRO_xxx


int main(int argc, char **argv) {

  loggerInit() ;
  initMacro(MACRO_COMPILED) ;
  runMacroUntilComplete() ;

  return 0 ;
}

#endif
