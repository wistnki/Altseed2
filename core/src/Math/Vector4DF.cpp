﻿
#include "Vector4DF.h"

namespace Altseed {

Vector4DF::Vector4DF() : X(0), Y(0), Z(0), W(0) {}

Vector4DF::Vector4DF(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

bool Vector4DF::operator==(const Vector4DF& o) { return X == o.X && Y == o.Y && Z == o.Z && W == o.W; }

bool Vector4DF::operator!=(const Vector4DF& o) { return !(X == o.X && Y == o.Y && Z == o.Z && W == o.W); }

Vector4DF Vector4DF::operator-() { return Vector4DF(-X, -Y, -Z, -W); }

Vector4DF Vector4DF::operator+(const Vector4DF& o) const { return Vector4DF(X + o.X, Y + o.Y, Z + o.Z, W + o.W); }

Vector4DF Vector4DF::operator-(const Vector4DF& o) const { return Vector4DF(X - o.X, Y - o.Y, Z - o.Z, W - o.W); }

Vector4DF Vector4DF::operator*(const Vector4DF& o) const { return Vector4DF(X * o.X, Y * o.Y, Z * o.Z, W * o.W); }

Vector4DF Vector4DF::operator/(const Vector4DF& o) const { return Vector4DF(X / o.X, Y / o.Y, Z / o.Z, W / o.W); }

Vector4DF Vector4DF::operator*(const float& o) const { return Vector4DF(X * o, Y * o, Z * o, W * o); }

Vector4DF Vector4DF::operator/(const float& o) const { return Vector4DF(X / o, Y / o, Z * o, W * o); }

float Vector4DF::Dot(const Vector4DF& v1, const Vector4DF& v2) { return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z + v1.W * v2.W; }

float Vector4DF::Distance(const Vector4DF& v1, const Vector4DF& v2) {
    float dx = v1.X - v2.X;
    float dy = v1.Y - v2.Y;
    float dz = v1.Z - v2.Z;
    float dw = v1.W - v2.W;
    return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

Vector4DI Vector4DF::To4DI() const { return Vector4DI((int32_t)X, (int32_t)Y, (int32_t)Z, (int32_t)W); }

}  // namespace Altseed