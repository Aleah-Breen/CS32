// Project 1 big bag o' test cases

// If you uncomment the #define line immediately below this comment and
// replace the integer with an integer in the range 1 to 20, when you build
// the program, the compiler will select the code below corresponding to
// that integer.  (For any other integer, it will use the original code that
// plays the game.)  Here is the line to uncomment:



//#define TESTNUM 20

// (If you know how to specify a value for TESTNUM at the time you invoke the
// compiler, using either a project setting in Visual C++ or Xcode or by
// defining the value on the command line, you could leave the #define line
// above commented out and instead set TESTNUM that other way.)

#ifndef TESTNUM  // In case you haven't already defined the name TESTNUM
#define TESTNUM 0
#endif

#if TESTNUM == 1

#include "Tooter.h"
int main() {}

#elif TESTNUM == 2

#include "Player.h"
int main() {}

#elif TESTNUM == 3

#include "City.h"
int main() {}

#elif TESTNUM == 4

#include "Game.h"
int main() {}

#elif TESTNUM == 5

#include "globals.h"
int main() {}

#elif TESTNUM == 6

#include "Game.h"
#include "Game.h"
#include "City.h"
#include "City.h"
#include "History.h"
#include "History.h"
#include "Player.h"
#include "Player.h"
#include "Tooter.h"
#include "Tooter.h"
#include "globals.h"
#include "globals.h"
int main() {}

#elif TESTNUM == 7

#include "History.h"
int main()
{
    History h(2, 2);
    h.record(1, 1);
    if (false)  // so that this test builds, but does not write anything
        h.display();
}

#elif TESTNUM == 8

#include "Tooter.h"
int main()
{
    if (false)  // so that this test builds, but does not write anything
        Tooter t(nullptr, 1, 1);
}

#elif TESTNUM == 9

#include "Player.h"
int main()
{
    if (false)  // so that this test builds, but does not write anything
        Player p(nullptr, 1, 1);
}

#elif TESTNUM == 10

#include "City.h"
int main()
{
    City ct(10, 20);
    ct.addPlayer(2, 2);
}

#elif TESTNUM == 11

#include "globals.h"
#include "Player.h"
#include "City.h"
int main()
{
    City ct(10, 20);
    Player p(&ct, 2, 3);
}

#elif TESTNUM == 12

#include "City.h"
#include "Player.h"
int main()
{
    City ct(10, 20);
    Player p(&ct, 2, 3);
}

#elif TESTNUM == 13

#include "Player.h"
#include "City.h"
int main()
{
    City ct(10, 20);
    Player p(&ct, 2, 3);
}

#elif TESTNUM == 14

#include "History.h"
#include "City.h"
#include <iostream>
using namespace std;

int main()
{
    City ct(1, 3);
    ct.history().record(1, 2);
    City ct2(1, 2);
    ct2.history().record(1, 1);
    ct.history().display();
    cout << "===" << endl;
}

 /************
This must produce exactly three lines of output:
.A.

===
 ************/

#elif TESTNUM == 15

#include "City.h"
#include "History.h"
#include <iostream>
using namespace std;

int main()
{
    for (int nTries = 0; nTries < 200; nTries++)
    {
        City ct(2, 3);
        ct.addPlayer(1, 2);
        ct.addTooter(2, 1);
        ct.addTooter(2, 3);

        ct.preachToTootersAroundPlayer();
          // If (2,1) was converted or (2,3) was not, try again.
        if (ct.nTootersAt(2, 1) == 0  ||  ct.nTootersAt(2, 3) != 0)
            continue;

        ct.preachToTootersAroundPlayer();
        if (ct.nTootersAt(2, 1) != 0)
        {
              // At this point, we know (2,3) was converted immediately
              // and (2,1) was unconverted twice.  The probability of
              // reaching this point on a given try is 7.4%.
            ct.history().display();
            break;
            }

          // At this point, we did not reach the call to display, so try
          // the test again.  There's a 99.99998% probability that in 200
          // tries, we'll have a test where the call to display will be
          // reached.
    }
    cout << "===" << endl;
}

 /************
This must produce exactly four lines of output:
...
B..

====
 ************/

#elif TESTNUM == 16  // This test must NOT compile

#include "Tooter.h"
#include "City.h"
int main()
{
    City ct(10, 20);
    Player p(&ct, 2, 3);
    Tooter t(&ct, 1, 1);
}

#elif TESTNUM == 17  // This test must NOT compile

#include "Player.h"
#include "City.h"
int main()
{
    City ct(10, 20);
    Player p(&ct, 2, 3);
    Tooter t(&ct, 1, 1);
}

#elif TESTNUM == 18  // This test must NOT compile

#include "globals.h"
#include "Tooter.h"
#include "Player.h"
int main()
{
    City ct(10, 10);
}

#elif TESTNUM == 19  // This test must NOT compile

#include "Game.h"
int main()
{
    City ct(10, 10);
}

#elif TESTNUM == 20  // This test must NOT compile

#include "History.h"
int main()
{
    History h;
}

#else  // use original main.cpp

#include "Game.h"

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}

#endif
