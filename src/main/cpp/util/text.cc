#include <climits>
#include "util/text.hh"

std::string formatByteCount(unsigned long long b) {
	return "";
	/*return b < 1024ull ? std::format("{:f} B", b)
	: b <= 0xfffccccccccccccull >> 40 ? std::format("{:1f} KiB", b / 0x1p10)
	: b <= 0xfffccccccccccccull >> 30 ? std::format("{:1f} MiB", b / 0x1p20)
	: b <= 0xfffccccccccccccull >> 20 ? std::format("{:1f} GiB", b / 0x1p30)
	: b <= 0xfffccccccccccccull >> 10 ? std::format("{:1f} TiB", b / 0x1p40)
	: b <= 0xfffccccccccccccull
	? std::format("{:1f} PiB", (b >> 10) / 0x1p40)
	: std::format("{:1f} EiB", (b >> 20) / 0x1p40);*/
}
