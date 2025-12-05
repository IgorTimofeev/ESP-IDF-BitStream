#pragma once

#include <cmath>
#include <algorithm>

namespace YOBA {
	class ReadableBitStream {
		public:
			explicit ReadableBitStream(const uint8_t* buffer, size_t byteIndex = 0, size_t bitIndex = 0) :
				buffer(buffer),
				byteIndex(byteIndex),
				bitIndex(bitIndex),
				byteBitIndex(bitIndex % 8)
			{

			}

			const uint8_t* getBuffer() const {
				return buffer;
			}

			size_t getBitIndex() const {
				return bitIndex;
			}

			size_t getByteIndex() const {
				return byteIndex;
			}

			size_t getBytesRead() const {
				return byteBitIndex == 0 ? byteIndex : byteIndex + 1;
			}

			bool readBool() {
				const auto result = (*(buffer + byteIndex) >> byteBitIndex) & 0b1;
				nextBit();

				return result;
			}

			uint8_t readUint8(uint8_t bits = 8) {
				return readUnsignedNumber<uint8_t>(bits);
			}

			uint16_t readUint16(uint8_t bits = 16) {
				return readUnsignedNumber<uint16_t>(bits);
			}

			int16_t readInt16(uint8_t bits = 16) {
				return readSignedNumber<int16_t>(bits);
			}

			uint32_t readUint32(uint8_t bits = 32) {
				return readUnsignedNumber<uint32_t>(bits);
			}

		private:
			const uint8_t* buffer;
			size_t byteIndex;
			size_t bitIndex;
			uint8_t byteBitIndex;

			void nextBit() {
				bitIndex++;
				byteBitIndex++;

				if (byteBitIndex >= 8) {
					byteIndex++;
					byteBitIndex = 0;
				}
			}

			template<typename TNumber>
			TNumber readUnsignedNumber(uint8_t bits) {
				TNumber result = 0;

				for (int i = 0; i < bits; ++i) {
					result |= (((*(buffer + byteIndex) >> byteBitIndex) & 0b1) << i);
					nextBit();
				}

				return result;
			}

			template<typename TNumber>
			TNumber readSignedNumber(uint8_t bits) {
				// Magnitude
				TNumber magnitude = 0;

				for (int i = 0; i < bits - 1; ++i) {
					magnitude |= (((*(buffer + byteIndex) >> byteBitIndex) & 0b1) << i);
					nextBit();
				}

				// Sign
				const auto sign = ((*(buffer + byteIndex) >> byteBitIndex) & 0b1) == 1;
				nextBit();

				return sign ? -magnitude : magnitude;
			}
	};
}