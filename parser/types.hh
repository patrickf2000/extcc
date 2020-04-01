#pragma once

enum class DataType {
	None,
	Void,
	Bool,
	Byte,
	UByte,
	Short,
	UShort,
	Int,
	UInt,
	Long,
	ULong,
	Float,
	Double,
	Char,
	Char16,
	Char32,
	Int64,
	Int128,
	Int256,
	UInt64,
	UInt128,
	UInt256,
	Float64,
	Float128,
	Float256,
	Double128,
	Double256,
	Matrix,
	Str
};

enum class CondOp {
	None,
	Equals,
	NotEquals,
	Greater,
	GreaterEq,
	Less,
	LessEq
};
