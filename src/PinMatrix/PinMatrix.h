#ifndef PinMatrix_h
#define PinMatrix_h

#include <BitArray.h>

typedef void (*PinMatrixUpdatedEventHandler) (uint8_t, uint8_t, bool);

template <typename row_uint8_t_array, typename column_uint8_t_array, uint8_t row_count = sizeof(row_uint8_t_array), uint8_t column_count = sizeof(column_uint8_t_array)>
class PinMatrix
{
public:

private:
	BitArray<row_count> matrixStates;
	uint32_t lastRead;
	uint16_t debounce;
};

#endif // !PinMatrix_h