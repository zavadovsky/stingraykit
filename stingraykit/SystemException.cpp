// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/SystemException.h>

#include <stingraykit/string/ToString.h>

#include <errno.h>
#include <string.h>

namespace stingray
{

	SystemException::SystemException(const std::string& message) noexcept
		: Exception(StringBuilder() % message % ": errno = " % GetErrorMessage(errno)), _error(errno)
	{ }


	SystemException::SystemException(const std::string& message, int err) noexcept
		: Exception(StringBuilder() % message % ": errno = " % GetErrorMessage(err)), _error(err)
	{ }


	SystemException::SystemException(const std::string& message, const std::string& path, int err) noexcept
		: Exception(StringBuilder() % message % " '" % path % "'" % ": errno = " % GetErrorMessage(err)), _error(err)
	{ }


	std::string SystemException::GetErrorMessage(int err) noexcept
	{
		char buf[256];
		char *msg = strerror_r(err, buf, sizeof(buf));

		return StringBuilder() % ErrnoToStr(err) % " (" % (msg ? msg : "Unknown error") % ")";
	}


	std::string SystemException::GetErrorMessage() noexcept
	{ return GetErrorMessage(errno); }


#define ERRNO_STR(val) case val: return #val
	std::string SystemException::ErrnoToStr(int e)
	{
		switch (e)
		{
		/* copied from errno-base.h */
		ERRNO_STR(EPERM);
		ERRNO_STR(ENOENT);
		ERRNO_STR(ESRCH);
		ERRNO_STR(EINTR);
		ERRNO_STR(EIO);
		ERRNO_STR(ENXIO);
		ERRNO_STR(E2BIG);
		ERRNO_STR(ENOEXEC);
		ERRNO_STR(EBADF);
		ERRNO_STR(ECHILD);
		ERRNO_STR(EAGAIN);
		ERRNO_STR(ENOMEM);
		ERRNO_STR(EACCES);
		ERRNO_STR(EFAULT);
		ERRNO_STR(ENOTBLK);
		ERRNO_STR(EBUSY);
		ERRNO_STR(EEXIST);
		ERRNO_STR(EXDEV);
		ERRNO_STR(ENODEV);
		ERRNO_STR(ENOTDIR);
		ERRNO_STR(EISDIR);
		ERRNO_STR(EINVAL);
		ERRNO_STR(ENFILE);
		ERRNO_STR(EMFILE);
		ERRNO_STR(ENOTTY);
		ERRNO_STR(ETXTBSY);
		ERRNO_STR(EFBIG);
		ERRNO_STR(ENOSPC);
		ERRNO_STR(ESPIPE);
		ERRNO_STR(EROFS);
		ERRNO_STR(EMLINK);
		ERRNO_STR(EPIPE);
		ERRNO_STR(EDOM);
		ERRNO_STR(ERANGE);

		/* copied from errno.h */
		ERRNO_STR(EDEADLK);
		ERRNO_STR(ENAMETOOLONG);
		ERRNO_STR(ENOLCK);
		ERRNO_STR(ENOSYS);
		ERRNO_STR(ENOTEMPTY);
		ERRNO_STR(ELOOP);
		ERRNO_STR(ENOMSG);
		ERRNO_STR(EIDRM);
		ERRNO_STR(ECHRNG);
		ERRNO_STR(EL2NSYNC);
		ERRNO_STR(EL3HLT);
		ERRNO_STR(EL3RST);
		ERRNO_STR(ELNRNG);
		ERRNO_STR(EUNATCH);
		ERRNO_STR(ENOCSI);
		ERRNO_STR(EL2HLT);
		ERRNO_STR(EBADE);
		ERRNO_STR(EBADR);
		ERRNO_STR(EXFULL);
		ERRNO_STR(ENOANO);
		ERRNO_STR(EBADRQC);
		ERRNO_STR(EBADSLT);

		ERRNO_STR(EBFONT);
		ERRNO_STR(ENOSTR);
		ERRNO_STR(ENODATA);
		ERRNO_STR(ETIME);
		ERRNO_STR(ENOSR);
		ERRNO_STR(ENONET);
		ERRNO_STR(ENOPKG);
		ERRNO_STR(EREMOTE);
		ERRNO_STR(ENOLINK);
		ERRNO_STR(EADV);
		ERRNO_STR(ESRMNT);
		ERRNO_STR(ECOMM);
		ERRNO_STR(EPROTO);
		ERRNO_STR(EMULTIHOP);
		ERRNO_STR(EDOTDOT);
		ERRNO_STR(EBADMSG);
		ERRNO_STR(EOVERFLOW);
		ERRNO_STR(ENOTUNIQ);
		ERRNO_STR(EBADFD);
		ERRNO_STR(EREMCHG);
		ERRNO_STR(ELIBACC);
		ERRNO_STR(ELIBBAD);
		ERRNO_STR(ELIBSCN);
		ERRNO_STR(ELIBMAX);
		ERRNO_STR(ELIBEXEC);
		ERRNO_STR(EILSEQ);
		ERRNO_STR(ERESTART);
		ERRNO_STR(ESTRPIPE);
		ERRNO_STR(EUSERS);
		ERRNO_STR(ENOTSOCK);
		ERRNO_STR(EDESTADDRREQ);
		ERRNO_STR(EMSGSIZE);
		ERRNO_STR(EPROTOTYPE);
		ERRNO_STR(ENOPROTOOPT);
		ERRNO_STR(EPROTONOSUPPORT);
		ERRNO_STR(ESOCKTNOSUPPORT);
		ERRNO_STR(EOPNOTSUPP);
		ERRNO_STR(EPFNOSUPPORT);
		ERRNO_STR(EAFNOSUPPORT);
		ERRNO_STR(EADDRINUSE);
		ERRNO_STR(EADDRNOTAVAIL);
		ERRNO_STR(ENETDOWN);
		ERRNO_STR(ENETUNREACH);
		ERRNO_STR(ENETRESET);
		ERRNO_STR(ECONNABORTED);
		ERRNO_STR(ECONNRESET);
		ERRNO_STR(ENOBUFS);
		ERRNO_STR(EISCONN);
		ERRNO_STR(ENOTCONN);
		ERRNO_STR(ESHUTDOWN);
		ERRNO_STR(ETOOMANYREFS);
		ERRNO_STR(ETIMEDOUT);
		ERRNO_STR(ECONNREFUSED);
		ERRNO_STR(EHOSTDOWN);
		ERRNO_STR(EHOSTUNREACH);
		ERRNO_STR(EALREADY);
		ERRNO_STR(EINPROGRESS);
		ERRNO_STR(ESTALE);
		ERRNO_STR(EUCLEAN);
		ERRNO_STR(ENOTNAM);
		ERRNO_STR(ENAVAIL);
		ERRNO_STR(EISNAM);
		ERRNO_STR(EREMOTEIO);
		ERRNO_STR(EDQUOT);

		ERRNO_STR(ENOMEDIUM);
		ERRNO_STR(EMEDIUMTYPE);
		ERRNO_STR(ECANCELED);

#ifdef ENOKEY
		ERRNO_STR(ENOKEY);
#endif
#ifdef EKEYEXPIRED
		ERRNO_STR(EKEYEXPIRED);
#endif
#ifdef EKEYREVOKED
		ERRNO_STR(EKEYREVOKED);
#endif
#ifdef EKEYREJECTED
		ERRNO_STR(EKEYREJECTED);
#endif

		ERRNO_STR(EOWNERDEAD);
		ERRNO_STR(ENOTRECOVERABLE);

		ERRNO_STR(ERFKILL);

		default:
			return ToString(e);
		};
	}
#undef ERRNO_STR

}
