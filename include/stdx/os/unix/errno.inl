#pragma once

/**
 * @internal
 * @namespace stdx::os::unix::captured
 * @brief The numeric values of the <errno.h> macros, read where they exist.
 *
 * The same arrangement as stdx::os::unix::sys::captured, for the same reason: a
 * macro does not cross `import :os`, so the value is read here and referenced
 * from unix.constants.inl, which is on the far side of the import and where the
 * POSIX name can therefore be declared.
 *
 * Deriving these matters more than it does for most blocks. Almost every errno
 * number differs between Linux and Darwin - EAGAIN is 11 against 35,
 * ECONNREFUSED 111 against 61, EINPROGRESS 115 against 36 - and a wrong one does
 * not fail, it misclassifies. stdx::net maps errno onto exception types, so a
 * stale number turns "connection refused" into a generic SocketException, or a
 * would-block into a fatal error. Nothing crashes; the library just starts lying
 * about why things failed.
 *
 * ENOTSUP and EOPNOTSUPP are the sharpest case: one value on Linux, two
 * different ones on Darwin (45 and 102). Code comparing against the wrong one is
 * simply wrong there, and no amount of care in copying would have caught it.
 */
namespace stdx::os::unix::captured {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<errno.h>)
    constexpr auto EPERM_VALUE = EPERM;
    constexpr auto ENOENT_VALUE = ENOENT;
    constexpr auto ESRCH_VALUE = ESRCH;
    constexpr auto EINTR_VALUE = EINTR;
    constexpr auto EIO_VALUE = EIO;
    constexpr auto ENXIO_VALUE = ENXIO;
    constexpr auto E2BIG_VALUE = E2BIG;
    constexpr auto ENOEXEC_VALUE = ENOEXEC;
    constexpr auto EBADF_VALUE = EBADF;
    constexpr auto ECHILD_VALUE = ECHILD;
    constexpr auto EAGAIN_VALUE = EAGAIN;
    constexpr auto EWOULDBLOCK_VALUE = EWOULDBLOCK;
    constexpr auto ENOMEM_VALUE = ENOMEM;
    constexpr auto EACCES_VALUE = EACCES;
    constexpr auto EFAULT_VALUE = EFAULT;
    constexpr auto ENOTBLK_VALUE = ENOTBLK;
    constexpr auto EBUSY_VALUE = EBUSY;
    constexpr auto EEXIST_VALUE = EEXIST;
    constexpr auto EXDEV_VALUE = EXDEV;
    constexpr auto ENODEV_VALUE = ENODEV;
    constexpr auto ENOTDIR_VALUE = ENOTDIR;
    constexpr auto EISDIR_VALUE = EISDIR;
    constexpr auto EINVAL_VALUE = EINVAL;
    constexpr auto ENFILE_VALUE = ENFILE;
    constexpr auto EMFILE_VALUE = EMFILE;
    constexpr auto ENOTTY_VALUE = ENOTTY;
    constexpr auto ETXTBSY_VALUE = ETXTBSY;
    constexpr auto EFBIG_VALUE = EFBIG;
    constexpr auto ENOSPC_VALUE = ENOSPC;
    constexpr auto ESPIPE_VALUE = ESPIPE;
    constexpr auto EROFS_VALUE = EROFS;
    constexpr auto EMLINK_VALUE = EMLINK;
    constexpr auto EPIPE_VALUE = EPIPE;
    constexpr auto EDOM_VALUE = EDOM;
    constexpr auto ERANGE_VALUE = ERANGE;
    constexpr auto EDEADLK_VALUE = EDEADLK;
    constexpr auto ENAMETOOLONG_VALUE = ENAMETOOLONG;
    constexpr auto ENOLCK_VALUE = ENOLCK;
    constexpr auto ENOSYS_VALUE = ENOSYS;
    constexpr auto ENOTEMPTY_VALUE = ENOTEMPTY;
    constexpr auto ELOOP_VALUE = ELOOP;
    constexpr auto ENOMSG_VALUE = ENOMSG;
    constexpr auto EIDRM_VALUE = EIDRM;
    constexpr auto ENOSTR_VALUE = ENOSTR;
    constexpr auto ENODATA_VALUE = ENODATA;
    constexpr auto ETIME_VALUE = ETIME;
    constexpr auto ENOSR_VALUE = ENOSR;
    constexpr auto EREMOTE_VALUE = EREMOTE;
    constexpr auto ENOLINK_VALUE = ENOLINK;
    constexpr auto EPROTO_VALUE = EPROTO;
    constexpr auto EMULTIHOP_VALUE = EMULTIHOP;
    constexpr auto EBADMSG_VALUE = EBADMSG;
    constexpr auto EOVERFLOW_VALUE = EOVERFLOW;
    constexpr auto EILSEQ_VALUE = EILSEQ;
    constexpr auto EUSERS_VALUE = EUSERS;
    constexpr auto ENOTSOCK_VALUE = ENOTSOCK;
    constexpr auto EDESTADDRREQ_VALUE = EDESTADDRREQ;
    constexpr auto EMSGSIZE_VALUE = EMSGSIZE;
    constexpr auto EPROTOTYPE_VALUE = EPROTOTYPE;
    constexpr auto ENOPROTOOPT_VALUE = ENOPROTOOPT;
    constexpr auto EPROTONOSUPPORT_VALUE = EPROTONOSUPPORT;
    constexpr auto ESOCKTNOSUPPORT_VALUE = ESOCKTNOSUPPORT;
    constexpr auto EOPNOTSUPP_VALUE = EOPNOTSUPP;
    constexpr auto EPFNOSUPPORT_VALUE = EPFNOSUPPORT;
    constexpr auto EAFNOSUPPORT_VALUE = EAFNOSUPPORT;
    constexpr auto EADDRINUSE_VALUE = EADDRINUSE;
    constexpr auto EADDRNOTAVAIL_VALUE = EADDRNOTAVAIL;
    constexpr auto ENETDOWN_VALUE = ENETDOWN;
    constexpr auto ENETUNREACH_VALUE = ENETUNREACH;
    constexpr auto ENETRESET_VALUE = ENETRESET;
    constexpr auto ECONNABORTED_VALUE = ECONNABORTED;
    constexpr auto ECONNRESET_VALUE = ECONNRESET;
    constexpr auto ENOBUFS_VALUE = ENOBUFS;
    constexpr auto EISCONN_VALUE = EISCONN;
    constexpr auto ENOTCONN_VALUE = ENOTCONN;
    constexpr auto ESHUTDOWN_VALUE = ESHUTDOWN;
    constexpr auto ETOOMANYREFS_VALUE = ETOOMANYREFS;
    constexpr auto ETIMEDOUT_VALUE = ETIMEDOUT;
    constexpr auto ECONNREFUSED_VALUE = ECONNREFUSED;
    constexpr auto EHOSTDOWN_VALUE = EHOSTDOWN;
    constexpr auto EHOSTUNREACH_VALUE = EHOSTUNREACH;
    constexpr auto EALREADY_VALUE = EALREADY;
    constexpr auto EINPROGRESS_VALUE = EINPROGRESS;
    constexpr auto ESTALE_VALUE = ESTALE;
    constexpr auto EDQUOT_VALUE = EDQUOT;
    constexpr auto ECANCELED_VALUE = ECANCELED;
    constexpr auto EOWNERDEAD_VALUE = EOWNERDEAD;
    constexpr auto ENOTRECOVERABLE_VALUE = ENOTRECOVERABLE;
    constexpr auto ENOTSUP_VALUE = ENOTSUP;

    /**
     * @brief Linux's own additions to errno.
     *
     * Absent on Darwin and the BSDs. They stay here beside the portable names
     * rather than moving to linux.constants.inl only because they are already
     * spelled stdx::os::unix::E* and consumers name them that way; a move would
     * be the tidier home but a breaking one.
     */
    #ifdef __linux__
    constexpr auto ECHRNG_VALUE = ECHRNG;
    constexpr auto EL2NSYNC_VALUE = EL2NSYNC;
    constexpr auto EL3HLT_VALUE = EL3HLT;
    constexpr auto EL3RST_VALUE = EL3RST;
    constexpr auto ELNRNG_VALUE = ELNRNG;
    constexpr auto EUNATCH_VALUE = EUNATCH;
    constexpr auto ENOCSI_VALUE = ENOCSI;
    constexpr auto EL2HLT_VALUE = EL2HLT;
    constexpr auto EBADE_VALUE = EBADE;
    constexpr auto EBADR_VALUE = EBADR;
    constexpr auto EXFULL_VALUE = EXFULL;
    constexpr auto ENOANO_VALUE = ENOANO;
    constexpr auto EBADRQC_VALUE = EBADRQC;
    constexpr auto EBADSLT_VALUE = EBADSLT;
    constexpr auto EBFONT_VALUE = EBFONT;
    constexpr auto ENONET_VALUE = ENONET;
    constexpr auto ENOPKG_VALUE = ENOPKG;
    constexpr auto EADV_VALUE = EADV;
    constexpr auto ESRMNT_VALUE = ESRMNT;
    constexpr auto ECOMM_VALUE = ECOMM;
    constexpr auto EDOTDOT_VALUE = EDOTDOT;
    constexpr auto ENOTUNIQ_VALUE = ENOTUNIQ;
    constexpr auto EBADFD_VALUE = EBADFD;
    constexpr auto EREMCHG_VALUE = EREMCHG;
    constexpr auto ELIBACC_VALUE = ELIBACC;
    constexpr auto ELIBBAD_VALUE = ELIBBAD;
    constexpr auto ELIBSCN_VALUE = ELIBSCN;
    constexpr auto ELIBMAX_VALUE = ELIBMAX;
    constexpr auto ELIBEXEC_VALUE = ELIBEXEC;
    constexpr auto ERESTART_VALUE = ERESTART;
    constexpr auto ESTRPIPE_VALUE = ESTRPIPE;
    constexpr auto EUCLEAN_VALUE = EUCLEAN;
    constexpr auto ENOTNAM_VALUE = ENOTNAM;
    constexpr auto ENAVAIL_VALUE = ENAVAIL;
    constexpr auto EISNAM_VALUE = EISNAM;
    constexpr auto EREMOTEIO_VALUE = EREMOTEIO;
    constexpr auto ENOMEDIUM_VALUE = ENOMEDIUM;
    constexpr auto EMEDIUMTYPE_VALUE = EMEDIUMTYPE;
    constexpr auto ENOKEY_VALUE = ENOKEY;
    constexpr auto EKEYEXPIRED_VALUE = EKEYEXPIRED;
    constexpr auto EKEYREVOKED_VALUE = EKEYREVOKED;
    constexpr auto EKEYREJECTED_VALUE = EKEYREJECTED;
    constexpr auto ERFKILL_VALUE = ERFKILL;
    constexpr auto EHWPOISON_VALUE = EHWPOISON;
    #endif
    #endif
}

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    [[nodiscard]]
    inline auto& errnov() noexcept {
        return errno;
    }
}
