#include "game/state/gamestate.h"
#include "game/state/rules/vehicle_type.h"
#include "library/sp.h"
#include "library/strings.h"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <map>

namespace OpenApoc
{

static const std::map<VehicleType::Direction, Vec3<float>> DirectionVectors = {
    {VehicleType::Direction::N, glm::normalize(Vec3<float>{0, -1, 0})},
    {VehicleType::Direction::NNE, glm::normalize(Vec3<float>{1, -2, 0})},
    {VehicleType::Direction::NE, glm::normalize(Vec3<float>{1, -1, 0})},
    {VehicleType::Direction::NEE, glm::normalize(Vec3<float>{2, -1, 0})},
    {VehicleType::Direction::E, glm::normalize(Vec3<float>{1, 0, 0})},
    {VehicleType::Direction::SEE, glm::normalize(Vec3<float>{2, 1, 0})},
    {VehicleType::Direction::SE, glm::normalize(Vec3<float>{1, 1, 0})},
    {VehicleType::Direction::SSE, glm::normalize(Vec3<float>{1, 2, 0})},
    {VehicleType::Direction::S, glm::normalize(Vec3<float>{0, 1, 0})},
    {VehicleType::Direction::SSW, glm::normalize(Vec3<float>{-1, 2, 0})},
    {VehicleType::Direction::SW, glm::normalize(Vec3<float>{-1, 1, 0})},
    {VehicleType::Direction::SWW, glm::normalize(Vec3<float>{-2, 1, 0})},
    {VehicleType::Direction::W, glm::normalize(Vec3<float>{-1, 0, 0})},
    {VehicleType::Direction::NWW, glm::normalize(Vec3<float>{-2, -1, 0})},
    {VehicleType::Direction::NW, glm::normalize(Vec3<float>{-1, -1, 0})},
    {VehicleType::Direction::NNW, glm::normalize(Vec3<float>{-1, -2, 0})}};

const Vec3<float> &VehicleType::directionToVector(Direction d)
{
	static Vec3<float> fallback = {1, 0, 0};
	auto it = DirectionVectors.find(d);
	if (it == DirectionVectors.end())
	{
		LogError("Failed to find a direction vector for %d", (int)d);
		return fallback;
	}
	return it->second;
}

static std::map<VehicleType::Direction, Vec3<float>> direction_vectors = {
    {VehicleType::Direction::N, {0, -1, 0}}, {VehicleType::Direction::NE, {1, -1, 0}},
    {VehicleType::Direction::E, {1, 0, 0}},  {VehicleType::Direction::SE, {1, 1, 0}},
    {VehicleType::Direction::S, {0, 1, 0}},  {VehicleType::Direction::SW, {-1, 1, 0}},
    {VehicleType::Direction::W, {-1, 0, 0}}, {VehicleType::Direction::NW, {-1, -1, 0}},
};

static std::map<VehicleType::Banking, Vec3<float>> banking_vectors = {
    {VehicleType::Banking::Flat, Vec3<float>{0, 0, 0}},
    {VehicleType::Banking::Ascending, Vec3<float>{0, 0, 1}},
    {VehicleType::Banking::Descending, Vec3<float>{0, 0, -1}},
};

sp<VehicleType> VehicleType::get(const GameState &state, const UString &id)
{
	auto it = state.vehicle_types.find(id);
	if (it == state.vehicle_types.end())
	{
		LogError("No vehicle type matching ID \"%s\"", id);
		return nullptr;
	}
	return it->second;
}

const UString &VehicleType::getPrefix()
{
	static UString prefix = "VEHICLETYPE_";
	return prefix;
}
const UString &VehicleType::getTypeName()
{
	static UString name = "VehicleType";
	return name;
}
const UString &VehicleType::getId(const GameState &state, const sp<VehicleType> ptr)
{
	static const UString emptyString = "";
	for (auto &v : state.vehicle_types)
	{
		if (v.second == ptr)
			return v.first;
	}
	LogError("No vehicle type matching pointer %p", ptr.get());
	return emptyString;
}

Vec3<float> VehicleType::getVoxelMapFacing(Vec3<float> direction) const
{

	float closestAngle = FLT_MAX;
	Vec3<float> closestFacing;
	for (auto &p : size)
	{
		float angle = glm::angle(glm::normalize(p.first), glm::normalize(direction));
		if (angle < closestAngle)
		{
			closestAngle = angle;
			closestFacing = p.first;
		}
	}
	return closestFacing;
}

}; // namespace OpenApoc
