#pragma once
#include <cinttypes>
#include <iostream>


struct int2025_t {
    // implement me
    uint8_t data[254];

};

static_assert(sizeof(int2025_t) <= 254,  "Size of int2025_t must be no higher than 254 bytes");
int2025_t from_int(int32_t i);
int2025_t from_string(const char* buff);
int2025_t operator+(const int2025_t& lhs, const int2025_t& rhs);
int2025_t operator-(const int2025_t& lhs, const int2025_t& rhs);
int2025_t operator*(const int2025_t& lhs, const int2025_t& rhs);
int2025_t operator/(const int2025_t& lhs, const int2025_t& rhs);
bool operator==(const int2025_t& lhs, const int2025_t& rhs);
bool operator!=(const int2025_t& lhs, const int2025_t& rhs);
std::ostream& operator<<(std::ostream& stream, const int2025_t& value);



static bool is_zero(const int2025_t& num);
static bool get_sign(const int2025_t& num);
static int2025_t negate(const int2025_t& num);
static int2025_t abs(const int2025_t& num);
static int compare_abs(const int2025_t& lhs, const int2025_t& rhs);
static int2025_t sub_abs(const int2025_t& lhs, const int2025_t& rhs);
static void format_chunk(uint32_t chunk, bool first_chunk, std::ostream& stream);