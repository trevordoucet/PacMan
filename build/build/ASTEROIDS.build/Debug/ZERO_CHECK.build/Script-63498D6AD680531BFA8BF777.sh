#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd "/Users/trevordoucet/Desktop/Computer Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build"
  make -f /Users/trevordoucet/Desktop/Computer\ Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd "/Users/trevordoucet/Desktop/Computer Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build"
  make -f /Users/trevordoucet/Desktop/Computer\ Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd "/Users/trevordoucet/Desktop/Computer Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build"
  make -f /Users/trevordoucet/Desktop/Computer\ Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd "/Users/trevordoucet/Desktop/Computer Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build"
  make -f /Users/trevordoucet/Desktop/Computer\ Graphics/Repositories/module-3-intersections-group-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi

