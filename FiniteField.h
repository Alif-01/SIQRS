#pragma once

class Integer {
public:
	Integer(unsigned int x=0);
	~Integer();

	friend Integer operator+(const Integer& x, const Integer& y);
	friend Integer operator-(const Integer& x, const Integer& y);
	friend Integer operator*(const Integer& x, const Integer& y);

	Integer square();
	Integer inv();
};

// scalar in F_{p^2}
class Scalar {
	// x + iy
	Scalar(unsigned int x=0, unsigned int y=0);
	Scalar(const Integer& x, const Integer& y);
	~Scalar();

	friend Scalar operator+(const Scalar& x, const Scalar& y);
	friend Scalar operator-(const Scalar& x, const Scalar& y);
	friend Scalar operator*(const Scalar& x, const Scalar& y); // 3M

	Scalar square(); // 2M
	Scalar inv(); // 1I + 2S + 2M
};

