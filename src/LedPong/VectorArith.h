#pragma once

#include "math.h"

// This class represents a zero-based (0,0) position on a map or a template.
class Vec2
{
public:
	Vec2() {}
	Vec2(bool isValid) : IsValid(isValid) {}
	Vec2(int x, int y) : IsValid(true), X(x), Y(y) {}

	// Be default, an "empty" position is invalid.
	bool IsValid = false;

	// Zero-based (0,0) position on a map
	int X = 0, Y = 0;

	// Operators
	inline Vec2 operator +(Vec2 other) { return Vec2(X + other.X, Y + other.Y); }
	inline Vec2 operator -(Vec2 other) { return Vec2(X - other.X, Y - other.Y); }

	// Scalar product
	inline Vec2 operator *(int i) { return Vec2(X * i, Y * i); }
	inline Vec2 operator *(double d) { return Vec2((int) (d * X), (int) (d * Y)); }

	// Hadamard product
	inline Vec2 operator *(Vec2 other) { return Vec2(X * other.X, Y * other.Y); }

	// Equality
	inline bool operator ==(Vec2 other) { return X == other.X && Y == other.Y; }
	inline bool operator !=(Vec2 other) { return X != other.X || Y != other.Y; }

	// Length
	inline double Length() { return sqrt(X * X + Y * Y); }
};

