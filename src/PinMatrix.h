#ifndef PinMatrix_h
#define PinMatrix_h

#include <BitArray.h>
#include <SketchBoundLibrary.h>

typedef void (*PinMatrixUpdatedEventHandler) (uint8_t, uint8_t, bool);

template <uint8_t row_count, uint8_t column_count>
class PinMatrix
{
public:
	PinMatrix(const uint8_t(&rowPins)[row_count], const uint8_t(&columnPins)[column_count], const time_t debounce)
		: updateHandler(nullptr), lastRead(0), debounce(debounce), rowPins(rowPins), columnPins(columnPins), read_callable(this, &PinMatrix::read), stopRecusion(false)
	{
		for (uint8_t iRow = 0; iRow < row_count; iRow++)
		{
			pinMode(rowPins[iRow], OUTPUT);
			digitalWrite(rowPins[iRow], HIGH);
		}

		for (uint8_t iColumn = 0; iColumn < column_count; iColumn++)
		{
			pinMode(columnPins[iColumn], INPUT_PULLUP);
		}

		addInternalSketchBinding(bind_loop, &read_callable);
	}

	void read()
	{
		if (stopRecusion)
			return;
		if (!intervalElapsed(lastRead, debounce))
			return;

		for (uint8_t iRow = 0; iRow < row_count; iRow++)
		{
			digitalWrite(rowPins[iRow], LOW);
			for (uint8_t iColumn = 0; iColumn < column_count; iColumn++)
			{
				bool result = !digitalRead(columnPins[iColumn]);
				if (result != matrixStates[iRow].get(iColumn))
				{
					matrixStates[iRow].set(iColumn, result);
					stopRecusion = true;
					if (updateHandler)
						updateHandler(iRow, iColumn, result);
					stopRecusion = false;
				}
			}
			digitalWrite(rowPins[iRow], HIGH);
		}
	}

	constexpr uint8_t rowCount() { return row_count; }

	constexpr uint8_t columnCount() { return column_count; }

	PinMatrixUpdatedEventHandler updateHandler;

private:
	BitArray<column_count> matrixStates[row_count];
	time_t lastRead;
	time_t debounce;
	const uint8_t(&rowPins)[row_count];
	const uint8_t(&columnPins)[column_count];
	VoidMemberVoid<PinMatrix> read_callable;
	bool stopRecusion;
};

template <typename row_uint8_t, typename column_uint8_t, uint8_t row_count = sizeof(row_uint8_t), uint8_t column_count = sizeof(column_uint8_t)>
PinMatrix<row_count, column_count> createMatrix(const row_uint8_t(&rowPins)[row_count], const column_uint8_t(&columnPins)[column_count], time_t debounce)
{
	return PinMatrix<row_count, column_count>(rowPins, columnPins, debounce);
}

#endif // !PinMatrix_h