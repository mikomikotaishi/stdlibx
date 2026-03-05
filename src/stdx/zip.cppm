/**
 * @file zip.cppm
 * @module stdx:zip
 * @brief Import of ZIP archive handling modules.
 *
 * This file imports the modules for the ZIP archive handling library.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
#include <zlib.h>
#endif

#include "Macros.hpp"

export module stdx:zip;

import :main;

using stdx::io::ByteBuffer;
using stdx::io::IOException;

using namespace stdx::core;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
namespace stdx::zip {
    [[nodiscard]]
    inline String build_error_message(StringView prefix, const z_stream& stream, i32 code) {
        String text(prefix);
        text += stdx::fmt::format(" (zlib code {})", code);
        if (stream.msg) {
            text += stdx::fmt::format(": {}", stream.msg);
        }
        return text;
    }
}
#endif

/**
 * @namespace stdx::zip
 * @brief Wrapper namespace for ZIP archive handling operations within stdx.
 */
export namespace stdx::zip {

class ZipException : public IOException {
public:
	explicit ZipException(const String& msg):
		IOException(msg) {}
};

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY

enum class FlushMode : u8 {
	NO_FLUSH = Z_NO_FLUSH,
	PARTIAL_FLUSH = Z_PARTIAL_FLUSH,
	SYNC_FLUSH = Z_SYNC_FLUSH,
	FULL_FLUSH = Z_FULL_FLUSH,
	FINISH = Z_FINISH,
	BLOCK = Z_BLOCK,
	TREES = Z_TREES,
};

enum class InflateResult: i8 {
    OK = Z_OK,
    STREAM_END = Z_STREAM_END,
    NEED_DICT = Z_NEED_DICT,
    ERRNO = Z_ERRNO,
    STREAM_ERROR = Z_STREAM_ERROR,
    DATA_ERROR = Z_DATA_ERROR,
    MEM_ERROR = Z_MEM_ERROR,
    BUF_ERROR = Z_BUF_ERROR,
    VERSION_ERROR = Z_VERSION_ERROR,
};

enum class CompressionLevel : i8 {
	NO_COMPRESSION = Z_NO_COMPRESSION,
	BEST_SPEED = Z_BEST_SPEED,
	BEST_COMPRESSION = Z_BEST_COMPRESSION,
	DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION,
};

enum class CompressionStrategy : u8 {
    FILTERED = Z_FILTERED,
    HUFFMAN_ONLY = Z_HUFFMAN_ONLY,
    RLE = Z_RLE,
    FIXED = Z_FIXED,
    DEFAULT_STRATEGY = Z_DEFAULT_STRATEGY,
};

enum class DataType : u8 {
    BINARY = Z_BINARY,
    TEXT = Z_TEXT,
    ASCII = Z_ASCII,
    UNKNOWN = Z_UNKNOWN,
};

class CRC32 {
private:
	u64 crc_value;
public:
	CRC32() noexcept:
		crc_value{::crc32(0L, nullptr, 0)} {}

	void reset() noexcept {
		crc_value = ::crc32(0L, nullptr, 0);
	}

	void update(Span<const u8> data) noexcept {
		if (data.empty()) {
			return;
		}
		crc_value = ::crc32(crc_value, reinterpret_cast<const u8*>(data.data()), static_cast<u32>(data.size()));
	}

	[[nodiscard]]
	u64 value() const noexcept {
		return crc_value;
	}
};

class Adler32 {
private:
	u64 adler_value;
public:
	Adler32() noexcept:
		adler_value{::adler32(0L, nullptr, 0)} {}

	void reset() noexcept {
		adler_value = ::adler32(0L, nullptr, 0);
	}

	void update(Span<const u8> data) noexcept {
		if (data.empty()) {
			return;
		}
		adler_value = ::adler32(adler_value, reinterpret_cast<const u8*>(data.data()), static_cast<u32>(data.size()));
	}

	[[nodiscard]]
	u64 value() const noexcept {
		return adler_value;
	}
};

class Deflater {
public:
    static inline constexpr usize CHUNK_SIZE = 16 * 1024;
private:
	z_stream stream = z_stream{ .zalloc = nullptr, .zfree = nullptr, .opaque = nullptr };
	ByteBuffer input;
    usize input_offset;
	bool opened;
	bool finished_stream;
public:
	explicit Deflater(CompressionLevel level = CompressionLevel::DEFAULT_COMPRESSION, bool nowrap = false) throws (ZipException):
		input_offset{0}, opened{false}, finished_stream{false} {
		const i32 window_bits = nowrap ? -MAX_WBITS : MAX_WBITS;
		const i32 rc = deflateInit2(&stream, static_cast<i32>(level), Z_DEFLATED, window_bits, 8, Z_DEFAULT_STRATEGY);
		if (rc != Z_OK) {
			throw ZipException(build_error_message("Failed to initialize deflater", stream, rc));
		}
		opened = true;
	}

