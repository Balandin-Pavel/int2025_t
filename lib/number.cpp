#include "number.h"
#include <cstring>

static bool is_zero(const int2025_t& num) {
    for (int i = 0; i < 254; ++i) {
        if (num.data[i] != 0) return false;
    }
    return true;
}

static bool get_sign(const int2025_t& num) {
    return (num.data[253] & 1) != 0;
}

static int2025_t negate(const int2025_t& num) {
    int2025_t result;
    for (int i = 0; i < 254; ++i) {
        result.data[i] = ~num.data[i];
    }
    
    int2025_t one = {};
    one.data[0] = 1;
    result = result + one;
    
    result.data[253] &= 0x01;
    return result;
}

static int2025_t abs(const int2025_t& num) {
    return get_sign(num) ? negate(num) : num;
}

static int compare_abs(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t abs_lhs = abs(lhs);
    int2025_t abs_rhs = abs(rhs);
    
    for (int i = 252; i >= 0; --i) {
        if (abs_lhs.data[i] > abs_rhs.data[i]) return 1;
        if (abs_lhs.data[i] < abs_rhs.data[i]) return -1;
    }
    return 0;
}

static int2025_t sub_abs(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t abs_lhs = abs(lhs);
    int2025_t abs_rhs = abs(rhs);
    return abs_lhs + negate(abs_rhs);
}

int2025_t from_int(int32_t i) {
    int2025_t result = {};
    bool sign = i < 0;
    uint32_t abs_val = sign ? -static_cast<uint32_t>(i) : static_cast<uint32_t>(i);
    std::memcpy(result.data, &abs_val, sizeof(abs_val));
    return sign ? negate(result) : result;
}

int2025_t from_string(const char* buff) {
    int2025_t result = {};
    bool sign = (*buff == '-');
    if (*buff == '-' || *buff == '+') ++buff;
    while (*buff >= '0' && *buff <= '9') {
        uint16_t carry = 0;
        for (int i = 0; i < 254; ++i) {
            uint16_t prod = (uint16_t)result.data[i] * 10 + carry;
            result.data[i] = prod & 0xFF;
            carry = prod >> 8;
        }
        result.data[253] &= 0x01;
        carry = *buff - '0';
        for (int i = 0; i < 254 && carry; ++i) {
            uint16_t sum = result.data[i] + carry;
            result.data[i] = sum & 0xFF;
            carry = sum >> 8;
        }
        result.data[253] &= 0x01;
        ++buff;
    }
    return sign ? negate(result) : result;
}

int2025_t operator+(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t result;
    uint16_t carry = 0;
    for (int i = 0; i < 254; ++i) {
        uint16_t sum = (uint16_t)lhs.data[i] + rhs.data[i] + carry;
        result.data[i] = sum & 0xFF;
        carry = sum >> 8;
    }
    result.data[253] &= 0x01;
    return result;
}

int2025_t operator-(const int2025_t& lhs, const int2025_t& rhs) {
    return lhs + negate(rhs);
}

int2025_t operator*(const int2025_t& lhs, const int2025_t& rhs) {
    int2025_t result = {};
    for (int i = 0; i < 254; ++i) {
        unsigned int carry = 0;
        for (int j = 0; j < 254 - i; ++j) {
            unsigned int prod = static_cast<unsigned int>(lhs.data[i]) * static_cast<unsigned int>(rhs.data[j]) + carry + static_cast<unsigned int>(result.data[i + j]);
            result.data[i + j] = static_cast<uint8_t>(prod & 0xFF);
            carry = prod >> 8;
        }
    }
    result.data[253] &= 0x01;
    return result;
}

int2025_t operator/(const int2025_t& lhs, const int2025_t& rhs) {
    if (is_zero(rhs)) return {};
    bool result_sign = get_sign(lhs) != get_sign(rhs);
    int2025_t abs_lhs = abs(lhs);
    int2025_t abs_rhs = abs(rhs);
    int2025_t quotient = {};
    int2025_t remainder = {};
    for (int k = 2024; k >= 0; --k) {
        uint16_t carry = 0;
        for (int i = 0; i < 254; ++i) {
            uint16_t temp = ((uint16_t)remainder.data[i] << 1) + carry;
            remainder.data[i] = temp & 0xFF;
            carry = temp >> 8;
        }
        remainder.data[253] &= 0x01;
        if ((abs_lhs.data[k / 8] & (1 << (k % 8))) != 0) {
            remainder.data[0] |= 1;
        }
        if (compare_abs(remainder, abs_rhs) >= 0) {
            remainder = sub_abs(remainder, abs_rhs);
            quotient.data[k / 8] |= 1 << (k % 8);
        }
    }
    return result_sign ? negate(quotient) : quotient;
}

bool operator==(const int2025_t& lhs, const int2025_t& rhs) {
    return std::memcmp(lhs.data, rhs.data, 254) == 0;
}

bool operator!=(const int2025_t& lhs, const int2025_t& rhs) {
    return !(lhs == rhs);
}

static void format_chunk(uint32_t chunk, bool first_chunk, std::ostream& stream) {
    if (first_chunk) {
        stream << chunk;
    } else {
        char buffer[10];
        int pos = 9;
        buffer[pos] = '\0';
        for (int i = 0; i < 9; i++) {
            buffer[--pos] = '0' + (chunk % 10);
            chunk /= 10;
        }
        stream << (buffer + pos);
    }
}

std::ostream& operator<<(std::ostream& stream, const int2025_t& value) {
    if(is_zero(value)){
        stream << "0";
        return stream; 
    }
    uint32_t chunks[76];
    const uint32_t divisor = 1000000000;
    int chunk_counter = 0;
    int2025_t num = abs(value);
    bool sign = get_sign(value);
    while (!is_zero(num)){
        int2025_t quotient = {};
        uint64_t current = 0;
        for (int i = 253; i >=0; i --){
            current = (current << 8) | num.data[i];
            if (current >= divisor) {
                uint32_t q = current / divisor;
                uint32_t r = current % divisor;
                quotient.data[i] = q;
                current = r;
            }
            else{
                quotient.data[i] = 0;
            }
        }
        chunks[chunk_counter++] = static_cast<uint32_t> (current);
        num = quotient;
    }
    if(sign) stream << "-";
    bool first_chunk = true;
    for (int i = chunk_counter - 1; i >=0; i --){
        format_chunk(chunks[i], first_chunk, stream);
        first_chunk = false;
    }
    return stream;

}