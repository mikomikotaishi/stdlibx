#pragma once

/**
 * @namespace stdx::os::win32
 * @brief CryptoAPI (wincrypt.h) wrappers (CSP / CertStore / CMS).
 *
 * Note: CryptoAPI is the legacy provider-based API. For modern code on
 * Vista+ prefer CNG (bcrypt.h / ncrypt.h); a separate .inl can wrap those.
 */
export namespace stdx::os::win32 {
    #ifdef _WIN32
    // Algorithm IDs
    using AlgId = ::ALG_ID;

    // Handles (all ULONG_PTR-backed opaque)
    using CryptProviderHandle = ::HCRYPTPROV;
    using CryptKeyHandle = ::HCRYPTKEY;
    using CryptHashHandle = ::HCRYPTHASH;
    using CryptMessageHandle = ::HCRYPTMSG;
    using CertStoreHandle = ::HCERTSTORE;
    using CryptProviderOrNCryptKeyHandle = ::HCRYPTPROV_OR_NCRYPT_KEY_HANDLE;

    // Common structures
    using CryptHashInfo = ::HMAC_INFO;
    using CryptProvEnumAlgs = ::PROV_ENUMALGS;
    using CryptProvEnumAlgsEx = ::PROV_ENUMALGS_EX;
    using PublicKeyStruct = ::PUBLICKEYSTRUC;
    using RsaPublicKey = ::RSAPUBKEY;
    using DssSeed = ::DSSSEED;
    using CertContext = ::CERT_CONTEXT;
    using CertContextPointer = ::PCCERT_CONTEXT;
    using CertInfo = ::CERT_INFO;
    using CertInfoPointer = ::PCERT_INFO;
    using CertPublicKeyInfo = ::CERT_PUBLIC_KEY_INFO;
    using CryptAlgorithmIdentifier = ::CRYPT_ALGORITHM_IDENTIFIER;
    using CryptKeyProvInfo = ::CRYPT_KEY_PROV_INFO;
    using CryptIntegerBlob = ::CRYPT_INTEGER_BLOB;
    using CryptDataBlob = ::CRYPT_DATA_BLOB;
    using CryptObjectBlob = ::CRYPT_OBJID_BLOB;

    // CSP lifecycle
    using ::CryptAcquireContextA;
    using ::CryptAcquireContextW;
    using ::CryptReleaseContext;
    using ::CryptGetProvParam;
    using ::CryptSetProvParam;
    using ::CryptEnumProvidersA;
    using ::CryptEnumProvidersW;

    // Key management
    using ::CryptGenKey;
    using ::CryptDestroyKey;
    using ::CryptImportKey;
    using ::CryptExportKey;
    using ::CryptDuplicateKey;
    using ::CryptDeriveKey;
    using ::CryptGetUserKey;

    // Hashing
    using ::CryptCreateHash;
    using ::CryptHashData;
    using ::CryptHashSessionKey;
    using ::CryptGetHashParam;
    using ::CryptSetHashParam;
    using ::CryptDestroyHash;
    using ::CryptDuplicateHash;
    using ::CryptSignHashA;
    using ::CryptSignHashW;
    using ::CryptVerifySignatureA;
    using ::CryptVerifySignatureW;

    // Encryption
    using ::CryptEncrypt;
    using ::CryptDecrypt;

    // Random
    using ::CryptGenRandom;

    // Certificate store
    using ::CertOpenStore;
    using ::CertOpenSystemStoreA;
    using ::CertOpenSystemStoreW;
    using ::CertCloseStore;
    using ::CertEnumCertificatesInStore;
    using ::CertFindCertificateInStore;
    using ::CertGetCertificateContextProperty;
    using ::CertSetCertificateContextProperty;
    using ::CertFreeCertificateContext;
    #endif
}