	Deflater(const Deflater&) = delete;
	Deflater& operator=(const Deflater&) = delete;

	~Deflater() {
		if (opened) {
			::deflateEnd(&stream);
		}
	}

	void set_input(Span<const u8> data) {
		input.assign(data.begin(), data.end());
		input_offset = 0;
	}

	[[nodiscard]]
	bool needs_input() const noexcept {
		return input_offset >= input.size();
	}

	[[nodiscard]]
	bool finished() const noexcept {
		return finished_stream;
	}

	void reset() throws (ZipException) {
		const i32 rc = ::deflateReset(&stream);
		if (rc != Z_OK) {
			throw ZipException(build_error_message("Failed to reset deflater", stream, rc));
		}
		finished_stream = false;
		input.clear();
		input_offset = 0;
	}

	[[nodiscard]]
	ByteBuffer deflate(FlushMode flush = FlushMode::NO_FLUSH) throws (ZipException) {
		if (!opened) {
			throw ZipException("Deflater is not initialized");
		}

		if (finished_stream) {
			return {};
		}

		Array<u8, CHUNK_SIZE> out_chunk{};
		ByteBuffer out;

		const bool have_input = input_offset < input.size();
		const u32 initial_avail = have_input
			? static_cast<u32>(input.size() - input_offset)
			: 0;

		stream.next_in = have_input
			? reinterpret_cast<u8*>(input.data() + input_offset)
			: nullptr;
		stream.avail_in = initial_avail;

		i32 rc = Z_OK;
		do {
			stream.next_out = reinterpret_cast<u8*>(out_chunk.data());
			stream.avail_out = static_cast<u32>(out_chunk.size());

			rc = ::deflate(&stream, static_cast<i32>(flush));
			if (rc == Z_STREAM_ERROR || rc == Z_BUF_ERROR) {
				throw ZipException(build_error_message("Deflate failed", stream, rc));
			}

			const usize produced = out_chunk.size() - stream.avail_out;
			if (produced > 0) {
				const usize out_old_size = out.size();
				out.resize(out_old_size + produced);
				std::copy(
					out_chunk.begin(),
					out_chunk.begin() + static_cast<ptrdiff>(produced),
					out.data() + static_cast<ptrdiff>(out_old_size)
				);
			}

			if (rc == Z_STREAM_END) {
				finished_stream = true;
				break;
			}
		} while (stream.avail_out == 0 || stream.avail_in > 0 || (flush == FlushMode::FINISH && rc != Z_STREAM_END));

		if (have_input) {
			const u32 consumed = initial_avail - stream.avail_in;
			input_offset += consumed;
			if (input_offset >= input.size()) {
				input.clear();
				input_offset = 0;
			}
		}

		return out;
	}

	[[nodiscard]]
	ByteBuffer finish() throws (ZipException) {
		return deflate(FlushMode::FINISH);
	}

	[[nodiscard]]
	u64 total_in() const noexcept {
		return stream.total_in;
	}

	[[nodiscard]]
	u64 total_out() const noexcept {
		return stream.total_out;
	}
};

class Inflater {
public:
    static inline constexpr usize CHUNK_SIZE = 16 * 1024;
private:
	z_stream stream = { .zalloc = nullptr, .zfree = nullptr, .opaque = nullptr };
	ByteBuffer input;
	usize input_offset;
	bool opened;
	bool finished_stream;
public:
	explicit Inflater(bool nowrap = false) throws (ZipException):
		opened{false}, finished_stream{false}, input{}, input_offset{0} {

		const i32 window_bits = nowrap ? -MAX_WBITS : MAX_WBITS;
		const i32 rc = inflateInit2(&stream, window_bits);
		if (rc != Z_OK) {
			throw ZipException(build_error_message("Failed to initialize inflater", stream, rc));
		}

		opened = true;
	}

	Inflater(const Inflater&) = delete;
	Inflater& operator=(const Inflater&) = delete;

