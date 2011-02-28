#ifndef __FWEELIN_LOOPLIBRARY_H
#define __FWEELIN_LOOPLIBRARY_H

/* Copyright 2004-2011 Jan Pekau
   
   This file is part of Freewheeling.
   
   Freewheeling is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.
   
   Freewheeling is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Freewheeling.  If not, see <http://www.gnu.org/licenses/>. */

#include <glob.h>

#include <string>
#include <sstream>

#include "fweelin_core.h"

class LibraryFileInfo {
public:
  LibraryFileInfo () : exists(0), c(UNKNOWN), name("") {};

  char exists;  // File exists?
  codec c;      // Codec (extension of file) for audio files
  std::string name;  // Name of file
};

class LibraryHelper {
public:
  // Returns the stub (base of filename) for a given loop in memory
  static const std::string GetStubnameFromLoop (Fweelin *app, Loop *l) {
    std::ostringstream tmp;
    GET_SAVEABLE_HASH_TEXT(l->GetSaveHash());
    tmp << app->getCFG()->GetLibraryPath() << "/" << FWEELIN_OUTPUT_LOOP_NAME << "-" <<
        hashtext;

    return tmp.str();
  };

  static const std::string GetNextAvailableStreamOutFilename (Fweelin *app, int &stream_num, std::string &timingname) {
    // Create appropriate filename for output
    char go = 1;

    printf("Start SS\n");
    fflush(stdout);

    do {
      std::ostringstream tmp;
      tmp << app->getCFG()->GetLibraryPath() << "/" << FWEELIN_OUTPUT_STREAM_NAME << stream_num <<
          app->getCFG()->GetAudioFileExt(app->getCFG()->GetStreamOutFormat());
      const std::string s = tmp.str();

      struct stat st;
      printf("DISK: Opening '%s' for streaming.\n",s.c_str());
      fflush(stdout);
      if (stat(s.c_str(),&st) == 0) {
        printf("DISK: File exists, trying another.\n");
        stream_num++;
      } else {
        // Also check for timing data file- don't overwrite that
        std::ostringstream ttmp;
        ttmp << app->getCFG()->GetLibraryPath() << "/" << FWEELIN_OUTPUT_STREAM_NAME << stream_num <<
            FWEELIN_OUTPUT_TIMING_EXT;
        const std::string ts = ttmp.str();
        if (stat(ts.c_str(),&st) == 0) {
          printf("DISK: Timing file exists, trying another.\n");
          stream_num++;
        } else {
          go = 0;
          timingname = ts;
          return s;
        }
      }
    } while (go);

    return "";
  };

  // Finds the loop with given stubname in the loop library. Returns information about it.
  static LibraryFileInfo GetLoopFilenameFromStub (Fweelin *app, const char *stubname) {
    struct stat st;

    LibraryFileInfo ret;
    
    // Try exact filename with all format types
    for (codec i = FIRST_FORMAT; i < END_OF_FORMATS; i = (codec) (i+1)) {
      std::ostringstream tmp;
      tmp << stubname << app->getCFG()->GetAudioFileExt(i);
      const std::string s = tmp.str();
      if (stat(s.c_str(),&st) == 0) {
        // Found it
        ret.exists = 1;
        ret.c = i;
        ret.name = s;
        return ret;
      }
    }

    // No go, try wildcard search with all format types
    for (codec i = FIRST_FORMAT; i < END_OF_FORMATS; i = (codec) (i+1)) {
      std::ostringstream tmp;
      tmp << stubname << "*" << app->getCFG()->GetAudioFileExt(i);
      const std::string s = tmp.str();

      glob_t globbuf;
      if (glob(s.c_str(), 0, NULL, &globbuf) == 0) {
        for (size_t j = 0; j < globbuf.gl_pathc; j++) {
          if (stat(globbuf.gl_pathv[j],&st) == 0) {
            // Found it
            ret.exists = 1;
            ret.c = i;
            ret.name = globbuf.gl_pathv[j];
            globfree(&globbuf);
            return ret;
          }
        }
        globfree(&globbuf);
      }
    }

    return ret;
  };

  // Finds the data (XML) file with given stubname in the loop library. Returns information about it.
  static LibraryFileInfo GetDataFilenameFromStub (Fweelin *app, const char *stubname) {
    struct stat st;

    LibraryFileInfo ret;

    // Try exact filename
    {
      std::ostringstream tmp;
      tmp << stubname << FWEELIN_OUTPUT_DATA_EXT;
      const std::string s = tmp.str();

      if (stat(s.c_str(),&st) == 0) {
        // Found it
        ret.exists = 1;
        ret.name = s;
        return ret;
      }
    }

    // No go, try wildcard search
    {
      std::ostringstream tmp;
      tmp << stubname << "*" << FWEELIN_OUTPUT_DATA_EXT;
      const std::string s = tmp.str();

      glob_t globbuf;
      if (glob(s.c_str(), 0, NULL, &globbuf) == 0) {
        for (size_t j = 0; j < globbuf.gl_pathc; j++) {
          if (stat(globbuf.gl_pathv[j],&st) == 0) {
            // Found it
            ret.exists = 1;
            ret.name = globbuf.gl_pathv[j];
            globfree(&globbuf);
            return ret;
          }
        }
        globfree(&globbuf);
      }
    }
    
    return ret;
  };
};

#endif