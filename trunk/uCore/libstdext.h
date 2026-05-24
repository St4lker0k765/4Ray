#pragma once

inline bool		_valid(const float x)
{
	// check for: Signaling NaN, Quiet NaN, Negative infinity ( –INF), Positive infinity (+INF), Negative denormalized, Positive denormalized
	int			cls = _fpclass(double(x));
	if (cls & (_FPCLASS_SNAN + _FPCLASS_QNAN + _FPCLASS_NINF + _FPCLASS_PINF + _FPCLASS_ND + _FPCLASS_PD))
		return	false;

	/*	*****other cases are*****
	_FPCLASS_NN Negative normalized non-zero
	_FPCLASS_NZ Negative zero ( – 0)
	_FPCLASS_PZ Positive 0 (+0)
	_FPCLASS_PN Positive normalized non-zero
	*/
	return		true;
}

inline bool		_valid(const double x)
{
	// check for: Signaling NaN, Quiet NaN, Negative infinity ( –INF), Positive infinity (+INF), Negative denormalized, Positive denormalized
	int			cls = _fpclass(x);
	if (cls & (_FPCLASS_SNAN + _FPCLASS_QNAN + _FPCLASS_NINF + _FPCLASS_PINF + _FPCLASS_ND + _FPCLASS_PD))
		return false;

	/*	*****other cases are*****
	_FPCLASS_NN Negative normalized non-zero
	_FPCLASS_NZ Negative zero ( – 0)
	_FPCLASS_PZ Positive 0 (+0)
	_FPCLASS_PN Positive normalized non-zero
	*/
	return		true;
}