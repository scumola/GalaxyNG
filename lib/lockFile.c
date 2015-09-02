#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>

static int readData(int fd, void* buf, size_t nbrBytes) {
	size_t toRead;
	size_t nbrRead = 0;
	ssize_t result;

	do {
		if (nbrBytes - nbrRead > SSIZE_MAX)
			toRead = SSIZE_MAX;
		else
			toRead = nbrBytes - nbrRead;

		if ((result = read(fd, (char*)buf+nbrRead, toRead)) >= 0)
			nbrRead += result;
		else if (errno != EINTR)
			return 0;
	} while (nbrRead < nbrBytes);

	return 1;
}

static int writeData(int fd, const void* buf, size_t nbrBytes) {
	size_t toWrite;
	size_t written = 0;
	ssize_t result;

	do {
		if (nbrBytes - written > SSIZE_MAX)
			toWrite = SSIZE_MAX;
		else
			toWrite = nbrBytes - written;

		if ((result = write(fd, (const char*)buf + written, toWrite)) >= 0)
			written += result;
		else if (errno != EINTR)
			return 0;

	} while (written < nbrBytes);

	return 1;
}

int spcLockFile(const char* lfpath) {
	int attempt;
	int fd;
	int result;
	pid_t pid;

	/* try 3 times, if we fail, we lose */
	for (attempt = 0; attempt < 3; attempt++) {
		if ((fd = open(lfpath, O_RDWR|O_CREAT|O_EXCL, S_IRWXU)) == -1) {
			if (errno != EEXIST)
				return -1;

			if ((fd = open(lfpath, O_RDONLY)) == -1)
				return -1;

			result = readData(fd, &pid, sizeof(pid));
			close(fd);
			if (result) {
				if (pid == getpid())
					return 1;
				if (kill(pid, 0) == -1) {
					if (errno != ESRCH)
						return -1;
					attempt--;
					unlink(lfpath);
					continue;
				}
			}
			sleep(1);
			continue;
		}

		pid = getpid();
		if (!writeData(fd, &pid, sizeof(pid))) {
			close(fd);
			return -1;
		}

		close(fd);
		attempt--;
	}

	/* If we've made it to here, three attempts have been made and the
	 * lock could not be obtained. Return an error code indicating
	 * failure to obtain the requested lock.
	 */

	return 0;
}
