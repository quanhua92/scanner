#include "scanner/util/util.h"

#include <cstdarg>
#include <sstream>
#include <limits.h>     /* PATH_MAX */
#include <sys/stat.h>   /* mkdir(2) */
#include <unistd.h>     /* access(2) */
#include <string.h>
#include <errno.h>
#include <libgen.h>

namespace scanner {

// Stolen from https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950
int mkdir_p(const char *path, mode_t mode) {
    /* Adapted from http://stackoverflow.com/a/2336245/119527 */
    const size_t len = strlen(path);
    char _path[PATH_MAX];
    char *p; 

    errno = 0;

    /* Copy string so its mutable */
    if (len > sizeof(_path)-1) {
        errno = ENAMETOOLONG;
        return -1;
    }
    strcpy(_path, path);

    /* Iterate the string */
    for (p = _path + 1; *p; p++) {
        if (*p == '/') {
            /* Temporarily truncate */
            *p = '\0';
            /* check if file exists before mkdir to avoid EACCES */
            if (access(_path, F_OK) != 0) {
              /* fail if error is anything but file does not exist */
              if (errno != ENOENT) {
                return -1;
              }
              if (mkdir(_path, mode) != 0) {
                if (errno != EEXIST)
                  return -1;
              }
            }

            *p = '/';
        }
    }

    if (mkdir(_path, mode) != 0) {
        if (errno != EEXIST)
            return -1;
    }

    return 0;
}

void temp_file(FILE** fp, std::string& name) {
  char n[] = "/tmp/scannerXXXXXX";
  int fd = mkstemp(n);
  *fp = fdopen(fd, "wb+");
  name = std::string(n);
}

}