	~Inflater() {
		if (opened) {
			::inflateEnd(&stream);
		}
	}

	void set_input(Span<const u8> data) {
		input.assign(data.begin(), data.end());
		input_offset = 0;
	}

	[[nodiscard]]
	bool needs_input() const noexcept {
		return input_offset >= input.size();
	}

	[[nodiscard]]
	bool finished() const noexcept {
		return finished_stream;
	}

	void reset() throws (ZipException) {
		const i32 rc = ::inflateReset(&stream);
		if (rc != Z_OK) {
			throw ZipException(build_error_message("Failed to reset inflater", stream, rc));
		}
		finished_stream = false;
		input.clear();
		input_offset = 0;
	}

	[[nodiscard]]
	ByteBuffer inflate() throws (ZipException) {
		if (!opened) {
			throw ZipException("Inflater is not initialized");
		}

		if (finished_stream) {
			return {};
		}

		Array<u8, CHUNK_SIZE> out_chunk{};
		ByteBuffer out;

		const bool have_input = input_offset < input.size();
		const u32 initial_avail = have_input
			? static_cast<u32>(input.size() - input_offset)
			: 0;

		stream.next_in = have_input
			? reinterpret_cast<u8*>(input.data() + input_offset)
			: nullptr;
		stream.avail_in = initial_avail;

		i32 rc = Z_OK;
		do {
			stream.next_out = reinterpret_cast<u8*>(out_chunk.data());
			stream.avail_out = static_cast<u32>(out_chunk.size());

			rc = ::inflate(&stream, Z_NO_FLUSH);
			if (rc == Z_STREAM_ERROR || rc == Z_NEED_DICT || rc == Z_DATA_ERROR || rc == Z_MEM_ERROR) {
				throw ZipException(build_error_message("Inflate failed", stream, rc));
			}

			const usize produced = out_chunk.size() - stream.avail_out;
			if (produced > 0) {
				const usize out_old_size = out.size();
				out.resize(out_old_size + produced);
				std::copy(
					out_chunk.begin(),
					out_chunk.begin() + static_cast<ptrdiff>(produced),
					out.data() + static_cast<ptrdiff>(out_old_size)
				);
			}

			if (rc == Z_STREAM_END) {
				finished_stream = true;
				break;
			}

			if (rc == Z_BUF_ERROR && stream.avail_in == 0) {
				break;
			}
		} while (stream.avail_out == 0 || stream.avail_in > 0);

		if (have_input) {
			const u32 consumed = initial_avail - stream.avail_in;
			input_offset += consumed;
			if (input_offset >= input.size()) {
				input.clear();
				input_offset = 0;
			}
		}

		return out;
	}

	[[nodiscard]]
	ByteBuffer inflate_all() throws (ZipException) {
		ByteBuffer out;
		while (true) {
			ByteBuffer chunk = inflate();
			if (!chunk.empty()) {
				const usize out_old_size = out.size();
				out.resize(out_old_size + chunk.size());
				std::copy(chunk.begin(), chunk.end(), out.data() + static_cast<ptrdiff>(out_old_size));
			}
			if (finished_stream || (needs_input() && chunk.empty())) {
				break;
			}
		}
		return out;
	}

	[[nodiscard]]
	u64 total_in() const noexcept {
		return stream.total_in;
	}

	[[nodiscard]]
	u64 total_out() const noexcept {
		return stream.total_out;
	}
};

[[nodiscard]]
ByteBuffer compress(Span<const u8> data, CompressionLevel level = CompressionLevel::DEFAULT_COMPRESSION, bool nowrap = false) throws (ZipException) {
	Deflater deflater(level, nowrap);
	deflater.set_input(data);
	return deflater.finish();
}

[[nodiscard]]
ByteBuffer compress(const ByteBuffer& data, CompressionLevel level = CompressionLevel::DEFAULT_COMPRESSION, bool nowrap = false) throws (ZipException) {
	return compress(Span<const u8>(data.data(), data.size()), level, nowrap);
}

[[nodiscard]]
ByteBuffer decompress(Span<const u8> data, bool nowrap = false) throws (ZipException) {
	Inflater inflater(nowrap);
	inflater.set_input(data);
	return inflater.inflate_all();
}

[[nodiscard]]
ByteBuffer decompress(const ByteBuffer& data, bool nowrap = false) throws (ZipException) {
	return decompress(Span<const u8>(data.data(), data.size()), nowrap);
}

#endif

}
