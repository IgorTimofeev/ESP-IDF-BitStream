#pragma once

#include <cmath>
#include <algorithm>

namespace YOBA {
	class ReadableBitStream {
		public:
			explicit ReadableBitStream(uint8_t* buffer, size_t bitIndex = 0) :
				buffer(buffer),
				bitIndex(bitIndex),
				byteBitIndex(bitIndex % 8)
			{

			}

			size_t getBitIndex() const {
				return bitIndex;
			}

			size_t getByteIndex() const {
				return bitIndex / 8;
			}

			bool readBool() {
				const auto result = ((*buffer) >> byteBitIndex) & 0b1;
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

		private:
			uint8_t* buffer;
			size_t bitIndex;
			uint8_t byteBitIndex;

			void nextBit() {
				bitIndex++;
				byteBitIndex++;

				if (byteBitIndex >= 8) {
					buffer++;
					byteBitIndex = 0;
				}
			}

			template<typename TNumber>
			TNumber readUnsignedNumber(uint8_t bits) {
				TNumber result = 0;

				for (int i = 0; i < bits; ++i) {
					result |= ((((*buffer) >> byteBitIndex) & 0b1) << i);
					nextBit();
				}

				return result;
			}

			template<typename TNumber>
			TNumber readSignedNumber(uint8_t bits) {
				// Magnitude
				TNumber magnitude = 0;

				for (int i = 0; i < bits - 1; ++i) {
					magnitude |= ((((*buffer) >> byteBitIndex) & 0b1) << i);
					nextBit();
				}

				// Sign
				const auto sign = (((*buffer) >> byteBitIndex) & 0b1) == 1;
				nextBit();

				return sign ? -magnitude : magnitude;
			}
	};
}