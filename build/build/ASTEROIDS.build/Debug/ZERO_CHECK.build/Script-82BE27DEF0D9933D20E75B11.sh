#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build
  make -f /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build
  make -f /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build
  make -f /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build
  make -f /Users/trevordoucet/Desktop/Arcade_Game/PacMan/build/CMakeScripts/ReRunCMake.make
fi

