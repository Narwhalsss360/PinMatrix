#ifndef PinMatrix_h
#define PinMatrix_h

#include <BitArray.h>
#include <SketchBoundLibrary.h>
#include <NEvents.h>

typedef void (*PinMatrixUpdatedEventHandler) (uint8_t, uint8_t, bool);

template <uint8_t row_count, uint8_t column_count>
class PinMatrix
{
public:
	/// @brief Class for reading and iterpreting matrices.
	/// @param rowPins array of pins of the rows
	/// @param columnPins array of pins of the columns
	/// @param debounce button debounce time
	PinMatrix(const uint8_t(&rowPins)[row_count], const uint8_t(&columnPins)[column_count], const time_t debounce)
		: updateHandler(Event<PinMatrix, byte, byte, bool>()), lastRead(0), debounce(debounce), rowPins(rowPins), columnPins(columnPins), stopRecusion(false)
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

		addSketchBinding(bind_loop, &invokable_get(this, &PinMatrix::read));
	}

	/// @brief Reads state of matrix.
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
					updateHandler(iRow, iColumn, result);
					stopRecusion = false;
				}
			}
			digitalWrite(rowPins[iRow], HIGH);
		}
	}

	/// @brief Get how many rows.
	/// @return `uint8_t` row count
	constexpr uint8_t rowCount() { return row_count; }

	/// @brief Get how many columns
	/// @return `uint8_t` column count
	constexpr uint8_t columnCount() { return column_count; }

	/// @brief Event invoked when a button on the matrix is either pushed or released.
	Event<PinMatrix, byte, byte, bool> updateHandler;

private:
	BitArray<column_count> matrixStates[row_count];
	time_t lastRead;
	time_t debounce;
	const uint8_t(&rowPins)[row_count];
	const uint8_t(&columnPins)[column_count];
	bool stopRecusion;
};

/// @brief Create a `PinMatrix` object according to arguments. (wrapper function).
/// @tparam row_uint8_t row array type
/// @tparam column_uint8_t column array type
/// @tparam row_count row count
/// @tparam column_count column count
/// @param rowPins row pins array
/// @param columnPins column pins array
/// @param debounce debounce time
/// @return `PinMatrix` object
template <typename row_uint8_t, typename column_uint8_t, uint8_t row_count = sizeof(row_uint8_t), uint8_t column_count = sizeof(column_uint8_t)>
PinMatrix<row_count, column_count> createMatrix(const row_uint8_t(&rowPins)[row_count], const column_uint8_t(&columnPins)[column_count], time_t debounce)
{
	return PinMatrix<row_count, column_count>(rowPins, columnPins, debounce);
}

#endif // !PinMatrix_h