#include "SpawnedItem.h"

#include "VectorArith.h"

// Note: Member initializer list is better, as the attribute do not receive first their default value and are
// then (again) written in the constructor's body (esp. if initialization is expensive!).

SpawnedItem::SpawnedItem() {}
SpawnedItem::SpawnedItem(Vec2 position, int frameCountdown)
	: Position(position), FrameCountdown(frameCountdown)
{
}
